#pragma once
// AUTOMATISCH ERZEUGT aus dashboard.html (tools/html2h.py) - nicht von Hand aendern.
static const char HTML_SEITE[] = R"DASH(<!doctype html>
<html lang="de">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover,user-scalable=no">
<meta name="theme-color" content="#0d0d0e">
<title>Parcours 1</title>
<script type="importmap">{"imports":{"three":"https://cdn.jsdelivr.net/npm/three@0.160.0/build/three.module.js","three/addons/":"https://cdn.jsdelivr.net/npm/three@0.160.0/examples/jsm/"}}</script>
<style>
/* Ein Bildschirm: oben die Bahn in 3D zum Anfassen, unten die Schalter in Fahrtrichtung der Kugel.
   Technik (Firmware, Log, Verlauf) steckt in der Systemansicht. Akzent = fischertechnik-Rot. */
:root{
  --bg:#0d0d0e;--panel:#151517;--tile:#1c1c1f;--tile2:#26262a;--line:rgba(255,255,255,.09);
  --ink:#f5f5f3;--muted:#9c9c97;--faint:#66665f;--red:#e30613;--ok:#3ddc84;
  --amb:#3ddc84;--amb2:rgba(61,220,132,.18);
  --mono:"SF Mono",ui-monospace,SFMono-Regular,Menlo,Consolas,monospace;
  --sans:-apple-system,BlinkMacSystemFont,"SF Pro Display","Helvetica Neue",Helvetica,Arial,sans-serif;
  --ease:cubic-bezier(.16,1,.3,1);
}
*{box-sizing:border-box;margin:0;padding:0}
html,body{height:100%;background:var(--bg);overflow:hidden;overscroll-behavior:none}
body.amp-o{--amb:#ff9f0a;--amb2:rgba(255,159,10,.2)}
body.amp-r{--amb:#ff3b30;--amb2:rgba(255,59,48,.24)}
body{color:var(--ink);font:15px/1.4 var(--sans);-webkit-font-smoothing:antialiased;display:flex;flex-direction:column;height:100vh;height:100dvh;user-select:none;-webkit-user-select:none}
button{font:inherit;color:inherit;background:none;border:0;cursor:pointer;-webkit-tap-highlight-color:transparent}
button:focus-visible{outline:2px solid #fff;outline-offset:2px}
.lbl{font:500 11px var(--mono);letter-spacing:.12em;text-transform:uppercase;color:var(--muted)}

/* ---------- Buehne ---------- */
#stage{position:relative;flex:1;min-height:0;overflow:hidden}
#stage::before{content:"";position:absolute;inset:-20% -10% 0;z-index:1;pointer-events:none;background:radial-gradient(60% 55% at 55% 62%,var(--amb2),transparent 70%);transition:background 1.2s;mix-blend-mode:screen}
.vig{position:absolute;inset:0;z-index:1;pointer-events:none;background:radial-gradient(120% 90% at 50% 45%,transparent 55%,rgba(0,0,0,.55))}
#gl{position:absolute;inset:0;width:100%;height:100%;display:block;touch-action:none;cursor:grab}
#gl:active{cursor:grabbing}
#load{position:absolute;inset:0;display:grid;place-items:center;text-align:center;padding:40px;color:var(--muted);font:13px var(--mono);pointer-events:none}
#stage::after{content:"";position:absolute;z-index:1;inset:auto 0 0;height:90px;background:linear-gradient(transparent,var(--bg));pointer-events:none}
.top{position:absolute;inset:0 0 auto;z-index:3;display:flex;justify-content:space-between;align-items:flex-start;gap:16px;padding:calc(22px + env(safe-area-inset-top)) 28px 0;pointer-events:none}
.top>*{pointer-events:auto}
.brand{display:none;align-items:center;gap:10px;height:32px;padding:0 14px 0 11px;border-radius:16px;background:rgba(255,255,255,.05);border:1px solid var(--line);backdrop-filter:blur(12px);-webkit-backdrop-filter:blur(12px);color:var(--ink);transition:border-color 1s}
.brand i{position:relative;width:9px;height:9px;border-radius:50%;background:var(--faint);transition:background .6s}
body.live .brand i{background:var(--amb)}
body.live .brand i::after{content:"";position:absolute;inset:-5px;border-radius:50%;border:2px solid var(--amb);opacity:0;animation:ping 2.4s var(--ease) infinite}
@keyframes ping{0%{transform:scale(.4);opacity:.9}100%{transform:scale(1.6);opacity:0}}
body.lost .brand i{background:var(--red)}
body.amp-r .brand{border-color:rgba(255,59,48,.5)}
body.lost .brand,body.amp-o .brand{display:inline-flex;margin-bottom:16px}
h1{font:800 clamp(48px,8vw,118px)/.92 var(--sans);letter-spacing:-.055em;margin-top:0;padding-bottom:.06em;background:linear-gradient(180deg,#fff 30%,rgba(255,255,255,.55));-webkit-background-clip:text;background-clip:text;color:transparent;overflow:hidden;pointer-events:none}
h1 b{display:block;font-weight:inherit;transition:transform .7s var(--ease)}
h1.swap b{transform:translateY(105%)}
body.run h1{background-image:linear-gradient(180deg,#ff5a4f 20%,#e30613)}
body.amp-r h1{background-image:linear-gradient(180deg,#ff6b61 20%,#c8000c)}
#sub{margin-top:10px;max-width:400px;color:var(--muted);font-size:15px;pointer-events:none}
.pill{height:38px;padding:0 15px;border-radius:19px;background:rgba(28,28,31,.72);backdrop-filter:blur(14px);-webkit-backdrop-filter:blur(14px);border:1px solid var(--line);display:inline-flex;align-items:center;gap:9px;font:600 13px var(--sans);transition:background .2s}
.pill:hover{background:var(--tile2)}
.pill u{width:7px;height:7px;border-radius:50%;background:var(--red);display:none}
.pill.alert u{display:block}
.view{position:absolute;right:28px;bottom:22px;z-index:3;display:flex;padding:3px;border-radius:19px;background:rgba(28,28,31,.72);backdrop-filter:blur(14px);-webkit-backdrop-filter:blur(14px);border:1px solid var(--line)}
.view button{height:30px;padding:0 13px;border-radius:15px;font:600 12px var(--sans);color:var(--muted);transition:background .25s,color .25s}
.view button.on{background:var(--ink);color:#111}
.hint{position:absolute;left:28px;bottom:30px;z-index:3;pointer-events:none;transition:opacity .6s}
#stage.used .hint{opacity:0}

/* Schaltpunkte direkt am Modell */
.hs{position:absolute;left:0;top:0;z-index:2;display:flex;align-items:center;gap:8px;margin:-16px 0 0 -16px;padding:5px 12px 5px 5px;border-radius:17px;background:rgba(13,13,14,.66);backdrop-filter:blur(10px);-webkit-backdrop-filter:blur(10px);border:1px solid var(--line);white-space:nowrap;opacity:0;visibility:hidden;transition:opacity .5s,background .2s,border-color .2s;will-change:transform}
#stage.ready .hs{opacity:1;visibility:visible}
.hs:hover,.hs.hot{background:var(--ink);color:#111;border-color:transparent}
.hs i{width:22px;height:22px;border-radius:50%;display:grid;place-items:center;background:rgba(255,255,255,.12)}
.hs i::after{content:"";width:8px;height:8px;border-radius:50%;background:var(--muted);transition:background .3s}
.hs.on i{background:rgba(227,6,19,.28)}
.hs.on i::after{background:var(--red)}
#hs_l.on i{background:var(--amb2);animation:none}#hs_l.on i::after{background:var(--amb)}
.hs.on i{animation:pulse 2s infinite}
@keyframes pulse{0%{box-shadow:0 0 0 0 rgba(227,6,19,.5)}100%{box-shadow:0 0 0 12px rgba(227,6,19,0)}}
.hs b{font:600 12px var(--sans)}
.hs em{font:500 12px var(--sans);font-style:normal;opacity:.6;margin-left:5px}

/* ---------- Schalterleiste ---------- */
#dock{position:relative;z-index:4;display:flex;gap:10px;align-items:stretch;padding:10px;margin:0 auto calc(16px + env(safe-area-inset-bottom));max-width:1320px;width:calc(100% - 32px);border-radius:28px;background:linear-gradient(180deg,rgba(255,255,255,.07),rgba(255,255,255,.025));border:1px solid rgba(255,255,255,.1);box-shadow:0 30px 80px rgba(0,0,0,.6),inset 0 1px 0 rgba(255,255,255,.08);backdrop-filter:blur(24px) saturate(1.4);-webkit-backdrop-filter:blur(24px) saturate(1.4)}
.go{flex:0 0 232px;min-height:84px;border-radius:20px;background:linear-gradient(160deg,#ff3b30,#d10010);position:relative;overflow:hidden;color:#fff;display:flex;align-items:center;gap:14px;padding:0 22px;text-align:left;transition:background .3s,transform .15s,box-shadow .3s;box-shadow:0 10px 34px rgba(227,6,19,.34)}
.go:active{transform:scale(.97)}
.go::after{content:"";position:absolute;inset:0;background:linear-gradient(110deg,transparent 35%,rgba(255,255,255,.22) 50%,transparent 65%);transform:translateX(-100%);transition:transform .9s var(--ease)}
.go:hover::after{transform:translateX(100%)}
.go svg{width:30px;height:30px;fill:currentColor;flex:none}
.go b{display:block;font:750 23px/1.1 var(--sans);letter-spacing:-.02em}
.go small{display:block;font:500 12px var(--sans);opacity:.75;margin-top:3px}
body.run .go{background:linear-gradient(160deg,#fff,#d9d9d4);color:#111;box-shadow:0 10px 30px rgba(255,255,255,.08)}
.tile{flex:1;min-width:0;border-radius:20px;background:rgba(0,0,0,.28);border:1px solid var(--line);padding:12px 14px 14px;display:flex;flex-direction:column;justify-content:space-between;gap:10px;transition:border-color .25s}
.tile.hot{border-color:rgba(255,255,255,.4)}
.th{display:flex;justify-content:space-between;align-items:baseline;gap:8px;white-space:nowrap}
.th span:last-child{font:500 12px var(--sans);color:var(--muted);overflow:hidden;text-overflow:ellipsis}
.th b{font:650 14px var(--sans)}
.seg{display:flex;padding:3px;border-radius:12px;background:rgba(0,0,0,.4);position:relative}
.seg button{flex:1;height:34px;border-radius:9px;font:600 13px var(--sans);color:var(--muted);transition:background .3s,color .3s}
.seg button.on{background:var(--ink);color:#111;box-shadow:0 4px 14px rgba(0,0,0,.4)}
.btn{height:40px;border-radius:12px;background:var(--tile2);font:600 13px var(--sans);position:relative;overflow:hidden;transition:background .2s,transform .15s}
.btn:hover{background:#323237}
.btn:active{transform:scale(.97)}
.btn s{position:absolute;inset:0 auto 0 0;width:0;background:rgba(255,255,255,.1);pointer-events:none}
.btn span{position:relative}
.btn.on{background:var(--red)}
.sw{align-self:flex-start;width:64px;height:40px;border-radius:20px;background:var(--tile2);position:relative;transition:background .3s}
.sw::after{content:"";position:absolute;top:4px;left:4px;width:32px;height:32px;border-radius:50%;background:#fff;transition:transform .45s var(--ease)}
.sw.on{background:var(--amb);box-shadow:0 0 18px var(--amb2)}
.sw.on::after{transform:translateX(24px)}
.narrow{flex:.55}
.stop{flex:0 0 84px;border-radius:20px;border:1px solid rgba(227,6,19,.55);color:#ff5b55;font:650 12px/1.25 var(--sans);display:flex;flex-direction:column;align-items:center;justify-content:center;gap:7px;transition:background .2s,color .2s}
.stop:hover{background:var(--red);color:#fff}
.stop svg{width:20px;height:20px;fill:none;stroke:currentColor;stroke-width:2.2}

/* ---------- Systemansicht ---------- */
#shade{position:fixed;inset:0;z-index:9;background:rgba(0,0,0,.5);opacity:0;visibility:hidden;transition:opacity .35s,visibility .35s}
#sys{position:fixed;z-index:10;top:0;right:0;bottom:0;width:min(440px,100%);background:var(--panel);border-left:1px solid var(--line);transform:translateX(102%);transition:transform .55s var(--ease);overflow:auto;-webkit-overflow-scrolling:touch;padding:22px 22px calc(30px + env(safe-area-inset-bottom));user-select:text;-webkit-user-select:text}
body.sys #sys{transform:none}
#stopbar{display:none;align-items:center;gap:14px;margin-top:16px;pointer-events:auto;padding:10px 12px 10px 16px;border-radius:16px;background:rgba(255,59,48,.14);border:1px solid rgba(255,59,48,.45);max-width:420px;font:600 14px var(--sans)}
body.amp-r #stopbar{display:flex}
#stopbar button{margin-left:auto;height:34px;padding:0 14px;border-radius:11px;background:#ff3b30;color:#fff;font:650 13px var(--sans)}
body.sys #shade{opacity:1;visibility:visible}
.sh{display:flex;justify-content:space-between;align-items:center;margin-bottom:20px}
.sh h2{font:750 26px var(--sans);letter-spacing:-.03em}
.sh button{width:38px;height:38px;border-radius:50%;background:var(--tile);font-size:20px;line-height:1}
#sys h3{margin:26px 0 10px}
.grid{display:grid;grid-template-columns:1fr 1fr;gap:8px}
.card{background:var(--tile);border-radius:16px;padding:14px}
.card .v{font:700 24px/1.15 var(--sans);letter-spacing:-.02em;margin-top:8px;font-variant-numeric:tabular-nums}
.card .s{font:12px var(--sans);color:var(--muted);margin-top:3px}
.bars{display:inline-flex;gap:3px;align-items:flex-end;height:14px;margin-left:8px;vertical-align:baseline}
.bars i{width:4px;border-radius:1px;background:var(--tile2)}
.bars i.on{background:var(--ink)}
.fwst{display:flex;align-items:center;gap:10px;font:650 16px var(--sans)}
.fwst i{width:9px;height:9px;border-radius:50%;background:var(--ok)}
.fwst.warn i{background:#ffb400}
.kv{margin-top:12px}
.kv div{display:flex;justify-content:space-between;gap:12px;padding:8px 0;border-top:1px solid var(--line);font:13px var(--mono)}
.kv span:first-child{color:var(--muted)}
.kv span:last-child{overflow:hidden;text-overflow:ellipsis;white-space:nowrap}
#otamsg{font:13px var(--sans);color:#ffb400;margin-top:10px}
#otamsg:empty{display:none}
.wide{width:100%;margin-top:12px}
details{margin-top:12px}
details summary{font:13px var(--sans);color:var(--muted);cursor:pointer}
details input{width:100%;height:40px;margin-top:10px;padding:0 12px;border-radius:10px;background:var(--bg);border:1px solid var(--line);font:12px var(--mono);color:var(--ink)}
#scope{display:block;width:100%;height:190px}
.log{font:12px/1.9 var(--mono);max-height:240px;overflow:auto}
.log div{display:flex;gap:14px;white-space:nowrap}
.log span:first-child{color:var(--faint)}
.keys{color:var(--muted);font-size:13px;line-height:1.9}
kbd{font:600 11px var(--mono);background:var(--tile);border:1px solid var(--line);border-radius:5px;padding:2px 6px;margin-right:3px;color:var(--ink)}

#toast{position:fixed;left:50%;top:calc(18px + env(safe-area-inset-top));z-index:20;background:var(--ink);color:#111;font:600 13px var(--sans);padding:11px 18px;border-radius:20px;opacity:0;visibility:hidden;transform:translate(-50%,-14px);transition:opacity .3s,visibility .3s,transform .5s var(--ease)}
#toast.show{opacity:1;visibility:visible;transform:translate(-50%,0)}

@media (max-width:1040px){
  #dock{display:grid;grid-template-columns:repeat(6,1fr);gap:8px;padding:8px;width:calc(100% - 16px);margin-bottom:calc(8px + env(safe-area-inset-bottom));border-radius:22px}
  .go{grid-column:span 5;min-height:68px}.stop{grid-column:span 1;order:1}
  .tile{grid-column:span 3;order:2}
  .top{padding:calc(16px + env(safe-area-inset-top)) 16px 0}
  .view{right:16px;bottom:14px}.hint{display:none}
  #sub{font-size:14px;max-width:270px}
}
@media (max-width:640px){
  .hs{padding:5px;margin:-17px 0 0 -17px}.hs span{display:none}
  .tile{padding:10px 10px 11px;border-radius:16px;gap:8px}.go,.stop{border-radius:16px}
  .th span:last-child{display:none}.tile.t-s .th span:last-child{display:block}
  #sub{max-width:46vw}
}
@media (hover:none){.kb{display:none}}
@media (max-height:520px){h1{font-size:40px}#sub{display:none}}
@media (prefers-reduced-motion:reduce){*{transition:none!important;animation:none!important}}
</style>
</head>
<body>
<div id="stage">
 <canvas id="gl"></canvas>
 <div id="load">Lade 3D-Modell …</div>
 <div class="vig"></div>
 <div class="top">
  <div>
   <div class="brand lbl"><i></i><span id="conn">verbinde</span></div>
   <h1 id="hl"><b id="hlt">Hallo.</b></h1>
   <p id="sub">Verbinde mit dem Giga …</p>
   <div id="stopbar"><span id="stopwhy">Not-Aus aktiv</span><button data-c="a">Freigeben & starten</button></div>
  </div>
  <button class="pill" id="sysbtn"><u></u>System</button>
 </div>
 <button class="hs" id="hs_a" data-c="a"><i></i><span><b>Aufzug</b><em></em></span></button>
 <button class="hs" id="hs_s" data-c="s"><i></i><span><b>Stoppstelle</b><em></em></span></button>
 <button class="hs" id="hs_w" data-c="w"><i></i><span><b>Weiche 1</b><em></em></span></button>
 <button class="hs" id="hs_h" data-c="h"><i></i><span><b>Weiche 2</b><em></em></span></button>
 <button class="hs" id="hs_l" data-c="l"><i></i><span><b>Licht</b><em></em></span></button>
 <div class="hint lbl">Ziehen dreht · Teile antippen schaltet</div>
 <div class="view" id="view"><button class="on" data-v="o">Übersicht</button><button data-v="f">Kugel folgen</button></div>
</div>

<div id="dock">
 <button class="go" data-c="a"><svg viewBox="0 0 24 24"><path id="go_i" d="M7 4l14 8-14 8z"/></svg><span><b id="go_t">Starten</b><small><span id="go_s">Aufzug an</span><span class="kb"> · Leertaste</span></small></span></button>
 <div class="tile t-s" data-k="s"><div class="th"><b>Stoppstelle</b><span id="s_v">–</span></div><button class="btn" id="s_b" data-c="s"><s id="s_p"></s><span id="s_t">Kugel freigeben</span></button></div>
 <div class="tile" data-k="w"><div class="th"><b>Weiche 1</b><span>oben</span></div><div class="seg" id="seg_w" data-c="w" data-f="weiche"><button data-v="1">Looping</button><button data-v="2">Vorbei</button></div></div>
 <div class="tile" data-k="h"><div class="th"><b>Weiche 2</b><span>Mitte</span></div><div class="seg" id="seg_h" data-c="h" data-f="hebel"><button data-v="1">Spirale</button><button data-v="2">S-Kurve</button></div></div>
 <div class="tile narrow" data-k="l"><div class="th"><b>Licht</b><span id="l_v">–</span></div><button class="sw" id="l_b" data-c="l" aria-label="Licht"></button></div>
 <button class="stop" data-c="x" title="Alle Motoren aus (X)"><svg viewBox="0 0 24 24"><circle cx="12" cy="12" r="9"/><path d="M12 7v6"/></svg>Not-Aus</button>
</div>

<div id="shade"></div>
<aside id="sys">
 <div class="sh"><h2>System</h2><button id="sysx" aria-label="Schließen">×</button></div>
 <div class="grid">
  <div class="card"><div class="lbl">WLAN</div><div class="v"><span id="k_wl">–</span><span class="bars" id="k_bars"><i style="height:4px"></i><i style="height:7px"></i><i style="height:10px"></i><i style="height:14px"></i></span></div><div class="s" id="k_wls">–</div></div>
  <div class="card"><div class="lbl">Giga online</div><div class="v" id="k_up">–</div><div class="s">seit dem letzten Neustart</div></div>
  <div class="card"><div class="lbl">Aufzug gelaufen</div><div class="v" id="k_run">00:00</div><div class="s"><span id="k_sw">0</span> Schaltungen · seit Seitenaufruf</div></div>
  <div class="card"><div class="lbl">Datenvolumen</div><div class="v" id="k_dat">–</div><div class="s" id="k_dat_s">vom Giga verbraucht, geschätzt</div></div>
 </div>

 <h3 class="lbl">Firmware</h3>
 <div class="card">
  <div class="fwst" id="f_st"><i></i><span id="f_auto">–</span></div>
  <div class="kv">
   <div><span>Läuft</span><span id="f_ver">–</span></div>
   <div><span>GitHub</span><span id="f_gh">–</span></div>
   <div><span>Gebaut</span><span id="f_date">–</span></div>
   <div><span>Adresse</span><span id="f_ip">–</span></div>
  </div>
  <div id="otamsg"></div>
  <button class="btn wide" id="ghbtn"><span>Jetzt von GitHub laden</span></button>
  <details><summary>Von eigener Adresse laden</summary><input id="url" spellcheck="false" autocomplete="off"><button class="btn wide" id="urlbtn"><span>Update starten</span></button></details>
 </div>

 <h3 class="lbl">Stoppstelle</h3>
 <div class="card"><p class="keys" style="line-height:1.5">Fährt den Servo einmal ganz auf und zu (an SERVO 1 und SERVO 2). Bewegt sich nichts: Stecker drehen, Signal muss an ~2.</p><button class="btn wide" data-c="v"><span>Servo testen</span></button></div>

 <h3 class="lbl">Verlauf · letzte 2 Minuten</h3>
 <div class="card" style="padding:6px 0"><canvas id="scope"></canvas></div>

 <h3 class="lbl">Ereignisse</h3>
 <div class="card"><div class="log" id="log"></div></div>

 <h3 class="lbl">Tastatur</h3>
 <p class="keys"><kbd>Leertaste</kbd>Start / Stopp<br><kbd>S</kbd>Kugel freigeben <kbd>W</kbd>Weiche 1 <kbd>H</kbd>Weiche 2<br><kbd>L</kbd>Licht <kbd>F</kbd>Kugel folgen <kbd>X</kbd>Not-Aus</p>
</aside>
<div id="toast"></div>

<script>
/* ===================== Daten, Steuerung, Anzeige ===================== */
(()=>{
const $=id=>document.getElementById(id),B=document.body;
const DPR=()=>Math.min(devicePixelRatio||1,3);
const MONO=getComputedStyle(document.documentElement).getPropertyValue('--mono');
const P1=window.P1={S:null,events:[],cmd:null,view:'o'};
/* Takt der Stoppstelle: muss zu STOPP_INTERVALL_MS in Parcours1.ino passen */
const STOPP_MS=8000;
let lastOk=0,lat=0,runMs=0,runSince=null,swc=0,up=null,stT=0,ota='';

const dur=s=>{s=Math.max(0,Math.floor(s));const h=Math.floor(s/3600),m=Math.floor(s%3600/60),x=s%60;return(h?h+':':'')+String(m).padStart(2,'0')+':'+String(x).padStart(2,'0')};
let tt;function toast(m){const t=$('toast');t.textContent=m;t.classList.add('show');clearTimeout(tt);tt=setTimeout(()=>t.classList.remove('show'),2400);}
function log(m){const l=$('log');const d=document.createElement('div');d.innerHTML='<span>'+new Date().toLocaleTimeString('de-DE')+'</span><span></span>';d.lastChild.textContent=m;l.prepend(d);while(l.children.length>80)l.lastChild.remove();}
function head(t,sub){$('sub').textContent=sub;const h=$('hl'),b=$('hlt');if(b.textContent===t||h._t===t)return;h._t=t;h.classList.add('swap');setTimeout(()=>{b.textContent=t;h.classList.remove('swap');},380);}
const hsSet=(k,on,txt)=>{const e=$('hs_'+k);e.classList.toggle('on',on);e.querySelector('em').textContent=txt;};

/* Verlauf (nur gezeichnet, wenn die Systemansicht offen ist) */
const sc=$('scope'),sg=sc.getContext('2d'),WIN=120000;
const CH=[['Aufzug',s=>s.aufzug],['Stopp',s=>s.stopp],['Vorbei',s=>s.weiche===2],['S-Kurve',s=>s.hebel===2],['Licht',s=>s.leds]];
const samples=[];
function sSize(){sc.width=Math.round(sc.clientWidth*DPR());sc.height=Math.round(sc.clientHeight*DPR());}
function sDraw(now){
  const d=DPR(),w=sc.width,h=sc.height;sg.clearRect(0,0,w,h);
  const L=86*d,rh=h/CH.length,X=t=>L+(w-L-8*d)*(1-(now-t)/WIN);
  sg.font=(11*d)+'px '+MONO;
  CH.forEach(([n,f],ci)=>{
    const y0=ci*rh,hi=y0+rh*.28,lo=y0+rh*.72;
    sg.fillStyle='#9c9c97';sg.fillText(n,14*d,y0+rh/2+4*d);
    if(ci){sg.strokeStyle='rgba(255,255,255,.07)';sg.lineWidth=1;sg.beginPath();sg.moveTo(0,Math.round(y0)+.5);sg.lineTo(w,Math.round(y0)+.5);sg.stroke();}
    sg.strokeStyle=ci===0?'#e30613':'#f5f5f3';sg.lineWidth=2*d;sg.beginPath();let on=false,py=0;
    for(let i=0;i<samples.length;i++){const sm=samples[i],x0=X(sm.t),x1=i+1<samples.length?X(samples[i+1].t):X(now);if(x1<L)continue;if(!sm.s){on=false;continue;}
      const y=f(sm.s)?hi:lo,xa=Math.max(x0,L);if(!on){sg.moveTo(xa,y);on=true;}else if(y!==py){sg.lineTo(xa,py);sg.lineTo(xa,y);}sg.lineTo(x1,y);py=y;}
    sg.stroke();
  });
}
setInterval(()=>{const n=performance.now();samples.push({t:n,s:P1.S&&Date.now()-lastOk<4000?P1.S:null});while(samples.length&&n-samples[0].t>WIN+5000)samples.shift();if(B.classList.contains('sys'))sDraw(n);},250);
addEventListener('resize',sSize);

/* Anzeige */
const ROUTE=s=>[s.weiche===2?'am Looping vorbei':'durch den Looping',s.hebel===2?'über die S-Kurve':'durch die Spiralröhre'];
function render(s){
  const p=P1.S;P1.S=s;
  const amp=s.ampel||'g';B.classList.toggle('amp-o',amp==='o');B.classList.toggle('amp-r',amp==='r');P1.amp=amp;
  $('stopwhy').textContent=s.notaus?'Not-Aus aktiv · Motoren aus':'Fehler: '+(s.grund||'unbekannt');
  B.classList.toggle('run',s.aufzug);
  const r=ROUTE(s);
  if(s.notaus)head('Not-Aus.','Alle Motoren sind aus. Die Lampen blinken schnell, bis du die Anlage wieder startest.');
  else head(s.aufzug?'Läuft.':'Steht.',s.aufzug?'Die Kugeln fahren '+r[0]+' und '+r[1]+'.':'Bereit. Nächste Fahrt: '+r[0]+', dann '+r[1]+'.');
  $('go_t').textContent=s.aufzug?'Anhalten':'Starten';$('go_s').textContent=s.aufzug?'Aufzug läuft':'Aufzug an';
  $('go_i').setAttribute('d',s.aufzug?'M6 6h12v12H6z':'M7 4l14 8-14 8z');
  document.querySelectorAll('.seg').forEach(g=>[...g.children].forEach(b=>b.classList.toggle('on',+b.dataset.v===s[g.dataset.f])));
  $('l_b').classList.toggle('on',s.leds);$('l_v').textContent=!s.leds?'aus':amp==='g'?'an · ok':amp==='o'?'blinkt · offline':'blinkt · Fehler';
  $('s_b').classList.toggle('on',s.stopp);$('s_t').textContent=s.stopp?'Offen':'Kugel freigeben';
  hsSet('a',s.aufzug,s.aufzug?'läuft':'aus');hsSet('s',s.stopp,s.stopp?'offen':'zu');
  hsSet('w',false,s.weiche===2?'Vorbei':'Looping');hsSet('h',false,s.hebel===2?'S-Kurve':'Spirale');hsSet('l',s.leds,s.leds?({g:'ok',o:'offline',r:'Fehler'})[amp]:'aus');
  if(s.aufzug&&!runSince)runSince=Date.now();if(!s.aufzug&&runSince){runMs+=Date.now()-runSince;runSince=null;}
  up={v:s.uptime,at:Date.now()};
  const q=s.rssi>=-55?4:s.rssi>=-67?3:s.rssi>=-75?2:1;
  $('k_wl').textContent=['','Schwach','Mittel','Gut','Sehr gut'][q];[...$('k_bars').children].forEach((e,i)=>e.classList.toggle('on',i<q));
  $('k_wls').textContent=s.rssi+' dBm · Antwort in '+lat+' ms';
  if(s.daten!==undefined){const b=s.daten+s.abrufe*7000;$('k_dat').textContent=b<1e6?Math.round(b/1e3)+' KB':(b/1e6).toFixed(1)+' MB';if(s.uptime>60)$('k_dat_s').textContent='≈ '+(b/s.uptime*3.6/1e3).toFixed(1)+' MB pro Stunde';}
  $('f_ver').textContent=s.version;$('f_gh').textContent=s.github||'–';$('f_date').textContent=s.datum;$('f_ip').textContent=s.ip;
  const pend=s.auto&&s.github&&s.github!==s.version;
  $('f_auto').textContent=!s.auto?'Lokale Version · kein Auto-Update':pend?'Update gefunden · lädt gleich':'Aktuell · Auto-Update an';
  $('f_st').classList.toggle('warn',!s.auto||pend);
  ota=s.ota||'';$('otamsg').textContent=ota;$('sysbtn').classList.toggle('alert',!!(pend||ota));
  if(p){
    const ev=k=>P1.events.push(k);
    if(p.version!==s.version){log('Neue Firmware '+s.version);toast('Neue Firmware '+s.version);}
    if(p.aufzug!==s.aufzug){log(s.aufzug?'Aufzug gestartet':'Aufzug gestoppt');ev('a');stT=Date.now();}
    if(p.weiche!==s.weiche){log('Weiche 1 → '+(s.weiche===2?'Vorbei':'Looping'));swc++;ev('w');}
    if(p.hebel!==s.hebel){log('Weiche 2 → '+(s.hebel===2?'S-Kurve':'Spirale'));swc++;ev('h');}
    if(p.stopp!==s.stopp){log(s.stopp?'Stoppstelle offen':'Stoppstelle zu');swc++;ev('s');stT=Date.now();}
    if(p.leds!==s.leds){log(s.leds?'Licht an':'Licht aus');ev('l');}
    if(p.ampel!==s.ampel)log('Status: '+({g:'alles ok',o:'kein Internet',r:s.grund||'Fehler'})[amp]);
    if(p.ota!==s.ota&&s.ota)log(s.ota);
  }else log('Verbunden · '+s.ip);
  $('k_sw').textContent=swc;
  tick();
}
/* Stoppstelle: Countdown bis zur naechsten automatischen Freigabe */
function tick(){
  const s=P1.S;if(!s)return;let t='zu',w=0;
  if(s.stopp)t='Kugel rollt';
  else if(s.aufzug){if(stT){const left=STOPP_MS-(Date.now()-stT);w=Math.min(1,Math.max(0,1-left/STOPP_MS));t=left>900?'nächste in '+Math.ceil(left/1000)+' s':'gleich';}else t='automatisch';}
  $('s_v').textContent=t;$('s_p').style.width=(w*100).toFixed(1)+'%';
}
setInterval(tick,200);
setInterval(()=>{
  $('k_run').textContent=dur((runMs+(runSince?Date.now()-runSince:0))/1000);
  if(up)$('k_up').textContent=dur(up.v+(Date.now()-up.at)/1000);
  const ok=Date.now()-lastOk<4000;B.classList.toggle('live',ok);B.classList.toggle('lost',!ok&&!!lastOk);
  const a=P1.amp,S=P1.S;$('conn').textContent=!ok?(lastOk?'getrennt':'verbinde'):a==='r'?(S&&S.notaus?'Not-Aus':'Fehler'):a==='o'?'Live · kein Internet':'Live · alles ok';
  if(!ok&&lastOk){B.classList.remove('run');if(ota)head('Update.','Der Giga lädt neue Firmware und startet neu. Dauert etwa 30 Sekunden.');else head('Getrennt.','Giga nicht erreichbar. Bist du im Hotspot? Die Seite verbindet sich von selbst wieder.');}
  else if(!lastOk&&performance.now()>5000)head('Getrennt.','Giga nicht erreichbar. Bist du im Hotspot? Die Seite verbindet sich von selbst wieder.');
},1000);

/* Kommunikation */
async function get(u){const t=performance.now();const r=await fetch(u,{cache:'no-store'});const j=await r.json();lat=Math.round(performance.now()-t);lastOk=Date.now();return j;}
async function poll(){try{render(await get('/status'));}catch(e){}}
async function cmd(c){
  if(navigator.vibrate)navigator.vibrate(8);
  try{render(await get('/cmd?c='+c));if(c==='x')toast('Alle Motoren aus');}catch(e){toast('Giga nicht erreichbar');}
}
P1.cmd=cmd;
document.querySelectorAll('button[data-c]').forEach(el=>el.addEventListener('click',()=>cmd(el.dataset.c)));
/* Weichen: Ziel antippen statt blind umschalten */
document.querySelectorAll('.seg').forEach(g=>g.addEventListener('click',e=>{const b=e.target.closest('button');if(b&&P1.S&&P1.S[g.dataset.f]!==+b.dataset.v)cmd(g.dataset.c);}));
/* Leiste und Modell zeigen gegenseitig, was zusammengehoert */
document.querySelectorAll('.tile[data-k]').forEach(t=>{const h=$('hs_'+t.dataset.k);t.onpointerenter=()=>h.classList.add('hot');t.onpointerleave=()=>h.classList.remove('hot');h.onpointerenter=()=>t.classList.add('hot');h.onpointerleave=()=>t.classList.remove('hot');});
function view(v){P1.view=v;[...$('view').children].forEach(b=>b.classList.toggle('on',b.dataset.v===v));}
$('view').onclick=e=>{const b=e.target.closest('button');if(b)view(b.dataset.v);};
function sys(on){B.classList.toggle('sys',on);if(on){sSize();sDraw(performance.now());}}
$('sysbtn').onclick=()=>sys(true);$('sysx').onclick=$('shade').onclick=()=>sys(false);
addEventListener('keydown',e=>{
  if(e.key==='Escape')return sys(false);
  if(e.target.tagName==='INPUT'||e.metaKey||e.ctrlKey||e.altKey||e.repeat)return;
  const k=e.key.toLowerCase();
  if(k===' '){e.preventDefault();cmd('a');}
  else if(k==='f')view(P1.view==='f'?'o':'f');
  else if(k.length===1&&'awhslx'.includes(k))cmd(k);
});
$('ghbtn').onclick=async()=>{toast('Update von GitHub gestartet');log('Update von GitHub angefordert');try{await fetch('/github',{cache:'no-store'});}catch(e){}};
$('urlbtn').onclick=async()=>{const u=$('url').value.trim();if(!u)return;toast('Update gestartet');log('Update von '+u);try{await fetch('/update?url='+encodeURIComponent(u));}catch(e){}};
$('url').value='%OTA_URL%';

poll();setInterval(poll,1000);
})();
</script>

<script type="module">
/* ===================== 3D-Nachbau Parcours 1 (fischertechnik Dynamic XXL, Bauanleitung S. 48/49) ===================== */
const frame=document.getElementById('load');
let THREE,RoomEnvironment,EffectComposer,RenderPass,UnrealBloomPass,OutputPass;
try{
  THREE=await import('three');
  [{RoomEnvironment},{EffectComposer},{RenderPass},{UnrealBloomPass},{OutputPass}]=await Promise.all([
    import('three/addons/environments/RoomEnvironment.js'),
    import('three/addons/postprocessing/EffectComposer.js'),
    import('three/addons/postprocessing/RenderPass.js'),
    import('three/addons/postprocessing/UnrealBloomPass.js'),
    import('three/addons/postprocessing/OutputPass.js')]);
}catch(e){frame.innerHTML='Die 3D-Ansicht braucht Internet (three.js).<br>Die Schalter unten funktionieren trotzdem.';throw e;}
const P1=window.P1,canvas=document.getElementById('gl');
const R=new THREE.WebGLRenderer({canvas,antialias:true,powerPreference:'high-performance'});
R.setPixelRatio(Math.min(devicePixelRatio,2));
R.shadowMap.enabled=true;R.shadowMap.type=THREE.PCFSoftShadowMap;
R.toneMapping=THREE.ACESFilmicToneMapping;R.toneMappingExposure=1.0;R.outputColorSpace=THREE.SRGBColorSpace;
const scene=new THREE.Scene();scene.background=new THREE.Color(0x0d0d0e);scene.fog=new THREE.Fog(0x0d0d0e,75,150);
const pm=new THREE.PMREMGenerator(R);scene.environment=pm.fromScene(new RoomEnvironment(),.04).texture;
const cam=new THREE.PerspectiveCamera(30,1,.1,300);
const composer=new EffectComposer(R);composer.addPass(new RenderPass(scene,cam));
const bloom=new UnrealBloomPass(new THREE.Vector2(1,1),.35,.45,.97);composer.addPass(bloom);composer.addPass(new OutputPass());

const sun=new THREE.DirectionalLight(0xffffff,2.6);sun.position.set(10,26,14);sun.castShadow=true;
sun.shadow.mapSize.set(2048,2048);Object.assign(sun.shadow.camera,{left:-12,right:12,top:20,bottom:-6,near:1,far:70});sun.shadow.bias=-.0004;sun.shadow.normalBias=.02;scene.add(sun);
const rim=new THREE.DirectionalLight(0xbfd4ff,.7);rim.position.set(-14,10,-12);scene.add(rim);

/* ---------- Materialien (fischertechnik-Farben) ---------- */
function holeTex(){const c=document.createElement('canvas');c.width=32;c.height=64;const g=c.getContext('2d');g.fillStyle='#202023';g.fillRect(0,0,32,64);
  g.fillStyle='#0a0a0b';g.beginPath();g.roundRect(9,10,14,20,4);g.fill();g.beginPath();g.roundRect(9,40,14,16,4);g.fill();
  const t=new THREE.CanvasTexture(c);t.wrapS=t.wrapT=THREE.RepeatWrapping;t.colorSpace=THREE.SRGBColorSpace;t.anisotropy=8;return t;}
const HOLE=holeTex();
function stripeTex(){const c=document.createElement('canvas');c.width=8;c.height=64;const g=c.getContext('2d');g.fillStyle='#bdbdbd';g.fillRect(0,0,8,64);g.fillStyle='#0b0b0b';g.fillRect(0,0,8,30);
  const t=new THREE.CanvasTexture(c);t.wrapS=t.wrapT=THREE.RepeatWrapping;t.colorSpace=THREE.SRGBColorSpace;return t;}
const M={
  plate:new THREE.MeshStandardMaterial({color:0x2c2c30,roughness:.85}),
  stud:new THREE.MeshStandardMaterial({color:0x3a3a3f,roughness:.6}),
  black:new THREE.MeshStandardMaterial({color:0x18181a,roughness:.5}),
  red:new THREE.MeshStandardMaterial({color:0xd8261c,roughness:.38}),
  green:new THREE.MeshStandardMaterial({color:0x22b14c,roughness:.42}),
  greenDark:new THREE.MeshStandardMaterial({color:0x138a35,roughness:.5}),
  grey:new THREE.MeshStandardMaterial({color:0x9a9a96,roughness:.45,metalness:.3}),
  steel:new THREE.MeshStandardMaterial({color:0xd4d4d4,roughness:.18,metalness:1,envMapIntensity:1.3}),
  white:new THREE.MeshStandardMaterial({color:0xf4f4f4,roughness:.3})
};
const V=(x,y,z)=>new THREE.Vector3(x,y,z),UP=V(0,1,0);
const clickable=[];const tag=(o,c)=>{o.traverse(x=>x.userData.cmd=c);clickable.push(o);return o;};
const build=[];   /* Teile fuer das Aufbau-Intro: {obj, delay, kind} */

/* ---------- Grundplatte (2 Bauplatten) ---------- */
const PW=13,PD=8.6;
[-PW/4,PW/4].forEach((x,i)=>{const p=new THREE.Mesh(new THREE.BoxGeometry(PW/2-.06,.36,PD),M.plate);p.position.set(x,-.18,0);p.receiveShadow=true;scene.add(p);});
{const g=new THREE.CylinderGeometry(.1,.1,.06,10);const cx=Math.floor(PW/.5),cz=Math.floor(PD/.5);const im=new THREE.InstancedMesh(g,M.stud,cx*cz);const m=new THREE.Matrix4();let i=0;
 for(let x=0;x<cx;x++)for(let z=0;z<cz;z++){m.makeTranslation(-PW/2+.25+x*.5,.03,-PD/2+.25+z*.5);im.setMatrixAt(i++,m);}im.receiveShadow=true;scene.add(im);}

/* ---------- Statik-Geruest ---------- */
const joints=[];
function post(x,z,h,y0=0){const t=HOLE.clone();t.needsUpdate=true;t.repeat.set(1,h/.5);
  const m=new THREE.Mesh(new THREE.BoxGeometry(.32,h,.32),new THREE.MeshStandardMaterial({map:t,roughness:.55}));
  m.geometry.translate(0,h/2,0);m.position.set(x,y0,z);m.castShadow=true;m.receiveShadow=true;scene.add(m);build.push({obj:m,delay:y0*.05+Math.random()*.25,kind:'grow'});
  for(let y=y0;y<=y0+h+.01;y+=Math.max(1,h/Math.round(h/2.6)))joints.push(V(x,y,z));return m;}
function beam(a,b,thick=.22){const d=b.clone().sub(a),L=d.length();const t=HOLE.clone();t.needsUpdate=true;t.repeat.set(1,L/.5);
  const m=new THREE.Mesh(new THREE.BoxGeometry(thick,L,thick),new THREE.MeshStandardMaterial({map:t,roughness:.55}));
  m.position.copy(a).add(b).multiplyScalar(.5);m.quaternion.setFromUnitVectors(UP,d.normalize());m.castShadow=true;scene.add(m);
  build.push({obj:m,delay:.5+Math.min(a.y,b.y)*.07+Math.random()*.2,kind:'pop'});joints.push(a.clone(),b.clone());return m;}
function brace(a,b){const d=b.clone().sub(a),L=d.length();const m=new THREE.Mesh(new THREE.BoxGeometry(.1,L,.1),M.black);
  m.position.copy(a).add(b).multiplyScalar(.5);m.quaternion.setFromUnitVectors(UP,d.normalize());m.castShadow=true;scene.add(m);build.push({obj:m,delay:.8+Math.min(a.y,b.y)*.07,kind:'pop'});}
/* Hauptturm */
const TX0=-3,TX1=1,TZ0=-2,TZ1=2,TH=15.2;
[[TX0,TZ0],[TX1,TZ0],[TX0,TZ1],[TX1,TZ1]].forEach(([x,z])=>post(x,z,TH));
const LEVELS=[3.8,7.8,11.4,TH];
LEVELS.forEach(y=>{beam(V(TX0,y,TZ0),V(TX1,y,TZ0));beam(V(TX0,y,TZ1),V(TX1,y,TZ1));beam(V(TX0,y,TZ0),V(TX0,y,TZ1));beam(V(TX1,y,TZ0),V(TX1,y,TZ1));});
[0,...LEVELS].forEach((y,i,a)=>{if(i===a.length-1)return;const y1=a[i+1];
  brace(V(TX0,y,TZ1),V(TX1,y1,TZ1));brace(V(TX1,y,TZ1),V(TX0,y1,TZ1));brace(V(TX1,y,TZ0),V(TX1,y1,TZ1));brace(V(TX0,y,TZ0),V(TX1,y1,TZ0));});
/* innere Aufzugssaeulen */
post(-1.45,.15,TH+.6);post(-.55,.15,TH+.6);
/* Anbau rechts */
const AX=5.2;[[AX,TZ0],[AX,TZ1]].forEach(([x,z])=>post(x,z,9.6));
[3.8,9.6].forEach(y=>{beam(V(TX1,y,TZ0),V(AX,y,TZ0));beam(V(TX1,y,TZ1),V(AX,y,TZ1));beam(V(AX,y,TZ0),V(AX,y,TZ1));});
brace(V(TX1,0,TZ1),V(AX,3.8,TZ1));brace(V(AX,3.8,TZ1),V(TX1,9.6,TZ1));brace(V(AX,0,TZ0),V(AX,3.8,TZ1));
{const land=new THREE.Mesh(new THREE.BoxGeometry(2.6,.12,1.6),M.green);land.position.set(4.1,9.72,1.1);land.rotation.z=-.08;land.castShadow=true;scene.add(land);build.push({obj:land,delay:1.6,kind:'pop'});}
/* niedriger Rahmen links */
[[-5.9,1.2],[-5.9,3.4]].forEach(([x,z])=>post(x,z,2.2));beam(V(-5.9,2.2,1.2),V(-5.9,2.2,3.4));beam(V(-5.9,2.2,3.4),V(TX0,2.2,TZ1));
/* rote Verbinder an allen Knoten */
{const g=new THREE.BoxGeometry(.38,.38,.38);const im=new THREE.InstancedMesh(g,M.red,joints.length);const m=new THREE.Matrix4();
 joints.forEach((p,i)=>{m.makeTranslation(p.x,p.y,p.z);im.setMatrixAt(i,m);});im.castShadow=true;scene.add(im);build.push({obj:im,delay:1.2,kind:'pop'});}
/* Motoren (rote XS-Motoren am Fuss) */
[[-3.4,3.2],[3.7,3.0]].forEach(([x,z])=>{const mo=new THREE.Mesh(new THREE.BoxGeometry(1.4,.7,.8),M.red);mo.position.set(x,.35,z);mo.castShadow=true;scene.add(mo);build.push({obj:mo,delay:.2,kind:'pop'});});

/* ---------- Strecke (Wege der Kugel, nach S. 48/49) ---------- */
const LIFT_X=-1,LIFT_Z=.55;
const N={gate:V(4.9,14.55,1.0),lsw:V(1.6,12.45,2.6),m1:V(-3.8,10.9,1.4),wsw:V(4.4,9.1,.4),m2:V(1.0,5.2,2.6)};
function helix(c,r,y0,y1,turns,a0){const pts=[];const n=Math.round(turns*14);for(let i=0;i<=n;i++){const u=i/n,a=a0+u*turns*Math.PI*2;pts.push(V(c.x+Math.cos(a)*r,y0+(y1-y0)*u,c.z+Math.sin(a)*r));}return pts;}
const HC=V(3.7,0,1.9);
const PTS={
 lift:[V(LIFT_X,.65,LIFT_Z),V(LIFT_X,16.0,LIFT_Z)],
 top:[V(LIFT_X,16.0,LIFT_Z),V(-.1,16.15,1.0),V(1.7,15.8,1.9),V(4.1,15.45,1.7),V(5.5,15.1,.4),V(5.4,14.85,-.8),N.gate],
 g2w:[N.gate,V(4.2,13.9,2.0),V(3.0,13.0,2.6),N.lsw],
 w1:[N.lsw,V(.6,12.25,2.6),V(-.4,12.15,2.62),V(-1.3,13.05,2.72),V(-.4,13.95,2.84),V(.5,13.05,2.95),V(-.4,12.15,3.05),V(-1.7,11.8,3.0),V(-3.1,11.3,2.4),N.m1],
 w2:[N.lsw,V(.4,12.0,2.1),V(-1.6,11.5,2.1),V(-3.0,11.15,1.9),N.m1],
 mid:[N.m1,V(-4.1,10.65,0),V(-3.5,10.35,-1.7),V(-1,10.05,-2.7),V(2.5,9.7,-2.7),V(4.5,9.4,-1.3),N.wsw],
 h1:[N.wsw,V(4.6,9.0,1.0),...helix(HC,1.15,8.7,6.0,1.75,-.4),V(2.4,5.6,2.7),N.m2],
 h2:[N.wsw,V(3.6,8.85,2.6),V(.5,8.25,3.1),V(-3.2,7.45,2.7),V(-4.1,7.0,1.0),V(-3.2,6.5,-.7),V(-.8,6.0,.6),V(.1,5.6,2.1),N.m2],
 ret:[N.m2,V(2.8,4.7,2.9),V(4.0,4.2,1.4),V(3.4,3.7,-.7),V(1.0,3.15,-1.1),V(-1.6,2.6,1.6),V(-3.6,2.05,3.1),V(-5.3,1.6,3.3),V(-6.1,1.25,2.0),V(-5.4,.95,.6),V(-3.0,.75,LIFT_Z),V(LIFT_X,.65,LIFT_Z)]
};
const TUBE={h1:true};
const SEG={};
for(const k in PTS){
  const c=new THREE.CatmullRomCurve3(PTS[k],false,'centripetal',.5);const len=c.getLength();const n=Math.max(30,Math.round(len*14));
  /* Parallel-Transport-Rahmen: stabil auch im Looping */
  const P=[],T=[],S=[],Nn=[];let side=null;
  for(let i=0;i<=n;i++){const u=i/n,p=c.getPointAt(u),t=c.getTangentAt(u);
    if(!side){side=t.clone().cross(UP);if(side.lengthSq()<1e-4)side=V(0,0,1);side.normalize();}
    else{side.addScaledVector(t,-side.dot(t)).normalize();}
    const nn=side.clone().cross(t).normalize();if(k!=='lift'&&nn.y<0&&i===0)nn.negate();
    P.push(p);T.push(t);S.push(side.clone());Nn.push(nn);}
  SEG[k]={c,len,n,P,T,S,N:Nn};
}
function frameAt(seg,s){const g=SEG[seg],f=Math.max(0,Math.min(1,s/g.len))*g.n,i=Math.min(g.n-1,Math.floor(f)),u=f-i;
  return{p:g.P[i].clone().lerp(g.P[i+1],u),n:g.N[i].clone().lerp(g.N[i+1],u).normalize(),t:g.T[i]};}

/* Flexschienen: zwei Randholme + Sprossen (wie die gruenen fischertechnik-Flexschienen) */
const tracks=[];
const TIE_G=new THREE.BoxGeometry(.13,.06,.52);
/* Aeste hinter den Weichen bekommen eigene Materialien, damit der nicht befahrene Ast verblassen kann */
const BR={w1:[],w2:[],h1:[],h2:[]};const bm=(k,m)=>{if(!BR[k])return m;m=m.clone();m.transparent=true;BR[k].push(m);return m;};
for(const k in SEG){
  if(k==='lift')continue;const g=SEG[k];
  if(TUBE[k]){
    const tex=stripeTex();tex.repeat.set(1,g.len*3.2);
    const m=new THREE.Mesh(new THREE.TubeGeometry(g.c,g.n*2,.33,20,false),bm(k,new THREE.MeshStandardMaterial({map:tex,roughness:.35})));
    m.castShadow=true;scene.add(m);tracks.push({mesh:m,kind:'tube',total:m.geometry.index.count,delay:2.0});continue;}
  [-.24,.24].forEach(o=>{
    const pts=g.P.map((p,i)=>p.clone().addScaledVector(g.S[i],o).addScaledVector(g.N[i],.06));
    const m=new THREE.Mesh(new THREE.TubeGeometry(new THREE.CatmullRomCurve3(pts),g.n*2,.06,8,false),bm(k,M.green));m.castShadow=true;scene.add(m);
    tracks.push({mesh:m,kind:'tube',total:m.geometry.index.count,delay:1.6+Object.keys(SEG).indexOf(k)*.18});});
  const cnt=Math.floor(g.len/.2);const im=new THREE.InstancedMesh(TIE_G,bm(k,M.greenDark),cnt);const mt=new THREE.Matrix4(),q=new THREE.Quaternion(),basis=new THREE.Matrix4();
  for(let i=0;i<cnt;i++){const f=frameAt(k,(i+.5)*.2);const side=f.n.clone().cross(f.t).normalize();basis.makeBasis(f.t,f.n,side.negate());q.setFromRotationMatrix(basis);
    mt.compose(f.p.clone().addScaledVector(f.n,-.02),q,V(1,1,1));im.setMatrixAt(i,mt);}
  im.castShadow=true;scene.add(im);tracks.push({mesh:im,kind:'inst',total:cnt,delay:1.6+Object.keys(SEG).indexOf(k)*.18});
  /* rote Halter an den Stuetzpunkten */
  const steps=Math.floor(g.len/2.2);
  for(let i=1;i<=steps;i++){const f=frameAt(k,g.len*i/(steps+1));const h=new THREE.Mesh(new THREE.BoxGeometry(.36,.3,.7),M.red);h.position.copy(f.p).addScaledVector(f.n,-.2);
    h.quaternion.setFromUnitVectors(V(1,0,0),V(f.t.x,0,f.t.z).normalize());h.castShadow=true;scene.add(h);build.push({obj:h,delay:1.4+Math.random()*.4,kind:'pop'});}
}

/* ---------- Kettenaufzug ---------- */
const LIFT_TOP=16.0;
{const head=new THREE.Mesh(new THREE.BoxGeometry(1.5,.9,1.1),M.red);head.position.set(LIFT_X,LIFT_TOP+.9,.1);head.castShadow=true;scene.add(head);build.push({obj:head,delay:1.0,kind:'pop'});
 const wheel=new THREE.Mesh(new THREE.CylinderGeometry(.55,.55,.25,28),M.grey);wheel.rotation.x=Math.PI/2;wheel.position.set(LIFT_X,LIFT_TOP+.3,.15);scene.add(wheel);
 const wheel2=wheel.clone();wheel2.position.y=.55;scene.add(wheel2);}
const LINKS=80,CH_L=2*(LIFT_TOP-.5)+2;
const chain=new THREE.InstancedMesh(new THREE.BoxGeometry(.3,.12,.16),M.black,LINKS);chain.castShadow=true;scene.add(chain);
const carriers=new THREE.InstancedMesh(new THREE.BoxGeometry(.34,.1,.36),M.red,LINKS/8);scene.add(carriers);
const liftHit=new THREE.Mesh(new THREE.BoxGeometry(1.6,LIFT_TOP,1.2),new THREE.MeshBasicMaterial({visible:false}));liftHit.position.set(LIFT_X,LIFT_TOP/2,.2);scene.add(liftHit);tag(liftHit,'a');

/* ---------- Stoppstelle ---------- */
const gate=new THREE.Group();gate.position.copy(N.gate);scene.add(gate);
const gateArm=new THREE.Group();gate.add(gateArm);
{const servo=new THREE.Mesh(new THREE.BoxGeometry(.6,.45,.5),M.black);servo.position.set(0,-.55,-.7);gate.add(servo);
 const bar=new THREE.Mesh(new THREE.BoxGeometry(.12,.75,.12),M.red);bar.position.y=.2;bar.castShadow=true;gateArm.add(bar);
 const bar2=new THREE.Mesh(new THREE.BoxGeometry(.12,.12,.9),M.red);bar2.position.set(0,.55,-.35);gateArm.add(bar2);
 gateArm.position.set(0,0,0);build.push({obj:gate,delay:2.6,kind:'pop'});}
tag(gate,'s');
/* ---------- Weichen ---------- */
function mkSwitch(node,cmd){const g=new THREE.Group();g.position.copy(node);scene.add(g);
  const box=new THREE.Mesh(new THREE.BoxGeometry(1.2,.5,.8),M.red);box.position.set(0,-.55,0);box.castShadow=true;g.add(box);
  const motor=new THREE.Mesh(new THREE.BoxGeometry(.7,.45,.45),M.black);motor.position.set(.2,-.6,-.65);g.add(motor);
  const arm=new THREE.Group();arm.position.y=-.05;g.add(arm);
  const a=new THREE.Mesh(new THREE.BoxGeometry(1.0,.08,.16),M.red);a.position.x=.5;a.castShadow=true;arm.add(a);
  const hub=new THREE.Mesh(new THREE.CylinderGeometry(.13,.13,.2,16),M.grey);arm.add(hub);
  build.push({obj:g,delay:2.6,kind:'pop'});tag(g,cmd);return arm;}
const armW=mkSwitch(N.lsw,'w'),armH=mkSwitch(N.wsw,'h');
const dirAng=s=>{const t=SEG[s].T[2];return Math.atan2(-t.z,t.x);};
const A={w1:dirAng('w1'),w2:dirAng('w2'),h1:dirAng('h1'),h2:dirAng('h2')};
/* ---------- LEDs (weisse Bausteine oben auf dem Turm) ---------- */
/* Statusschein unter dem Modell */
const glowTex=(()=>{const c=document.createElement('canvas');c.width=c.height=128;const g=c.getContext('2d'),r=g.createRadialGradient(64,64,0,64,64,64);r.addColorStop(0,'#fff');r.addColorStop(.45,'rgba(255,255,255,.45)');r.addColorStop(1,'rgba(255,255,255,0)');g.fillStyle=r;g.fillRect(0,0,128,128);return new THREE.CanvasTexture(c);})();
const glow=new THREE.Mesh(new THREE.CircleGeometry(13,64),new THREE.MeshBasicMaterial({map:glowTex,color:0x2bff7a,transparent:true,opacity:.12,depthWrite:false,blending:THREE.AdditiveBlending}));glow.rotation.x=-Math.PI/2;glow.position.y=-.37;glow.scale.set(1,.72,1);scene.add(glow);
const ledMats=[],ledLights=[];const ledGroup=new THREE.Group();scene.add(ledGroup);
[V(-2.4,TH+.35,-1.3),V(.3,TH+.35,-1.3)].forEach(p=>{const mat=new THREE.MeshStandardMaterial({color:0xf4f4f4,emissive:0x000000,roughness:.25});ledMats.push(mat);
  const cube=new THREE.Mesh(new THREE.BoxGeometry(.55,.55,.55),mat);cube.position.copy(p);cube.castShadow=true;ledGroup.add(cube);
  const L=new THREE.PointLight(0xff3355,0,12,1.6);L.position.copy(p).add(V(0,.6,0));ledGroup.add(L);ledLights.push(L);});
build.push({obj:ledGroup,delay:2.9,kind:'pop'});tag(ledGroup,'l');

/* ---------- Kugeln ---------- */
const marbles=[],GEO_M=new THREE.SphereGeometry(.26,32,24);
function addMarble(seg,s){const m=new THREE.Mesh(GEO_M,M.steel);m.castShadow=true;m.visible=false;scene.add(m);marbles.push({m,seg,s,v:2});}
for(let i=0;i<6;i++)addMarble('lift',1+i*2.2);
for(let i=0;i<5;i++)addMarble('ret',SEG.ret.len-.4-i*.62);
const GAP=.62,LIFT_V=1.6,G=11;
function nextSeg(k){const S=P1.S;return{lift:'top',top:'g2w',g2w:S&&S.weiche===2?'w2':'w1',w1:'mid',w2:'mid',mid:S&&S.hebel===2?'h2':'h1',h1:'ret',h2:'ret',ret:'lift'}[k];}
function stepMarbles(dt){
  const S=P1.S,run=!!(S&&S.aufzug),open=!!(S&&S.stopp);
  const by={};marbles.forEach(o=>(by[o.seg]=by[o.seg]||[]).push(o));
  for(const k in by){const list=by[k].sort((a,b)=>b.s-a.s),len=SEG[k].len;
    list.forEach((o,idx)=>{if(o.seg!==k)return;
      if(k==='lift')o.v=run?LIFT_V:0;
      else{const t=frameAt(k,o.s).t;o.v+=(-t.y*G-.28*o.v)*dt;o.v=Math.min(9,Math.max(1.4,o.v));}
      let lim=len;if(idx>0&&list[idx-1].seg===k)lim=Math.min(lim,list[idx-1].s-GAP);
      let ns=Math.max(o.s,Math.min(o.s+o.v*dt,lim));if(ns<o.s+o.v*dt-1e-6&&k!=='lift')o.v=Math.max(1.4,o.v*.5);
      if(ns>=len){const hold=(k==='top'&&!open)||(k==='ret'&&!run);const nx=nextSeg(k);const blocked=marbles.some(q=>q!==o&&q.seg===nx&&q.s<GAP);
        if(hold||blocked){ns=len;o.v=1.4;}else{o.seg=nx;ns=Math.min(ns-len,GAP*.4);}}
      o.s=ns;});}
  marbles.forEach(o=>{const f=frameAt(o.seg,o.s);
    if(o.seg==='lift')o.m.position.set(f.p.x,f.p.y,f.p.z+.05);
    else if(TUBE[o.seg])o.m.position.copy(f.p);
    else o.m.position.copy(f.p).addScaledVector(f.n,.3);
    o.m.rotation.x+=o.v*dt*3.2;o.m.rotation.z+=o.v*dt*1.7;});
}

/* ---------- Ereignis-Ringe ---------- */
const rings=[];const RING_G=new THREE.RingGeometry(.92,1,64);
function ring(pos){const m=new THREE.Mesh(RING_G,new THREE.MeshBasicMaterial({color:0xff3b2f,transparent:true,side:THREE.DoubleSide,depthWrite:false}));
  m.rotation.x=-Math.PI/2;m.position.copy(pos);scene.add(m);rings.push({m,t:0});}
const EVPOS={a:V(LIFT_X,LIFT_TOP+.4,.1),s:N.gate,w:N.lsw,h:N.wsw,l:V(-1,TH+.4,-1.3)};

/* ---------- Kamera: Ziehen dreht, Rad/Zwei Finger zoomt, "Kugel folgen" haengt sich an eine Kugel ---------- */
const stage=document.getElementById('stage');
const HOT={a:V(LIFT_X,5.4,.7),s:N.gate,w:N.lsw,h:N.wsw,l:V(-1,TH+.7,-1.3)};
const hs={};for(const k in HOT)hs[k]=document.getElementById('hs_'+k);
const OV_T=V(-.5,8.3,0),OV_R=41,FOLLOW_R=17;
const orb={az:.69,el:.24,d:1};
const camPos=V(30,26,34),camTgt=V(0,7,0),wantP=V(0,0,0),wantT=V(0,0,0),v3=V(0,0,0);
const clamp=(x,a,b)=>Math.min(b,Math.max(a,x));
let mx=0,my=0,moved=0,pinch=0;const ptrs=new Map();
const pd=()=>{const [a,b]=[...ptrs.values()];return Math.hypot(a[0]-b[0],a[1]-b[1])||1;};
const ray=new THREE.Raycaster(),ndc=new THREE.Vector2();
function pick(e){const r=canvas.getBoundingClientRect();ndc.set((e.clientX-r.left)/r.width*2-1,-(e.clientY-r.top)/r.height*2+1);ray.setFromCamera(ndc,cam);const h=ray.intersectObjects(clickable,true)[0];return h?h.object.userData.cmd:null;}
canvas.addEventListener('pointerdown',e=>{canvas.setPointerCapture(e.pointerId);ptrs.set(e.pointerId,[e.clientX,e.clientY]);moved=0;if(ptrs.size===2)pinch=pd();});
canvas.addEventListener('pointermove',e=>{
  const p=ptrs.get(e.pointerId);
  if(!p){mx=e.clientX/innerWidth-.5;my=e.clientY/innerHeight-.5;canvas.style.cursor=pick(e)?'pointer':'';return;}
  const dx=e.clientX-p[0],dy=e.clientY-p[1];p[0]=e.clientX;p[1]=e.clientY;
  if(ptrs.size===2){const n=pd();orb.d=clamp(orb.d*pinch/n,.4,1.3);pinch=n;moved=99;return;}
  moved+=Math.abs(dx)+Math.abs(dy);orb.az-=dx*.006;orb.el=clamp(orb.el+dy*.005,-.08,1.25);
  if(moved>6)stage.classList.add('used');
});
const pend=e=>{if(ptrs.delete(e.pointerId)&&e.type==='pointerup'&&moved<6){const c=pick(e);if(c)P1.cmd(c);}};
canvas.addEventListener('pointerup',pend);canvas.addEventListener('pointercancel',pend);
canvas.addEventListener('wheel',e=>{e.preventDefault();orb.d=clamp(orb.d*Math.exp(e.deltaY*.0012),.4,1.3);},{passive:false});
function size(){const w=canvas.clientWidth,h=canvas.clientHeight;if(!w||!h)return;R.setSize(w,h,false);composer.setSize(w,h);bloom.setSize(w,h);cam.aspect=w/h;cam.updateProjectionMatrix();}
new ResizeObserver(size).observe(canvas);size();

/* ---------- Aufbau-Intro (wie in der Bauanleitung) ---------- */
build.forEach(b=>{b.obj.scale.set(b.kind==='grow'?1:.001,.001,b.kind==='grow'?1:.001);});
tracks.forEach(t=>{if(t.kind==='inst')t.mesh.count=0;else t.mesh.geometry.setDrawRange(0,0);});
/* Instanz-Gruppen nie wegschneiden: ihr Ursprung liegt oft ausserhalb des Bildes */
scene.traverse(o=>{if(o.isInstancedMesh)o.frustumCulled=false;});
const T0=performance.now(),INTRO=4.2;let introDone=false;
const outBack=x=>{const c1=1.70158,c3=c1+1;return 1+c3*Math.pow(x-1,3)+c1*Math.pow(x-1,2);};
function intro(t){
  build.forEach(b=>{const u=Math.min(1,Math.max(0,(t-b.delay)/.7));const e=b.kind==='grow'?1-Math.pow(1-u,3):outBack(u);const s=Math.max(.001,e);
    if(b.kind==='grow')b.obj.scale.set(1,s,1);else b.obj.scale.set(s,s,s);});
  tracks.forEach(tr=>{const u=Math.min(1,Math.max(0,(t-tr.delay)/1.1));const e=1-Math.pow(1-u,2);
    if(tr.kind==='inst')tr.mesh.count=Math.floor(tr.total*e);else tr.mesh.geometry.setDrawRange(0,Math.floor(tr.total*e/3)*3);});
  if(t>INTRO){introDone=true;stage.classList.add('ready');marbles.forEach(o=>o.m.visible=true);build.forEach(b=>b.obj.scale.set(1,1,1));
    tracks.forEach(tr=>{if(tr.kind==='inst')tr.mesh.count=tr.total;else tr.mesh.geometry.setDrawRange(0,Infinity);});}
}

/* ---------- Hauptschleife (laeuft mit der Bildrate des Displays, 120 Hz auf ProMotion) ---------- */
const st={gate:0,w:A.w1,h:A.h1,led:0,chain:0,w1:1,w2:0,h1:1,h2:0};const col=new THREE.Color(),m4=new THREE.Matrix4();
let last=performance.now(),first=true;
function loop(now){
  const dt=Math.min(.05,(now-last)/1000);last=now;const S=P1.S,t=(now-T0)/1000;
  if(!introDone)intro(t);
  /* Kamera */
  const follow=P1.view==='f'&&introDone,ia=1-Math.min(1,t/(INTRO+.5));   /* ia: Kamera kreist im Intro heran */
  const fit=clamp(.78/cam.aspect,1,1.6);
  if(follow)wantT.copy(marbles[0].m.position);else wantT.copy(OV_T);
  const r=(follow?FOLLOW_R:OV_R*fit)*orb.d*(1+ia*.3),az=orb.az+ia*1.1,el=follow?Math.max(orb.el,.38):orb.el;
  wantP.set(Math.sin(az)*Math.cos(el),Math.sin(el),Math.cos(az)*Math.cos(el)).multiplyScalar(r).add(wantT);
  const k=1-Math.pow(follow?.02:.0025,dt);camPos.lerp(wantP,k);camTgt.lerp(wantT,k);
  cam.position.set(camPos.x+mx*1.2,camPos.y-my*.8,camPos.z);cam.lookAt(camTgt);
  if(introDone)stepMarbles(dt);
  const e=1-Math.pow(.0006,dt);
  st.gate+=((S&&S.stopp?1:0)-st.gate)*e;gateArm.rotation.x=-st.gate*1.25;
  st.w+=((S&&S.weiche===2?A.w2:A.w1)-st.w)*e;armW.rotation.y=st.w;
  st.h+=((S&&S.hebel===2?A.h2:A.h1)-st.h)*e;armH.rotation.y=st.h;
  st.led+=((S&&S.leds?1:0)-st.led)*e;
  /* aktive Route voll sichtbar, der andere Ast der Weiche wird blass */
  const w2=!!(S&&S.weiche===2),h2=!!(S&&S.hebel===2),act={w1:!w2,w2:w2,h1:!h2,h2:h2};
  for(const b in BR){st[b]+=((act[b]?1:0)-st[b])*e*.6;const o=.13+.87*st[b];BR[b].forEach(m=>m.opacity=o);}
  /* LEDs in Ampelfarbe, blinken wie auf der Anlage */
  const amp=P1.amp||'g';col.setHex(amp==='r'?0xff2a1f:amp==='o'?0xff9500:0x2bff7a);
  const blink=amp==='r'?(now/150|0)%2===0:amp==='o'?(now/1000|0)%2===0:true;
  st.blk=(st.blk??1)+(((blink?1:0))-(st.blk??1))*(1-Math.pow(.000001,dt));
  ledMats.forEach(m=>{m.emissive.copy(col).multiplyScalar(st.led*st.blk*3);});
  ledLights.forEach(L=>{L.color.copy(col);L.intensity=st.led*st.blk*22;});
  glow.material.color.copy(col);glow.material.opacity=(.3+.1*Math.sin(now/900))*(.4+.6*st.blk);
  if(S&&S.aufzug)st.chain=(st.chain+LIFT_V*dt)%CH_L;
  let ci=0;
  for(let i=0;i<LINKS;i++){const d=(i*CH_L/LINKS+st.chain)%CH_L,H=LIFT_TOP-.5;let y,z;
    if(d<H){y=.55+d;z=.42;}else if(d<H+1){y=LIFT_TOP;z=.42-(d-H)*.84;}else if(d<2*H+1){y=LIFT_TOP-(d-H-1);z=-.42;}else{y=.55;z=-.42+(d-2*H-1)*.84;}
    m4.makeTranslation(LIFT_X,y,z+.15);chain.setMatrixAt(i,m4);
    if(i%8===0&&ci<LINKS/8){m4.makeTranslation(LIFT_X,y,z+.15+(z>0?.2:-.2));carriers.setMatrixAt(ci++,m4);}}
  chain.instanceMatrix.needsUpdate=true;carriers.instanceMatrix.needsUpdate=true;
  while(P1.events.length){const ev=P1.events.shift();if(EVPOS[ev])ring(EVPOS[ev]);}
  for(let i=rings.length-1;i>=0;i--){const r=rings[i];r.t+=dt;const s=.6+r.t*3.6;r.m.scale.set(s,s,s);r.m.material.opacity=Math.max(0,1-r.t/1.1);if(r.t>1.1){scene.remove(r.m);r.m.material.dispose();rings.splice(i,1);}}
  bloom.strength=.3+st.led*.5;
  composer.render();
  /* Schaltpunkte an ihre Teile heften */
  const cw=canvas.clientWidth,ch=canvas.clientHeight;
  for(const h in HOT){v3.copy(HOT[h]).project(cam);hs[h].style.transform=v3.z>1?'translate(-999px,0)':'translate('+((v3.x+1)/2*cw).toFixed(1)+'px,'+((1-v3.y)/2*ch).toFixed(1)+'px)';}
  if(first){first=false;document.getElementById('load').remove();}
  requestAnimationFrame(loop);
}
requestAnimationFrame(loop);
</script>
</body>
</html>
)DASH";
