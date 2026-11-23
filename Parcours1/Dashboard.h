#pragma once
// AUTOMATISCH ERZEUGT aus dashboard.html (tools/html2h.py) - nicht von Hand aendern.
static const char HTML_SEITE[] = R"DASH(<!doctype html>
<html lang="de">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover,user-scalable=no">
<meta name="theme-color" content="#07080b">
<meta name="apple-mobile-web-app-capable" content="yes">
<title>Parcours 1</title>
<script type="importmap">{"imports":{"three":"https://cdn.jsdelivr.net/npm/three@0.160.0/build/three.module.js","three/addons/":"https://cdn.jsdelivr.net/npm/three@0.160.0/examples/jsm/"}}</script>
<style>
/* Aufbau wie eine Fahrzeug-App: Modell als Buehne, darunter Status, Schnellaktionen, Karten, Tab-Leiste. */
:root{
  --bg:#07080b;--bg2:#0c0e13;--card:#13161d;--card2:#1b1f28;--card3:#242a35;--line:rgba(255,255,255,.07);
  --ink:#f3f5f9;--muted:#8a92a3;--faint:#555c6b;--blue:#2f86ff;--red:#ff3b30;--ok:#34d399;
  --amb:#34d399;--amb2:rgba(52,211,153,.16);
  --mono:"SF Mono",ui-monospace,SFMono-Regular,Menlo,Consolas,monospace;
  --sans:-apple-system,BlinkMacSystemFont,"SF Pro Display","SF Pro Text","Helvetica Neue",Helvetica,Arial,sans-serif;
  --ease:cubic-bezier(.16,1,.3,1);
  --tabh:64px;
}
body.amp-o{--amb:#ff9f0a;--amb2:rgba(255,159,10,.18)}
body.amp-r{--amb:#ff3b30;--amb2:rgba(255,59,48,.22)}
*{box-sizing:border-box;margin:0;padding:0}
html{background:var(--bg)}
body{background:var(--bg);color:var(--ink);font:15px/1.4 var(--sans);-webkit-font-smoothing:antialiased;-webkit-tap-highlight-color:transparent;overscroll-behavior:none}
button{font:inherit;color:inherit;background:none;border:0;cursor:pointer;-webkit-tap-highlight-color:transparent}
button:focus-visible{outline:2px solid var(--blue);outline-offset:2px}
svg.i{width:24px;height:24px;fill:none;stroke:currentColor;stroke-width:1.8;stroke-linecap:round;stroke-linejoin:round;flex:none}
.lbl{font:600 11px var(--sans);letter-spacing:.08em;text-transform:uppercase;color:var(--muted)}

/* ---------- Buehne ---------- */
#stage{position:relative;height:58vh;height:58svh;min-height:380px;overflow:hidden;background:radial-gradient(90% 70% at 50% 40%,#151a24,var(--bg) 75%)}
#gl{position:absolute;inset:0;width:100%;height:100%;display:block;touch-action:none;cursor:grab}
#gl:active{cursor:grabbing}
#load{position:absolute;inset:0;display:grid;place-items:center;text-align:center;padding:40px;color:var(--muted);font:13px var(--sans);pointer-events:none}
#stage::before{content:"";position:absolute;inset:0;z-index:1;pointer-events:none;background:radial-gradient(50% 40% at 50% 78%,var(--amb2),transparent 70%);transition:background 1.2s;mix-blend-mode:screen}
#stage::after{content:"";position:absolute;inset:auto 0 0;z-index:1;height:38%;background:linear-gradient(transparent,var(--bg) 92%);pointer-events:none}
.top{position:absolute;inset:0 0 auto;z-index:3;display:flex;justify-content:space-between;align-items:flex-start;padding:calc(18px + env(safe-area-inset-top)) 20px 0;pointer-events:none}
.top>*{pointer-events:auto}
.name{font:700 26px/1.1 var(--sans);letter-spacing:-.02em}
.name small{display:flex;align-items:center;gap:8px;margin-top:6px;font:500 13px var(--sans);color:var(--muted);letter-spacing:0}
.name small i{position:relative;width:8px;height:8px;border-radius:50%;background:var(--faint)}
body.live .name small i{background:var(--amb)}
body.live .name small i::after{content:"";position:absolute;inset:-4px;border-radius:50%;border:2px solid var(--amb);opacity:0;animation:ping 2.4s var(--ease) infinite}
@keyframes ping{0%{transform:scale(.4);opacity:.9}100%{transform:scale(1.7);opacity:0}}
body.lost .name small i{background:var(--red)}
.round{width:44px;height:44px;border-radius:50%;display:grid;place-items:center;background:rgba(255,255,255,.07);border:1px solid var(--line);backdrop-filter:blur(14px);-webkit-backdrop-filter:blur(14px);transition:background .2s,color .2s}
.round:hover{background:rgba(255,255,255,.14)}
.round.on{background:var(--ink);color:#111}
.hero{position:absolute;left:0;right:0;bottom:18px;z-index:3;text-align:center;pointer-events:none;padding:0 20px}
#hl{font:700 30px/1.1 var(--sans);letter-spacing:-.02em;overflow:hidden}
#hl b{display:block;font-weight:inherit;transition:transform .6s var(--ease),opacity .4s}
#hl.swap b{transform:translateY(60%);opacity:0}
#sub{margin-top:6px;font:14px var(--sans);color:var(--muted)}
.hint{position:absolute;left:20px;top:calc(96px + env(safe-area-inset-top));z-index:3;pointer-events:none;font:12px var(--sans);color:var(--faint);transition:opacity .6s}
#stage.used .hint{opacity:0}

/* Schaltpunkte direkt am Modell */
.hs{position:absolute;left:0;top:0;z-index:2;display:flex;align-items:center;gap:7px;margin:-15px 0 0 -15px;padding:4px 11px 4px 4px;border-radius:16px;background:rgba(10,12,16,.62);backdrop-filter:blur(10px);-webkit-backdrop-filter:blur(10px);border:1px solid var(--line);white-space:nowrap;opacity:0;visibility:hidden;transition:opacity .5s,background .2s;will-change:transform}
#stage.ready .hs{opacity:1;visibility:visible}
.hs:hover{background:var(--ink);color:#111}
.hs i{width:22px;height:22px;border-radius:50%;display:grid;place-items:center;background:rgba(255,255,255,.1)}
.hs i::after{content:"";width:8px;height:8px;border-radius:50%;background:var(--muted);transition:background .3s}
.hs.on i{background:rgba(47,134,255,.3);animation:pulse 2s infinite}
.hs.on i::after{background:var(--blue)}
#hs_l.on i{background:var(--amb2);animation:none}#hs_l.on i::after{background:var(--amb)}
@keyframes pulse{0%{box-shadow:0 0 0 0 rgba(47,134,255,.5)}100%{box-shadow:0 0 0 12px rgba(47,134,255,0)}}
.hs b{font:600 12px var(--sans)}
.hs em{font:500 12px var(--sans);font-style:normal;opacity:.6;margin-left:4px}

/* ---------- Inhalt ---------- */
#pane{position:relative;z-index:4;padding:0 16px calc(var(--tabh) + 28px + env(safe-area-inset-bottom));max-width:560px;margin:-6px auto 0}
.tab{display:none;animation:in .45s var(--ease)}
.tab.on{display:block}
@keyframes in{from{opacity:0;transform:translateY(10px)}}
h2.t{font:700 28px var(--sans);letter-spacing:-.02em;margin:18px 4px 14px}
.card{background:var(--card);border-radius:22px;padding:16px;margin-bottom:12px;border:1px solid var(--line)}
.ch{display:flex;justify-content:space-between;align-items:center;margin-bottom:10px}
.ch b{font:650 16px var(--sans)}

/* Schnellaktionen */
.qa{display:grid;grid-template-columns:repeat(4,1fr);gap:8px;margin:4px 0 16px}
.qb{display:flex;flex-direction:column;align-items:center;gap:9px;padding:6px 0;font:500 12px var(--sans);color:var(--muted);transition:color .2s}
.qb .c{position:relative;width:64px;height:64px;border-radius:50%;display:grid;place-items:center;background:var(--card2);color:var(--ink);border:1px solid var(--line);transition:background .25s,transform .15s,color .25s}
.qb:hover .c{background:var(--card3)}
.qb:active .c{transform:scale(.93)}
.qb.on .c{background:var(--blue);border-color:transparent;color:#fff;box-shadow:0 8px 26px rgba(47,134,255,.35)}
.qb.on{color:var(--ink)}
.qb.danger .c{color:var(--red)}
.qb.danger:hover .c{background:var(--red);color:#fff}
.qb .c::after{content:"";position:absolute;inset:-4px;border-radius:50%;border:2px solid transparent;border-top-color:var(--blue);opacity:0;transition:opacity .2s}
.qb.busy .c::after{opacity:1;animation:spin .8s linear infinite}
.qb.done .c{background:var(--ok);color:#06281b}
@keyframes spin{to{transform:rotate(360deg)}}

/* Hinweis bei Not-Aus / Fehler */
#alert{display:none;align-items:center;gap:12px;padding:14px 14px 14px 16px;border-radius:20px;background:rgba(255,59,48,.12);border:1px solid rgba(255,59,48,.4);margin-bottom:14px}
body.amp-r #alert{display:flex}
#alert svg{color:var(--red)}
#alert div{flex:1;font:14px var(--sans)}
#alert div b{display:block;font-weight:650}
#alert div span{color:var(--muted);font-size:13px}
#alert button{height:38px;padding:0 16px;border-radius:19px;background:var(--red);color:#fff;font:650 13px var(--sans)}

/* Fahrweg */
.way{display:flex;justify-content:space-between;align-items:center;gap:12px;padding:10px 0}
.way+.way{border-top:1px solid var(--line)}
.way span{font:14px var(--sans)}
.way span small{display:block;color:var(--muted);font-size:12px;margin-top:2px}
.seg{display:flex;padding:3px;border-radius:14px;background:var(--bg);flex:none}
.seg button{height:34px;padding:0 14px;border-radius:11px;font:600 13px var(--sans);color:var(--muted);transition:background .3s,color .3s}
.seg button.on{background:var(--ink);color:#111}

/* Zustandsliste */
.row{display:flex;align-items:center;gap:14px;width:100%;padding:12px 0;text-align:left}
.row+.row{border-top:1px solid var(--line)}
.row .ic{width:38px;height:38px;border-radius:12px;display:grid;place-items:center;background:var(--card2);color:var(--muted);flex:none}
.row .ic svg{width:20px;height:20px}
.row.on .ic{color:var(--blue);background:rgba(47,134,255,.14)}
.row .tx{flex:1;min-width:0}
.row .tx b{display:block;font:600 14px var(--sans)}
.row .tx small{display:block;font:13px var(--sans);color:var(--muted);margin-top:1px;white-space:nowrap;overflow:hidden;text-overflow:ellipsis}
.row .bar{height:3px;border-radius:2px;background:var(--card3);margin-top:7px;overflow:hidden;display:none}
.row .bar i{display:block;height:100%;width:0;background:var(--blue);transition:width .2s linear}
.row.timer .bar{display:block}
.row>svg{color:var(--faint);width:18px;height:18px}
.dot{width:8px;height:8px;border-radius:50%;background:var(--amb);flex:none}

/* Kennzahlen */
.stats{display:grid;grid-template-columns:repeat(3,1fr)}
.stats div+div{border-left:1px solid var(--line);padding-left:14px}
.stats b{display:block;font:700 22px var(--sans);letter-spacing:-.02em;font-variant-numeric:tabular-nums;margin-top:6px}
.stats small{font:12px var(--sans);color:var(--muted)}

/* Einstellen */
.saved{font:13px var(--sans);color:var(--muted);margin:-4px 4px 14px}
.act header{display:flex;justify-content:space-between;align-items:center;gap:10px;margin-bottom:4px}
.act header b{font:650 16px var(--sans)}
.act header small{display:block;font:12px var(--sans);color:var(--muted);margin-top:2px}
.act header button{height:34px;padding:0 14px;border-radius:17px;background:var(--blue);color:#fff;font:600 12px var(--sans);flex:none}
.act header button.sec{background:var(--card2);color:var(--ink)}
.act header button:active{transform:scale(.96)}
.rng{padding:12px 0 2px}
.rng label{display:flex;justify-content:space-between;font:13px var(--sans);color:var(--muted);margin-bottom:6px}
.rng label output{font:600 13px var(--mono);color:var(--ink)}
.rng label output.chg::after{content:" ●";color:var(--blue)}
input[type=range]{-webkit-appearance:none;appearance:none;width:100%;height:28px;background:none;cursor:pointer;touch-action:pan-y}
input[type=range]::-webkit-slider-runnable-track{height:6px;border-radius:3px;background:linear-gradient(90deg,var(--blue) var(--p,50%),var(--card3) var(--p,50%))}
input[type=range]::-moz-range-track{height:6px;border-radius:3px;background:linear-gradient(90deg,var(--blue) var(--p,50%),var(--card3) var(--p,50%))}
input[type=range]::-webkit-slider-thumb{-webkit-appearance:none;width:24px;height:24px;margin-top:-9px;border-radius:50%;background:#fff;box-shadow:0 2px 10px rgba(0,0,0,.5)}
input[type=range]::-moz-range-thumb{width:24px;height:24px;border:0;border-radius:50%;background:#fff}
.tg{display:flex;justify-content:space-between;align-items:center;gap:12px;padding:12px 0 2px;font:13px var(--sans);color:var(--muted)}
.sw{flex:none;width:52px;height:32px;border-radius:16px;background:var(--card3);position:relative;transition:background .3s}
.sw::after{content:"";position:absolute;top:4px;left:4px;width:24px;height:24px;border-radius:50%;background:#fff;transition:transform .4s var(--ease)}
.sw.on{background:var(--blue)}.sw.on::after{transform:translateX(20px)}
.arc{display:block;width:100%;height:96px;margin:6px 0 0}
.lamp{display:grid;grid-template-columns:repeat(4,1fr);gap:6px;margin-top:12px}
.lamp button{height:56px;border-radius:14px;background:var(--card2);font:600 12px var(--sans);display:flex;flex-direction:column;align-items:center;justify-content:center;gap:6px;border:1px solid transparent;transition:background .2s,border-color .2s}
.lamp button:hover{background:var(--card3)}
.lamp button.on{border-color:var(--blue)}
.lamp i{width:14px;height:14px;border-radius:50%;background:var(--c);box-shadow:0 0 12px var(--c)}
.lamp button[data-f="a"] i{background:conic-gradient(#34d399,#ff9f0a,#ff3b30,#34d399);box-shadow:none}
.lamp button[data-f="0"] i{background:none;border:2px solid var(--faint);box-shadow:none}
.keys{color:var(--muted);font-size:13px;line-height:1.9}
kbd{font:600 11px var(--mono);background:var(--card2);border:1px solid var(--line);border-radius:5px;padding:2px 6px;margin-right:3px;color:var(--ink)}
.btn{height:44px;border-radius:22px;background:var(--card2);font:600 14px var(--sans);transition:background .2s}
.btn:hover{background:var(--card3)}
.wide{width:100%;margin-top:12px}

/* System */
.grid{display:grid;grid-template-columns:1fr 1fr;gap:10px;margin-bottom:12px}
.grid .card{margin:0}
.grid .v{font:700 22px/1.15 var(--sans);letter-spacing:-.02em;margin-top:8px;font-variant-numeric:tabular-nums}
.grid .s{font:12px var(--sans);color:var(--muted);margin-top:3px}
.bars{display:inline-flex;gap:3px;align-items:flex-end;height:14px;margin-left:8px}
.bars i{width:4px;border-radius:1px;background:var(--card3)}
.bars i.on{background:var(--ink)}
.fwst{display:flex;align-items:center;gap:10px;font:650 15px var(--sans)}
.fwst i{width:9px;height:9px;border-radius:50%;background:var(--ok)}
.fwst.warn i{background:#ff9f0a}
.kv{margin-top:12px}
.kv div{display:flex;justify-content:space-between;gap:12px;padding:9px 0;border-top:1px solid var(--line);font:13px var(--mono)}
.kv span:first-child{color:var(--muted);font-family:var(--sans)}
.kv span:last-child{overflow:hidden;text-overflow:ellipsis;white-space:nowrap}
#otamsg{font:13px var(--sans);color:#ff9f0a;margin-top:10px}
#otamsg:empty{display:none}
details{margin-top:12px}
details summary{font:13px var(--sans);color:var(--muted);cursor:pointer}
details input{width:100%;height:40px;margin-top:10px;padding:0 12px;border-radius:12px;background:var(--bg);border:1px solid var(--line);font:12px var(--mono);color:var(--ink)}
#scope{display:block;width:100%;height:180px}
.log{font:12px/1.9 var(--mono);max-height:240px;overflow:auto}
.log div{display:flex;gap:14px;white-space:nowrap}
.log span:first-child{color:var(--faint)}

/* Tab-Leiste */
#tabs{position:fixed;z-index:8;left:50%;bottom:calc(12px + env(safe-area-inset-bottom));transform:translateX(-50%);display:flex;gap:4px;padding:6px;border-radius:26px;background:rgba(22,26,34,.78);border:1px solid var(--line);backdrop-filter:blur(22px) saturate(1.5);-webkit-backdrop-filter:blur(22px) saturate(1.5);box-shadow:0 18px 50px rgba(0,0,0,.55)}
#tabs button{position:relative;min-width:96px;height:52px;border-radius:20px;display:flex;flex-direction:column;align-items:center;justify-content:center;gap:3px;font:600 11px var(--sans);color:var(--muted);transition:background .25s,color .25s}
#tabs button svg{width:22px;height:22px}
#tabs button.on{background:rgba(255,255,255,.09);color:var(--ink)}
#tabs u{position:absolute;top:8px;right:28px;width:7px;height:7px;border-radius:50%;background:#ff9f0a;display:none}
#tabs .alert u{display:block}

.xm{display:flex;align-items:center;gap:14px}
.xm .ic{width:44px;height:44px;border-radius:14px;display:grid;place-items:center;background:var(--card2);font-size:22px;flex:none}
.xm .tx{flex:1;min-width:0}.xm .tx b{display:block;font:650 15px var(--sans)}.xm .tx small{display:block;font:13px var(--sans);color:var(--muted);margin-top:2px}
body.xmas .xm .ic{background:linear-gradient(140deg,#c81d25,#1f7a3a)}
.up{display:flex;gap:8px;margin-top:12px}
.up .btn{flex:1;height:40px;font-size:13px}
.prog{height:4px;border-radius:2px;background:var(--card3);margin-top:10px;overflow:hidden;display:none}
.prog i{display:block;height:100%;width:0;background:var(--blue);transition:width .2s}
.prog.on{display:block}
.sen{display:flex;align-items:center;gap:14px}
.sen .led{position:relative;width:46px;height:46px;border-radius:50%;flex:none;background:var(--card2);border:1px solid var(--line);display:grid;place-items:center;transition:background .15s,box-shadow .15s}
.sen .led::after{content:"";width:16px;height:16px;border-radius:50%;background:var(--faint);transition:background .15s}
.sen.on .led{background:rgba(52,211,153,.16);box-shadow:0 0 0 6px rgba(52,211,153,.08),0 0 24px rgba(52,211,153,.35)}
.sen.on .led::after{background:var(--ok);box-shadow:0 0 12px var(--ok)}
.sen.off .led::after{background:var(--card3)}
.sen .tx{flex:1;min-width:0}.sen .tx b{display:block;font:650 17px var(--sans)}.sen .tx small{display:block;font:12px var(--mono);color:var(--muted);margin-top:3px}
.sen .n{text-align:right}.sen .n b{display:block;font:700 26px var(--sans);font-variant-numeric:tabular-nums;letter-spacing:-.02em}.sen .n small{font:11px var(--sans);color:var(--muted)}
.sen.hit .led{background:rgba(255,255,255,.2);box-shadow:0 0 0 10px rgba(255,255,255,.06),0 0 30px rgba(255,255,255,.5)}
.sen.hit .led::after{background:#fff}
.sen .n b.bump{animation:bump .5s var(--ease)}
@keyframes bump{30%{transform:scale(1.25);color:var(--ok)}}
#sen_t{display:block;width:100%;height:34px;margin-top:12px}
#hs_k.on i{background:rgba(52,211,153,.25);animation:none}#hs_k.on i::after{background:var(--ok)}
#snow{position:fixed;inset:0;z-index:7;pointer-events:none;display:none}
body.xmas #snow{display:block}
body.xmas #stage{background:radial-gradient(90% 70% at 50% 40%,#18233a,var(--bg) 75%)}
#toast{position:fixed;left:50%;top:calc(16px + env(safe-area-inset-top));z-index:20;background:var(--ink);color:#111;font:600 13px var(--sans);padding:11px 18px;border-radius:20px;opacity:0;visibility:hidden;transform:translate(-50%,-14px);transition:opacity .3s,visibility .3s,transform .5s var(--ease)}
#toast.show{opacity:1;visibility:visible;transform:translate(-50%,0)}

/* Grosser Bildschirm: Modell links, Inhalt rechts */
@media (min-width:980px){
  body{display:grid;grid-template-columns:1fr 460px;height:100vh;height:100dvh;overflow:hidden}
  #stage{height:100%;min-height:0}
  #stage::after{display:none}
  #pane{margin:0;max-width:none;height:100%;overflow:auto;padding:calc(84px + env(safe-area-inset-top)) 22px 40px;background:var(--bg2);border-left:1px solid var(--line)}
  #tabs{left:auto;right:22px;transform:none;top:18px;bottom:auto;width:416px;justify-content:space-between}
  #tabs button{flex:1;min-width:0;height:44px;flex-direction:row;gap:8px;font-size:13px}
  #tabs button svg{width:18px;height:18px}
  #tabs u{top:10px;right:16px}
  .hero{bottom:34px}
  #hl{font-size:40px}
  h2.t{margin-top:4px}
}
@media (max-width:420px){.qb .c{width:58px;height:58px}#tabs button{min-width:84px}.seg button{padding:0 11px}}
@media (prefers-reduced-motion:reduce){*{transition:none!important;animation:none!important}}
</style>
</head>
<body>
<svg width="0" height="0" style="position:absolute">
 <symbol id="i-play" viewBox="0 0 24 24"><path d="M8 5.5v13l10.5-6.5z" fill="currentColor" stroke="none"/></symbol>
 <symbol id="i-stop" viewBox="0 0 24 24"><rect x="7" y="7" width="10" height="10" rx="2" fill="currentColor" stroke="none"/></symbol>
 <symbol id="i-drop" viewBox="0 0 24 24"><circle cx="12" cy="7.5" r="3.5"/><path d="M12 13.5v7M8.5 17l3.5 3.5 3.5-3.5"/></symbol>
 <symbol id="i-bulb" viewBox="0 0 24 24"><path d="M9 18h6M10 21h4M12 3a6 6 0 0 0-3.5 10.9c.6.4 1 1.1 1 1.8V16h5v-.3c0-.7.4-1.4 1-1.8A6 6 0 0 0 12 3z"/></symbol>
 <symbol id="i-power" viewBox="0 0 24 24"><path d="M12 3v8M6.3 6.3a8 8 0 1 0 11.4 0"/></symbol>
 <symbol id="i-home" viewBox="0 0 24 24"><path d="M4 10.5 12 4l8 6.5V20h-5v-6H9v6H4z"/></symbol>
 <symbol id="i-sliders" viewBox="0 0 24 24"><path d="M4 6h9M17 6h3M4 12h3M11 12h9M4 18h11M19 18h1"/><circle cx="15" cy="6" r="2"/><circle cx="9" cy="12" r="2"/><circle cx="17" cy="18" r="2"/></symbol>
 <symbol id="i-pulse" viewBox="0 0 24 24"><path d="M3 12h4l3-7 4 14 3-7h4"/></symbol>
 <symbol id="i-eye" viewBox="0 0 24 24"><path d="M2 12s3.6-6.5 10-6.5S22 12 22 12s-3.6 6.5-10 6.5S2 12 2 12z"/><circle cx="12" cy="12" r="3"/></symbol>
 <symbol id="i-chev" viewBox="0 0 24 24"><path d="M9 6l6 6-6 6"/></symbol>
 <symbol id="i-lift" viewBox="0 0 24 24"><path d="M12 21V4M7.5 8.5 12 4l4.5 4.5M8 21h8"/></symbol>
 <symbol id="i-gate" viewBox="0 0 24 24"><path d="M5 20V8M19 20V8M5 12h9"/><circle cx="17" cy="12" r="2"/></symbol>
 <symbol id="i-wifi" viewBox="0 0 24 24"><path d="M2.5 9a14 14 0 0 1 19 0M5.5 12.5a9.5 9.5 0 0 1 13 0M8.8 16a5 5 0 0 1 6.4 0"/><circle cx="12" cy="19" r="1" fill="currentColor"/></symbol>
 <symbol id="i-chip" viewBox="0 0 24 24"><rect x="6" y="6" width="12" height="12" rx="2"/><path d="M9 2.5V6M15 2.5V6M9 18v3.5M15 18v3.5M2.5 9H6M2.5 15H6M18 9h3.5M18 15h3.5"/></symbol>
 <symbol id="i-check" viewBox="0 0 24 24"><path d="M5 12.5l4.5 4.5L19 7.5"/></symbol>
 <symbol id="i-warn" viewBox="0 0 24 24"><path d="M12 3.5 2.5 20h19z"/><path d="M12 10v4.5M12 17.2v.3"/></symbol>
</svg>

<div id="stage">
 <canvas id="gl"></canvas>
 <div id="load">Lade 3D-Modell …</div>
 <div class="top">
  <div class="name">Parcours 1<small><i></i><span id="conn">verbinde …</span></small></div>
  <button class="round" id="follow" title="Kugel folgen (F)" aria-label="Kugel folgen"><svg class="i"><use href="#i-eye"/></svg></button>
 </div>
 <div class="hint">Ziehen dreht · Teile antippen schaltet</div>
 <button class="hs" id="hs_a" data-c="a"><i></i><span><b>Aufzug</b><em></em></span></button>
 <button class="hs" id="hs_s" data-c="s"><i></i><span><b>Stoppstelle</b><em></em></span></button>
 <button class="hs" id="hs_w" data-c="w"><i></i><span><b>Weiche 1</b><em></em></span></button>
 <button class="hs" id="hs_h" data-c="h"><i></i><span><b>Weiche 2</b><em></em></span></button>
 <button class="hs" id="hs_k" data-go="cfg"><i></i><span><b>Sensor</b><em></em></span></button>
 <button class="hs" id="hs_l" data-c="l"><i></i><span><b>Licht</b><em></em></span></button>
 <div class="hero"><div id="hl"><b id="hlt">Verbinde …</b></div><p id="sub">Parcours 1 wird gesucht</p></div>
</div>

<main id="pane">
 <!-- ===== Anlage ===== -->
 <section class="tab on" id="t_home">
  <div id="alert"><svg class="i"><use href="#i-warn"/></svg><div><b id="al_t">Not-Aus aktiv</b><span id="al_s">Alle Motoren sind aus</span></div><button data-c="a">Freigeben</button></div>
  <div class="qa">
   <button class="qb" id="qb_a" data-c="a"><span class="c"><svg class="i"><use id="qb_ai" href="#i-play"/></svg></span><span id="qb_at">Starten</span></button>
   <button class="qb" id="qb_s" data-c="s"><span class="c"><svg class="i"><use href="#i-drop"/></svg></span>Kugel frei</button>
   <button class="qb" id="qb_l" data-c="l"><span class="c"><svg class="i"><use href="#i-bulb"/></svg></span>Licht</button>
   <button class="qb danger" id="qb_x" data-c="x"><span class="c"><svg class="i"><use href="#i-power"/></svg></span>Not-Aus</button>
  </div>

  <div class="card" id="sen_c">
   <div class="sen" id="sen"><span class="led"></span><span class="tx"><b id="sen_s">Kugelsensor</b><small id="sen_p">–</small></span><span class="n"><b id="sen_n">0</b><small>Kugeln durch</small></span></div>
   <canvas id="sen_t" title="Sensor, letzte 20 Sekunden"></canvas>
   <div class="rng" id="emp_r" style="padding-top:10px"><label><span>Empfindlichkeit</span><output id="emp_o">–</output></label><input type="range" id="emp" min="1" max="10" step="1"></div>
  </div>

  <div class="card">
   <div class="ch"><b>Fahrweg</b><span class="lbl" id="way_s">–</span></div>
   <div class="way"><span>Weiche 1<small>oben, vor dem Looping</small></span><div class="seg" data-c="w" data-f="weiche"><button data-v="1">Looping</button><button data-v="2">Vorbei</button></div></div>
   <div class="way"><span>Weiche 2<small>Mitte</small></span><div class="seg" data-c="h" data-f="hebel"><button data-v="1">Spirale</button><button data-v="2">S-Kurve</button></div></div>
  </div>

  <div class="card">
   <div class="ch"><b>Zustand</b><span class="lbl" id="z_t">–</span></div>
   <button class="row" id="z_a" data-go="cfg"><span class="ic"><svg class="i"><use href="#i-lift"/></svg></span><span class="tx"><b>Kettenaufzug</b><small>–</small></span><svg class="i"><use href="#i-chev"/></svg></button>
   <button class="row" id="z_s" data-go="cfg"><span class="ic"><svg class="i"><use href="#i-gate"/></svg></span><span class="tx"><b>Stoppstelle</b><small>–</small><span class="bar"><i id="s_p"></i></span></span><svg class="i"><use href="#i-chev"/></svg></button>
   <button class="row" id="z_k" data-go="cfg"><span class="ic"><svg class="i"><use href="#i-eye"/></svg></span><span class="tx"><b>Kugelsensor</b><small>–</small></span><svg class="i"><use href="#i-chev"/></svg></button>
   <button class="row" id="z_l" data-go="cfg"><span class="ic"><svg class="i"><use href="#i-bulb"/></svg></span><span class="tx"><b>Licht</b><small>–</small></span><svg class="i"><use href="#i-chev"/></svg></button>
   <button class="row" id="z_c" data-go="sys"><span class="ic"><svg class="i"><use href="#i-wifi"/></svg></span><span class="tx"><b>Verbindung</b><small>–</small></span><svg class="i"><use href="#i-chev"/></svg></button>
   <button class="row" id="z_m" data-go="sys"><span class="ic"><svg class="i"><use href="#i-chip"/></svg></span><span class="tx"><b>M5Stack-Panel</b><small>–</small></span><svg class="i"><use href="#i-chev"/></svg></button>
   <button class="row" id="z_f" data-go="sys"><span class="ic"><svg class="i"><use href="#i-chip"/></svg></span><span class="tx"><b>Software</b><small>–</small></span><svg class="i"><use href="#i-chev"/></svg></button>
  </div>

  <div class="card">
   <div class="xm"><span class="ic">🎄</span><span class="tx"><b>Weihnachtsmodus</b><small id="xm_s">–</small></span><button class="sw" id="xm_b" data-c="j" aria-label="Weihnachtsmodus"></button></div>
   <div class="up"><button class="btn" id="up_b">Lied hochladen (MP3)</button><button class="btn" id="up_d" style="flex:0 0 auto;padding:0 16px">Löschen</button></div>
   <div class="rng" style="padding-top:14px"><label><span>Lautstärke</span><output id="vol_o">–</output></label><input type="range" id="vol" min="0" max="100" step="1"></div>
   <input type="file" id="up_f" accept="audio/*,.mp3" hidden>
   <div class="prog" id="up_p"><i></i></div>
  </div>

  <div class="card stats">
   <div><small>Aufzug gelaufen</small><b id="k_run">00:00</b></div>
   <div><small>Kugeln durch</small><b id="k_sw">0</b></div>
   <div><small>Giga online</small><b id="k_up">–</b></div>
  </div>
 </section>

 <!-- ===== Einstellen ===== -->
 <section class="tab" id="t_cfg">
  <h2 class="t">Einstellen</h2>
  <p class="saved" id="cfg_s">Lade Werte vom Giga …</p>
  <div id="cfg_l"></div>
  <div class="card act">
   <header><div><b>Statuslampe oben</b><small>RGB-Modul: R an A0/D18, G an A1/D19, B an A2, „–“ an GND</small></div></header>
   <div class="lamp" id="lamp">
    <button data-f="a"><i></i>Auto</button>
    <button data-f="g" style="--c:#34d399"><i></i>Grün</button>
    <button data-f="o" style="--c:#ff9f0a"><i></i>Orange</button>
    <button data-f="r" style="--c:#ff3b30"><i></i>Rot</button>
    <button data-f="b" style="--c:#2f86ff"><i></i>Blau</button>
    <button data-f="w" style="--c:#fff"><i></i>Weiß</button>
    <button data-f="0"><i></i>Aus</button>
   </div>
  </div>
  <button class="btn wide" id="cfg_r">Alles auf Startwerte zurücksetzen</button>
 </section>

 <!-- ===== System ===== -->
 <section class="tab" id="t_sys">
  <h2 class="t">System</h2>
  <div class="grid">
   <div class="card"><div class="lbl">WLAN</div><div class="v"><span id="k_wl">–</span><span class="bars" id="k_bars"><i style="height:4px"></i><i style="height:7px"></i><i style="height:10px"></i><i style="height:14px"></i></span></div><div class="s" id="k_wls">–</div></div>
   <div class="card"><div class="lbl">Datenvolumen</div><div class="v" id="k_dat">–</div><div class="s" id="k_dat_s">vom Giga, geschätzt</div></div>
  </div>
  <div class="card">
   <div class="fwst" id="f_st"><i></i><span id="f_auto">–</span></div>
   <div class="kv">
    <div><span>Läuft</span><span id="f_ver">–</span></div>
    <div><span>GitHub</span><span id="f_gh">–</span></div>
    <div><span>Gebaut</span><span id="f_date">–</span></div>
    <div><span>Adresse</span><span id="f_ip">–</span></div>
   </div>
   <div id="otamsg"></div>
   <button class="btn wide" id="ghbtn">Jetzt von GitHub laden</button>
   <details><summary>Von eigener Adresse laden</summary><input id="url" spellcheck="false" autocomplete="off"><button class="btn wide" id="urlbtn">Update starten</button></details>
  </div>
  <div class="card" style="padding:12px 0 6px"><div class="lbl" style="padding:0 16px 6px">Verlauf · letzte 2 Minuten</div><canvas id="scope"></canvas></div>
  <div class="card"><div class="lbl" style="margin-bottom:8px">Ereignisse</div><div class="log" id="log"></div></div>
  <div class="card"><div class="lbl" style="margin-bottom:8px">Tastatur</div>
   <p class="keys"><kbd>Leertaste</kbd>Start / Stopp<br><kbd>S</kbd>Kugel frei <kbd>W</kbd>Weiche 1 <kbd>H</kbd>Weiche 2<br><kbd>L</kbd>Licht <kbd>F</kbd>Kugel folgen <kbd>X</kbd>Not-Aus<br><kbd>1</kbd><kbd>2</kbd><kbd>3</kbd>Anlage / Einstellen / System</p></div>
 </section>
</main>

<nav id="tabs">
 <button class="on" data-t="home"><svg class="i"><use href="#i-home"/></svg>Anlage</button>
 <button data-t="cfg"><svg class="i"><use href="#i-sliders"/></svg>Einstellen</button>
 <button data-t="sys" id="tab_sys"><u></u><svg class="i"><use href="#i-pulse"/></svg>System</button>
</nav>
<canvas id="snow"></canvas>
<div id="toast"></div>

<script>
/* ===================== Daten, Steuerung, Anzeige ===================== */
(()=>{
const $=id=>document.getElementById(id),B=document.body;
const DPR=()=>Math.min(devicePixelRatio||1,3);
const MONO=getComputedStyle(document.documentElement).getPropertyValue('--mono');
const P1=window.P1={S:null,events:[],cmd:null,view:'o',amp:'g'};
const SEN=[];let polling=false,UPL=false,lastOk=0,lat=0,runMs=0,runSince=null,swc=0,up=null,stT=0,ota='',TAKT=8000,TAB='home';

const dur=s=>{s=Math.max(0,Math.floor(s));const h=Math.floor(s/3600),m=Math.floor(s%3600/60),x=s%60;return(h?h+':':'')+String(m).padStart(2,'0')+':'+String(x).padStart(2,'0')};
let tt;function toast(m){const t=$('toast');t.textContent=m;t.classList.add('show');clearTimeout(tt);tt=setTimeout(()=>t.classList.remove('show'),2400);}
function log(m){const l=$('log');const d=document.createElement('div');d.innerHTML='<span>'+new Date().toLocaleTimeString('de-DE')+'</span><span></span>';d.lastChild.textContent=m;l.prepend(d);while(l.children.length>80)l.lastChild.remove();}
function head(t,sub){$('sub').textContent=sub;const h=$('hl'),b=$('hlt');if(h._t===t)return;h._t=t;h.classList.add('swap');setTimeout(()=>{b.textContent=t;h.classList.remove('swap');},300);}
const hsSet=(k,on,txt)=>{const e=$('hs_'+k);e.classList.toggle('on',on);e.querySelector('em').textContent=txt;};
const rowSet=(id,on,txt)=>{const r=$(id);r.classList.toggle('on',on);r.querySelector('small').textContent=txt;};

/* Verlauf (nur gezeichnet, wenn System offen ist) */
const sc=$('scope'),sg=sc.getContext('2d'),WIN=120000;
const CH=[['Aufzug',s=>s.aufzug],['Stopp',s=>s.stopp],['Vorbei',s=>s.weiche===2],['S-Kurve',s=>s.hebel===2],['Licht',s=>s.leds]];
const samples=[];
function sSize(){sc.width=Math.round(sc.clientWidth*DPR());sc.height=Math.round(sc.clientHeight*DPR());}
function sDraw(now){
  const d=DPR(),w=sc.width,h=sc.height;if(!w)return;sg.clearRect(0,0,w,h);
  const L=80*d,rh=h/CH.length,X=t=>L+(w-L-12*d)*(1-(now-t)/WIN);
  sg.font=(11*d)+'px '+MONO;
  CH.forEach(([n,f],ci)=>{
    const y0=ci*rh,hi=y0+rh*.28,lo=y0+rh*.72;
    sg.fillStyle='#8a92a3';sg.fillText(n,16*d,y0+rh/2+4*d);
    if(ci){sg.strokeStyle='rgba(255,255,255,.06)';sg.lineWidth=1;sg.beginPath();sg.moveTo(0,Math.round(y0)+.5);sg.lineTo(w,Math.round(y0)+.5);sg.stroke();}
    sg.strokeStyle=ci===0?'#2f86ff':'#f3f5f9';sg.lineWidth=2*d;sg.beginPath();let on=false,py=0;
    for(let i=0;i<samples.length;i++){const sm=samples[i],x0=X(sm.t),x1=i+1<samples.length?X(samples[i+1].t):X(now);if(x1<L)continue;if(!sm.s){on=false;continue;}
      const y=f(sm.s)?hi:lo,xa=Math.max(x0,L);if(!on){sg.moveTo(xa,y);on=true;}else if(y!==py){sg.lineTo(xa,py);sg.lineTo(xa,y);}sg.lineTo(x1,y);py=y;}
    sg.stroke();
  });
}
setInterval(()=>{const n=performance.now();samples.push({t:n,s:P1.S&&Date.now()-lastOk<4000?P1.S:null});while(samples.length&&n-samples[0].t>WIN+5000)samples.shift();if(TAB==='sys')sDraw(n);},250);
addEventListener('resize',()=>{if(TAB==='sys')sSize();});

/* Anzeige */
const ROUTE=s=>[s.weiche===2?'am Looping vorbei':'durch den Looping',s.hebel===2?'über die S-Kurve':'durch die Spirale'];
function render(s){
  const p=P1.S;P1.S=s;
  const amp=s.ampel||'g';B.classList.toggle('amp-o',amp==='o');B.classList.toggle('amp-r',amp==='r');P1.amp=amp;
  $('al_t').textContent=s.notaus?'Not-Aus aktiv':s.m5==='verloren'?'Verbindung zum M5Stack verloren':s.stau?'Störung: Kugelstau':'Störung';$('al_s').textContent=s.notaus?'Alle Motoren sind aus':(s.grund||'unbekannt');
  B.classList.toggle('run',s.aufzug);
  const r=ROUTE(s);
  if(s.notaus)head('Not-Aus','Alle Motoren aus · zum Freigeben starten');
  else head(s.aufzug?'In Betrieb':'Bereit',s.aufzug?'Kugeln fahren '+r[0]+' und '+r[1]:'Alles bereit für die nächste Fahrt');
  $('qb_a').classList.toggle('on',s.aufzug);$('qb_at').textContent=s.aufzug?'Anhalten':'Starten';$('qb_ai').setAttribute('href',s.aufzug?'#i-stop':'#i-play');
  $('qb_l').classList.toggle('on',s.leds);$('qb_s').classList.toggle('on',s.stopp);
  document.querySelectorAll('.seg').forEach(g=>[...g.children].forEach(b=>b.classList.toggle('on',+b.dataset.v===s[g.dataset.f])));
  $('way_s').textContent=(s.weiche===2?'Vorbei':'Looping')+' · '+(s.hebel===2?'S-Kurve':'Spirale');
  rowSet('z_a',s.aufzug,s.aufzug?'Läuft · fördert Kugeln nach oben':'Aus');
  rowSet('z_l',s.leds,!s.leds?'Aus':amp==='g'?'An · Status ok':amp==='o'?'Blinkt langsam · kein Internet':'Blinkt schnell · '+(s.notaus?'Not-Aus':'Fehler'));
  hsSet('a',s.aufzug,s.aufzug?'läuft':'aus');hsSet('s',s.stopp,s.stopp?'offen':'zu');
  hsSet('w',false,s.weiche===2?'Vorbei':'Looping');hsSet('h',false,s.hebel===2?'S-Kurve':'Spirale');hsSet('l',s.leds,s.leds?({g:'ok',o:'offline',r:'Fehler'})[amp]:'aus');
  if(s.aufzug&&!runSince)runSince=Date.now();if(!s.aufzug&&runSince){runMs+=Date.now()-runSince;runSince=null;}
  up={v:s.uptime,at:Date.now()};
  const q=s.rssi>=-55?4:s.rssi>=-67?3:s.rssi>=-75?2:1,ql=['','Schwach','Mittel','Gut','Sehr gut'][q];
  $('k_wl').textContent=ql;[...$('k_bars').children].forEach((e,i)=>e.classList.toggle('on',i<q));
  $('k_wls').textContent=s.rssi+' dBm · Antwort in '+lat+' ms';
  rowSet('z_m',s.m5==='ok',({ok:'Verbunden über MQTT · synchron',warte:'MQTT verbunden · wartet auf Lebenszeichen',getrennt:'Gefunden, aber nicht verbunden',verloren:'Verbindung verloren · Anlage gestoppt',aus:'Nicht gefunden'})[s.m5]||'–');
  rowSet('z_c',false,'WLAN '+ql.toLowerCase()+' · '+(amp==='o'?'kein Internet':'online')+' · '+lat+' ms');
  if(s.daten!==undefined){const b=s.daten+s.abrufe*7000;$('k_dat').textContent=b<1e6?Math.round(b/1e3)+' KB':(b/1e6).toFixed(1)+' MB';if(s.uptime>60)$('k_dat_s').textContent='≈ '+(b/s.uptime*3.6/1e3).toFixed(1)+' MB pro Stunde';}
  $('f_ver').textContent=s.version;$('f_gh').textContent=s.github||'–';$('f_date').textContent=s.datum;$('f_ip').textContent=s.ip;
  const pend=s.auto&&s.github&&s.github!==s.version;
  const fw=!s.auto?'Lokale Version · kein Auto-Update':pend?'Update gefunden · lädt gleich':'Aktuell · Auto-Update an';
  $('f_auto').textContent=fw;$('f_st').classList.toggle('warn',!s.auto||pend);rowSet('z_f',false,fw+' · '+s.version);
  [...$('lamp').children].forEach(b=>b.classList.toggle('on',b.dataset.f===(s.lampe||'a')));
  ota=s.ota||'';$('otamsg').textContent=ota;$('tab_sys').classList.toggle('alert',!!(pend||ota));
  $('z_t').textContent=amp==='g'?'Alles ok':amp==='o'?'Offline':'Achtung';
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
  $('k_sw').textContent=s.kugeln!==undefined?s.kugeln:swc;
  rowSet('z_k',!!s.kugel,(!s.sensor?'Aus · gibt ohne Prüfung frei':s.kugel?'Kugel wartet':'Keine Kugel')+(s.pegel?' · '+s.pegel:''));
  {const e=$('sen');e.classList.toggle('on',!!s.kugel);e.classList.toggle('off',!s.sensor);
   const shock=s.styp===1,hit=shock&&s.stoss<700;e.classList.toggle('hit',hit);
   $('sen_s').textContent=shock?(hit?'Stoß! Kugel angekommen':s.kugel?'Kugel wartet':'Warte auf Kugel'):(s.kugel?'Kugel erkannt':'Keine Kugel');
   if(s.flanken!==undefined){const now=performance.now();if(P1.fl){const d=(s.flanken-P1.fl[0])/((now-P1.fl[1])/1000);P1.fr=Math.round(d);}P1.fl=[s.flanken,now];}
   $('sen_p').textContent=(shock?'Shock · ':'Reflex · ')+(s.pegel||'')+(shock&&P1.fr!==undefined?' · '+P1.fr+' Zucker/s':'')+(s.sensor?' · Automatik prüft':' · Prüfung aus');
   $('emp_r').style.display=shock?'':'none';
   const nb=$('sen_n');if(nb.textContent!==String(s.kugeln)){nb.textContent=s.kugeln;nb.classList.remove('bump');nb.offsetWidth;nb.classList.add('bump');}
   hsSet('k',!!s.kugel||hit,hit?'Stoß':s.kugel?'Kugel':'leer');SEN.push([performance.now(),!!s.kugel||hit]);}
  B.classList.toggle('xmas',!!s.weihnacht);$('xm_b').classList.toggle('on',!!s.weihnacht);
  const lm=s.lied?Math.floor(s.lied/60)+':'+String(s.lied%60).padStart(2,'0'):'';
  if(!UPL)$('xm_s').textContent=!s.lied?'Noch kein Lied auf dem Giga · erst hochladen':s.weihnacht?'Es schneit · Lied läuft ('+lm+')':'Lied auf dem Giga ('+lm+') · Schnee und Musik';
  if(p&&p.weihnacht!==s.weihnacht)log(s.weihnacht?'Weihnachtsmodus an':'Weihnachtsmodus aus');
  if(p&&p.kugeln!==undefined&&s.kugeln>p.kugeln)P1.events.push('s');
  tick();
}
/* Stoppstelle: Countdown bis zur naechsten automatischen Freigabe */
function tick(){
  const s=P1.S;if(!s)return;let t='Geschlossen',w=0,on=false;
  if(s.stopp){t='Offen · Kugel rollt';on=true;}
  else if(s.aufzug&&CFG.stopp_auto!==0){if(stT){const left=TAKT-(Date.now()-stT);w=Math.min(1,Math.max(0,1-left/TAKT));t=left>900?'Nächste Kugel in '+Math.ceil(left/1000)+' s':'Nächste Kugel gleich';}else t='Gibt automatisch Kugeln frei';}
  const r=$('z_s');r.classList.toggle('on',on);r.classList.toggle('timer',w>0);r.querySelector('small').textContent=t;$('s_p').style.width=(w*100).toFixed(1)+'%';
}
setInterval(tick,200);
setInterval(()=>{
  $('k_run').textContent=dur((runMs+(runSince?Date.now()-runSince:0))/1000);
  if(up)$('k_up').textContent=dur(up.v+(Date.now()-up.at)/1000);
  const ok=Date.now()-lastOk<4000;B.classList.toggle('live',ok);B.classList.toggle('lost',!ok&&!!lastOk);
  const a=P1.amp,S=P1.S;
  $('conn').textContent=!ok?(lastOk?'Getrennt':'Verbinde …'):a==='r'?(S&&S.notaus?'Not-Aus':'Fehler'):a==='o'?'Verbunden · kein Internet':'Verbunden · gerade aktualisiert';
  if(!ok&&lastOk){B.classList.remove('run');if(ota)head('Update läuft','Neue Software wird installiert · ca. 30 s');else head('Getrennt','Giga nicht erreichbar · bist du im Hotspot?');}
  else if(!lastOk&&performance.now()>5000)head('Getrennt','Giga nicht erreichbar · bist du im Hotspot?');
},1000);

/* Kommunikation */
async function get(u){const t=performance.now();const r=await fetch(u,{cache:'no-store'});const j=await r.json();lat=Math.round(performance.now()-t);lastOk=Date.now();return j;}
async function poll(){if(polling)return;polling=true;try{render(await get('/status'));}catch(e){}polling=false;}
const QB={a:'qb_a',s:'qb_s',l:'qb_l',x:'qb_x'};
async function cmd(c){
  if(navigator.vibrate)navigator.vibrate(8);
  const b=QB[c]&&$(QB[c]);if(b){b.classList.remove('done');b.classList.add('busy');}
  try{const s=await get('/cmd?c='+c);render(s);
    if(c==='j'&&!s.weihnacht&&!s.lied)toast('Erst ein Lied hochladen');
    if(b){b.classList.remove('busy');b.classList.add('done');setTimeout(()=>b.classList.remove('done'),700);}
    if(c==='x')toast('Alle Motoren aus');
  }catch(e){if(b)b.classList.remove('busy');toast('Giga nicht erreichbar');}
}
P1.cmd=cmd;
document.querySelectorAll('button[data-c]').forEach(el=>el.addEventListener('click',()=>cmd(el.dataset.c)));
/* Weichen: Ziel antippen statt blind umschalten */
document.querySelectorAll('.seg').forEach(g=>g.addEventListener('click',e=>{const b=e.target.closest('button');if(b&&P1.S&&P1.S[g.dataset.f]!==+b.dataset.v)cmd(g.dataset.c);}));
function view(v){P1.view=v;$('follow').classList.toggle('on',v==='f');}
$('follow').onclick=()=>view(P1.view==='f'?'o':'f');

/* Tabs */
function tab(t){
  TAB=t;document.querySelectorAll('.tab').forEach(e=>e.classList.toggle('on',e.id==='t_'+t));
  document.querySelectorAll('#tabs button').forEach(b=>b.classList.toggle('on',b.dataset.t===t));
  if(t==='sys'){sSize();sDraw(performance.now());}
  if(t==='cfg')cfgLoad();
  if(innerWidth<980&&t!=='home')$('pane').scrollIntoView({behavior:'smooth'});
}
$('tabs').onclick=e=>{const b=e.target.closest('button');if(b)tab(b.dataset.t);};
document.querySelectorAll('[data-go]').forEach(r=>r.onclick=()=>tab(r.dataset.go));

/* ---------- Einstellen: jeder Aktor mit Reglern und Test ---------- */
const pct=v=>Math.round(v/2.55)+' %',ms=v=>v+' ms',deg=v=>v+'°',sec=v=>(v/1000).toFixed(1)+' s';
const ACT=[
 {t:'Kettenaufzug',d:'XS-Motor an M2',test:['a','Start / Stopp'],p:[
   ['aufzug_pwm','Geschwindigkeit',0,255,5,pct],['aufzug_umk','Richtung umkehren']]},
 {t:'Weiche 1',d:'Flexschiene · XS-Motor an M3',test:['w','Umschalten'],p:[
   ['weiche_pwm','Kraft',0,255,5,pct],['weiche_ms','Stellweg (Fahrzeit bis Anschlag)',20,2000,10,ms],['weiche_umk','Richtung umkehren']]},
 {t:'Weiche 2',d:'Weichenhebel · XS-Motor an M4',test:['h','Umschalten'],p:[
   ['hebel_pwm','Kraft',0,255,5,pct],['hebel_ms','Stellweg (Fahrzeit bis Anschlag)',20,2000,10,ms],['hebel_umk','Richtung umkehren']]},
 {t:'Stoppstelle',d:'Servo an SERVO 1 (D2)',test:['s','Freigeben'],extra:['v','Servo-Test'],arc:1,p:[
   ['stopp_zu','Winkel geschlossen',0,180,1,deg],['stopp_auf','Winkel offen',0,180,1,deg],
   ['stopp_ms','Offen für',30,1500,10,ms],['stopp_tempo','Tempo',0,30,1,v=>v?v*100+' °/s':'maximal'],
   ['stopp_auto','Automatisch freigeben, wenn der Aufzug läuft'],['stopp_takt','Abstand der Freigaben',1000,30000,500,sec],
   ['sensor_an','Automatik gibt nur frei, wenn der Sensor eine Kugel meldet'],['sensor_typ','Erschütterungssensor (Shock) statt Reflexsensor'],
   ['sensor_empf','Shock: Empfindlichkeit',1,10,1,v=>v===10?'10 · maximal':v+''],['sensor_sperr','Shock: Mindestabstand zwischen zwei Kugeln',10,2000,10,ms],['sensor_inv','Reflex: Signal umkehren'],
   ['stopp_nach','Reflex: nach dem Durchrollen noch offen',0,1000,10,ms]]},
 {t:'Lautsprecher',d:'P15 über 470 Ω oder lauter an M6 über 100 Ω',test:['j','An / Aus'],p:[['laut','Lautstärke',0,100,5,v=>v+' %'],['boost','Verstärkung (lauter, etwas verzerrter)',1,10,1,v=>v===1?'aus':v+'×']]},
 {t:'Licht',d:'Rainbow-LEDs an M1',test:['l','An / Aus'],p:[['led_blink','Status als Blinkcode zeigen']]},
 {t:'Überwachung',d:'Störungen und M5Stack-Verbindung',test:['x','Not-Aus'],p:[['stau_s','Kugelstau melden nach',0,300,5,v=>v?v+' s ohne Kugel':'aus'],['m5_waechter','Anlage stoppen, wenn das M5Stack nicht mehr antwortet']]}
];
let CFG={};
function cfgBuild(){
  const L=$('cfg_l');L.innerHTML='';
  ACT.forEach(a=>{
    const el=document.createElement('section');el.className='card act';
    el.innerHTML='<header><div><b></b><small></small></div><span style="display:flex;gap:6px"></span></header>';
    el.querySelector('b').textContent=a.t;el.querySelector('small').textContent=a.d;
    const bs=el.querySelector('header span');
    if(a.extra){const b=document.createElement('button');b.className='sec';b.textContent=a.extra[1];b.onclick=()=>cmd(a.extra[0]);bs.append(b);}
    {const b=document.createElement('button');b.textContent=a.test[1];b.onclick=()=>cmd(a.test[0]);bs.append(b);}
    if(a.arc){const c=document.createElement('canvas');c.className='arc';c.id='arc';el.append(c);}
    a.p.forEach(([k,n,mi,ma,st,f])=>{
      if(mi===undefined){
        const r=document.createElement('div');r.className='tg';r.innerHTML='<span></span><button class="sw"></button>';
        r.firstChild.textContent=n;const b=r.lastChild;b.dataset.k=k;b.setAttribute('aria-label',n);
        b.onclick=()=>cfgSet(k,b.classList.contains('on')?0:1);el.append(r);return;}
      const r=document.createElement('div');r.className='rng';
      r.innerHTML='<label><span></span><output></output></label><input type="range">';
      r.querySelector('span').textContent=n;const i=r.querySelector('input'),o=r.querySelector('output');
      Object.assign(i,{min:mi,max:ma,step:st});i.dataset.k=k;i._o=o;
      i._show=()=>{o.textContent=f(+i.value);i.style.setProperty('--p',((i.value-mi)/(ma-mi)*100)+'%');if(a.arc)arcDraw();};
      i.oninput=i._show;i.onchange=()=>cfgSet(k,+i.value);el.append(r);
    });
    L.append(el);
  });
}
function cfgFill(j){
  for(const k in j)if(Array.isArray(j[k]))CFG[k]=j[k][0];
  if(P1.volSet)P1.volSet(CFG.laut);if(P1.empSet)P1.empSet(CFG.sensor_empf);
  if(CFG.stopp_takt)TAKT=CFG.stopp_takt;
  let chg=0;
  document.querySelectorAll('#cfg_l [data-k]').forEach(e=>{const v=j[e.dataset.k];if(!v)return;const c=v[0]!==v[1];chg+=c;
    if(e.tagName==='INPUT'){if(document.activeElement!==e)e.value=v[0];e._show();e._o.classList.toggle('chg',c);}
    else e.classList.toggle('on',!!v[0]);});
  $('cfg_s').textContent=(j.gespeichert?'Änderungen gelten sofort und bleiben auf dem Giga gespeichert.':'Achtung: Speicher nicht verfügbar, Werte gelten nur bis zum Neustart.')+(chg?' '+chg+' Wert'+(chg>1?'e':'')+' geändert (●).':'');
}
async function cfgGet(q,quiet){try{cfgFill(await (await fetch('/einst'+q,{cache:'no-store'})).json());return true;}catch(e){if(!quiet)toast('Giga nicht erreichbar');return false;}}
function cfgLoad(){if(!$('cfg_l').children.length)cfgBuild();cfgGet('');}
async function cfgSet(k,v){if(await cfgGet('?k='+k+'&v='+v)&&navigator.vibrate)navigator.vibrate(6);}
$('cfg_r').onclick=async()=>{if(await cfgGet('?reset=1'))toast('Startwerte wiederhergestellt');};
/* Servo-Winkel als Bogen */
function arcDraw(){
  const c=$('arc');if(!c)return;const d=DPR(),w=c.clientWidth,h=c.clientHeight;if(!w)return;c.width=w*d;c.height=h*d;const g=c.getContext('2d');g.scale(d,d);
  const val=k=>{const e=document.querySelector('#cfg_l input[data-k="'+k+'"]');return e?+e.value:0;};
  const cx=w/2,cy=h-8,R=Math.min(h-22,w/2-30),A=x=>Math.PI+x/180*Math.PI;
  g.lineWidth=2;g.strokeStyle='rgba(255,255,255,.1)';g.beginPath();g.arc(cx,cy,R,Math.PI,2*Math.PI);g.stroke();
  const z=val('stopp_zu'),o=val('stopp_auf');
  g.strokeStyle='rgba(47,134,255,.35)';g.lineWidth=6;g.beginPath();g.arc(cx,cy,R,A(Math.min(z,o)),A(Math.max(z,o)));g.stroke();
  [[z,'#8a92a3','zu'],[o,'#2f86ff','auf']].forEach(([v,col,n])=>{
    const x=Math.cos(A(v)),y=Math.sin(A(v));
    g.strokeStyle=col;g.lineWidth=4;g.lineCap='round';g.beginPath();g.moveTo(cx,cy);g.lineTo(cx+x*R,cy+y*R);g.stroke();
    g.fillStyle=col;g.font='600 11px '+MONO;g.textAlign='center';g.fillText(n,cx+x*(R+14),cy+y*(R+14)+4);});
  g.fillStyle='#fff';g.beginPath();g.arc(cx,cy,5,0,7);g.fill();
}
addEventListener('keydown',e=>{
  if(e.target.tagName==='INPUT'||e.metaKey||e.ctrlKey||e.altKey||e.repeat)return;
  const k=e.key.toLowerCase();
  if(k===' '){e.preventDefault();cmd('a');}
  else if(k==='escape')tab('home');
  else if(k==='f')view(P1.view==='f'?'o':'f');
  else if(k==='e')tab('cfg');
  else if('123'.includes(k)&&k.length===1)tab(['home','cfg','sys'][+k-1]);
  else if(k.length===1&&'awhslx'.includes(k))cmd(k);
});
$('lamp').onclick=async e=>{const b=e.target.closest('button');if(!b)return;try{render(await get('/lampe?f='+b.dataset.f));}catch(x){toast('Giga nicht erreichbar');}};
$('ghbtn').onclick=async()=>{toast('Update von GitHub gestartet');log('Update von GitHub angefordert');try{await fetch('/github',{cache:'no-store'});}catch(e){}};
$('urlbtn').onclick=async()=>{const u=$('url').value.trim();if(!u)return;toast('Update gestartet');log('Update von '+u);try{await fetch('/update?url='+encodeURIComponent(u));}catch(e){}};
$('url').value='%OTA_URL%';

/* ---------- Lied hochladen: MP3 im Browser in 8 kHz / 8 Bit / mono umwandeln, in Stuecken zum Giga ---------- */
$('up_b').onclick=()=>$('up_f').click();
$('up_d').onclick=async()=>{try{await fetch('/lied?loeschen=1',{cache:'no-store'});toast('Lied gelöscht');poll();}catch(e){toast('Giga nicht erreichbar');}};
$('up_f').onchange=async e=>{
  const f=e.target.files[0];e.target.value='';if(!f)return;
  const say=t=>$('xm_s').textContent=t,P=$('up_p');UPL=true;P.classList.add('on');P.firstChild.style.width='0';
  try{
    say('Wandle um …');
    const ac=new (window.AudioContext||window.webkitAudioContext)();
    const src=await ac.decodeAudioData(await f.arrayBuffer());ac.close();
    const n=Math.floor(src.duration*8000),oc=new OfflineAudioContext(1,n,8000);
    const bs=oc.createBufferSource();bs.buffer=src;bs.connect(oc.destination);bs.start();
    const pcm=(await oc.startRendering()).getChannelData(0);
    let pk=0;for(let i=0;i<pcm.length;i++)pk=Math.max(pk,Math.abs(pcm[i]));const g=pk>0?.98/pk:1;
    const u8=new Uint8Array(pcm.length);for(let i=0;i<pcm.length;i++)u8[i]=Math.max(0,Math.min(255,Math.round(128+pcm[i]*g*127)));
    const CH=16384;
    for(let off=0;off<u8.length;off+=CH){
      let ok=false;
      for(let t=0;t<3&&!ok;t++){try{const r=await fetch('/lied?off='+off,{method:'POST',body:u8.subarray(off,off+CH),cache:'no-store'});ok=r.ok;}catch(x){}}
      if(!ok)throw new Error('Upload abgebrochen');
      const pr=Math.min(1,(off+CH)/u8.length);P.firstChild.style.width=(pr*100)+'%';say('Lade auf den Giga … '+Math.round(pr*100)+' %');
    }
    toast('Lied ist auf dem Giga');
  }catch(x){toast(x.message||'Umwandeln fehlgeschlagen');}
  UPL=false;P.classList.remove('on');poll();
};

/* Lautstaerke direkt auf der Karte: waehrend des Ziehens alle 150 ms senden */
{const i=$('vol'),o=$('vol_o');let tm=0,last=-1;
 const show=()=>{o.textContent=i.value+' %';i.style.setProperty('--p',i.value+'%');};
 const send=()=>{if(+i.value!==last){last=+i.value;fetch('/einst?k=laut&v='+last,{cache:'no-store'}).catch(()=>{});}};
 i.oninput=()=>{show();if(!tm)tm=setTimeout(()=>{tm=0;send();},150);};
 i.onchange=()=>{show();send();};
 P1.volSet=v=>{if(document.activeElement!==i&&v!==undefined){i.value=v;last=v;show();}};}
/* Empfindlichkeit direkt auf der Sensorkarte */
{const i=$('emp'),o=$('emp_o');let tm=0,last=-1;
 const show=()=>{o.textContent=i.value==10?'10 · maximal':i.value;i.style.setProperty('--p',((i.value-1)/9*100)+'%');};
 const send=()=>{if(+i.value!==last){last=+i.value;fetch('/einst?k=sensor_empf&v='+last,{cache:'no-store'}).catch(()=>{});}};
 i.oninput=()=>{show();if(!tm)tm=setTimeout(()=>{tm=0;send();},150);};i.onchange=()=>{show();send();};
 P1.empSet=v=>{if(document.activeElement!==i&&v!==undefined){i.value=v;last=v;show();}};}
/* ---------- Schnee ---------- */
(()=>{
  const c=$('snow'),g=c.getContext('2d');let W=0,H=0,F=[];
  const size=()=>{const d=DPR();W=innerWidth;H=innerHeight;c.width=W*d;c.height=H*d;g.setTransform(d,0,0,d,0,0);
    F=Array.from({length:Math.round(W*H/9000)},()=>({x:Math.random()*W,y:Math.random()*H,r:.8+Math.random()*2.6,v:.3+Math.random()*1.1,p:Math.random()*6.28}));};
  addEventListener('resize',size);size();
  let t0=performance.now();
  (function fr(now){const dt=Math.min(.05,(now-t0)/1000)*60;t0=now;
    if(B.classList.contains('xmas')){g.clearRect(0,0,W,H);g.fillStyle='rgba(255,255,255,.85)';
      for(const f of F){f.p+=.02*dt;f.y+=f.v*dt*(.6+f.r*.25);f.x+=Math.sin(f.p)*.4*dt;if(f.y>H+4){f.y=-4;f.x=Math.random()*W;}
        g.globalAlpha=.35+f.r/4;g.beginPath();g.arc(f.x,f.y,f.r,0,7);g.fill();}g.globalAlpha=1;}
    requestAnimationFrame(fr);})(t0);
})();

cfgGet('',true);   /* Takt der Stoppstelle fuer den Countdown */
/* Sensor-Verlauf der letzten 20 s */
(()=>{const c=$('sen_t'),g=c.getContext('2d'),WIN=20000;
  (function fr(){const d=DPR(),w=c.clientWidth,h=c.clientHeight;if(w){if(c.width!==w*d){c.width=w*d;c.height=h*d;}
    const now=performance.now();while(SEN.length>2&&now-SEN[1][0]>WIN)SEN.shift();
    g.setTransform(d,0,0,d,0,0);g.clearRect(0,0,w,h);g.fillStyle='rgba(255,255,255,.05)';g.fillRect(0,h-6,w,2);
    g.fillStyle='#34d399';
    const X=t=>w*(1-(now-t)/WIN);   /* zusammenhaengende "Kugel"-Abschnitte als ein Balken */
    for(let i=0;i<SEN.length;i++){if(!SEN[i][1]||(i&&SEN[i-1][1]))continue;let j=i;while(j+1<SEN.length&&SEN[j+1][1])j++;
      const x0=Math.max(0,X(SEN[i][0])),x1=j+1<SEN.length?X(SEN[j+1][0]):w;g.beginPath();g.roundRect(x0,4,Math.max(3,x1-x0),h-12,4);g.fill();}
    g.fillStyle='#555c6b';g.font='10px '+MONO;g.fillText('−20 s',0,h);g.textAlign='right';g.fillText('jetzt',w,h);g.textAlign='left';}
    requestAnimationFrame(fr);})();
})();

poll();setInterval(poll,300);   /* ~3x pro Sekunde: Sensor wirkt live */
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
const scene=new THREE.Scene();scene.background=new THREE.Color(0x0b0d12);scene.fog=new THREE.Fog(0x0b0d12,75,150);
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
const HOT={a:V(LIFT_X,5.4,.7),s:N.gate,w:N.lsw,h:N.wsw,l:V(-1,TH+.7,-1.3),k:frameAt('top',SEG.top.len-1.3).p.clone().add(V(0,.9,0))};
const hs={};for(const k in HOT)hs[k]=document.getElementById('hs_'+k);
const OV_T=V(-.5,5.4,0),OV_R=48,FOLLOW_R=17;
const orb={az:.69,el:.24,d:1};
const camPos=V(30,26,34),camTgt=V(0,7,0),wantP=V(0,0,0),wantT=V(0,0,0),v3=V(0,0,0);
const clamp=(x,a,b)=>Math.min(b,Math.max(a,x));
let mx=0,my=0,moved=0,pinch=0,lastUse=-1e9;const ptrs=new Map();
const pd=()=>{const [a,b]=[...ptrs.values()];return Math.hypot(a[0]-b[0],a[1]-b[1])||1;};
const ray=new THREE.Raycaster(),ndc=new THREE.Vector2();
function pick(e){const r=canvas.getBoundingClientRect();ndc.set((e.clientX-r.left)/r.width*2-1,-(e.clientY-r.top)/r.height*2+1);ray.setFromCamera(ndc,cam);const h=ray.intersectObjects(clickable,true)[0];return h?h.object.userData.cmd:null;}
canvas.addEventListener('pointerdown',e=>{lastUse=performance.now();canvas.setPointerCapture(e.pointerId);ptrs.set(e.pointerId,[e.clientX,e.clientY]);moved=0;if(ptrs.size===2)pinch=pd();});
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
canvas.addEventListener('wheel',e=>{e.preventDefault();lastUse=performance.now();orb.d=clamp(orb.d*Math.exp(e.deltaY*.0012),.4,1.3);},{passive:false});
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
  if(introDone&&P1.view!=='f'&&!ptrs.size&&now-lastUse>5000)orb.az+=dt*.07;
  const follow=P1.view==='f'&&introDone,ia=1-Math.min(1,t/(INTRO+.5));   /* ia: Kamera kreist im Intro heran */
  const fit=clamp(1.45/cam.aspect,1,1.9);
  if(follow)wantT.copy(marbles[0].m.position);else{wantT.copy(OV_T);wantT.y+=clamp((1.5-cam.aspect)*4,0,3);}   /* hochkant: Bild hoeher ansetzen */
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
