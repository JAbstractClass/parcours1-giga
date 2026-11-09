#pragma once
// AUTOMATISCH ERZEUGT aus dashboard.html (tools/html2h.py) - nicht von Hand aendern.
static const char HTML_SEITE[] = R"DASH(<!doctype html>
<html lang="de">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover">
<meta name="theme-color" content="#f1f1ef">
<title>Parcours 1</title>
<script type="importmap">{"imports":{"three":"https://cdn.jsdelivr.net/npm/three@0.160.0/build/three.module.js","three/addons/":"https://cdn.jsdelivr.net/npm/three@0.160.0/examples/jsm/"}}</script>
<style>
/* Hell wie Lusion/Daylight, 3D-Buehne dunkel, Akzent = fischertechnik-Rot */
:root{
  --bg:#f1f1ef;--ink:#111;--muted:#6d6d6a;--faint:#a8a8a3;--line:#dcdcd7;--card:#fff;
  --red:#e30613;--stage:#0e0e0f;
  --mono:"SF Mono",ui-monospace,SFMono-Regular,Menlo,Consolas,monospace;
  --sans:-apple-system,BlinkMacSystemFont,"SF Pro Display","Helvetica Neue",Helvetica,Arial,sans-serif;
  --ease:cubic-bezier(.16,1,.3,1);
}
*{box-sizing:border-box;margin:0;padding:0}
html{background:var(--bg)}
body{background:var(--bg);color:var(--ink);font:15px/1.45 var(--sans);-webkit-font-smoothing:antialiased;overflow-x:hidden}
button{font:inherit;color:inherit;background:none;border:0;cursor:pointer;-webkit-tap-highlight-color:transparent}
.wrap{max-width:1360px;margin:0 auto;padding:0 32px}
.lbl{font:500 11px var(--mono);letter-spacing:.14em;text-transform:uppercase;color:var(--muted)}

/* Kopfzeile */
header{position:fixed;inset:0 0 auto;z-index:40;pointer-events:none}
header .wrap{height:68px;display:flex;align-items:center;justify-content:space-between}
header .b{pointer-events:auto;font:700 15px var(--sans);letter-spacing:-.01em;color:#fff;transition:color .3s}
header .c{pointer-events:auto;font:500 12px var(--mono);letter-spacing:.1em;text-transform:uppercase;color:#fff;display:flex;gap:10px;align-items:center;transition:color .3s}
header.lt{background:color-mix(in srgb,var(--bg) 88%,transparent);backdrop-filter:blur(10px);-webkit-backdrop-filter:blur(10px)}
header.lt .b,header.lt .c{color:var(--ink)}
header .c i{width:7px;height:7px;background:currentColor;display:inline-block}
header .c.ok i{animation:bl 1.2s steps(1) infinite}
@keyframes bl{50%{opacity:.2}}

/* ---------- 3D-Buehne mit Scroll-Kamerafahrt ---------- */
#stage{position:relative;height:600vh}
.sticky{position:sticky;top:0;height:100vh;height:100svh;padding:12px}
.frame{position:relative;height:100%;border-radius:22px;overflow:hidden;background:var(--stage)}
#gl{position:absolute;inset:0;width:100%;height:100%;display:block}
.ov{position:absolute;inset:0;pointer-events:none;color:#fff;padding:84px 40px 36px;display:flex;flex-direction:column;justify-content:space-between}
.hl{font:800 clamp(56px,10.5vw,170px)/.86 var(--sans);letter-spacing:-.055em}
.hl span{display:block;overflow:hidden}
.hl span b{display:block;font-weight:inherit;transition:transform 1s var(--ease)}
.hl .st{color:var(--red)}
.hl.swap span b{transform:translateY(105%)}
.capbox{display:flex;justify-content:space-between;align-items:flex-end;gap:24px}
.cap{pointer-events:auto;max-width:460px}
.cap .n{font:500 12px var(--mono);letter-spacing:.14em;color:rgba(255,255,255,.55)}
.cap .t{font:700 40px/1 var(--sans);letter-spacing:-.04em;margin-top:10px}
.cap .s{font:500 13px var(--mono);letter-spacing:.08em;text-transform:uppercase;color:rgba(255,255,255,.7);margin-top:10px}
.cap .t,.cap .s{transition:opacity .35s,transform .5s var(--ease)}
.cap.out .t,.cap.out .s{opacity:0;transform:translateY(10px)}
.act{margin-top:18px;height:46px;padding:0 20px;background:#fff;color:#111;font:600 13px var(--mono);letter-spacing:.1em;text-transform:uppercase;display:inline-flex;align-items:center;gap:12px;transition:background .2s,color .2s,transform .15s}
.act:hover{background:var(--red);color:#fff}
.act:active{transform:scale(.97)}
.act kbd{font:inherit;opacity:.45}
.steps{display:flex;flex-direction:column;gap:8px;align-items:flex-end}
.steps i{display:block;height:2px;width:22px;background:rgba(255,255,255,.25);transition:width .5s var(--ease),background .3s}
.steps i.on{width:54px;background:#fff}
.hint{font:500 11px var(--mono);letter-spacing:.14em;text-transform:uppercase;color:rgba(255,255,255,.5)}
.nogl{position:absolute;inset:0;display:none;place-items:center;color:rgba(255,255,255,.6);font:13px var(--mono);text-align:center;padding:40px}
.frame.fail .nogl{display:grid}

/* ---------- helle Abschnitte ---------- */
section{padding:140px 0 0}
.sh{display:flex;justify-content:space-between;align-items:flex-end;gap:24px;margin-bottom:40px}
.sh h2{font:800 clamp(48px,8vw,120px)/.88 var(--sans);letter-spacing:-.055em}
.sh p{max-width:340px;color:var(--muted)}

/* Steuerung als riesige Zeilen */
.rows{border-top:1px solid var(--ink)}
.r{width:100%;display:grid;grid-template-columns:70px 1fr auto 96px;align-items:center;gap:20px;padding:22px 0;border-bottom:1px solid var(--line);text-align:left;position:relative;overflow:hidden}
.r::before{content:"";position:absolute;inset:0;background:var(--ink);transform:scaleY(0);transform-origin:bottom;transition:transform .55s var(--ease);z-index:0}
.r:hover::before{transform:scaleY(1)}
.r>*{position:relative;z-index:1;transition:color .35s}
.r:hover>*{color:#fff}
.r .i{font:500 12px var(--mono);color:var(--muted);padding-left:6px}
.r .nm{font:750 clamp(34px,5.6vw,82px)/1 var(--sans);letter-spacing:-.045em}
.r .v{font:500 13px var(--mono);letter-spacing:.1em;text-transform:uppercase;color:var(--muted)}
.r:hover .i,.r:hover .v{color:rgba(255,255,255,.65)}
.sw{justify-self:end;margin-right:6px;width:72px;height:38px;border-radius:19px;background:var(--line);position:relative;transition:background .3s}
.sw::after{content:"";position:absolute;top:4px;left:4px;width:30px;height:30px;border-radius:50%;background:#fff;box-shadow:0 2px 8px rgba(0,0,0,.25);transition:transform .45s var(--ease)}
.r.on .sw{background:var(--red)}
.r.on .sw::after{transform:translateX(34px)}
.pos{justify-self:end;margin-right:6px;display:flex;gap:6px}
.pos b{width:38px;height:38px;display:grid;place-items:center;font:600 14px var(--mono);border:1px solid var(--line);transition:background .3s,color .3s,border-color .3s}
.pos b.a{background:var(--red);border-color:var(--red);color:#fff}
.r:hover .pos b{border-color:rgba(255,255,255,.3)}
.r.stop .nm{color:var(--red)}
.r.stop::before{background:var(--red)}
.r.press{animation:press .35s var(--ease)}
@keyframes press{30%{transform:scale(.985)}}

/* Kennzahlen */
.nums{display:grid;grid-template-columns:repeat(5,1fr);border-top:1px solid var(--ink)}
.nu{padding:26px 20px 30px 0;border-right:1px solid var(--line)}
.nu+.nu{padding-left:20px}
.nu:last-child{border-right:0}
.nu .v{font:750 clamp(34px,4.4vw,64px)/1.15 var(--sans);letter-spacing:-.04em;margin-top:14px;height:1.15em;overflow:hidden;white-space:nowrap}
.nu .s{font:12px var(--mono);color:var(--faint);margin-top:6px}
.roll{display:flex}
.roll .d{display:inline-block;height:1.15em;overflow:hidden}
.roll .c{display:flex;flex-direction:column;transition:transform 1s var(--ease)}
.roll .c i{font-style:normal;height:1.15em;line-height:1.15em}
.roll .x{display:inline-block;line-height:1.15em;white-space:pre}

/* Signal */
.card{background:var(--card);border:1px solid var(--line)}
#scope{display:block;width:100%;height:250px}
.sl{display:flex;justify-content:space-between;padding:10px 18px 14px 128px;border-top:1px solid var(--line)}

/* Log + Firmware */
.two{display:grid;grid-template-columns:1.3fr 1fr;gap:16px}
.ch{display:flex;justify-content:space-between;padding:14px 18px;border-bottom:1px solid var(--line)}
.log{font:13px/1.8 var(--mono);padding:12px 18px;height:320px;overflow:auto}
.log div{display:flex;gap:16px;white-space:nowrap}
.log div span:first-child{color:var(--faint)}
.log div.new{animation:hi 1.6s var(--ease)}
@keyframes hi{0%{background:var(--red);color:#fff}100%{background:transparent}}
.kv div{display:flex;justify-content:space-between;gap:12px;padding:12px 18px;border-bottom:1px solid var(--line);font:13px var(--mono)}
.kv div span:first-child{color:var(--muted);font-size:11px;letter-spacing:.12em;text-transform:uppercase}
.kv div span:last-child{overflow:hidden;text-overflow:ellipsis;white-space:nowrap}
#f_auto{color:var(--red)}
.fw{padding:16px 18px;display:grid;gap:12px}
.bb{height:52px;background:var(--ink);color:#fff;font:600 12px var(--mono);letter-spacing:.14em;text-transform:uppercase;position:relative;overflow:hidden}
.bb::before{content:"";position:absolute;inset:0;background:var(--red);transform:translateY(101%);transition:transform .5s var(--ease)}
.bb:hover::before{transform:none}
.bb span{position:relative}
details summary{font:11px var(--mono);letter-spacing:.12em;text-transform:uppercase;color:var(--muted);cursor:pointer;list-style:none}
details summary::-webkit-details-marker{display:none}
details input{width:100%;height:42px;margin:10px 0;padding:0 12px;background:var(--bg);border:1px solid var(--line);font:12px var(--mono);color:var(--ink)}
#otamsg{font:12px var(--mono);color:var(--red)}
#otamsg:empty{display:none}

footer{padding:160px 0 50px;overflow:hidden}
footer .big{font:800 clamp(80px,17vw,280px)/.8 var(--sans);letter-spacing:-.065em;white-space:nowrap;will-change:transform}
footer .lbl{margin-top:26px}

/* Einblenden */
.up{opacity:0;transform:translateY(60px);transition:opacity 1.1s var(--ease),transform 1.1s var(--ease);transition-delay:calc(var(--i,0)*60ms)}
.up.in{opacity:1;transform:none}

#toast{position:fixed;left:24px;bottom:24px;z-index:50;background:var(--ink);color:#fff;font:600 12px var(--mono);letter-spacing:.12em;text-transform:uppercase;padding:14px 18px;
  opacity:0;visibility:hidden;transform:translateY(16px);transition:opacity .3s,visibility .3s,transform .5s var(--ease)}
#toast.show{opacity:1;visibility:visible;transform:none}

@media (max-width:980px){
  .two{grid-template-columns:1fr}
  .nums{grid-template-columns:repeat(2,1fr)}
  .nu{border-bottom:1px solid var(--line)}.nu:nth-child(odd){padding-left:0}.nu:nth-child(even){border-right:0}
  .sh{flex-direction:column;align-items:flex-start}
}
@media (max-width:640px){
  .wrap{padding:0 16px}
  .sticky{padding:8px}.frame{border-radius:16px}
  .ov{padding:72px 20px 24px}
  .cap .t{font-size:30px}
  .steps,.hint{display:none}
  .r{grid-template-columns:36px 1fr auto;padding:18px 0}.r .v{display:none}
  section{padding-top:100px}
}
@media (prefers-reduced-motion:reduce){*{transition:none!important;animation:none!important}.up{opacity:1;transform:none}}
</style>
</head>
<body>
<header><div class="wrap"><span class="b">Parcours 1</span><span class="c" id="conn"><i></i><span id="conntxt">Verbinde</span></span></div></header>

<div id="stage">
 <div class="sticky"><div class="frame" id="frame">
  <canvas id="gl"></canvas>
  <div class="nogl">3D-Ansicht braucht Internet (three.js).<br>Die Steuerung weiter unten funktioniert trotzdem.</div>
  <div class="ov">
   <div class="hl" id="hl"><span><b>Kugelbahn</b></span><span class="st"><b id="hlst">steht.</b></span></div>
   <div class="capbox">
    <div class="cap" id="cap"><div class="n" id="cap_n">00 / 05</div><div class="t" id="cap_t">Übersicht</div><div class="s" id="cap_s">Scrollen für die Kamerafahrt</div><button class="act" id="cap_b" data-c="a"><span id="cap_bt">Aufzug starten</span><kbd id="cap_k">A</kbd></button></div>
    <div><div class="steps" id="steps"><i class="on"></i><i></i><i></i><i></i><i></i><i></i></div><div class="hint" style="margin-top:16px">Teile anklicken zum Schalten</div></div>
   </div>
  </div>
 </div></div>
</div>

<main class="wrap">
 <section>
  <div class="sh up"><h2>Steuerung</h2><p>Jeder Befehl geht sofort an den Giga. Alle Geräte im Hotspot sehen denselben Zustand. Tasten: A W H S L, X stoppt alles.</p></div>
  <div class="rows">
   <button class="r up" data-c="a" id="r_a"><span class="i">01</span><span class="nm">Kettenaufzug</span><span class="v" id="v_a">aus</span><span class="sw"></span></button>
   <button class="r up" data-c="w" id="r_w"><span class="i">02</span><span class="nm">Stellweiche</span><span class="v">Flexschiene</span><span class="pos"><b id="w1">1</b><b id="w2">2</b></span></button>
   <button class="r up" data-c="h" id="r_h"><span class="i">03</span><span class="nm">Weichenhebel</span><span class="v">mit Hebel</span><span class="pos"><b id="h1">1</b><b id="h2">2</b></span></button>
   <button class="r up" data-c="s" id="r_s"><span class="i">04</span><span class="nm">Stoppstelle</span><span class="v" id="v_s">zu</span><span class="sw"></span></button>
   <button class="r up" data-c="l" id="r_l"><span class="i">05</span><span class="nm">LEDs</span><span class="v" id="v_l">aus</span><span class="sw"></span></button>
   <button class="r stop up" data-c="x"><span class="i">X</span><span class="nm">Alles stoppen</span><span class="v">Motoren aus</span><span></span></button>
  </div>
 </section>

 <section>
  <div class="sh up"><h2>Zahlen</h2><p>Live vom Giga. Laufzeit und Schaltungen zählen ab dem Öffnen dieser Seite.</p></div>
  <div class="nums up">
   <div class="nu"><div class="lbl">Aufzug-Laufzeit</div><div class="v roll" id="k_run"></div><div class="s">diese Sitzung</div></div>
   <div class="nu"><div class="lbl">Giga online</div><div class="v roll" id="k_up"></div><div class="s">seit Neustart</div></div>
   <div class="nu"><div class="lbl">WLAN</div><div class="v roll" id="k_rssi"></div><div class="s" id="k_lat">dBm</div></div>
   <div class="nu"><div class="lbl">Schaltungen</div><div class="v roll" id="k_sw"></div><div class="s">Weichen + Stopp</div></div>
   <div class="nu"><div class="lbl">Daten Giga</div><div class="v roll" id="k_dat"></div><div class="s" id="k_dat_s">geschätzt</div></div>
  </div>
 </section>

 <section>
  <div class="sh up"><h2>Signal</h2><p>Alle Ausgänge, die letzten zwei Minuten.</p></div>
  <div class="card up"><canvas id="scope"></canvas><div class="sl lbl"><span>−120 s</span><span>−60 s</span><span>jetzt</span></div></div>
 </section>

 <section>
  <div class="sh up"><h2>System</h2><p>Ereignisse von allen Geräten und die laufende Firmware. Updates kommen automatisch von GitHub.</p></div>
  <div class="two">
   <div class="card up"><div class="ch lbl"><span>Log</span><span id="logc">000</span></div><div class="log" id="log"></div></div>
   <div class="card up" style="--i:1">
    <div class="ch lbl"><span>Firmware</span><span id="f_auto">—</span></div>
    <div class="kv">
     <div><span>Läuft</span><span id="f_ver">—</span></div>
     <div><span>GitHub</span><span id="f_gh">—</span></div>
     <div><span>Gebaut</span><span id="f_date">—</span></div>
     <div><span>Adresse</span><span id="f_ip">—</span></div>
    </div>
    <div class="fw">
     <div id="otamsg"></div>
     <button class="bb" id="ghbtn"><span>Von GitHub laden</span></button>
     <details><summary>+ Eigene Adresse</summary><input id="url" spellcheck="false" autocomplete="off"><button class="bb" id="urlbtn" style="width:100%"><span>Update starten</span></button></details>
    </div>
   </div>
  </div>
 </section>
</main>

<footer><div class="big" id="big">PARCOURS 1 · PARCOURS 1 · PARCOURS 1</div><div class="wrap lbl">github.com/JAbstractClass/parcours1-giga</div></footer>
<div id="toast"></div>

<script>
/* ===================== Daten, Steuerung, 2D-Teile ===================== */
(()=>{
const $=id=>document.getElementById(id);
const DPR=()=>Math.min(devicePixelRatio||1,3);
const RED='#e30613',MONO=getComputedStyle(document.documentElement).getPropertyValue('--mono');
const P1=window.P1={S:null,events:[],station:0,cmd:null};
let lastOk=0,lat=0,runMs=0,runSince=null,swc=0,up=null;

/* rollende Ziffern */
function roll(el,str){
  if(el._s===str)return;const pat=str.replace(/\d/g,'#');
  if(el._p!==pat){el._p=pat;el.innerHTML=[...str].map(ch=>/\d/.test(ch)?'<span class="d"><span class="c">'+[...'0123456789'].map(n=>'<i>'+n+'</i>').join('')+'</span></span>':'<span class="x">'+ch+'</span>').join('');el.offsetWidth;}
  const cols=el.querySelectorAll('.c');let k=0;[...str].forEach(ch=>{if(/\d/.test(ch)){cols[k].style.transform='translateY(-'+(+ch*1.15)+'em)';k++;}});el._s=str;
}
const dur=s=>{s=Math.max(0,Math.floor(s));const h=Math.floor(s/3600),m=Math.floor(s%3600/60),x=s%60;return(h?h+':':'')+String(m).padStart(2,'0')+':'+String(x).padStart(2,'0')};
let tt;function toast(m){const t=$('toast');t.textContent=m;t.classList.add('show');clearTimeout(tt);tt=setTimeout(()=>t.classList.remove('show'),2400);}
let logn=0;function log(m){const l=$('log');const d=document.createElement('div');d.className='new';d.innerHTML='<span>'+new Date().toLocaleTimeString('de-DE')+'</span><span></span>';d.lastChild.textContent=m;l.prepend(d);while(l.children.length>80)l.lastChild.remove();$('logc').textContent=String(++logn).padStart(3,'0');}

/* Kamerastationen: Beschriftung + Schalter pro Station */
const ST=[
 {t:'Übersicht',c:'a',s:s=>'Scrollen für die Kamerafahrt',b:s=>s&&s.aufzug?'Aufzug stoppen':'Aufzug starten'},
 {t:'Kettenaufzug',c:'a',s:s=>s&&s.aufzug?'läuft · fördert Kugeln':'steht',b:s=>s&&s.aufzug?'Aufzug stoppen':'Aufzug starten'},
 {t:'Stoppstelle',c:'s',s:s=>s&&s.stopp?'offen · Kugeln rollen':'zu · Kugeln warten',b:s=>s&&s.stopp?'Schließen':'Öffnen'},
 {t:'Stellweiche',c:'w',s:s=>'Stellung '+(s?s.weiche:1),b:s=>'Umschalten'},
 {t:'Weichenhebel',c:'h',s:s=>'Stellung '+(s?s.hebel:1),b:s=>'Umschalten'},
 {t:'Rainbow-LEDs',c:'l',s:s=>s&&s.leds?'an':'aus',b:s=>s&&s.leds?'Ausschalten':'Einschalten'}
];
let capIdx=-1;
P1.updateCap=function(force){
  const i=P1.station,st=ST[i],s=P1.S,cap=$('cap');
  const apply=()=>{$('cap_n').textContent=String(i).padStart(2,'0')+' / 05';$('cap_t').textContent=st.t;$('cap_s').textContent=st.s(s);$('cap_bt').textContent=st.b(s);$('cap_k').textContent=st.c.toUpperCase();$('cap_b').dataset.c=st.c;};
  if(i!==capIdx){capIdx=i;cap.classList.add('out');setTimeout(()=>{apply();cap.classList.remove('out');},220);
    [...$('steps').children].forEach((e,k)=>e.classList.toggle('on',k===i));}
  else apply();
};

/* Oszilloskop */
const sc=$('scope'),sg=sc.getContext('2d'),WIN=120000;
const CH=[['Aufzug',s=>s.aufzug],['Stopp',s=>s.stopp],['Weiche',s=>s.weiche===2],['Hebel',s=>s.hebel===2],['LEDs',s=>s.leds]];
const samples=[];
function sSize(){sc.width=Math.round(sc.clientWidth*DPR());sc.height=Math.round(sc.clientHeight*DPR());}
function sDraw(now){
  const d=DPR(),w=sc.width,h=sc.height;sg.clearRect(0,0,w,h);
  const L=110*d,rh=h/CH.length,X=t=>L+(w-L)*(1-(now-t)/WIN);
  sg.strokeStyle='#ececea';sg.lineWidth=1;
  for(let i=0;i<=12;i++){const x=Math.round(L+(w-L)*i/12)+.5;sg.beginPath();sg.moveTo(x,0);sg.lineTo(x,h);sg.stroke();}
  sg.font=(11*d)+'px '+MONO;
  CH.forEach(([n,f],ci)=>{
    const y0=ci*rh,hi=y0+rh*.26,lo=y0+rh*.74;
    sg.fillStyle='#6d6d6a';sg.fillText(n.toUpperCase(),18*d,y0+rh/2+4*d);
    sg.strokeStyle='#ececea';sg.lineWidth=1;sg.beginPath();sg.moveTo(0,Math.round(y0+rh)+.5);sg.lineTo(w,Math.round(y0+rh)+.5);sg.stroke();
    sg.strokeStyle=ci===0?RED:'#111';sg.lineWidth=2*d;sg.beginPath();let on=false,py=0;
    for(let i=0;i<samples.length;i++){const sm=samples[i],x0=X(sm.t),x1=i+1<samples.length?X(samples[i+1].t):w;if(x1<L)continue;if(!sm.s){on=false;continue;}
      const y=f(sm.s)?hi:lo,xa=Math.max(x0,L);if(!on){sg.moveTo(xa,y);on=true;}else if(y!==py){sg.lineTo(xa,py);sg.lineTo(xa,y);}sg.lineTo(Math.min(x1,w),y);py=y;}
    sg.stroke();
    if(P1.S){sg.fillStyle=ci===0?RED:'#111';sg.beginPath();sg.arc(w-4*d,f(P1.S)?hi:lo,4*d,0,7);sg.fill();}
  });
}
setInterval(()=>{samples.push({t:performance.now(),s:P1.S&&Date.now()-lastOk<4000?P1.S:null});while(samples.length&&performance.now()-samples[0].t>WIN+5000)samples.shift();},250);

/* Footer-Laufschrift folgt dem Scrollen */
const big=$('big'),hdr=document.querySelector('header'),stg=$('stage');
function loop(now){sDraw(now);hdr.classList.toggle('lt',stg.getBoundingClientRect().bottom<70);const r=big.getBoundingClientRect();big.style.transform='translate3d('+(-(innerHeight-r.top)*.35).toFixed(1)+'px,0,0)';requestAnimationFrame(loop);}
addEventListener('resize',sSize);sSize();requestAnimationFrame(loop);

/* Anzeige */
function render(s){
  const p=P1.S;P1.S=s;
  const tog=(id,on)=>$(id).classList.toggle('on',on);
  tog('r_a',s.aufzug);tog('r_s',s.stopp);tog('r_l',s.leds);
  $('v_a').textContent=s.aufzug?'läuft':'aus';$('v_s').textContent=s.stopp?'offen':'zu';$('v_l').textContent=s.leds?'an':'aus';
  $('w1').className=s.weiche===1?'a':'';$('w2').className=s.weiche===2?'a':'';$('h1').className=s.hebel===1?'a':'';$('h2').className=s.hebel===2?'a':'';
  const newSt=s.aufzug?'läuft.':'steht.';
  if($('hlst').textContent!==newSt){const hl=$('hl');hl.classList.add('swap');setTimeout(()=>{$('hlst').textContent=newSt;hl.classList.remove('swap');},450);}
  if(s.aufzug&&!runSince)runSince=Date.now();if(!s.aufzug&&runSince){runMs+=Date.now()-runSince;runSince=null;}
  up={v:s.uptime,at:Date.now()};
  roll($('k_rssi'),s.rssi?String(s.rssi):'0');$('k_lat').textContent='dBm · '+lat+' ms';
  if(s.daten!==undefined){const b=s.daten+s.abrufe*7000;roll($('k_dat'),b<1e6?Math.round(b/1e3)+' KB':(b/1e6).toFixed(1)+' MB');$('k_dat_s').textContent=s.uptime>60?'≈ '+(b/s.uptime*3.6/1e3).toFixed(1)+' MB/h':'geschätzt';}
  $('f_ver').textContent=s.version;$('f_gh').textContent=s.github||'—';$('f_date').textContent=s.datum;$('f_ip').textContent=s.ip;
  $('f_auto').textContent=!s.auto?'Auto-Update aus':(s.github&&s.github!==s.version?'Update folgt':'Auto-Update an');
  $('otamsg').textContent=s.ota||'';
  if(p){
    const ev=k=>P1.events.push(k);
    if(p.version!==s.version){log('Neue Firmware '+s.version);toast('Neue Firmware '+s.version);}
    if(p.aufzug!==s.aufzug){log(s.aufzug?'Aufzug gestartet':'Aufzug gestoppt');ev('a');}
    if(p.weiche!==s.weiche){log('Weiche → Stellung '+s.weiche);swc++;ev('w');}
    if(p.hebel!==s.hebel){log('Hebel → Stellung '+s.hebel);swc++;ev('h');}
    if(p.stopp!==s.stopp){log(s.stopp?'Stoppstelle offen':'Stoppstelle zu');swc++;ev('s');}
    if(p.leds!==s.leds){log(s.leds?'LEDs an':'LEDs aus');ev('l');}
    if(p.ota!==s.ota&&s.ota)log(s.ota);
  }else log('Verbunden · '+s.ip);
  roll($('k_sw'),String(swc));
  P1.updateCap();
}
setInterval(()=>{
  roll($('k_run'),dur((runMs+(runSince?Date.now()-runSince:0))/1000));
  if(up)roll($('k_up'),dur(up.v+(Date.now()-up.at)/1000));
  const ok=Date.now()-lastOk<4000;$('conn').className='c'+(ok?' ok':'');
  $('conntxt').textContent=ok?'Live · '+lat+' ms':(lastOk?'Verbindung weg':'Verbinde');
},1000);
roll($('k_run'),'00:00');roll($('k_up'),'00:00');roll($('k_rssi'),'0');roll($('k_sw'),'0');roll($('k_dat'),'0 KB');

/* Kommunikation */
async function get(u){const t=performance.now();const r=await fetch(u,{cache:'no-store'});const j=await r.json();lat=Math.round(performance.now()-t);lastOk=Date.now();return j;}
async function poll(){try{render(await get('/status'));}catch(e){}}
async function cmd(c){
  const row=document.querySelector('.r[data-c="'+c+'"]');if(row){row.classList.remove('press');row.offsetWidth;row.classList.add('press');}
  try{render(await get('/cmd?c='+c));if(c==='x')toast('Alles gestoppt');}catch(e){toast('Giga nicht erreichbar');}
}
P1.cmd=cmd;
document.querySelectorAll('[data-c]').forEach(el=>el.addEventListener('click',()=>cmd(el.dataset.c)));
addEventListener('keydown',e=>{if(e.target.tagName==='INPUT'||e.metaKey||e.ctrlKey||e.altKey||e.repeat)return;const k=e.key.toLowerCase();if(k.length===1&&'awhslx'.includes(k))cmd(k);});
$('ghbtn').onclick=async()=>{toast('Update von GitHub gestartet');log('Update von GitHub angefordert');try{await fetch('/github',{cache:'no-store'});}catch(e){}};
$('urlbtn').onclick=async()=>{const u=$('url').value.trim();if(!u)return;toast('Update gestartet');log('Update von '+u);try{await fetch('/update?url='+encodeURIComponent(u));}catch(e){}};
$('url').value='%OTA_URL%';

const io=new IntersectionObserver(es=>es.forEach(e=>{if(e.isIntersecting){e.target.classList.add('in');io.unobserve(e.target);}}),{threshold:.1});
document.querySelectorAll('.up').forEach(el=>io.observe(el));

poll();setInterval(poll,1000);
})();
</script>

<script type="module">
/* ===================== 3D-Kugelbahn (three.js) ===================== */
const frame=document.getElementById('frame');
let THREE,RoomEnvironment;
try{
  THREE=await import('three');
  ({RoomEnvironment}=await import('three/addons/environments/RoomEnvironment.js'));
}catch(e){frame.classList.add('fail');throw e;}
const P1=window.P1;
const canvas=document.getElementById('gl');
const R=new THREE.WebGLRenderer({canvas,antialias:true,powerPreference:'high-performance'});
R.setPixelRatio(Math.min(devicePixelRatio,2));
R.shadowMap.enabled=true;R.shadowMap.type=THREE.PCFSoftShadowMap;
R.toneMapping=THREE.ACESFilmicToneMapping;R.toneMappingExposure=1.05;R.outputColorSpace=THREE.SRGBColorSpace;
const scene=new THREE.Scene();scene.background=new THREE.Color(0x0e0e0f);scene.fog=new THREE.Fog(0x0e0e0f,30,60);
const pm=new THREE.PMREMGenerator(R);scene.environment=pm.fromScene(new RoomEnvironment(),.04).texture;
const cam=new THREE.PerspectiveCamera(32,1,.1,200);

const sun=new THREE.DirectionalLight(0xffffff,2.4);sun.position.set(7,16,9);sun.castShadow=true;
sun.shadow.mapSize.set(2048,2048);Object.assign(sun.shadow.camera,{left:-13,right:13,top:13,bottom:-13,near:1,far:50});sun.shadow.bias=-.0004;sun.shadow.radius=4;scene.add(sun);
const rim=new THREE.DirectionalLight(0xffffff,.6);rim.position.set(-10,6,-8);scene.add(rim);

const M={
  plate:new THREE.MeshStandardMaterial({color:0x2b2b2e,roughness:.85}),
  stud:new THREE.MeshStandardMaterial({color:0x37373b,roughness:.6}),
  black:new THREE.MeshStandardMaterial({color:0x141416,roughness:.45,metalness:.1}),
  red:new THREE.MeshStandardMaterial({color:0xe30613,roughness:.35,metalness:.05}),
  rail:new THREE.MeshStandardMaterial({color:0xd8d8d4,roughness:.3,metalness:.35}),
  steel:new THREE.MeshStandardMaterial({color:0xe9e9e9,roughness:.16,metalness:1,envMapIntensity:2.2}),
  grey:new THREE.MeshStandardMaterial({color:0x8a8a86,roughness:.5})
};
const clickable=[];
const tag=(o,c)=>{o.traverse(x=>{x.userData.cmd=c;});clickable.push(o);return o;};

/* Grundplatte mit Noppen (fischertechnik-Bauplatte) */
const plate=new THREE.Mesh(new THREE.BoxGeometry(20,.4,13),M.plate);plate.position.y=-.2;plate.receiveShadow=true;scene.add(plate);
{const g=new THREE.CylinderGeometry(.11,.11,.06,10);const cols=40,rows=26;const im=new THREE.InstancedMesh(g,M.stud,cols*rows);const m=new THREE.Matrix4();let i=0;
 for(let x=0;x<cols;x++)for(let z=0;z<rows;z++){m.makeTranslation(-9.75+x*.5,.03,-6.25+z*.5);im.setMatrixAt(i++,m);}im.receiveShadow=true;scene.add(im);}

/* Strecke */
const V=(x,y,z)=>new THREE.Vector3(x,y,z);
const LIFT_X=-7,LIFT_Z=.6;
const NODE={gate:V(-2.4,5.05,2),w:V(1.2,4.35,2),h:V(3.2,2.9,-1.2),leds:V(-3.4,0,3.6)};
const SEGS={
 lift:[V(LIFT_X,.35,LIFT_Z),V(LIFT_X,6.4,LIFT_Z)],
 top:[V(LIFT_X,6.4,LIFT_Z),V(-6.2,6.25,1.6),V(-4.4,5.6,2.3),NODE.gate],
 g2w:[NODE.gate,V(-.6,4.7,2.1),NODE.w],
 w1:[NODE.w,V(2.4,3.7,.6),NODE.h],
 w2:[NODE.w,V(3.6,4.0,3.2),V(6.4,3.6,2.4),V(7.2,3.3,-.6),V(5.6,3.05,-2.6),NODE.h],
 h1:[NODE.h,V(1.6,2.3,-3),V(-2.5,1.5,-3.2),V(-5.6,.9,-1.6),V(LIFT_X,.35,LIFT_Z)],
 h2:[NODE.h,V(5.2,2.2,-4.4),V(1,1.4,-5.4),V(-4.4,.9,-4.4),V(-6.6,.5,-1.6),V(LIFT_X,.35,LIFT_Z)]
};
const SEG={};
for(const k in SEGS){const c=new THREE.CatmullRomCurve3(SEGS[k],false,'centripetal');SEG[k]={c,len:c.getLength()};}
const UP=V(0,1,0),tmp=V(0,0,0);
function offsetCurve(c,off,n){const pts=[];for(let i=0;i<=n;i++){const u=i/n,p=c.getPointAt(u),t=c.getTangentAt(u);const side=tmp.copy(t).cross(UP);if(side.lengthSq()<1e-4)side.set(1,0,0);side.normalize();pts.push(p.clone().addScaledVector(side,off).addScaledVector(UP,-.18));}return new THREE.CatmullRomCurve3(pts);}
for(const k in SEG){
  if(k==='lift')continue;const c=SEG[k].c,n=Math.max(24,Math.round(SEG[k].len*10));
  [-.17,.17].forEach(o=>{const m=new THREE.Mesh(new THREE.TubeGeometry(offsetCurve(c,o,n),n*2,.055,8,false),M.rail);m.castShadow=true;scene.add(m);});
  const steps=Math.floor(SEG[k].len/2.8);
  for(let i=1;i<=steps;i++){const p=c.getPointAt(i/(steps+1));if(p.y<1.1)continue;
    const post=new THREE.Mesh(new THREE.BoxGeometry(.16,p.y-.25,.16),M.black);post.position.set(p.x,(p.y-.25)/2,p.z);post.castShadow=true;scene.add(post);
    const cross=new THREE.Mesh(new THREE.BoxGeometry(.5,.08,.12),M.red);cross.position.set(p.x,p.y-.26,p.z);cross.lookAt(p.x+c.getTangentAt(i/(steps+1)).x,p.y-.26,p.z+c.getTangentAt(i/(steps+1)).z);cross.rotateY(Math.PI/2);cross.castShadow=true;scene.add(cross);}
}

/* Kettenaufzug */
const lift=new THREE.Group();scene.add(lift);
{const tower=new THREE.Mesh(new THREE.BoxGeometry(.9,7,.5),M.black);tower.position.set(LIFT_X,3.5,0);tower.castShadow=true;lift.add(tower);
 const cap=new THREE.Mesh(new THREE.BoxGeometry(1.2,.3,1.2),M.red);cap.position.set(LIFT_X,7.1,.2);cap.castShadow=true;lift.add(cap);
 [.3,7].forEach(y=>{const wh=new THREE.Mesh(new THREE.CylinderGeometry(.38,.38,.22,24),M.grey);wh.rotation.z=Math.PI/2;wh.position.set(LIFT_X,y,.42);lift.add(wh);});}
const LINKS=34,chain=new THREE.InstancedMesh(new THREE.BoxGeometry(.26,.1,.1),M.red,LINKS);chain.castShadow=true;scene.add(chain);
tag(lift,'a');
/* Stoppstelle */
const gate=new THREE.Group();gate.position.copy(NODE.gate);scene.add(gate);
{const base=new THREE.Mesh(new THREE.BoxGeometry(.9,.25,.9),M.black);base.position.y=-.45;gate.add(base);}
const gateBar=new THREE.Mesh(new THREE.BoxGeometry(.12,.7,.7),M.red);gateBar.castShadow=true;gate.add(gateBar);
const servo=new THREE.Mesh(new THREE.BoxGeometry(.5,.4,.3),M.black);servo.position.set(0,-.1,-.6);gate.add(servo);
tag(gate,'s');
/* Weichen */
function mkSwitch(node,cmd){const g=new THREE.Group();g.position.copy(node);scene.add(g);
  const disc=new THREE.Mesh(new THREE.CylinderGeometry(.55,.55,.16,32),M.black);disc.position.y=-.32;disc.castShadow=true;g.add(disc);
  const arm=new THREE.Group();arm.position.y=-.18;g.add(arm);
  const a=new THREE.Mesh(new THREE.BoxGeometry(.9,.1,.14),M.red);a.position.x=.45;a.castShadow=true;arm.add(a);
  const hub=new THREE.Mesh(new THREE.CylinderGeometry(.12,.12,.2,16),M.grey);arm.add(hub);
  const motor=new THREE.Mesh(new THREE.BoxGeometry(.5,.35,.35),M.black);motor.position.set(0,-.6,0);g.add(motor);
  tag(g,cmd);return arm;}
const armW=mkSwitch(NODE.w,'w'),armH=mkSwitch(NODE.h,'h');
const dirAng=(seg)=>{const t=SEG[seg].c.getTangentAt(.02);return Math.atan2(-t.z,t.x);};
const A={w1:dirAng('w1'),w2:dirAng('w2'),h1:dirAng('h1'),h2:dirAng('h2')};
/* LEDs */
const leds=new THREE.Group();leds.position.copy(NODE.leds);scene.add(leds);
const ledMats=[],ledLights=[];
[-.7,.7].forEach(x=>{const post=new THREE.Mesh(new THREE.BoxGeometry(.3,1.2,.3),M.black);post.position.set(x,.6,0);post.castShadow=true;leds.add(post);
  const mat=new THREE.MeshStandardMaterial({color:0x222222,emissive:0x000000,roughness:.2});ledMats.push(mat);
  const cube=new THREE.Mesh(new THREE.BoxGeometry(.42,.42,.42),mat);cube.position.set(x,1.42,0);leds.add(cube);
  const L=new THREE.PointLight(0xff3355,0,9,1.6);L.position.set(x,1.6,.3);leds.add(L);ledLights.push(L);});
tag(leds,'l');

/* Kugeln */
const marbles=[],GEO_M=new THREE.SphereGeometry(.24,32,24);
function addMarble(seg,s){const m=new THREE.Mesh(GEO_M,M.steel);m.castShadow=true;scene.add(m);marbles.push({m,seg,s,v:1.5});}
for(let i=0;i<4;i++)addMarble('lift',.4+i*1.25);
for(let i=0;i<4;i++)addMarble('h1',SEG.h1.len-.3-i*.6);
const GAP=.56,LIFT_V=1.5,G=9;
function nextSeg(k){const S=P1.S;if(k==='lift')return'top';if(k==='top')return'g2w';if(k==='g2w')return S&&S.weiche===2?'w2':'w1';if(k==='w1'||k==='w2')return S&&S.hebel===2?'h2':'h1';return'lift';}
function stepMarbles(dt){
  const S=P1.S,run=!!(S&&S.aufzug),open=!!(S&&S.stopp);
  const by={};marbles.forEach(o=>(by[o.seg]=by[o.seg]||[]).push(o));
  for(const k in by){const list=by[k].sort((a,b)=>b.s-a.s),len=SEG[k].len;
    list.forEach((o,idx)=>{if(o.seg!==k)return;
      if(k==='lift')o.v=run?LIFT_V:0;
      else{const t=SEG[k].c.getTangentAt(Math.min(.999,o.s/len));o.v+=(-t.y*G-.35*o.v)*dt;o.v=Math.min(7,Math.max(1.3,o.v));}
      let lim=len;if(idx>0&&list[idx-1].seg===k)lim=Math.min(lim,list[idx-1].s-GAP);
      let ns=Math.max(o.s,Math.min(o.s+o.v*dt,lim));if(ns<o.s+o.v*dt-1e-6&&k!=='lift')o.v=Math.max(1.3,o.v*.6);
      if(ns>=len){const hold=(k==='top'&&!open)||((k==='h1'||k==='h2')&&!run);const nx=nextSeg(k);const blocked=marbles.some(q=>q!==o&&q.seg===nx&&q.s<GAP);
        if(hold||blocked){ns=len;o.v=Math.min(o.v,1.3);}else{o.seg=nx;ns=Math.min(ns-len,GAP*.4);}}
      o.s=ns;});}
  marbles.forEach(o=>{const c=SEG[o.seg].c,u=Math.min(1,o.s/SEG[o.seg].len),p=c.getPointAt(u);
    if(o.seg==='lift')o.m.position.set(p.x,p.y,p.z);else o.m.position.set(p.x,p.y+.08,p.z);
    o.m.rotation.x+=o.v*dt*3;o.m.rotation.z+=o.v*dt*2;});
}

/* Ereignis-Ringe */
const rings=[];const RING_G=new THREE.RingGeometry(.9,1,64);
function ring(pos){const m=new THREE.Mesh(RING_G,new THREE.MeshBasicMaterial({color:0xe30613,transparent:true,opacity:1,side:THREE.DoubleSide,depthWrite:false}));m.rotation.x=-Math.PI/2;m.position.copy(pos);m.position.y=Math.max(.05,pos.y-.4);scene.add(m);rings.push({m,t:0});}
const EVPOS={a:V(LIFT_X,.4,0),s:NODE.gate,w:NODE.w,h:NODE.h,l:V(NODE.leds.x,.05,NODE.leds.z)};

/* Kamerafahrt */
const KEYS=[
 {p:V(15,13,17),t:V(0,2.6,0)},
 {p:V(-1.5,6.5,9),t:V(-7,3.6,0)},
 {p:V(1.2,7.8,7.5),t:V(-2.4,4.8,2)},
 {p:V(5.5,7,7.5),t:V(1.2,4.1,1.8)},
 {p:V(8.5,5.2,4),t:V(3.2,2.7,-1.2)},
 {p:V(0,3.2,10),t:V(-3.4,1,3.6)}
];
const stage=document.getElementById('stage');
const camPos=KEYS[0].p.clone(),camTgt=KEYS[0].t.clone(),wantP=V(0,0,0),wantT=V(0,0,0);
let mx=0,my=0;addEventListener('pointermove',e=>{mx=e.clientX/innerWidth-.5;my=e.clientY/innerHeight-.5;},{passive:true});
const ease=x=>x<.5?4*x*x*x:1-Math.pow(-2*x+2,3)/2;
function camTarget(){
  const r=stage.getBoundingClientRect(),total=stage.offsetHeight-innerHeight;
  const p=Math.min(1,Math.max(0,-r.top/Math.max(1,total)));
  const f=p*(KEYS.length-1),i=Math.min(KEYS.length-2,Math.floor(f)),u=ease(f-i);
  wantP.lerpVectors(KEYS[i].p,KEYS[i+1].p,u);wantT.lerpVectors(KEYS[i].t,KEYS[i+1].t,u);
  const st=Math.round(f);if(st!==P1.station){P1.station=st;P1.updateCap();}
  document.getElementById('hl').style.opacity=String(Math.max(0,1-f*1.6));
}

/* Klicken und Hover */
const ray=new THREE.Raycaster(),ndc=new THREE.Vector2();
function pick(e){const r=canvas.getBoundingClientRect();ndc.set((e.clientX-r.left)/r.width*2-1,-(e.clientY-r.top)/r.height*2+1);ray.setFromCamera(ndc,cam);const h=ray.intersectObjects(clickable,true)[0];return h?h.object.userData.cmd:null;}
frame.addEventListener('click',e=>{if(e.target.closest('.act'))return;const c=pick(e);if(c)P1.cmd(c);});
frame.addEventListener('pointermove',e=>{frame.style.cursor=pick(e)?'pointer':'';},{passive:true});

/* Groesse */
function size(){const w=canvas.clientWidth,h=canvas.clientHeight;R.setSize(w,h,false);cam.aspect=w/h;cam.updateProjectionMatrix();}
addEventListener('resize',size);size();

/* Hauptschleife – laeuft mit der Bildrate des Displays (120 Hz auf ProMotion) */
const st={gate:0,w:A.w1,h:A.h1,led:0,chain:0};const col=new THREE.Color();
let last=performance.now();
function frameLoop(now){
  const dt=Math.min(.05,(now-last)/1000);last=now;const S=P1.S;
  camTarget();
  const k=1-Math.pow(.002,dt);
  camPos.lerp(wantP,k);camTgt.lerp(wantT,k);
  cam.position.set(camPos.x+mx*1.2,camPos.y-my*.8,camPos.z);cam.lookAt(camTgt);
  stepMarbles(dt);
  const e=1-Math.pow(.0006,dt);
  st.gate+=((S&&S.stopp?1:0)-st.gate)*e;gateBar.position.y=st.gate*.75;gateBar.rotation.z=st.gate*.25;
  st.w+=((S&&S.weiche===2?A.w2:A.w1)-st.w)*e;armW.rotation.y=st.w;
  st.h+=((S&&S.hebel===2?A.h2:A.h1)-st.h)*e;armH.rotation.y=st.h;
  st.led+=((S&&S.leds?1:0)-st.led)*e;
  col.setHSL((now/4000)%1,1,.55);
  ledMats.forEach((m,i)=>{const c2=col.clone().offsetHSL(i*.5,0,0);m.emissive.copy(c2).multiplyScalar(st.led*2.2);});
  ledLights.forEach((L,i)=>{L.color.copy(col).offsetHSL(i*.5,0,0);L.intensity=st.led*14;});
  if(S&&S.aufzug)st.chain=(st.chain+LIFT_V*dt)%(14/LINKS*1);
  const m4=new THREE.Matrix4();
  for(let i=0;i<LINKS;i++){let d=(i*14/LINKS+st.chain)%14;let x=LIFT_X,y,z;
    if(d<6.7){y=.3+d;z=.33;}else if(d<7){y=7;z=.33-(d-6.7)/.3*.66;}else if(d<13.7){y=7-(d-7);z=-.33;}else{y=.3;z=-.33+(d-13.7)/.3*.66;}
    m4.makeTranslation(x,y,z);chain.setMatrixAt(i,m4);}
  chain.instanceMatrix.needsUpdate=true;
  while(P1.events.length){const ev=P1.events.shift();if(EVPOS[ev])ring(EVPOS[ev]);}
  for(let i=rings.length-1;i>=0;i--){const r=rings[i];r.t+=dt;const s=1+r.t*3.2;r.m.scale.set(s,s,s);r.m.material.opacity=Math.max(0,1-r.t/1.1);if(r.t>1.1){scene.remove(r.m);r.m.material.dispose();rings.splice(i,1);}}
  R.render(scene,cam);
  requestAnimationFrame(frameLoop);
}
requestAnimationFrame(frameLoop);
</script>
</body>
</html>
)DASH";
