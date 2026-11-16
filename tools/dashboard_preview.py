#!/usr/bin/env python3
"""Vorschau des Dashboards: http://localhost:8080
  python3 tools/dashboard_preview.py               ohne Giga, simuliert /status, /cmd, /github, /update
  python3 tools/dashboard_preview.py 172.20.10.2   neue Seite, aber echte Anlage: Befehle gehen an den Giga
"""
import json, os, sys, time, urllib.request
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import urlparse, parse_qs

HTML = os.path.join(os.path.dirname(__file__), "..", "Parcours1", "dashboard.html")
GIGA = sys.argv[1] if len(sys.argv) > 1 else ""
START = time.time()
E = {"aufzug_pwm": 200, "aufzug_umk": 0, "weiche_pwm": 255, "weiche_ms": 750, "weiche_umk": 0, "hebel_pwm": 180,
     "hebel_ms": 350, "hebel_umk": 0, "stopp_zu": 60, "stopp_auf": 120, "stopp_ms": 180, "stopp_tempo": 0,
     "stopp_auto": 1, "stopp_takt": 8000, "led_blink": 1, "sensor_an": 1, "sensor_inv": 0, "stopp_nach": 120, "laut": 100, "boost": 5}
E0 = dict(E)
def einst(): return json.dumps({**{k: [v, E0[k]] for k, v in E.items()}, "gespeichert": True})
S = {"aufzug": False, "leds": True, "stopp": False, "weiche": 1, "hebel": 1,
     "version": "abc1234", "ip": "127.0.0.1", "datum": "Vorschau", "rssi": -58,
     "github": "abc1234", "auto": True, "ota": "", "ampel": "g", "notaus": False, "grund": "", "lampe": "a", "kugel": True, "kugeln": 0, "sensor": True, "pegel": "A6=1", "weihnacht": False, "lied": 0}

def status():
    if S.get("_zu") and time.time() > S["_zu"]: S["stopp"] = False; S.pop("_zu")
    s = {k: v for k, v in S.items() if k[0] != "_"}; s["uptime"] = int(time.time() - START)
    s["abrufe"] = s["uptime"] // 60 + 1; s["daten"] = s["abrufe"] * 8
    if S["aufzug"]:  # Stoppstelle taktet wie auf dem Giga
        s["stopp"] = int(time.time()) % 9 == 0
    return s

class H(BaseHTTPRequestHandler):
    def log_message(self, *a): pass
    def send(self, body, typ="application/json"):
        b = body.encode()
        self.send_response(200); self.send_header("Content-Type", typ)
        self.send_header("Content-Length", str(len(b))); self.end_headers(); self.wfile.write(b)
    def do_POST(self):  # Lied-Upload simulieren
        n = int(self.headers.get("Content-Length", 0)); self.rfile.read(n)
        q = parse_qs(urlparse(self.path).query); off = int(q.get("off", ["0"])[0])
        S["lied"] = (off + n) // 8000
        return self.send(json.dumps({"ok": True, "bytes": off + n}))
    def do_GET(self):
        u = urlparse(self.path)
        if u.path == "/":
            html = open(HTML, encoding="utf-8").read().replace("%OTA_URL%", "http://192.168.0.10:8000/Parcours1.ota")
            return self.send(html, "text/html; charset=utf-8")
        if GIGA:  # alles ausser der Seite selbst an den echten Giga weiterreichen
            try:
                with urllib.request.urlopen("http://" + GIGA + self.path, timeout=4) as r:
                    return self.send(r.read().decode(), r.headers.get("Content-Type", "text/plain"))
            except Exception as e:
                return self.send_error(502, str(e))
        if u.path == "/status": return self.send(json.dumps(status()))
        if u.path == "/cmd":
            c = parse_qs(u.query).get("c", [""])[0]
            if c == "a": S["aufzug"] = not S["aufzug"]; S.update(notaus=False, ampel="g", grund="")
            elif c == "w": S["weiche"] = 3 - S["weiche"]
            elif c == "h": S["hebel"] = 3 - S["hebel"]
            elif c == "s" and S["kugel"]: S["stopp"] = True; S["_zu"] = time.time() + .3; S["kugeln"] += 1
            elif c == "l": S["leds"] = not S["leds"]
            elif c == "v": pass
            elif c == "j": S["weihnacht"] = not S["weihnacht"] and S["lied"] > 0
            elif c == "x": S.update(aufzug=False, stopp=False, notaus=True, ampel="r", grund="Not-Aus")
            return self.send(json.dumps(status()))
        if u.path == "/einst":
            q = parse_qs(u.query)
            if q.get("reset"): E.update(E0)
            if q.get("k") and q["k"][0] in E: E[q["k"][0]] = int(q.get("v", ["0"])[0])
            return self.send(einst())
        if u.path == "/lampe":
            S["lampe"] = parse_qs(u.query).get("f", ["a"])[0]; return self.send(json.dumps(status()))
        if u.path == "/lied":
            if parse_qs(u.query).get("loeschen"): S["lied"] = 0; S["weihnacht"] = False
            return self.send(json.dumps({"ok": True, "bytes": S["lied"] * 8000}))
        if u.path in ("/github", "/update"):
            S["ota"] = "Update laeuft ... (Vorschau)"; return self.send("ok", "text/plain")
        self.send_error(404)

ThreadingHTTPServer(("127.0.0.1", 8080), H).serve_forever()
