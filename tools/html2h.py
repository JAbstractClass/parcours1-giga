#!/usr/bin/env python3
"""Erzeugt Parcours1/Dashboard.h aus Parcours1/dashboard.html (Rohstring fuer die Firmware).
Wird von flash.sh, ota.sh und GitHub Actions vor dem Kompilieren aufgerufen."""
import os
basis = os.path.join(os.path.dirname(os.path.abspath(__file__)), "..", "Parcours1")
html = open(os.path.join(basis, "dashboard.html"), encoding="utf-8").read()
ende = ')DASH"'
if ende in html:
    raise SystemExit("dashboard.html enthaelt " + ende)
h = ("#pragma once\n// AUTOMATISCH ERZEUGT aus dashboard.html (tools/html2h.py) - nicht von Hand aendern.\n"
     "static const char HTML_SEITE[] = R\"DASH(" + html + ")DASH\";\n")
ziel = os.path.join(basis, "Dashboard.h")
if not os.path.exists(ziel) or open(ziel, encoding="utf-8").read() != h:
    open(ziel, "w", encoding="utf-8").write(h)
print("Dashboard.h:", len(html.encode()), "Bytes")
