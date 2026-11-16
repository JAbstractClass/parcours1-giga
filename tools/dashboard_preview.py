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
S = {"aufzug": False, "leds": True, "stopp": False, "weiche": 1, "hebel": 1,
     "version": "abc1234", "ip": "127.0.0.1", "datum": "Vorschau", "rssi": -58,
     "github": "abc1234", "auto": True, "ota": "", "ampel": "g", "notaus": False, "grund": "", "lampe": "a"}

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
            elif c == "s": S["stopp"] = True; S["_zu"] = time.time() + .3
            elif c == "l": S["leds"] = not S["leds"]
            elif c == "v": pass
            elif c == "x": S.update(aufzug=False, stopp=False, notaus=True, ampel="r", grund="Not-Aus")
            return self.send(json.dumps(status()))
        if u.path == "/lampe":
            S["lampe"] = parse_qs(u.query).get("f", ["a"])[0]; return self.send(json.dumps(status()))
        if u.path in ("/github", "/update"):
            S["ota"] = "Update laeuft ... (Vorschau)"; return self.send("ok", "text/plain")
        self.send_error(404)

ThreadingHTTPServer(("127.0.0.1", 8080), H).serve_forever()
