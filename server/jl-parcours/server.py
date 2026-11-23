#!/usr/bin/env python3
# ---------------------------------------------------------------------------
# jl-parcours - Sprachsteuerung fuer Parcours 1: Sprache -> Text -> Claude Code im Repo.
#
# Ablauf: Sprechtaste am Giga gedrueckt halten -> Giga schickt das Mikrofon (A7) per MQTT
# ans M5Stack -> das M5 streamt die Aufnahme hierher (POST /stimme, chunked).
# Hier: Whisper (ueber jl-minime /stt) -> Claude Code CLI headless in einer Kopie des
# Repos (eigener Branch) -> Zwischenschritte live abrufbar -> Antwort als Sprache.
# Aenderungen bleiben auf dem Branch, bis am M5 "uebernehmen" gedrueckt wird; dann
# landen sie auf main, GitHub Actions baut, der Giga holt sich die Firmware selbst.
#
#   POST /stimme              rohes Audio u8 8 kHz mono (auch chunked) -> {"id"}
#                             Header X-P1-Status: aktueller Zustand der Anlage (JSON vom Giga), optional
#   POST /text                {"text"} (zum Testen ohne Mikrofon)      -> {"id"}
#   GET  /job/<id>?ab=<n>     -> {status, frage, schritte[n:], n, antwort, aenderung, aktionen, fehler}
#                             aktionen: Befehle fuer die Anlage, die das M5 per MQTT ausfuehrt
#   GET  /tts/<id>            Antwort als u8 8 kHz mono (wartet auf die Synthese)
#   POST /job/<id>/ja         Branch auf main bringen (push)      -> {"ok"} | {"fehler"}
#   POST /job/<id>/nein       Branch verwerfen
#   GET  /health
# Alle Anfragen ausser /health, GET /job/<id> und GET /tts/<id> brauchen den Header X-P1-Token.
# Lesen geht ueber die zufaellige Job-Nummer (das M5 hat zu wenig RAM fuer HTTPS und liest per HTTP);
# alles, was etwas ausloest (/stimme, /text, /ja, /nein), braucht den Token und kommt per HTTPS vom Giga.
#
# Laeuft als jlclaude (systemd jl-parcours.service) auf 127.0.0.1:3065,
# Apache: https://minime.love66.app/p1/ -> 127.0.0.1:3065/. Konfiguration: /etc/jl-parcours.env
# ---------------------------------------------------------------------------
import asyncio
import hmac
import json
import os
import re
import secrets
import shutil
import subprocess
import threading
import time
import urllib.request
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer

import edge_tts
import miniaudio
import numpy as np

PORT = int(os.environ.get("PORT", "3065"))
TOKEN = os.environ.get("P1_TOKEN", "")
MINIME_URL = os.environ.get("MINIME_URL", "http://127.0.0.1:3060/stt")
MINIME_TOKEN = os.environ.get("MINIME_TOKEN", "")
BASE = os.environ.get("JL_CLAUDE_HOME", "/var/lib/jl-claude")
CLAUDE = os.environ.get("CLAUDE_BIN", os.path.join(BASE, ".local/bin/claude"))
ACCOUNT = os.environ.get("CLAUDE_ACCOUNT", "").strip()
REPO_URL = os.environ.get("P1_REPO", "git@github.com:JAbstractClass/parcours1-giga.git")
REPO = os.path.join(BASE, "parcours1")
DEPLOY_KEY = os.path.join(BASE, ".ssh/parcours1_deploy")
VOICE = os.environ.get("P1_VOICE", "de-DE-ConradNeural")
MINUTEN = int(os.environ.get("P1_MINUTEN", "8"))
MAX_AUDIO = 30 * 8000          # 30 s

if len(TOKEN) < 24:
    raise SystemExit("P1_TOKEN fehlt in /etc/jl-parcours.env")
if not re.fullmatch(r"[a-z0-9]+", ACCOUNT):
    raise SystemExit("CLAUDE_ACCOUNT fehlt")

GIT_ENV = dict(os.environ, HOME=BASE,
               GIT_SSH_COMMAND=f"ssh -i {DEPLOY_KEY} -o IdentitiesOnly=yes -o StrictHostKeyChecking=accept-new",
               GIT_AUTHOR_NAME="Claude (Sprachsteuerung)", GIT_AUTHOR_EMAIL="noreply@anthropic.com",
               GIT_COMMITTER_NAME="Claude (Sprachsteuerung)", GIT_COMMITTER_EMAIL="noreply@anthropic.com")

SYSTEM = """Du arbeitest per Sprachbefehl an "Parcours 1": einer fischertechnik-Kugelbahn, gesteuert von einem
Arduino Giga R1 WiFi (Ordner Parcours1/, Arduino-Sketch), mit Weboberflaeche (Parcours1/dashboard.html, daraus wird
Parcours1/Dashboard.h erzeugt mit: python3 tools/html2h.py) und einem M5Stack-Bedienpanel (Ordner M5Panel/).
Die README.md beschreibt Belegung und Bedienung. Lies sie, bevor du etwas aenderst.

Die Frage kommt aus einer Spracherkennung und kann Hoerfehler enthalten: deute sie sinnvoll.
Regeln:
- Aendere nur, was gefragt ist. Kleine, saubere Aenderungen im vorhandenen Stil (deutsche Kommentare ohne Umlaute im Code).
- Wenn du dashboard.html aenderst, fuehre danach python3 tools/html2h.py aus.
- Kompilieren kannst du hier nicht. Pruefe deine Aenderung deshalb besonders sorgfaeltig (Klammern, Typen, Deklarationsreihenfolge der .ino-Dateien).
- Jede Firmware muss Web.ino mit dem OTA-Teil behalten.
- Wenn du etwas geaendert hast: git add und git commit mit einer kurzen deutschen Nachricht. NICHT pushen.
- Bei reinen Fragen nichts aendern, nur antworten.

Du kannst die Anlage auch direkt BEDIENEN (ohne Code zu aendern). Schreib dafuer am Ende deiner Antwort pro Aktion
eine eigene Zeile "AKTION: <name>". Diese Zeilen werden nicht vorgelesen, sondern der Reihe nach ausgefuehrt.
Erlaubte Aktionen:
  aufzug_an, aufzug_aus, freigeben (eine Kugel durch die Stoppstelle), weiche1_looping, weiche1_vorbei,
  weiche2_spirale, weiche2_skurve, licht_an, licht_aus, servo_test, weihnacht_an, weihnacht_aus, notaus,
  warte_<sekunden> (z. B. warte_10, hoechstens 120),
  set_<einstellung>=<zahl> (z. B. set_aufzug_pwm=150, set_stopp_ms=200; Namen wie in WebEinstellungen.ino).
Den aktuellen Zustand der Anlage bekommst du mit (JSON wie /status). Fuer Bedienwuensche reicht AKTION - dafuer
keine Dateien lesen und nichts committen. Beispiel: "starte den Aufzug fuer zehn Sekunden" ->
  "Ich lasse den Aufzug zehn Sekunden laufen." + AKTION: aufzug_an + AKTION: warte_10 + AKTION: aufzug_aus
- Deine letzte Nachricht wird laut vorgelesen: 1 bis 3 kurze Saetze, gesprochene Sprache, kein Markdown, keine Dateipfade
  mit Sonderzeichen, keine Listen. Sag, was du gemacht hast und dass man am Panel mit Taste C uebernimmt, falls du etwas
  geaendert hast."""

jobs = {}
jobs_lock = threading.Lock()
claude_lock = threading.Lock()        # ein Claude zur Zeit (2 GB RAM)
git_lock = threading.Lock()


def git(*args, check=True):
    r = subprocess.run(["git", *args], cwd=REPO, env=GIT_ENV, capture_output=True, text=True, timeout=120)
    if check and r.returncode != 0:
        raise RuntimeError(f"git {' '.join(args)}: {(r.stderr or r.stdout).strip()[:300]}")
    return r.stdout.strip()


def repo_bereit():
    if not os.path.isdir(os.path.join(REPO, ".git")):
        subprocess.run(["git", "clone", REPO_URL, REPO], env=GIT_ENV, check=True, capture_output=True, timeout=300)
    git("fetch", "--prune", "origin")


# ---------- Sprache ----------
def stt(u8):
    """u8 8 kHz -> s16 16 kHz -> Whisper ueber jl-minime."""
    x = (np.frombuffer(u8, dtype=np.uint8).astype(np.float32) - 128.0) / 128.0
    x -= x.mean() if x.size else 0
    pk = float(np.percentile(np.abs(x), 99.5)) if x.size else 0
    if pk > 1e-3:
        x *= 0.8 / pk
    x16 = np.interp(np.arange(x.size * 2) / 2.0, np.arange(x.size), x) if x.size else x
    pcm = (np.clip(x16, -1, 1) * 32000).astype("<i2").tobytes()
    req = urllib.request.Request(MINIME_URL, data=pcm, method="POST",
                                 headers={"Content-Type": "application/octet-stream", "X-Minime-Token": MINIME_TOKEN})
    with urllib.request.urlopen(req, timeout=120) as r:
        return json.loads(r.read().decode()).get("text", "").strip()


def tts(text):
    async def run():
        mp3 = bytearray()
        async for ch in edge_tts.Communicate(text, VOICE, rate="+5%").stream():
            if ch["type"] == "audio":
                mp3 += ch["data"]
        return bytes(mp3)
    dec = miniaudio.decode(asyncio.run(run()), output_format=miniaudio.SampleFormat.SIGNED16, nchannels=1, sample_rate=8000)
    x = np.frombuffer(dec.samples, dtype=np.int16).astype(np.float32) / 32768.0
    if x.size == 0:
        return b""
    x *= 0.9 / (float(np.percentile(np.abs(x), 99.7)) or 1e-4)
    x = np.tanh(3.0 * x) / np.tanh(3.0)            # laut genug fuer den kleinen Lautsprecher
    return (np.clip(x, -1, 1) * 127 + 128).astype(np.uint8).tobytes()


def fuer_sprache(t):
    t = re.sub(r"[*_#`>|~]", "", t)
    t = re.sub(r"\[([^\]]*)\]\([^)]*\)", r"\1", t)
    t = re.sub(r"https?://\S+", "", t)
    return re.sub(r"\s+", " ", t).strip()


# ---------- Claude ----------
def schritt_text(block):
    """Kurzer deutscher Zwischenschritt aus einem tool_use-Block."""
    name, inp = block.get("name", ""), block.get("input", {}) or {}
    datei = os.path.basename(str(inp.get("file_path") or inp.get("path") or ""))
    if name == "Read":
        return f"liest {datei}"
    if name in ("Edit", "MultiEdit"):
        return f"aendert {datei}"
    if name == "Write":
        return f"schreibt {datei}"
    if name in ("Grep", "Glob"):
        return f"sucht {str(inp.get('pattern', ''))[:40]}"
    if name == "Bash":
        return "fuehrt aus: " + str(inp.get("command", ""))[:60]
    if name == "TodoWrite":
        return "plant die Schritte"
    return name


verlauf = []                          # letzte Fragen/Antworten fuer Rueckfragen ("und jetzt schneller")
letzter_job_mit_aenderung = None
AKTIONEN = re.compile(r"^(aufzug_an|aufzug_aus|freigeben|weiche1_looping|weiche1_vorbei|weiche2_spirale|weiche2_skurve|"
                      r"licht_an|licht_aus|servo_test|weihnacht_an|weihnacht_aus|notaus|warte_\d{1,3}|set_[a-z0-9_]{2,20}=-?\d{1,6})$")
JA = re.compile(r"^\W*(ja|jawohl|jo|ok|okay|uebernehmen|übernehmen|übernimm|uebernimm|mach das|passt|gerne)\b.{0,20}$", re.I)
NEIN = re.compile(r"^\W*(nein|nee|ne|verwerfen|verwirf|lass es|abbrechen|vergiss es)\b.{0,20}$", re.I)


def neuer_job(frage=None, audio=None, zustand=""):
    jid = secrets.token_hex(6)
    job = {"id": jid, "status": "hoert" if audio is not None else "denkt", "frage": frage or "", "schritte": [],
           "antwort": "", "aenderung": "", "aktionen": [], "fehler": "", "branch": "", "tts": None,
           "tts_ev": threading.Event(), "t": time.time(), "zustand": zustand}
    with jobs_lock:
        for k in [k for k, v in jobs.items() if time.time() - v["t"] > 3600]:
            del jobs[k]
        jobs[jid] = job
    threading.Thread(target=arbeiten, args=(job, audio), daemon=True).start()
    return jid


def schritt(job, text):
    job["schritte"].append(text[:120])


def arbeiten(job, audio):
    try:
        if audio is not None:
            schritt(job, "Spracherkennung ...")
            job["frage"] = stt(audio)
            if not job["frage"]:
                raise RuntimeError("Ich habe nichts verstanden.")
            schritt(job, "Verstanden: " + job["frage"])
        job["status"] = "denkt"
        global letzter_job_mit_aenderung
        offen = letzter_job_mit_aenderung if letzter_job_mit_aenderung and letzter_job_mit_aenderung["branch"] else None
        if offen and JA.match(job["frage"]):
            r = uebernehmen(offen)
            job["antwort"] = "Alles klar, ist übernommen. GitHub baut jetzt, und der Giga holt sich die neue Firmware in ein bis zwei Minuten." if r.get("ok") else r.get("fehler", "Das ging nicht.")
            schritt(job, "Aenderung von vorhin uebernommen" if r.get("ok") else "Uebernehmen ging nicht")
        elif offen and NEIN.match(job["frage"]):
            verwerfen(offen)
            job["antwort"] = "Okay, ich habe die Änderung verworfen."
            schritt(job, "Aenderung von vorhin verworfen")
        else:
            with claude_lock:
                claude_lauf(job)
            if job["branch"]:
                letzter_job_mit_aenderung = job
        verlauf.append((job["frage"], fuer_sprache(job["antwort"])[:300]))
        del verlauf[:-4]
        job["status"] = "fertig"
    except Exception as e:  # noqa: BLE001 - jede Stoerung wird angezeigt und vorgelesen
        job["fehler"] = str(e)[:300]
        job["antwort"] = job["antwort"] or f"Das hat nicht geklappt. {job['fehler']}"
        job["status"] = "fehler"
    finally:
        try:
            job["tts"] = tts(fuer_sprache(job["antwort"]))[: 8000 * 60]
        except Exception as e:  # noqa: BLE001
            print(f"tts: {e}", flush=True)
            job["tts"] = b""
        job["tts_ev"].set()
        print(f"job {job['id']} {job['status']}: {job['frage'][:60]!r} -> {job['antwort'][:80]!r}", flush=True)


def claude_lauf(job):
    with git_lock:
        repo_bereit()
        branch = f"stimme/{job['id']}"
        git("checkout", "-f", "-B", branch, "origin/main")
        git("clean", "-fdq")
    start = git("rev-parse", "HEAD")
    env = dict(GIT_ENV, USER="jlclaude", LOGNAME="jlclaude", SHELL="/bin/bash", LANG="C.UTF-8", TERM="dumb", NO_COLOR="1",
               PATH=f"{os.path.join(BASE, '.local/bin')}:/usr/local/bin:/usr/bin:/bin",
               CLAUDE_CONFIG_DIR=os.path.join(BASE, "accounts", ACCOUNT))
    teile = []
    if verlauf:
        teile.append("Bisheriges Gespraech:\n" + "\n".join(f"Frage: {f}\nDeine Antwort: {a}" for f, a in verlauf[-3:]))
    if job["zustand"]:
        teile.append("Aktueller Zustand der Anlage (JSON): " + job["zustand"][:1500])
    teile.append("Neue Sprachanweisung: " + job["frage"])
    args = [CLAUDE, "-p", "\n\n".join(teile), "--output-format", "stream-json", "--verbose",
            "--no-session-persistence", "--disable-slash-commands", "--append-system-prompt", SYSTEM,
            "--tools", "Read,Edit,Write,Grep,Glob,Bash,TodoWrite",
            "--allowedTools", "Read Edit Write Grep Glob TodoWrite Bash(git status:*) Bash(git diff:*) Bash(git log:*) "
                              "Bash(git add:*) Bash(git commit:*) Bash(python3 tools/html2h.py:*)",
            "--permission-mode", "acceptEdits"]
    schritt(job, "Claude startet ...")
    p = subprocess.Popen(args, cwd=REPO, env=env, stdout=subprocess.PIPE, stderr=subprocess.PIPE, text=True)
    timer = threading.Timer(MINUTEN * 60, p.kill)
    timer.start()
    ergebnis = ""
    try:
        for zeile in p.stdout:
            try:
                ev = json.loads(zeile)
            except ValueError:
                continue
            if ev.get("type") == "assistant":
                for b in ev.get("message", {}).get("content", []):
                    if b.get("type") == "tool_use":
                        schritt(job, schritt_text(b))
                    elif b.get("type") == "text" and b.get("text", "").strip():
                        schritt(job, "denkt: " + b["text"].strip().replace("\n", " ")[:100])
            elif ev.get("type") == "result":
                ergebnis = (ev.get("result") or "").strip()
                if ev.get("is_error"):
                    raise RuntimeError(ergebnis or ev.get("subtype") or "Claude-Fehler")
        p.wait()
    finally:
        timer.cancel()
    if not ergebnis:
        err = p.stderr.read()[-400:]
        if re.search(r"log.?in|auth|credential|401", err, re.I):
            raise RuntimeError("Claude ist auf dem Server nicht angemeldet.")
        if re.search(r"rate.?limit|usage limit|429|quota", err, re.I):
            raise RuntimeError("Das Claude-Limit ist gerade aufgebraucht.")
        raise RuntimeError("Claude hat keine Antwort geliefert." if p.returncode == 0 else f"Claude abgebrochen ({p.returncode})")
    akt, rest = [], []
    for z in ergebnis.splitlines():
        m = re.match(r"^\W*AKTION:\s*(\S+)\s*$", z.strip(), re.I)
        if m and AKTIONEN.match(m.group(1).lower()):
            akt.append(m.group(1).lower())
        elif not m:
            rest.append(z)
    job["aktionen"] = akt[:30]
    job["antwort"] = "\n".join(rest).strip() or ("Mache ich." if akt else "")
    if akt:
        schritt(job, "Aktionen: " + ", ".join(akt))
    with git_lock:
        # nicht committete Reste mitnehmen, damit nichts verloren geht
        if git("status", "--porcelain"):
            git("add", "-A")
            git("commit", "-q", "-m", "Sprachbefehl: " + job["frage"][:60])
        if git("rev-parse", "HEAD") != start:
            stat = git("diff", "--stat", "origin/main", "HEAD")
            git("push", "-q", "-f", "origin", f"HEAD:refs/heads/stimme/{job['id']}")
            job["branch"] = f"stimme/{job['id']}"
            job["aenderung"] = stat.splitlines()[-1].strip() if stat else "geaendert"
            schritt(job, "Aenderung bereit: " + job["aenderung"])


def uebernehmen(job):
    if not job["branch"]:
        return {"fehler": "keine Aenderung"}
    with git_lock:
        git("fetch", "origin")
        try:
            git("push", "origin", f"origin/{job['branch']}:refs/heads/main")      # nur vorspulen, nie ueberschreiben
        except RuntimeError:
            return {"fehler": "main hat sich inzwischen geaendert - bitte den Befehl neu sprechen"}
        git("push", "-q", "origin", f":refs/heads/{job['branch']}", check=False)
    job["branch"] = ""
    schritt(job, "Uebernommen: GitHub baut, der Giga holt sich die neue Firmware")
    return {"ok": True}


def verwerfen(job):
    if job["branch"]:
        with git_lock:
            git("push", "-q", "origin", f":refs/heads/{job['branch']}", check=False)
        job["branch"] = ""
        schritt(job, "Verworfen")
    return {"ok": True}


# ---------- HTTP ----------
class Handler(BaseHTTPRequestHandler):
    protocol_version = "HTTP/1.1"
    server_version = "jl-parcours"

    def log_message(self, fmt, *args):
        if "/job/" not in (args[0] if args else ""):
            print(f"{self.address_string()} {fmt % args}", flush=True)

    def send_json(self, code, obj):
        body = json.dumps(obj, ensure_ascii=False).encode()
        self.send_response(code)
        self.send_header("Content-Type", "application/json; charset=utf-8")
        self.send_header("Content-Length", str(len(body)))
        self.end_headers()
        self.wfile.write(body)

    def authed(self):
        if hmac.compare_digest(self.headers.get("X-P1-Token", ""), TOKEN):
            return True
        self.send_json(401, {"fehler": "unauthorized"})
        return False

    def body(self, limit):
        if "chunked" in (self.headers.get("Transfer-Encoding") or "").lower():
            out = bytearray()
            while True:
                n = int(self.rfile.readline().split(b";")[0].strip() or b"0", 16)
                if n == 0:
                    self.rfile.readline()
                    break
                out += self.rfile.read(n)
                self.rfile.readline()
                if len(out) > limit:
                    return None
            return bytes(out)
        n = int(self.headers.get("Content-Length") or 0)
        return self.rfile.read(n) if 0 < n <= limit else None

    def job(self, jid):
        with jobs_lock:
            return jobs.get(jid)

    def do_GET(self):
        if self.path == "/health":
            return self.send_json(200, {"ok": True})
        m = re.fullmatch(r"/job/([a-f0-9]{12})(?:\?ab=(\d+))?", self.path)
        if m and (j := self.job(m.group(1))):
            ab = int(m.group(2) or 0)
            return self.send_json(200, {"status": j["status"], "frage": j["frage"], "schritte": j["schritte"][ab:],
                                        "n": len(j["schritte"]), "antwort": fuer_sprache(j["antwort"]) if j["status"] in ("fertig", "fehler") else "",
                                        "aenderung": j["aenderung"] if j["branch"] else "", "fehler": j["fehler"],
                                        "aktionen": j["aktionen"] if j["status"] in ("fertig", "fehler") else [],
                                        "tts": j["tts_ev"].is_set()})
        m = re.fullmatch(r"/tts/([a-f0-9]{12})", self.path)
        if m and (j := self.job(m.group(1))):
            j["tts_ev"].wait(120)
            pcm = j["tts"] or b""
            self.send_response(200)
            self.send_header("Content-Type", "audio/basic; rate=8000")
            self.send_header("Content-Length", str(len(pcm)))
            self.end_headers()
            self.wfile.write(pcm)
            return
        if not self.authed():
            return
        self.send_json(404, {"fehler": "nicht gefunden"})

    def do_POST(self):
        if not self.authed():
            return
        if self.path == "/stimme":
            audio = self.body(MAX_AUDIO)
            if not audio or len(audio) < 2400:
                return self.send_json(400, {"fehler": "Aufnahme zu kurz oder zu lang"})
            return self.send_json(200, {"id": neuer_job(audio=audio, zustand=self.headers.get("X-P1-Status", ""))})
        if self.path == "/text":
            b = self.body(16 * 1024)
            try:
                text = str(json.loads(b.decode()).get("text", "")).strip()[:2000] if b else ""
            except ValueError:
                text = ""
            if not text:
                return self.send_json(400, {"fehler": "text fehlt"})
            return self.send_json(200, {"id": neuer_job(frage=text, zustand=self.headers.get("X-P1-Status", ""))})
        m = re.fullmatch(r"/job/([a-f0-9]{12})/(ja|nein)", self.path)
        if m and (j := self.job(m.group(1))):
            self.body(1024)
            return self.send_json(200, uebernehmen(j) if m.group(2) == "ja" else verwerfen(j))
        self.send_json(404, {"fehler": "nicht gefunden"})


if __name__ == "__main__":
    threading.Thread(target=lambda: (git_lock.acquire(), repo_bereit(), git_lock.release()), daemon=True).start()
    print(f"jl-parcours auf 127.0.0.1:{PORT}, Konto {ACCOUNT}", flush=True)
    ThreadingHTTPServer(("127.0.0.1", PORT), Handler).serve_forever()
