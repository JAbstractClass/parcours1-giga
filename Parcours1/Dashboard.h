#pragma once
// AUTOMATISCH ERZEUGT aus dashboard.html (tools/html2h.py) - nicht von Hand aendern.
static const char HTML_SEITE[] = R"DASH(<!doctype html>
<html lang="de">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1,viewport-fit=cover">
<title>Parcours 1</title>
<style>
/* Neutral wie Vercel/Geist, Farbe nur fuer Zustand: gruen = laeuft, rot = Stopp */
:root{
  --bg:#fafafa;--surface:#fff;--sub:#f4f4f5;--line:#e4e4e7;--line-strong:#d4d4d8;
  --text:#09090b;--muted:#71717a;--faint:#a1a1aa;
  --on:#16a34a;--on-bg:#dcfce7;--off:#d4d4d8;--stop:#dc2626;--stop-bg:#fef2f2;--warn:#ca8a04;
  --mono:ui-monospace,"SF Mono",SFMono-Regular,Menlo,Consolas,monospace;
}
@media (prefers-color-scheme:dark){:root{
  --bg:#09090b;--surface:#0f0f11;--sub:#18181b;--line:#27272a;--line-strong:#3f3f46;
  --text:#fafafa;--muted:#a1a1aa;--faint:#71717a;
  --on:#22c55e;--on-bg:rgba(34,197,94,.12);--off:#3f3f46;--stop:#ef4444;--stop-bg:rgba(239,68,68,.1);--warn:#eab308;
}}
*{box-sizing:border-box;margin:0;padding:0}
body{background:var(--bg);color:var(--text);font:14px/1.5 -apple-system,BlinkMacSystemFont,"Inter","Segoe UI",Roboto,system-ui,sans-serif;-webkit-font-smoothing:antialiased}
button{font:inherit;color:inherit;background:none;border:0;cursor:pointer}
.num{font-variant-numeric:tabular-nums}
.wrap{max-width:1180px;margin:0 auto;padding:0 24px}

/* Kopfzeile */
header{position:sticky;top:0;z-index:10;background:var(--bg);border-bottom:1px solid var(--line)}
header .wrap{height:56px;display:flex;align-items:center;gap:20px}
.brand{font-weight:600;letter-spacing:-.01em}
.brand span{color:var(--faint);font-weight:400;margin-left:8px}
.grow{flex:1}
.conn{font-size:13px;color:var(--muted);display:flex;align-items:center;gap:8px}
.conn i{width:6px;height:6px;border-radius:1px;background:var(--off)}
.conn.ok i{background:var(--on)}
.conn.bad i{background:var(--stop)}
.btn{height:32px;padding:0 12px;border-radius:6px;border:1px solid var(--line-strong);background:var(--surface);font-size:13px;font-weight:500;display:inline-flex;align-items:center;gap:8px}
.btn:hover{background:var(--sub)}
.btn.stop{border-color:var(--stop);color:var(--stop)}
.btn.stop:hover{background:var(--stop-bg)}
.btn.primary{background:var(--text);color:var(--bg);border-color:var(--text)}
.btn.primary:hover{opacity:.88}
kbd{font:500 11px var(--mono);color:var(--faint);border:1px solid var(--line);border-radius:4px;padding:0 4px;line-height:16px}

/* Kopfbereich mit Zustand */
.top{padding:40px 0 28px;display:flex;align-items:flex-end;justify-content:space-between;gap:24px;flex-wrap:wrap}
.top h1{font-size:32px;font-weight:600;letter-spacing:-.03em;line-height:1.15}
.top p{color:var(--muted);margin-top:6px}
.status{display:inline-flex;align-items:center;gap:8px;font-size:13px;font-weight:500;color:var(--muted);margin-bottom:10px}
.status b{width:8px;height:8px;border-radius:2px;background:var(--off)}
.running .status b{background:var(--on)}
.running .status{color:var(--on)}
.actions{display:flex;gap:8px}
.actions .btn{height:40px;padding:0 16px;font-size:14px}

/* Kennzahlen wie Stripe: Zahlen mit Trennlinien, keine Kacheln */
.stats{display:grid;grid-template-columns:repeat(5,1fr);border:1px solid var(--line);border-radius:8px;background:var(--surface)}
.stat{padding:16px 20px;border-left:1px solid var(--line)}
.stat:first-child{border-left:0}
.stat dt{font-size:12.5px;color:var(--muted)}
.stat dd{font-size:24px;font-weight:600;letter-spacing:-.02em;margin-top:4px}
.stat small{display:block;font-size:12px;color:var(--faint);margin-top:2px}

/* Raster */
.grid{display:grid;grid-template-columns:minmax(0,1fr) 380px;gap:24px;margin-top:24px;align-items:start}
.box{border:1px solid var(--line);border-radius:8px;background:var(--surface)}
.box>h2{font-size:13px;font-weight:600;padding:12px 16px;border-bottom:1px solid var(--line);display:flex;justify-content:space-between;align-items:center}
.box>h2 span{font-weight:400;color:var(--faint)}
.stack{display:grid;gap:24px}

/* Schema */
.schema svg{display:block;width:100%;height:auto}
.schema .rail{fill:none;stroke:var(--line-strong);stroke-width:2;stroke-linecap:round}
.schema .flow{fill:none;stroke:var(--on);stroke-width:2;stroke-linecap:round;stroke-dasharray:4 8;opacity:0}
.running .schema .flow{opacity:1;animation:flow 1s linear infinite}
@keyframes flow{to{stroke-dashoffset:-24}}
.schema .el{cursor:pointer}
.schema .el rect,.schema .el circle.b{fill:var(--surface);stroke:var(--line-strong);stroke-width:1.5}
.schema .el:hover rect,.schema .el:hover circle.b{stroke:var(--text)}
.schema text{font-size:12px;fill:var(--muted)}
.schema .val{fill:var(--text);font-weight:600}
.schema .arm{stroke:var(--text);stroke-width:3;stroke-linecap:round;transition:transform .35s ease;transform-box:fill-box;transform-origin:0 50%}
.schema .el .gatebar{fill:var(--stop);stroke:none;transition:fill .2s}
.schema .el.open .gatebar{fill:var(--on)}
.schema .el .lamp{fill:var(--off);stroke:none;transition:fill .2s}
.leds-on .schema .el .lamp{fill:var(--on)}
.schema .el .lift{fill:var(--off)}
.running .schema .el .lift{fill:var(--on)}

/* Zeitleiste wie Datadog: Zustand je Bauteil */
.tl{padding:12px 16px 14px}
.tl-row{display:grid;grid-template-columns:110px 1fr 52px;align-items:center;gap:12px;height:28px}
.tl-row span{font-size:12.5px;color:var(--muted)}
.tl-row em{font-style:normal;font-size:12px;color:var(--faint);text-align:right}
.tl canvas{width:100%;height:14px;display:block;border-radius:2px;background:var(--sub)}
.tl-axis{display:grid;grid-template-columns:110px 1fr 52px;gap:12px;font-size:11px;color:var(--faint);margin-top:4px}
.tl-axis div{display:flex;justify-content:space-between}

/* Steuerung als Liste wie Linear */
.ctl{width:100%;display:flex;align-items:center;gap:12px;padding:12px 16px;border-top:1px solid var(--line);text-align:left}
.ctl:first-of-type{border-top:0}
.ctl:hover{background:var(--sub)}
.ctl .n{flex:1}
.ctl .n b{display:block;font-weight:500}
.ctl .n small{color:var(--muted);font-size:12.5px}
.sw{width:36px;height:20px;border-radius:10px;background:var(--off);position:relative;transition:background .15s;flex:none}
.sw::after{content:"";position:absolute;top:2px;left:2px;width:16px;height:16px;border-radius:50%;background:#fff;transition:transform .15s}
.ctl.on .sw{background:var(--on)}
.ctl.on .sw::after{transform:translateX(16px)}
.pos{display:inline-flex;border:1px solid var(--line-strong);border-radius:6px;overflow:hidden;font-size:12px;font-weight:500;flex:none}
.pos i{font-style:normal;padding:2px 9px;color:var(--faint)}
.pos i.a{background:var(--text);color:var(--bg)}
.ctl.busy{opacity:.55;pointer-events:none}

/* Tabellen */
table{width:100%;border-collapse:collapse;font-size:13px}
td{padding:9px 16px;border-top:1px solid var(--line);vertical-align:top}
tr:first-child td{border-top:0}
td.k{color:var(--muted);width:44%}
td.v{text-align:right;font-weight:500;word-break:break-all}
.log td.t{color:var(--faint);font-family:var(--mono);font-size:12px;width:84px;white-space:nowrap}
.log td.m{color:var(--text)}
.log .empty{color:var(--faint);text-align:center;padding:20px}
.tag{font-size:12px;font-weight:500;padding:1px 6px;border-radius:4px;border:1px solid var(--line-strong);color:var(--muted)}
.tag.on{border-color:transparent;background:var(--on-bg);color:var(--on)}
.tag.warn{color:var(--warn);border-color:currentColor}
.fwact{padding:12px 16px;border-top:1px solid var(--line);display:grid;gap:8px}
.fwact .btn{justify-content:center;height:36px}
.fwact input{width:100%;height:32px;padding:0 10px;border:1px solid var(--line-strong);border-radius:6px;background:var(--bg);color:var(--text);font:12px var(--mono)}
.fwact details summary{font-size:12.5px;color:var(--muted);cursor:pointer}
.fwact details[open] summary{margin-bottom:8px}
#otamsg{font-size:12.5px;color:var(--muted)}
#otamsg:empty{display:none}

footer{color:var(--faint);font-size:12.5px;padding:32px 0 48px;text-align:center}

/* dezentes Einblenden beim Scrollen */
.fade{opacity:0;transform:translateY(8px);transition:opacity .5s ease,transform .5s ease}
.fade.in{opacity:1;transform:none}

#toast{position:fixed;bottom:20px;left:50%;transform:translate(-50%,8px);background:var(--text);color:var(--bg);font-size:13px;padding:8px 14px;border-radius:6px;opacity:0;transition:opacity .2s,transform .2s;pointer-events:none;z-index:20}
#toast.show{opacity:1;transform:translate(-50%,0)}

@media (max-width:960px){.grid{grid-template-columns:1fr}}
@media (max-width:640px){
  .wrap{padding:0 16px}
  .top{padding:24px 0 20px}
  .top h1{font-size:26px}
  .actions{width:100%}.actions .btn{flex:1;justify-content:center}
  .stats{grid-template-columns:1fr 1fr}
  .stat:nth-child(odd){border-left:0}
  .stat:nth-child(n+3){border-top:1px solid var(--line)}
  .stat:last-child{grid-column:1/-1}
  .hide-sm,kbd{display:none}
  .tl-row,.tl-axis{grid-template-columns:84px 1fr 40px}
}
@media (prefers-reduced-motion:reduce){*{animation:none!important;transition:none!important}.fade{opacity:1;transform:none}}
</style>
</head>
<body>
<header>
 <div class="wrap">
  <div class="brand">Parcours 1<span class="hide-sm">fischertechnik Dynamic XXL</span></div>
  <div class="grow"></div>
  <div class="conn" id="conn"><i></i><span id="conntxt">verbinde …</span></div>
 </div>
</header>

<main class="wrap">
 <section class="top">
  <div>
   <div class="status"><b></b><span id="stxt">Bereit</span></div>
   <h1 id="headline">Kugelbahn steht</h1>
   <p id="sub">Arduino Giga R1 · Steuerung für alle Geräte im Hotspot</p>
  </div>
  <div class="actions">
   <button class="btn primary" data-c="a" id="mainbtn"><span id="maintxt">Aufzug starten</span><kbd>A</kbd></button>
   <button class="btn stop" data-c="x">Alles stoppen<kbd>X</kbd></button>
  </div>
 </section>

 <dl class="stats fade">
  <div class="stat"><dt>Aufzug-Laufzeit</dt><dd class="num" id="k_run">0:00</dd><small>in dieser Sitzung</small></div>
  <div class="stat"><dt>Giga online</dt><dd class="num" id="k_up">–</dd><small>seit Neustart</small></div>
  <div class="stat"><dt>WLAN</dt><dd class="num" id="k_rssi">–</dd><small id="k_lat">Antwortzeit –</small></div>
  <div class="stat"><dt>Schaltvorgänge</dt><dd class="num" id="k_sw">0</dd><small>Weichen und Stoppstelle</small></div>
  <div class="stat"><dt>Daten des Giga</dt><dd class="num" id="k_dat">–</dd><small id="k_dat_s">seit Neustart, geschätzt</small></div>
 </dl>

 <div class="grid">
  <div class="stack">
   <div class="box schema fade">
    <h2>Bahn <span>antippen zum Schalten</span></h2>
    <svg viewBox="0 0 760 300" role="img" aria-label="Schema der Kugelbahn">
     <path class="rail" d="M90 50 H250 L330 130 H430 L520 70 H640"/>
     <path class="rail" d="M430 130 L520 200 H640"/>
     <path class="rail" d="M520 200 L560 250 H90 V50"/>
     <path class="rail" d="M640 70 V200"/>
     <path class="flow" d="M90 250 V50 H250 L330 130 H430 L520 200 L560 250 H90"/>

     <g class="el" data-c="a">
      <rect x="66" y="40" width="48" height="220" rx="6"/>
      <rect class="lift" x="84" y="60" width="12" height="180" rx="2" style="stroke:none"/>
      <text x="90" y="284" text-anchor="middle">Aufzug</text>
     </g>
     <g class="el" id="gate" data-c="s" transform="translate(290 90)">
      <circle class="b" r="22"/>
      <rect class="gatebar" x="-3" y="-12" width="6" height="24" rx="1"/>
      <text y="44" text-anchor="middle">Stoppstelle</text>
      <text class="val" y="60" text-anchor="middle" id="t_s">zu</text>
     </g>
     <g class="el" data-c="w" transform="translate(430 130)">
      <circle class="b" r="22"/>
      <line class="arm" id="arm_w" x1="0" y1="0" x2="16" y2="-10"/>
      <text y="-32" text-anchor="middle">Stellweiche</text>
      <text class="val" y="44" text-anchor="middle" id="t_w">1</text>
     </g>
     <g class="el" data-c="h" transform="translate(520 200)">
      <circle class="b" r="22"/>
      <line class="arm" id="arm_h" x1="0" y1="0" x2="18" y2="0"/>
      <text x="34" y="-4">Weichenhebel</text>
      <text class="val" x="34" y="12" id="t_h">1</text>
     </g>
     <g class="el" data-c="l" transform="translate(640 70)">
      <rect x="-34" y="-16" width="68" height="32" rx="4"/>
      <rect class="lamp" x="-22" y="-6" width="12" height="12" rx="2"/>
      <rect class="lamp" x="10" y="-6" width="12" height="12" rx="2"/>
      <text y="-26" text-anchor="middle">LEDs</text>
     </g>
    </svg>
   </div>

   <div class="box fade">
    <h2>Verlauf <span>letzte 5 Minuten</span></h2>
    <div class="tl" id="tl"></div>
   </div>
  </div>

  <div class="stack">
   <div class="box fade" id="ctls">
    <h2>Steuerung <span class="hide-sm">Tastenkürzel</span></h2>
    <button class="ctl" data-c="a" id="c_a"><div class="n"><b>Kettenaufzug</b><small id="s_a">aus</small></div><kbd>A</kbd><span class="sw"></span></button>
    <button class="ctl" data-c="w" id="c_w"><div class="n"><b>Stellweiche</b><small>Flexschiene</small></div><kbd>W</kbd><span class="pos"><i id="w1">1</i><i id="w2">2</i></span></button>
    <button class="ctl" data-c="h" id="c_h"><div class="n"><b>Weichenhebel</b><small>Stellweiche mit Hebel</small></div><kbd>H</kbd><span class="pos"><i id="h1">1</i><i id="h2">2</i></span></button>
    <button class="ctl" data-c="s" id="c_s"><div class="n"><b>Stoppstelle</b><small id="s_s">zu</small></div><kbd>S</kbd><span class="sw"></span></button>
    <button class="ctl" data-c="l" id="c_l"><div class="n"><b>LEDs</b><small id="s_l">aus</small></div><kbd>L</kbd><span class="sw"></span></button>
   </div>

   <div class="box fade">
    <h2>Ereignisse</h2>
    <table class="log"><tbody id="log"><tr><td class="empty" colspan="2">Noch keine Ereignisse</td></tr></tbody></table>
   </div>

   <div class="box fade">
    <h2>Firmware</h2>
    <table><tbody>
     <tr><td class="k">Läuft</td><td class="v num" id="f_ver">–</td></tr>
     <tr><td class="k">Neueste auf GitHub</td><td class="v num" id="f_gh">–</td></tr>
     <tr><td class="k">Auto-Update</td><td class="v" id="f_auto">–</td></tr>
     <tr><td class="k">Gebaut</td><td class="v" id="f_date">–</td></tr>
     <tr><td class="k">Adresse</td><td class="v num" id="f_ip">–</td></tr>
    </tbody></table>
    <div class="fwact">
     <div id="otamsg"></div>
     <button class="btn" id="ghbtn">Jetzt von GitHub laden</button>
     <details><summary>Von eigener Adresse laden</summary>
      <input id="url" spellcheck="false" autocomplete="off">
      <button class="btn" id="urlbtn" style="margin-top:8px;width:100%;justify-content:center">Update starten</button>
     </details>
    </div>
   </div>
  </div>
 </div>
</main>
<footer>Code: github.com/JAbstractClass/parcours1-giga</footer>
<div id="toast"></div>

<script>
(()=>{
const $=id=>document.getElementById(id);
const B=document.body;
let S=null,lastOk=0,lat=0,runMs=0,runSince=null,sw=0,up=null;

/* ---------- Verlauf (5 min, 1 Wert pro Sekunde) ---------- */
const N=300;
const TL=[
 {k:'aufzug',name:'Aufzug',  on:v=>v},
 {k:'stopp', name:'Stoppstelle',on:v=>v},
 {k:'weiche',name:'Stellweiche',on:v=>v===2,two:true},
 {k:'hebel', name:'Weichenhebel',on:v=>v===2,two:true},
 {k:'leds',  name:'LEDs',    on:v=>v},
];
const hist=Object.fromEntries(TL.map(t=>[t.k,[]]));
$('tl').innerHTML=TL.map(t=>`<div class="tl-row"><span>${t.name}</span><canvas id="cv_${t.k}" height="14"></canvas><em id="pc_${t.k}">–</em></div>`).join('')+
 '<div class="tl-axis"><span></span><div><span>−5 min</span><span>−2,5 min</span><span>jetzt</span></div><span></span></div>';
const css=n=>getComputedStyle(document.documentElement).getPropertyValue(n).trim();
function drawTL(){
  const cOn=css('--on'),cOff=css('--off'),cAlt=css('--muted');
  TL.forEach(t=>{
    const cv=$('cv_'+t.k),w=cv.clientWidth,dpr=devicePixelRatio||1;
    if(cv.width!==Math.round(w*dpr)){cv.width=Math.round(w*dpr);cv.height=14*dpr;}
    const g=cv.getContext('2d');g.clearRect(0,0,cv.width,cv.height);
    const h=hist[t.k],bw=cv.width/N;let onCount=0;
    for(let i=0;i<h.length;i++){
      const v=h[i];if(v===null)continue;
      const on=t.on(v);if(on)onCount++;
      g.fillStyle=t.two?(on?cAlt:cOff):(on?cOn:cOff);
      g.fillRect(cv.width-(h.length-i)*bw,0,Math.ceil(bw),cv.height);
    }
    const valid=h.filter(v=>v!==null).length;
    $('pc_'+t.k).textContent=valid?(t.two?'Stg. '+(h[h.length-1]):Math.round(onCount/valid*100)+' %'):'–';
  });
}
setInterval(()=>{TL.forEach(t=>{hist[t.k].push(S&&Date.now()-lastOk<4000?S[t.k]:null);if(hist[t.k].length>N)hist[t.k].shift();});drawTL();},1000);
addEventListener('resize',drawTL);

/* ---------- Hilfen ---------- */
const dur=s=>{s=Math.max(0,Math.floor(s));const h=Math.floor(s/3600),m=Math.floor(s%3600/60),x=s%60;return h?`${h}:${String(m).padStart(2,'0')}:${String(x).padStart(2,'0')}`:`${m}:${String(x).padStart(2,'0')}`};
let tt;function toast(m){const t=$('toast');t.textContent=m;t.classList.add('show');clearTimeout(tt);tt=setTimeout(()=>t.classList.remove('show'),2200);}
function log(m){
  const tb=$('log');const e=tb.querySelector('.empty');if(e)e.parentNode.remove();
  const tr=document.createElement('tr');
  tr.innerHTML=`<td class="t">${new Date().toLocaleTimeString('de-DE')}</td><td class="m"></td>`;
  tr.lastChild.textContent=m;tb.prepend(tr);while(tb.children.length>12)tb.lastChild.remove();
}

/* ---------- Anzeige ---------- */
function render(s){
  const p=S;S=s;
  B.classList.toggle('running',s.aufzug);B.classList.toggle('leds-on',s.leds);
  $('stxt').textContent=s.aufzug?'Läuft':'Bereit';
  $('headline').textContent=s.aufzug?'Kugelbahn läuft':'Kugelbahn steht';
  $('maintxt').textContent=s.aufzug?'Aufzug stoppen':'Aufzug starten';
  $('c_a').classList.toggle('on',s.aufzug);$('s_a').textContent=s.aufzug?'läuft':'aus';
  $('c_s').classList.toggle('on',s.stopp);$('s_s').textContent=s.stopp?'offen':'zu';
  $('c_l').classList.toggle('on',s.leds);$('s_l').textContent=s.leds?'an':'aus';
  $('w1').className=s.weiche===1?'a':'';$('w2').className=s.weiche===2?'a':'';
  $('h1').className=s.hebel===1?'a':'';$('h2').className=s.hebel===2?'a':'';
  $('gate').classList.toggle('open',s.stopp);$('t_s').textContent=s.stopp?'offen':'zu';
  $('arm_w').style.transform=s.weiche===1?'none':'rotate(65deg)';
  $('arm_h').style.transform=s.hebel===1?'none':'rotate(50deg)';
  $('t_w').textContent='Stellung '+s.weiche;$('t_h').textContent='Stellung '+s.hebel;

  if(s.aufzug&&!runSince)runSince=Date.now();
  if(!s.aufzug&&runSince){runMs+=Date.now()-runSince;runSince=null;}
  up={v:s.uptime,at:Date.now()};
  $('k_rssi').textContent=s.rssi?s.rssi+' dBm':'–';$('k_lat').textContent='Antwortzeit '+lat+' ms';

  /* Giga-Datenverbrauch: Nutzdaten + ca. 7 KB je HTTPS-Verbindungsaufbau (Schaetzung) */
  if(s.daten!==undefined){
    const b=s.daten+s.abrufe*7000, mb=b/1e6, perH=s.uptime>60?b/s.uptime*3600/1e6:null;
    $('k_dat').textContent=mb<1?Math.round(b/1e3)+' KB':mb.toFixed(1)+' MB';
    $('k_dat_s').textContent=perH!==null?'≈ '+perH.toFixed(1)+' MB/h, geschätzt':'seit Neustart, geschätzt';
  }
  $('f_ver').textContent=s.version;$('f_gh').textContent=s.github||'–';
  $('f_auto').innerHTML=!s.auto?'<span class="tag">aus (lokal gebaut)</span>':(s.github&&s.github!==s.version?'<span class="tag warn">Update folgt</span>':'<span class="tag on">an</span>');
  $('f_date').textContent=s.datum;$('f_ip').textContent=s.ip;$('otamsg').textContent=s.ota||'';

  if(p){
    if(p.version!==s.version){log('Neue Firmware '+s.version);toast('Neue Firmware läuft: '+s.version);}
    if(p.aufzug!==s.aufzug)log(s.aufzug?'Aufzug gestartet':'Aufzug gestoppt');
    if(p.weiche!==s.weiche){log('Stellweiche auf Stellung '+s.weiche);sw++;}
    if(p.hebel!==s.hebel){log('Weichenhebel auf Stellung '+s.hebel);sw++;}
    if(p.stopp!==s.stopp){log(s.stopp?'Stoppstelle geöffnet':'Stoppstelle geschlossen');sw++;}
    if(p.leds!==s.leds)log(s.leds?'LEDs an':'LEDs aus');
    if(p.ota!==s.ota&&s.ota)log(s.ota);
  }else log('Verbunden mit '+s.ip);
  $('k_sw').textContent=sw;
}
setInterval(()=>{
  $('k_run').textContent=dur((runMs+(runSince?Date.now()-runSince:0))/1000);
  if(up)$('k_up').textContent=dur(up.v+(Date.now()-up.at)/1000);
  const ok=Date.now()-lastOk<4000;
  $('conn').className='conn '+(ok?'ok':(lastOk?'bad':''));
  $('conntxt').textContent=ok?'Verbunden · '+lat+' ms':(lastOk?'Verbindung verloren':'verbinde …');
},1000);

/* ---------- Kommunikation ---------- */
async function get(u){const t=performance.now();const r=await fetch(u,{cache:'no-store'});const j=await r.json();lat=Math.round(performance.now()-t);lastOk=Date.now();return j;}
async function poll(){try{render(await get('/status'));}catch(e){}}
async function cmd(c,el){
  if(el)el.classList.add('busy');
  try{render(await get('/cmd?c='+c));if(c==='x')toast('Alles gestoppt');}
  catch(e){toast('Giga nicht erreichbar');}
  finally{if(el)setTimeout(()=>el.classList.remove('busy'),(c==='w'||c==='h')?400:0);}
}
document.querySelectorAll('[data-c]').forEach(el=>el.addEventListener('click',()=>cmd(el.dataset.c,el.classList.contains('ctl')?el:null)));
addEventListener('keydown',e=>{
  if(e.target.tagName==='INPUT'||e.metaKey||e.ctrlKey||e.altKey)return;
  const k=e.key.toLowerCase();if(k.length===1&&'awhslx'.includes(k))cmd(k,document.querySelector('.ctl[data-c="'+k+'"]'));
});
$('ghbtn').onclick=async()=>{toast('Update von GitHub gestartet');log('Update von GitHub angefordert');try{await fetch('/github',{cache:'no-store'});}catch(e){}};
$('urlbtn').onclick=async()=>{const u=$('url').value.trim();if(!u)return;toast('Update gestartet');log('Update von '+u);try{await fetch('/update?url='+encodeURIComponent(u));}catch(e){}};
$('url').value='%OTA_URL%';

/* dezentes Einblenden */
const io=new IntersectionObserver(es=>es.forEach(e=>{if(e.isIntersecting){e.target.classList.add('in');io.unobserve(e.target);}}),{threshold:.08});
document.querySelectorAll('.fade').forEach(el=>io.observe(el));

poll();setInterval(poll,1000);
})();
</script>
</body>
</html>
)DASH";
