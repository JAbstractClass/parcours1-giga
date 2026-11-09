#pragma once
// AUTOMATISCH ERZEUGT aus dashboard.html (tools/html2h.py) - nicht von Hand aendern.
static const char HTML_SEITE[] = R"DASH(<!doctype html>
<html lang="de">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover">
<meta name="theme-color" content="#0a0a0a">
<title>PARCOURS 1</title>
<style>
/* Hardware-Look: Schwarz, ein Signal-Orange, Monospace. Keine Verlaeufe. */
:root{
  --bg:#0a0a0a;--panel:#121212;--panel2:#171717;--line:#242424;--line2:#333;
  --text:#ededed;--muted:#8c8c8c;--faint:#555;
  --sig:#ff4f00;
  --mono:"SF Mono",ui-monospace,SFMono-Regular,"JetBrains Mono",Menlo,Consolas,monospace;
  --sans:-apple-system,BlinkMacSystemFont,"SF Pro Display","Helvetica Neue",Helvetica,Arial,sans-serif;
  --ease:cubic-bezier(.16,1,.3,1);
}
*{box-sizing:border-box;margin:0;padding:0}
html{background:var(--bg)}
body{background:var(--bg);color:var(--text);font:14px/1.45 var(--sans);-webkit-font-smoothing:antialiased;overflow-x:hidden}
button{font:inherit;color:inherit;background:none;border:0;cursor:pointer;-webkit-tap-highlight-color:transparent}
.wrap{max-width:1240px;margin:0 auto;padding:0 28px}
.lbl{font-family:var(--mono);font-size:11px;letter-spacing:.14em;text-transform:uppercase;color:var(--muted)}

/* ---------- Kopfzeile ---------- */
header{position:fixed;inset:0 0 auto 0;z-index:30;mix-blend-mode:difference;color:#fff}
header .wrap{height:64px;display:flex;align-items:center;gap:24px;font-family:var(--mono);font-size:12px;letter-spacing:.12em;text-transform:uppercase}
header .grow{flex:1}
.sq{display:inline-block;width:8px;height:8px;background:currentColor;margin-right:10px;vertical-align:1px}
#conn.ok .sq{animation:blink 1s steps(1) infinite}
@keyframes blink{50%{opacity:.25}}

/* ---------- Hero ---------- */
.hero{min-height:100svh;display:flex;flex-direction:column;justify-content:flex-end;padding:96px 28px 0}
.hero-top{display:flex;justify-content:space-between;align-items:flex-end;gap:24px;flex-wrap:wrap;margin-bottom:22px}
.hero-top .lbl b{color:var(--text);font-weight:500}
#dm{display:block;width:100%;height:auto}
.hero-bar{display:grid;grid-template-columns:repeat(5,1fr);border-top:1px solid var(--line);margin-top:26px}
.cell{padding:18px 0 22px 18px;border-right:1px solid var(--line)}
.cell:first-child{padding-left:0}
.cell:last-child{border-right:0}
.cell .v{font:600 30px/1.2 var(--mono);letter-spacing:-.02em;margin-top:6px;height:1.2em;overflow:hidden;white-space:nowrap}
.cell .s{font:11px var(--mono);color:var(--faint);margin-top:8px;letter-spacing:.04em}
/* Rollende Ziffern */
.roll{display:flex}
.roll .d{display:inline-block;height:1.2em;overflow:hidden;line-height:1.2em}
.roll .c{display:flex;flex-direction:column;transition:transform .9s var(--ease)}
.roll .c i{font-style:normal;height:1.2em;line-height:1.2em}
.roll .x{display:inline-block;line-height:1.2em;white-space:pre}

/* ---------- Laufband ---------- */
.band{border-top:1px solid var(--line);border-bottom:1px solid var(--line);overflow:hidden;white-space:nowrap;background:var(--sig);color:#0a0a0a}
.band-in{display:inline-block;font:600 13px/44px var(--mono);letter-spacing:.18em;text-transform:uppercase;will-change:transform}
.band-in span{padding:0 28px}

/* ---------- Abschnitte ---------- */
section.wrap{padding-top:140px;padding-bottom:20px}
.sec{display:grid;grid-template-columns:200px 1fr;gap:28px;align-items:start}
.sec-h{position:sticky;top:88px}
.sec-h .n{font:600 13px var(--mono);color:var(--sig);letter-spacing:.1em}
.sec-h h2{font:700 44px/1 var(--sans);letter-spacing:-.04em;margin-top:10px}
.sec-h p{color:var(--muted);margin-top:14px;font-size:13.5px;max-width:200px}
.panel{background:var(--panel);border:1px solid var(--line);position:relative}
.corner::before,.corner::after{content:"";position:absolute;width:10px;height:10px;border:1px solid var(--muted);z-index:1}
.corner::before{top:-1px;left:-1px;border-right:0;border-bottom:0}
.corner::after{bottom:-1px;right:-1px;border-left:0;border-top:0}
.panel-h{display:flex;justify-content:space-between;align-items:center;padding:12px 16px;border-bottom:1px solid var(--line)}
#track{display:block;width:100%;height:auto;cursor:pointer}

/* Tasten */
.keys{display:grid;grid-template-columns:repeat(3,1fr);gap:12px;padding-bottom:6px}
.key{position:relative;aspect-ratio:1.35;background:var(--panel2);border:1px solid var(--line2);border-radius:10px;padding:16px;text-align:left;
  display:flex;flex-direction:column;justify-content:space-between;
  box-shadow:0 6px 0 #050505,0 7px 0 var(--line);transition:transform .09s ease,box-shadow .09s ease,background .2s,opacity 1s var(--ease)}
.key:hover{background:#1b1b1b}
.key:active,.key.press{transform:translateY(5px)!important;box-shadow:0 1px 0 #050505,0 2px 0 var(--line)}
.key .row{display:flex;justify-content:space-between;align-items:center}
.led{width:12px;height:12px;border-radius:2px;background:#262626;box-shadow:inset 0 0 0 1px #000;transition:background .15s,box-shadow .25s}
.key.on .led{background:var(--sig);box-shadow:0 0 14px 2px rgba(255,79,0,.55)}
.key .k{font:600 11px var(--mono);color:var(--faint);border:1px solid var(--line2);border-radius:4px;padding:1px 6px}
.key .t{font:600 19px/1.1 var(--sans);letter-spacing:-.02em}
.key .st{font:11px var(--mono);color:var(--muted);letter-spacing:.1em;text-transform:uppercase;margin-top:6px}
.key .pos{display:flex;gap:4px}
.key .pos i{width:22px;height:6px;background:#262626;border-radius:1px;transition:background .2s}
.key .pos i.a{background:var(--sig)}
.key.red{background:var(--sig);border-color:var(--sig);color:#0a0a0a;box-shadow:0 6px 0 #7a2600,0 7px 0 #000}
.key.red:hover{background:#ff5f17}
.key.red:active,.key.red.press{box-shadow:0 1px 0 #7a2600,0 2px 0 #000}
.key.red .k{color:#0a0a0a;border-color:rgba(0,0,0,.35)}
.key.red .st{color:rgba(0,0,0,.6)}
.key.busy .led{animation:blink .25s steps(1) infinite;background:var(--sig)}

/* Oszilloskop */
#scope{display:block;width:100%;height:220px}
.scope-legend{display:flex;justify-content:space-between;padding:8px 16px 12px 126px;border-top:1px solid var(--line)}

/* Log + Firmware */
.two{display:grid;grid-template-columns:1.2fr 1fr;gap:12px}
.log{font:12.5px/1.75 var(--mono);padding:14px 16px;height:300px;overflow:auto}
.log div{display:flex;gap:14px;white-space:nowrap;padding:0 4px}
.log div span:first-child{color:var(--faint)}
.log div.new{animation:flash 1.4s var(--ease)}
@keyframes flash{0%{background:var(--sig);color:#000}100%{background:transparent}}
.kv{font:12.5px var(--mono)}
.kv div{display:flex;justify-content:space-between;gap:12px;padding:10px 16px;border-bottom:1px solid var(--line)}
.kv div span:first-child{color:var(--muted);text-transform:uppercase;letter-spacing:.1em;font-size:11px}
.kv div span:last-child{overflow:hidden;text-overflow:ellipsis;white-space:nowrap}
.tag{color:var(--sig)}
.fwbtns{padding:14px 16px;display:grid;gap:10px}
.bigbtn{height:48px;border:1px solid var(--text);font:600 12px var(--mono);letter-spacing:.16em;text-transform:uppercase;position:relative;overflow:hidden}
.bigbtn::before{content:"";position:absolute;inset:0;background:var(--text);transform:scaleX(0);transform-origin:left;transition:transform .45s var(--ease)}
.bigbtn:hover::before{transform:scaleX(1)}
.bigbtn span{position:relative;mix-blend-mode:difference;color:#fff}
details summary{font:11px var(--mono);letter-spacing:.12em;text-transform:uppercase;color:var(--muted);cursor:pointer;list-style:none}
details summary::-webkit-details-marker{display:none}
details input{width:100%;height:40px;margin:10px 0;padding:0 12px;background:var(--bg);color:var(--text);border:1px solid var(--line2);font:12px var(--mono)}
#otamsg{font:12px var(--mono);color:var(--sig)}
#otamsg:empty{display:none}

footer{padding:120px 0 60px}
footer .big{font:800 clamp(60px,13vw,200px)/.85 var(--sans);letter-spacing:-.06em;color:#161616;user-select:none}
footer .lbl{margin-top:20px}

/* ---------- Scroll-Einblendungen ---------- */
.wipe{clip-path:inset(0 0 100% 0);transition:clip-path 1.1s var(--ease)}
.wipe.in{clip-path:inset(0 0 0 0)}
.up{opacity:0;transform:translateY(40px);transition:opacity 1s var(--ease),transform 1s var(--ease),box-shadow .09s,background .2s;transition-delay:calc(var(--i,0)*70ms)}
.up.in{opacity:1;transform:none}

#toast{position:fixed;left:28px;bottom:28px;z-index:40;background:var(--sig);color:#0a0a0a;font:600 12px var(--mono);letter-spacing:.12em;text-transform:uppercase;padding:12px 16px;
  transform:translateY(20px);opacity:0;visibility:hidden;transition:transform .5s var(--ease),opacity .3s,visibility .3s}
#toast.show{transform:none;opacity:1;visibility:visible}

@media (max-width:980px){
  .sec{grid-template-columns:1fr}.sec-h{position:static}.sec-h p{max-width:none}
  .two{grid-template-columns:1fr}
  .hero-bar{grid-template-columns:repeat(2,1fr)}
  .cell{border-bottom:1px solid var(--line);padding-left:0}
  .cell:nth-child(even){padding-left:18px;border-right:0}
}
@media (max-width:600px){
  .wrap{padding:0 16px}.hero{padding:96px 16px 0}
  .keys{grid-template-columns:1fr 1fr}
  .key{aspect-ratio:1.1}.key .k{display:none}
  .sec-h h2{font-size:34px}
  section.wrap{padding-top:90px;padding-bottom:10px}
  .cell .v{font-size:22px}
  .hide-sm{display:none}
}
@media (prefers-reduced-motion:reduce){*{transition:none!important;animation:none!important}.wipe{clip-path:none}.up{opacity:1;transform:none}}
</style>
</head>
<body>
<header><div class="wrap">
 <span>Parcours 1</span><span class="hide-sm" style="opacity:.5">Dynamic XXL · Giga R1</span>
 <span class="grow"></span>
 <span id="conn"><span class="sq"></span><span id="conntxt">Verbinde</span></span>
</div></header>

<main>
 <div class="hero" style="max-width:1240px;margin:0 auto">
  <div class="hero-top">
   <div class="lbl">Status <b id="sline">—</b></div>
   <div class="lbl hide-sm">Scroll ↓</div>
  </div>
  <canvas id="dm" aria-label="Status"></canvas>
  <div class="hero-bar">
   <div class="cell"><div class="lbl">Aufzug-Laufzeit</div><div class="v roll" id="k_run"></div><div class="s">diese Sitzung</div></div>
   <div class="cell"><div class="lbl">Giga online</div><div class="v roll" id="k_up"></div><div class="s">seit Neustart</div></div>
   <div class="cell"><div class="lbl">WLAN dBm</div><div class="v roll" id="k_rssi"></div><div class="s" id="k_lat">— ms</div></div>
   <div class="cell"><div class="lbl">Schaltungen</div><div class="v roll" id="k_sw"></div><div class="s">Weichen + Stopp</div></div>
   <div class="cell"><div class="lbl">Daten Giga</div><div class="v roll" id="k_dat"></div><div class="s" id="k_dat_s">geschätzt</div></div>
  </div>
 </div>

 <div class="band"><div class="band-in" id="band"></div></div>

 <section class="wrap"><div class="sec">
  <div class="sec-h up"><div class="n">01</div><h2>Bahn</h2><p>Live-Simulation nach echtem Zustand. Antippen schaltet Weichen, Stoppstelle, LEDs und Aufzug.</p></div>
  <div class="panel corner wipe"><canvas id="track"></canvas></div>
 </div></section>

 <section class="wrap"><div class="sec">
  <div class="sec-h up"><div class="n">02</div><h2>Steuerung</h2><p>Tasten oder Tastatur: A W H S L, X für Stopp.</p></div>
  <div class="keys">
   <button class="key up" style="--i:0" data-c="a" id="c_a"><div class="row"><span class="led"></span><span class="k">A</span></div><div><div class="t">Kettenaufzug</div><div class="st" id="s_a">aus</div></div></button>
   <button class="key up" style="--i:1" data-c="w" id="c_w"><div class="row"><span class="pos"><i id="w1"></i><i id="w2"></i></span><span class="k">W</span></div><div><div class="t">Stellweiche</div><div class="st" id="s_w">Stellung 1</div></div></button>
   <button class="key up" style="--i:2" data-c="h" id="c_h"><div class="row"><span class="pos"><i id="h1"></i><i id="h2"></i></span><span class="k">H</span></div><div><div class="t">Weichenhebel</div><div class="st" id="s_h">Stellung 1</div></div></button>
   <button class="key up" style="--i:3" data-c="s" id="c_s"><div class="row"><span class="led"></span><span class="k">S</span></div><div><div class="t">Stoppstelle</div><div class="st" id="s_s">zu</div></div></button>
   <button class="key up" style="--i:4" data-c="l" id="c_l"><div class="row"><span class="led"></span><span class="k">L</span></div><div><div class="t">LEDs</div><div class="st" id="s_l">aus</div></div></button>
   <button class="key red up" style="--i:5" data-c="x"><div class="row"><span></span><span class="k">X</span></div><div><div class="t">Alles stoppen</div><div class="st">Motoren aus</div></div></button>
  </div>
 </div></section>

 <section class="wrap"><div class="sec">
  <div class="sec-h up"><div class="n">03</div><h2>Signal</h2><p>Zustand aller Ausgänge, die letzten 2 Minuten.</p></div>
  <div class="panel corner wipe"><canvas id="scope"></canvas><div class="scope-legend lbl"><span>−120 s</span><span>−60 s</span><span>jetzt</span></div></div>
 </div></section>

 <section class="wrap"><div class="sec">
  <div class="sec-h up"><div class="n">04</div><h2>System</h2><p>Ereignisse von allen Geräten und die laufende Firmware.</p></div>
  <div class="two">
   <div class="panel corner wipe"><div class="panel-h lbl"><span>Log</span><span id="logc">000</span></div><div class="log" id="log"></div></div>
   <div class="panel corner wipe">
    <div class="panel-h lbl"><span>Firmware</span><span class="tag" id="f_auto">—</span></div>
    <div class="kv">
     <div><span>Läuft</span><span id="f_ver">—</span></div>
     <div><span>GitHub</span><span id="f_gh">—</span></div>
     <div><span>Gebaut</span><span id="f_date">—</span></div>
     <div><span>Adresse</span><span id="f_ip">—</span></div>
    </div>
    <div class="fwbtns">
     <div id="otamsg"></div>
     <button class="bigbtn" id="ghbtn"><span>Von GitHub laden</span></button>
     <details><summary>+ Eigene Adresse</summary><input id="url" spellcheck="false" autocomplete="off"><button class="bigbtn" id="urlbtn" style="width:100%"><span>Update starten</span></button></details>
    </div>
   </div>
  </div>
 </div></section>
</main>

<footer class="wrap"><div class="big">PARCOURS</div><div class="lbl">github.com/JAbstractClass/parcours1-giga</div></footer>
<div id="toast"></div>

<script>
(()=>{
const $=id=>document.getElementById(id);
const DPR=()=>Math.min(window.devicePixelRatio||1,3);
const SIG='#ff4f00';
const MONO=getComputedStyle(document.documentElement).getPropertyValue('--mono');
let S=null,lastOk=0,lat=0,runMs=0,runSince=null,swc=0,up=null;
const reduce=matchMedia('(prefers-reduced-motion: reduce)').matches;

/* =========== Punktmatrix-Anzeige =========== */
const G={
 A:["01110","10001","10001","11111","10001","10001","10001"],B:["11110","10001","10001","11110","10001","10001","11110"],
 D:["11100","10010","10001","10001","10001","10010","11100"],E:["11111","10000","10000","11110","10000","10000","11111"],
 F:["11111","10000","10000","11110","10000","10000","10000"],I:["01110","00100","00100","00100","00100","00100","01110"],
 L:["10000","10000","10000","10000","10000","10000","11111"],N:["10001","11001","10101","10011","10001","10001","10001"],
 O:["01110","10001","10001","10001","10001","10001","01110"],P:["11110","10001","10001","11110","10000","10000","10000"],
 R:["11110","10001","10001","11110","10100","10010","10001"],S:["01111","10000","10000","01110","00001","00001","11110"],
 T:["11111","00100","00100","00100","00100","00100","00100"],U:["10001","10001","10001","10001","10001","10001","01110"],
 "Ä":["01010","00000","01110","10001","11111","10001","10001"]," ":["00000","00000","00000","00000","00000","00000","00000"]
};
const COLS=7*6-1,ROWS=7;
const dm=$('dm'),dg=dm.getContext('2d');
const dots=Array.from({length:COLS*ROWS},()=>({v:0,t:0,delay:0}));
let dmText='',dmHot=null;
function dmSet(txt,hot){
  if(txt===dmText&&hot===dmHot)return;dmText=txt;dmHot=hot;
  const bits=new Array(COLS*ROWS).fill(0);
  [...txt].forEach((ch,i)=>{const g=G[ch]||G[' '];for(let r=0;r<7;r++)for(let c=0;c<5;c++)if(g[r][c]==='1'){const x=i*6+c;if(x<COLS)bits[r*COLS+x]=1;}});
  const now=performance.now();
  dots.forEach((d,i)=>{const x=i%COLS;if(d.t!==bits[i]){d.t=bits[i];d.delay=now+x*14+Math.random()*140;}});
}
function dmSize(){const w=dm.clientWidth;dm.width=Math.round(w*DPR());dm.height=Math.round(w*(ROWS/COLS)*DPR());}
function dmDraw(now){
  const w=dm.width,h=dm.height,p=w/COLS,r=p*.36;dg.clearRect(0,0,w,h);
  dg.fillStyle='#181818';
  for(let i=0;i<dots.length;i++){const x=(i%COLS+.5)*p,y=(Math.floor(i/COLS)+.5)*p;dg.beginPath();dg.arc(x,y,r,0,7);dg.fill();}
  dg.fillStyle=dmHot?SIG:'#ededed';
  for(let i=0;i<dots.length;i++){
    const d=dots[i];if(now>d.delay)d.v+=(d.t-d.v)*(reduce?1:.2);
    if(d.v>.02){const x=(i%COLS+.5)*p,y=(Math.floor(i/COLS)+.5)*p;dg.globalAlpha=Math.min(1,d.v);dg.beginPath();dg.arc(x,y,r*(.7+.3*d.v),0,7);dg.fill();}
  }
  dg.globalAlpha=1;
}

/* =========== Rollende Ziffern =========== */
function roll(el,str){
  if(el._s===str)return;
  const pat=str.replace(/\d/g,'#');
  if(el._p!==pat){
    el._p=pat;el.innerHTML=[...str].map(ch=>/\d/.test(ch)?'<span class="d"><span class="c">'+'0123456789'.split('').map(n=>'<i>'+n+'</i>').join('')+'</span></span>':'<span class="x">'+ch+'</span>').join('');
    el.offsetWidth;
  }
  const cols=el.querySelectorAll('.c');let k=0;
  [...str].forEach(ch=>{if(/\d/.test(ch)){cols[k].style.transform='translateY(-'+(+ch*1.2)+'em)';k++;}});
  el._s=str;
}

/* =========== Bahn-Simulation =========== */
const tc=$('track'),tg=tc.getContext('2d');const VW=1000,VH=430;
const SEG={
 lift:[[90,370],[90,70]],
 top:[[90,70],[300,70],[380,150]],
 g2w:[[380,150],[540,150]],
 w1:[[540,150],[660,260]],
 w2:[[540,150],[720,80],[880,80],[910,110],[910,240],[870,275],[660,260]],
 h1:[[660,260],[620,370],[90,370]],
 h2:[[660,260],[790,395],[140,395],[90,370]]
};
const NODE={gate:[380,150],w:[540,150],h:[660,260],lift:[90,220],leds:[240,265]};
for(const k in SEG){const p=SEG[k];const L=[0];for(let i=1;i<p.length;i++)L.push(L[i-1]+Math.hypot(p[i][0]-p[i-1][0],p[i][1]-p[i-1][1]));SEG[k]={p,L,len:L[L.length-1]};}
function at(seg,s){const g=SEG[seg];s=Math.max(0,Math.min(g.len,s));let i=1;while(i<g.L.length-1&&g.L[i]<s)i++;const a=g.p[i-1],b=g.p[i],f=(s-g.L[i-1])/((g.L[i]-g.L[i-1])||1);return[a[0]+(b[0]-a[0])*f,a[1]+(b[1]-a[1])*f];}
const ang=(a,b)=>Math.atan2(b[1]-a[1],b[0]-a[0]);
const A_W1=ang(SEG.w1.p[0],SEG.w1.p[1]),A_W2=ang(SEG.w2.p[0],SEG.w2.p[1]),A_H1=ang(SEG.h1.p[0],SEG.h1.p[1]),A_H2=ang(SEG.h2.p[0],SEG.h2.p[1]);
const marbles=[];
for(let i=0;i<4;i++)marbles.push({seg:'lift',s:30+i*70});
for(let i=0;i<3;i++)marbles.push({seg:'h1',s:SEG.h1.len-20-i*30});
const anim={gate:0,w:A_W1,h:A_H1,chain:0,leds:0};
const GAP=22,ROLL=190,LIFT=70;
function nextSeg(seg){
  if(seg==='lift')return'top';if(seg==='top')return'g2w';
  if(seg==='g2w')return S&&S.weiche===2?'w2':'w1';
  if(seg==='w1'||seg==='w2')return S&&S.hebel===2?'h2':'h1';
  return'lift';
}
function step(dt){
  const run=!!(S&&S.aufzug),open=!!(S&&S.stopp);
  const by={};marbles.forEach(m=>(by[m.seg]=by[m.seg]||[]).push(m));
  for(const k in by){
    const list=by[k].sort((a,b)=>b.s-a.s),len=SEG[k].len;
    list.forEach((m,idx)=>{
      if(m.seg!==k)return;
      const v=k==='lift'?(run?LIFT:0):ROLL;
      let lim=len;
      if(idx>0&&list[idx-1].seg===k)lim=Math.min(lim,list[idx-1].s-GAP);
      let ns=Math.max(m.s,Math.min(m.s+v*dt,lim));
      if(ns>=len){
        const hold=(k==='top'&&!open)||((k==='h1'||k==='h2')&&!run);
        const nx=nextSeg(k);
        const blocked=marbles.some(o=>o!==m&&o.seg===nx&&o.s<GAP);
        if(hold||blocked)ns=len;else{m.seg=nx;ns=Math.min(ns-len,GAP*.5);}
      }
      m.s=ns;
    });
  }
  const e=1-Math.pow(.0004,dt);
  anim.gate+=((open?1:0)-anim.gate)*e;
  anim.w+=((S&&S.weiche===2?A_W2:A_W1)-anim.w)*e;
  anim.h+=((S&&S.hebel===2?A_H2:A_H1)-anim.h)*e;
  anim.leds+=((S&&S.leds?1:0)-anim.leds)*e;
  if(run)anim.chain=(anim.chain+LIFT*dt)%24;
}
function tSize(){const w=tc.clientWidth;tc.width=Math.round(w*DPR());tc.height=Math.round(w*(VH/VW)*DPR());}
function path(pts){tg.beginPath();tg.moveTo(pts[0][0],pts[0][1]);for(let i=1;i<pts.length;i++)tg.lineTo(pts[i][0],pts[i][1]);}
function label(t,x,y,al,col){tg.font='600 12px '+MONO;tg.fillStyle=col||'#8c8c8c';tg.textAlign=al||'center';tg.fillText(t.toUpperCase(),x,y);}
function tDraw(now){
  const sc=tc.width/VW;tg.setTransform(sc,0,0,sc,0,0);tg.clearRect(0,0,VW,VH);
  tg.lineCap='round';tg.lineJoin='round';
  tg.fillStyle='#1c1c1c';for(let x=20;x<VW;x+=40)for(let y=20;y<VH;y+=40)tg.fillRect(x-1,y-1,2,2);
  for(const k in SEG){if(k==='lift')continue;path(SEG[k].p);tg.strokeStyle='#272727';tg.lineWidth=18;tg.stroke();tg.strokeStyle='#0d0d0d';tg.lineWidth=12;tg.stroke();}
  const route=['top','g2w',S&&S.weiche===2?'w2':'w1',S&&S.hebel===2?'h2':'h1'];
  tg.setLineDash([2,9]);tg.lineDashOffset=S&&S.aufzug?-now/30:0;tg.strokeStyle=SIG;tg.lineWidth=2.5;
  route.forEach(k=>{path(SEG[k].p);tg.stroke();});tg.setLineDash([]);
  /* Aufzug */
  tg.fillStyle='#151515';tg.strokeStyle='#333';tg.lineWidth=1.5;tg.beginPath();tg.roundRect(62,46,56,346,8);tg.fill();tg.stroke();
  tg.fillStyle=S&&S.aufzug?SIG:'#2a2a2a';
  for(let y=380-anim.chain;y>58;y-=24)tg.fillRect(80,y-4,20,6);
  label('Aufzug',90,420);
  /* Stoppstelle */
  const[gx,gy]=NODE.gate;tg.save();tg.translate(gx,gy);
  tg.fillStyle='#121212';tg.strokeStyle='#3a3a3a';tg.lineWidth=1.5;tg.beginPath();tg.arc(0,0,26,0,7);tg.fill();tg.stroke();
  tg.rotate(Math.PI/4);tg.fillStyle=anim.gate>.5?'#ededed':SIG;tg.fillRect(-4,-20-anim.gate*20,8,40-anim.gate*14);tg.restore();
  label('Stopp',gx,gy+50);label(S&&S.stopp?'offen':'zu',gx,gy+66,'center','#ededed');
  /* Weichen */
  [['w','Weiche',anim.w,S?S.weiche:1,0,-40,'center'],['h','Hebel',anim.h,S?S.hebel:1,40,-24,'left']].forEach(([k,n,a,v,dx,dy,al])=>{
    const[x,y]=NODE[k];tg.fillStyle='#121212';tg.strokeStyle='#3a3a3a';tg.lineWidth=1.5;tg.beginPath();tg.arc(x,y,24,0,7);tg.fill();tg.stroke();
    tg.strokeStyle='#ededed';tg.lineWidth=4;tg.beginPath();tg.moveTo(x,y);tg.lineTo(x+Math.cos(a)*19,y+Math.sin(a)*19);tg.stroke();
    tg.fillStyle=SIG;tg.beginPath();tg.arc(x,y,4,0,7);tg.fill();
    label(n,x+dx,y+dy,al);label('Stellung '+v,x+dx,y+dy+16,al,'#ededed');
  });
  /* LEDs */
  const[lx,ly]=NODE.leds;
  [-18,18].forEach(o=>{tg.fillStyle='#1d1d1d';tg.fillRect(lx+o-11,ly-11,22,22);
    if(anim.leds>.02){tg.globalAlpha=Math.min(1,anim.leds);tg.shadowColor=SIG;tg.shadowBlur=26*anim.leds;tg.fillStyle=SIG;tg.fillRect(lx+o-9,ly-9,18,18);tg.shadowBlur=0;tg.globalAlpha=1;}});
  label('LEDs',lx,ly+36);
  /* Kugeln */
  marbles.forEach(m=>{const[x,y]=at(m.seg,m.s);
    tg.fillStyle='rgba(0,0,0,.55)';tg.beginPath();tg.arc(x+2,y+3,8,0,7);tg.fill();
    tg.fillStyle='#d6d6d6';tg.beginPath();tg.arc(x,y,8,0,7);tg.fill();
    tg.fillStyle='#fff';tg.beginPath();tg.arc(x-2.6,y-2.6,2.6,0,7);tg.fill();});
}
tc.addEventListener('click',e=>{
  const r=tc.getBoundingClientRect(),x=(e.clientX-r.left)/r.width*VW,y=(e.clientY-r.top)/r.height*VH;
  const hit=[['s',NODE.gate,42],['w',NODE.w,42],['h',NODE.h,42],['l',NODE.leds,46],['a',NODE.lift,70]].find(([,p,rr])=>Math.hypot(x-p[0],y-p[1])<rr);
  if(hit)cmd(hit[0]);
});

/* =========== Oszilloskop =========== */
const sc=$('scope'),sg=sc.getContext('2d');const WIN=120000;
const CH=[['Aufzug',s=>s.aufzug],['Stopp',s=>s.stopp],['Weiche',s=>s.weiche===2],['Hebel',s=>s.hebel===2],['LEDs',s=>s.leds]];
const samples=[];
function sSize(){sc.width=Math.round(sc.clientWidth*DPR());sc.height=Math.round(sc.clientHeight*DPR());}
function sDraw(now){
  const d=DPR(),w=sc.width,h=sc.height;sg.clearRect(0,0,w,h);
  const L=110*d,rowH=h/CH.length,X=t=>L+(w-L)*(1-(now-t)/WIN);
  sg.strokeStyle='#1d1d1d';sg.lineWidth=1;
  for(let i=0;i<=12;i++){const x=Math.round(L+(w-L)*i/12)+.5;sg.beginPath();sg.moveTo(x,0);sg.lineTo(x,h);sg.stroke();}
  sg.font=(11*d)+'px '+MONO;sg.textAlign='left';
  CH.forEach(([n,f],ci)=>{
    const y0=ci*rowH,hi=y0+rowH*.28,lo=y0+rowH*.72;
    sg.fillStyle='#8c8c8c';sg.fillText(n.toUpperCase(),16*d,y0+rowH/2+4*d);
    sg.strokeStyle='#1d1d1d';sg.lineWidth=1;sg.beginPath();sg.moveTo(0,Math.round(y0+rowH)+.5);sg.lineTo(w,Math.round(y0+rowH)+.5);sg.stroke();
    sg.strokeStyle=SIG;sg.lineWidth=2*d;sg.lineJoin='miter';sg.beginPath();let on=false,py=0;
    for(let i=0;i<samples.length;i++){
      const sm=samples[i],x0=X(sm.t),x1=i+1<samples.length?X(samples[i+1].t):w;
      if(x1<L)continue;
      if(!sm.s){on=false;continue;}
      const y=f(sm.s)?hi:lo,xa=Math.max(x0,L);
      if(!on){sg.moveTo(xa,y);on=true;}else if(y!==py){sg.lineTo(xa,py);sg.lineTo(xa,y);}
      sg.lineTo(Math.min(x1,w),y);py=y;
    }
    sg.stroke();
    if(S){sg.fillStyle=SIG;sg.beginPath();sg.arc(w-3*d,f(S)?hi:lo,3*d,0,7);sg.fill();}
  });
}
setInterval(()=>{samples.push({t:performance.now(),s:S&&Date.now()-lastOk<4000?S:null});while(samples.length&&performance.now()-samples[0].t>WIN+5000)samples.shift();},250);

/* =========== Laufband =========== */
const band=$('band');let bx=0,lastY=scrollY,vel=0;
function bandText(){
  const s=S;const parts=s?['Aufzug '+(s.aufzug?'läuft':'aus'),'Weiche '+s.weiche,'Hebel '+s.hebel,'Stopp '+(s.stopp?'offen':'zu'),'LEDs '+(s.leds?'an':'aus'),'Firmware '+s.version,s.rssi+' dBm','Giga '+s.ip]:['Verbinde mit dem Giga','Parcours 1','Dynamic XXL'];
  const one=parts.map(p=>'<span>'+p+'</span><span>■</span>').join('');band.innerHTML=one+one+one+one;
}

/* =========== Hauptschleife: laeuft mit der Bildrate des Displays (120 Hz auf ProMotion) =========== */
let last=performance.now();
function frame(now){
  const dt=Math.min(.05,(now-last)/1000);last=now;
  step(dt);tDraw(now);dmDraw(now);sDraw(now);
  const y=scrollY;vel+=((y-lastY)/Math.max(dt,.001)-vel)*.12;lastY=y;
  bx-=(70+Math.min(3000,Math.abs(vel))*.7)*dt;const bw=band.scrollWidth/4;if(bw>0)while(-bx>bw)bx+=bw;band.style.transform='translate3d('+bx.toFixed(2)+'px,0,0)';
  requestAnimationFrame(frame);
}
function sizes(){dmSize();tSize();sSize();}
addEventListener('resize',sizes);sizes();requestAnimationFrame(frame);

/* =========== Anzeige =========== */
const dur=s=>{s=Math.max(0,Math.floor(s));const h=Math.floor(s/3600),m=Math.floor(s%3600/60),x=s%60;return(h?h+':':'')+String(m).padStart(2,'0')+':'+String(x).padStart(2,'0')};
let tt;function toast(m){const t=$('toast');t.textContent=m;t.classList.add('show');clearTimeout(tt);tt=setTimeout(()=>t.classList.remove('show'),2400);}
let logn=0;function log(m){const l=$('log');const d=document.createElement('div');d.className='new';d.innerHTML='<span>'+new Date().toLocaleTimeString('de-DE')+'</span><span></span>';d.lastChild.textContent=m;l.prepend(d);while(l.children.length>60)l.lastChild.remove();$('logc').textContent=String(++logn).padStart(3,'0');}
function render(s){
  const p=S;S=s;
  dmSet(s.aufzug?'LÄUFT':'BEREIT',s.aufzug);
  $('sline').textContent=s.aufzug?'Aufzug läuft · Stoppstelle taktet':'Bahn steht · bereit';
  const k=(id,on)=>$(id).classList.toggle('on',on);
  k('c_a',s.aufzug);k('c_s',s.stopp);k('c_l',s.leds);
  $('s_a').textContent=s.aufzug?'läuft':'aus';$('s_s').textContent=s.stopp?'offen':'zu';$('s_l').textContent=s.leds?'an':'aus';
  $('s_w').textContent='Stellung '+s.weiche;$('s_h').textContent='Stellung '+s.hebel;
  $('w1').className=s.weiche===1?'a':'';$('w2').className=s.weiche===2?'a':'';
  $('h1').className=s.hebel===1?'a':'';$('h2').className=s.hebel===2?'a':'';
  if(s.aufzug&&!runSince)runSince=Date.now();if(!s.aufzug&&runSince){runMs+=Date.now()-runSince;runSince=null;}
  up={v:s.uptime,at:Date.now()};
  roll($('k_rssi'),s.rssi?String(s.rssi):'0');$('k_lat').textContent=lat+' ms Antwort';
  if(s.daten!==undefined){const b=s.daten+s.abrufe*7000;roll($('k_dat'),b<1e6?Math.round(b/1e3)+' KB':(b/1e6).toFixed(1)+' MB');
    $('k_dat_s').textContent=s.uptime>60?'≈ '+(b/s.uptime*3.6/1e3).toFixed(1)+' MB/h':'geschätzt';}
  $('f_ver').textContent=s.version;$('f_gh').textContent=s.github||'—';$('f_date').textContent=s.datum;$('f_ip').textContent=s.ip;
  $('f_auto').textContent=!s.auto?'Auto-Update aus':(s.github&&s.github!==s.version?'Update folgt':'Auto-Update an');
  $('otamsg').textContent=s.ota||'';
  if(p){
    if(p.version!==s.version){log('Neue Firmware '+s.version);toast('Neue Firmware '+s.version);}
    if(p.aufzug!==s.aufzug)log(s.aufzug?'Aufzug gestartet':'Aufzug gestoppt');
    if(p.weiche!==s.weiche){log('Weiche → Stellung '+s.weiche);swc++;}
    if(p.hebel!==s.hebel){log('Hebel → Stellung '+s.hebel);swc++;}
    if(p.stopp!==s.stopp){log(s.stopp?'Stoppstelle offen':'Stoppstelle zu');swc++;}
    if(p.leds!==s.leds)log(s.leds?'LEDs an':'LEDs aus');
    if(p.ota!==s.ota&&s.ota)log(s.ota);
  }else log('Verbunden · '+s.ip);
  roll($('k_sw'),String(swc));
  if(!p||p.aufzug!==s.aufzug||p.weiche!==s.weiche||p.hebel!==s.hebel||p.stopp!==s.stopp||p.leds!==s.leds||p.version!==s.version)bandText();
}
setInterval(()=>{
  roll($('k_run'),dur((runMs+(runSince?Date.now()-runSince:0))/1000));
  if(up)roll($('k_up'),dur(up.v+(Date.now()-up.at)/1000));
  const ok=Date.now()-lastOk<4000;$('conn').className=ok?'ok':'';
  $('conntxt').textContent=ok?'Live '+lat+' ms':(lastOk?'Verbindung weg':'Verbinde');
  if(!ok&&lastOk)dmSet('OFFLINE',true);
},1000);
roll($('k_run'),'00:00');roll($('k_up'),'00:00');roll($('k_rssi'),'0');roll($('k_sw'),'0');roll($('k_dat'),'0 KB');
dmSet('BEREIT',false);bandText();

/* =========== Kommunikation =========== */
async function get(u){const t=performance.now();const r=await fetch(u,{cache:'no-store'});const j=await r.json();lat=Math.round(performance.now()-t);lastOk=Date.now();return j;}
async function poll(){try{render(await get('/status'));}catch(e){}}
async function cmd(c){
  const key=document.querySelector('.key[data-c="'+c+'"]');
  if(key){key.classList.add('press','busy');setTimeout(()=>key.classList.remove('press'),130);}
  try{render(await get('/cmd?c='+c));if(c==='x')toast('Alles gestoppt');}
  catch(e){toast('Giga nicht erreichbar');}
  finally{if(key)setTimeout(()=>key.classList.remove('busy'),c==='w'||c==='h'?400:150);}
}
document.querySelectorAll('.key[data-c]').forEach(el=>el.addEventListener('click',()=>cmd(el.dataset.c)));
addEventListener('keydown',e=>{if(e.target.tagName==='INPUT'||e.metaKey||e.ctrlKey||e.altKey||e.repeat)return;const k=e.key.toLowerCase();if(k.length===1&&'awhslx'.includes(k))cmd(k);});
$('ghbtn').onclick=async()=>{toast('Update von GitHub gestartet');log('Update von GitHub angefordert');try{await fetch('/github',{cache:'no-store'});}catch(e){}};
$('urlbtn').onclick=async()=>{const u=$('url').value.trim();if(!u)return;toast('Update gestartet');log('Update von '+u);try{await fetch('/update?url='+encodeURIComponent(u));}catch(e){}};
$('url').value='%OTA_URL%';

/* =========== Scroll-Einblendungen =========== */
/* beobachtet wird der Abschnitt: ein per clip-path verstecktes Element gilt sonst nie als sichtbar */
const io=new IntersectionObserver(es=>es.forEach(e=>{if(e.isIntersecting){e.target.querySelectorAll('.wipe,.up').forEach(el=>el.classList.add('in'));io.unobserve(e.target);}}),{threshold:.12});
document.querySelectorAll('section').forEach(el=>io.observe(el));

poll();setInterval(poll,1000);
})();
</script>
</body>
</html>
)DASH";
