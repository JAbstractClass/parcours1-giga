#!/usr/bin/env python3
"""Vorschau des Dashboards ohne Giga: simuliert /status, /cmd, /github, /update.
Start: python3 tools/dashboard_preview.py  ->  http://localhost:8080
"""
import json, os, time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from urllib.parse import urlparse, parse_qs

HTML = os.path.join(os.path.dirname(__file__), "..", "Parcours1", "dashboard.html")
START = time.time()
S = {"aufzug": False, "leds": True, "stopp": False, "weiche": 1, "hebel": 1,
     "version": "abc1234", "ip": "127.0.0.1", "datum": "Vorschau", "rssi": -58,
     "github": "abc1234", "auto": True, "ota": ""}

def status():
    s = dict(S); s["uptime"] = int(time.time() - START)
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
        if u.path == "/status": return self.send(json.dumps(status()))
        if u.path == "/cmd":
            c = parse_qs(u.query).get("c", [""])[0]
            if c == "a": S["aufzug"] = not S["aufzug"]
            elif c == "w": S["weiche"] = 3 - S["weiche"]
            elif c == "h": S["hebel"] = 3 - S["hebel"]
            elif c == "s": S["stopp"] = not S["stopp"]
            elif c == "l": S["leds"] = not S["leds"]
            elif c == "x": S.update(aufzug=False, stopp=False)
            return self.send(json.dumps(status()))
        if u.path in ("/github", "/update"):
            S["ota"] = "Update laeuft ... (Vorschau)"; return self.send("ok", "text/plain")
        self.send_error(404)

ThreadingHTTPServer(("127.0.0.1", 8080), H).serve_forever()
