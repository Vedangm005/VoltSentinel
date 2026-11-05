// Auto-generated from dashboard.html: the VoltSentinel web dashboard (served from flash).
#pragma once
#include <pgmspace.h>

const char DASHBOARD_HTML[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1, viewport-fit=cover">
<meta name="color-scheme" content="dark light">
<title>VoltSentinel</title>
<link id="fav" rel="icon" href="data:,">
<style>
:root{
  --bg:#0b1016;--bg2:#0f1620;--card:#131c27;--card2:#192534;--line:#223042;--txt:#e8eef6;--mut:#8da0b5;--dim:#5d7186;
  --ok:#2ecc8f;--warn:#ffa931;--crit:#ff4d5e;--acc:#4ea1ff;
  --c-v:#4ea1ff;--c-i:#b794ff;--c-p:#ffb454;--c-c:#ff7a6b;
  --shadow:0 1px 0 rgba(255,255,255,.03) inset,0 8px 24px rgba(0,0,0,.28);
  --r:16px;
}
:root[data-theme=light]{
  --bg:#eef2f7;--bg2:#e6ecf3;--card:#ffffff;--card2:#f3f6fa;--line:#d9e1ea;--txt:#0f1b2a;--mut:#53667b;--dim:#8697aa;
  --ok:#12a56c;--warn:#d98200;--crit:#e0283c;--acc:#1f7ae0;
  --c-v:#1f7ae0;--c-i:#7a4fe0;--c-p:#d97a00;--c-c:#e0472f;
  --shadow:0 1px 2px rgba(15,27,42,.06),0 8px 24px rgba(15,27,42,.07);
}
*{box-sizing:border-box;margin:0}
html{-webkit-text-size-adjust:100%}
body{background:radial-gradient(1200px 500px at 70% -10%,color-mix(in srgb,var(--st,var(--ok)) 10%,transparent),transparent),var(--bg);
  color:var(--txt);font:15px/1.45 system-ui,-apple-system,"Segoe UI",Roboto,"Helvetica Neue",Arial,sans-serif;min-height:100vh;
  padding:env(safe-area-inset-top) env(safe-area-inset-right) env(safe-area-inset-bottom) env(safe-area-inset-left);transition:background .6s}
body[data-s=NORMAL]{--st:var(--ok)}body[data-s=WARNING]{--st:var(--warn)}body[data-s=CRITICAL]{--st:var(--crit)}
.num{font-variant-numeric:tabular-nums}
.wrap{max-width:1180px;margin:0 auto;padding:16px 16px 40px}
button{font:inherit;color:inherit;cursor:pointer}
button:focus-visible,a:focus-visible,[tabindex]:focus-visible{outline:2px solid var(--acc);outline-offset:2px}

/* header */
header{display:flex;align-items:center;gap:12px;padding:6px 0 16px;flex-wrap:wrap}
.logo{display:flex;align-items:center;gap:10px;margin-right:auto}
.logo i{width:38px;height:38px;border-radius:11px;display:grid;place-items:center;background:linear-gradient(135deg,var(--acc),#7b5cff);box-shadow:0 6px 18px rgba(78,161,255,.35)}
.logo h1{font-size:18px;letter-spacing:.2px;line-height:1.1}
.logo small{display:block;color:var(--mut);font-size:12px}
.chip{display:inline-flex;align-items:center;gap:7px;padding:6px 11px;border-radius:999px;background:var(--card);border:1px solid var(--line);font-size:12.5px;color:var(--mut);white-space:nowrap}
.chip b{color:var(--txt);font-weight:600}
.dot{width:8px;height:8px;border-radius:50%;background:var(--ok);box-shadow:0 0 0 0 var(--ok);animation:pulse 2s infinite}
.offline .dot{background:var(--crit);animation:none}
.chip.mode.data{color:var(--acc);border-color:color-mix(in srgb,var(--acc) 45%,var(--line))}
.iconbtn{width:34px;height:34px;border-radius:50%;border:1px solid var(--line);background:var(--card);display:grid;place-items:center;color:var(--mut)}
.iconbtn:hover{color:var(--txt)}
@keyframes pulse{0%{box-shadow:0 0 0 0 color-mix(in srgb,var(--ok) 60%,transparent)}70%,100%{box-shadow:0 0 0 7px transparent}}

/* hero */
.hero{display:flex;align-items:center;gap:18px;padding:18px 20px;border-radius:var(--r);background:
  linear-gradient(100deg,color-mix(in srgb,var(--st) 16%,var(--card)),var(--card) 70%);
  border:1px solid color-mix(in srgb,var(--st) 40%,var(--line));box-shadow:var(--shadow);transition:border-color .4s,background .4s}
.hero .ico{flex:none;width:56px;height:56px;border-radius:16px;display:grid;place-items:center;background:color-mix(in srgb,var(--st) 18%,transparent);color:var(--st)}
.hero .ico svg{width:30px;height:30px}
.hero .lbl{font-size:12px;color:var(--mut);text-transform:uppercase;letter-spacing:.12em}
.hero h2{font-size:26px;line-height:1.15;color:var(--st);letter-spacing:.3px}
.hero p{color:var(--mut);margin-top:2px}
.hero .side{margin-left:auto;text-align:right;color:var(--mut);font-size:12.5px;white-space:nowrap}
.hero .side b{display:block;color:var(--txt);font-size:15px;font-weight:600}
body[data-s=CRITICAL] .hero{animation:glow 1.6s ease-in-out infinite}
@keyframes glow{50%{box-shadow:0 0 0 4px color-mix(in srgb,var(--crit) 25%,transparent),var(--shadow)}}
.offline .hero,.offline .metrics,.offline .panel{opacity:.55;filter:saturate(.5)}
.banner{display:none;margin:12px 0 0;padding:10px 14px;border-radius:12px;background:color-mix(in srgb,var(--crit) 14%,var(--card));border:1px solid color-mix(in srgb,var(--crit) 50%,var(--line));font-size:13.5px}
.offline .banner{display:block}

/* metric cards */
.metrics{display:grid;grid-template-columns:repeat(4,1fr);gap:12px;margin:14px 0}
.metric{display:block;text-align:left;width:100%;padding:14px 14px 12px;border-radius:var(--r);background:var(--card);border:1px solid var(--line);box-shadow:var(--shadow);position:relative;overflow:hidden;transition:border-color .2s,transform .15s}
.metric:hover{transform:translateY(-1px);border-color:color-mix(in srgb,var(--mc) 50%,var(--line))}
.metric[aria-pressed=true]{border-color:var(--mc);box-shadow:0 0 0 1px var(--mc) inset,var(--shadow)}
.metric[data-k=v]{--mc:var(--c-v)}.metric[data-k=i]{--mc:var(--c-i)}.metric[data-k=p]{--mc:var(--c-p)}.metric[data-k=c]{--mc:var(--c-c)}
.metric .t{display:flex;align-items:center;gap:7px;color:var(--mut);font-size:12.5px}
.metric .t i{width:8px;height:8px;border-radius:3px;background:var(--mc)}
.metric .t em{margin-left:auto;font-style:normal;font-size:11px;padding:2px 7px;border-radius:999px;background:var(--card2);color:var(--mut)}
.metric .t em.warn{color:var(--warn);background:color-mix(in srgb,var(--warn) 14%,transparent)}
.metric .t em.crit{color:var(--crit);background:color-mix(in srgb,var(--crit) 14%,transparent)}
.metric .val{display:block;font-size:32px;font-weight:650;letter-spacing:-.5px;margin:6px 0 2px;line-height:1.1}
.metric .val small{font-size:14px;color:var(--mut);font-weight:500;margin-left:4px;letter-spacing:0}
.metric canvas{display:block;width:100%;height:38px;margin-top:6px}
.zone{display:block;position:relative;height:6px;border-radius:99px;margin-top:10px;background:var(--card2);overflow:visible}
.zone .mk{position:absolute;top:-4px;width:3px;height:14px;border-radius:2px;background:var(--txt);box-shadow:0 0 0 2px var(--card);left:50%;transition:left .5s}
.zone.hide{visibility:hidden}

/* panels */
.layout{display:grid;grid-template-columns:minmax(0,2fr) minmax(0,1fr);gap:12px}
.panel{background:var(--card);border:1px solid var(--line);border-radius:var(--r);box-shadow:var(--shadow);padding:16px}
.panel h3{font-size:13px;font-weight:600;color:var(--mut);text-transform:uppercase;letter-spacing:.1em;margin-bottom:12px;display:flex;align-items:center;gap:8px}
.stack{display:grid;gap:12px;align-content:start}
.chead{display:flex;align-items:center;gap:10px;flex-wrap:wrap;margin-bottom:10px}
.chead h3{margin:0}
.seg{display:inline-flex;background:var(--card2);border:1px solid var(--line);border-radius:10px;padding:2px;margin-left:auto}
.seg button{border:0;background:transparent;padding:5px 11px;border-radius:8px;font-size:12.5px;color:var(--mut)}
.seg button[aria-pressed=true]{background:var(--card);color:var(--txt);box-shadow:0 1px 3px rgba(0,0,0,.25)}
.chartbox{position:relative;height:330px;touch-action:pan-y}
.chartbox canvas{position:absolute;inset:0;width:100%;height:100%}
.tip{position:absolute;pointer-events:none;background:var(--card2);border:1px solid var(--line);border-radius:9px;padding:6px 9px;font-size:12px;box-shadow:0 6px 20px rgba(0,0,0,.35);opacity:0;transition:opacity .1s;white-space:nowrap;z-index:2}
.tip b{display:block;font-size:14px}
.cfoot{display:flex;gap:18px;flex-wrap:wrap;margin-top:10px;color:var(--mut);font-size:12.5px}
.cfoot b{color:var(--txt);font-weight:600}
.leg{display:inline-flex;align-items:center;gap:6px}.leg i{width:14px;border-top:2px dashed currentColor}

.rows{display:grid;gap:10px}
.row{display:flex;align-items:center;justify-content:space-between;gap:10px;font-size:14px}
.row span:first-child{color:var(--mut)}
.pill{padding:3px 10px;border-radius:999px;font-size:12px;font-weight:600;background:var(--card2);color:var(--mut);display:inline-flex;gap:5px;align-items:center}
.pill.ok{color:var(--ok);background:color-mix(in srgb,var(--ok) 14%,transparent)}
.pill.warn{color:var(--warn);background:color-mix(in srgb,var(--warn) 14%,transparent)}
.pill.crit{color:var(--crit);background:color-mix(in srgb,var(--crit) 14%,transparent)}
.pill.info{color:var(--acc);background:color-mix(in srgb,var(--acc) 14%,transparent)}
hr{border:0;border-top:1px solid var(--line);margin:12px 0}
.prog{height:8px;border-radius:99px;background:var(--card2);overflow:hidden;margin:8px 0 6px}
.prog i{display:block;height:100%;width:0;background:linear-gradient(90deg,var(--acc),#7b5cff);border-radius:99px;transition:width .6s}
.hint{font-size:12.5px;color:var(--mut)}
.btns{display:flex;gap:8px;flex-wrap:wrap;margin-top:12px}
.btn{padding:8px 13px;border-radius:10px;border:1px solid var(--line);background:var(--card2);font-size:13px;text-decoration:none;color:var(--txt);display:inline-flex;gap:7px;align-items:center}
.btn:hover{border-color:var(--acc)}
.btn.pri{background:var(--acc);border-color:var(--acc);color:#fff}
.btn[aria-disabled=true]{opacity:.45;pointer-events:none}
.soh{font-size:30px;font-weight:650;margin:2px 0}
table{width:100%;border-collapse:collapse;font-size:13.5px}
td{padding:6px 0;border-top:1px solid var(--line)}tr:first-child td{border-top:0}
td:last-child{text-align:right;font-weight:600}
td i{display:inline-block;width:8px;height:8px;border-radius:50%;margin-right:8px}
footer{margin-top:16px;display:flex;gap:10px;flex-wrap:wrap;align-items:center;color:var(--dim);font-size:12.5px}
footer .sp{margin-left:auto}

#toasts{position:fixed;left:50%;bottom:calc(18px + env(safe-area-inset-bottom));transform:translateX(-50%);display:grid;gap:8px;z-index:9;width:min(92vw,420px)}
.toast{background:var(--card2);border:1px solid var(--line);border-left:4px solid var(--acc);border-radius:12px;padding:10px 14px;box-shadow:0 10px 30px rgba(0,0,0,.4);font-size:13.5px;animation:in .25s}
.toast.warn{border-left-color:var(--warn)}.toast.crit{border-left-color:var(--crit)}.toast.ok{border-left-color:var(--ok)}
@keyframes in{from{opacity:0;transform:translateY(8px)}}

@media (max-width:980px){.layout{grid-template-columns:1fr}.metrics{grid-template-columns:repeat(2,1fr)}}
@media (max-width:560px){
  .wrap{padding:12px 12px 32px}.hero{flex-wrap:wrap;padding:16px}.hero h2{font-size:22px}
  .hero .side{margin:0;text-align:left;width:100%;display:flex;gap:6px;align-items:baseline}.hero .side b{display:inline}
  .metric .val{font-size:26px}.chartbox{height:260px}.seg{margin-left:0;width:100%}.seg button{flex:1}
}
@media (prefers-reduced-motion:reduce){*{animation:none!important;transition:none!important}}
</style>
</head>
<body data-s="NORMAL">
<div class="wrap" id="app">

<header>
  <div class="logo">
    <i><svg width="20" height="20" viewBox="0 0 24 24" fill="none" stroke="#fff" stroke-width="2.4" stroke-linecap="round" stroke-linejoin="round"><path d="M13 2 4 14h7l-1 8 9-12h-7z"/></svg></i>
    <div><h1>VoltSentinel</h1><small>Battery monitor &amp; analytics</small></div>
  </div>
  <span class="chip mode" id="modeChip">Mode <b id="modeTxt">—</b></span>
  <span class="chip" id="liveChip"><span class="dot"></span><b id="liveTxt">Connecting…</b></span>
  <button class="iconbtn" id="themeBtn" title="Toggle light / dark" aria-label="Toggle light or dark theme">
    <svg width="17" height="17" viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2" stroke-linecap="round"><circle cx="12" cy="12" r="4.5"/><path d="M12 2v2M12 20v2M4.9 4.9l1.4 1.4M17.7 17.7l1.4 1.4M2 12h2M20 12h2M4.9 19.1l1.4-1.4M17.7 6.3l1.4-1.4"/></svg>
  </button>
</header>

<section class="hero">
  <div class="ico" id="heroIco"></div>
  <div aria-live="polite">
    <div class="lbl">System status</div>
    <h2 id="heroTitle">Connecting…</h2>
    <p id="heroMsg">Waiting for the first reading from the device.</p>
  </div>
  <div class="side"><span>Uptime</span><b class="num" id="uptime">—</b></div>
</section>
<div class="banner" role="alert">Connection to the device was lost. Showing the last known values — retrying automatically…</div>

<section class="metrics" aria-label="Live readings">
  <button class="metric" data-k="v" aria-pressed="true">
    <span class="t"><i></i>Voltage<em id="vTag">—</em></span>
    <span class="val num" id="vVal">—<small>V</small></span>
    <span class="zone" id="vZone"><i class="mk"></i></span>
    <canvas data-spark="v"></canvas>
  </button>
  <button class="metric" data-k="i" aria-pressed="false">
    <span class="t"><i></i>Current<em id="iTag">—</em></span>
    <span class="val num" id="iVal">—<small>mA</small></span>
    <span class="zone hide"><i class="mk"></i></span>
    <canvas data-spark="i"></canvas>
  </button>
  <button class="metric" data-k="p" aria-pressed="false">
    <span class="t"><i></i>Power<em id="pTag">live</em></span>
    <span class="val num" id="pVal">—<small>mW</small></span>
    <span class="zone hide"><i class="mk"></i></span>
    <canvas data-spark="p"></canvas>
  </button>
  <button class="metric" data-k="c" aria-pressed="false">
    <span class="t"><i></i>Temperature<em id="cTag">—</em></span>
    <span class="val num" id="cVal">—<small>°C</small></span>
    <span class="zone" id="cZone"><i class="mk"></i></span>
    <canvas data-spark="c"></canvas>
  </button>
</section>

<div class="layout">
  <section class="panel">
    <div class="chead">
      <h3 id="chartTitle">Voltage</h3>
      <div class="seg" role="group" aria-label="Time range" id="rangeSeg">
        <button data-r="30" aria-pressed="false">30 s</button>
        <button data-r="300" aria-pressed="true">5 min</button>
        <button data-r="1800" aria-pressed="false">30 min</button>
        <button data-r="0" aria-pressed="false">All</button>
      </div>
    </div>
    <div class="chartbox" id="chartBox"><canvas id="chart" aria-label="Historical chart"></canvas><div class="tip" id="tip"></div></div>
    <div class="cfoot">
      <span>Min <b class="num" id="sMin">—</b></span><span>Avg <b class="num" id="sAvg">—</b></span><span>Max <b class="num" id="sMax">—</b></span>
      <span id="legend"></span>
    </div>
  </section>

  <aside class="stack">
    <section class="panel">
      <h3>Behaviour</h3>
      <div class="rows">
        <div class="row"><span>Voltage status</span><span class="pill" id="vStat">—</span></div>
        <div class="row"><span>Temperature status</span><span class="pill" id="tStat">—</span></div>
        <div class="row"><span>Voltage trend</span><span class="pill" id="vTrend">—</span></div>
        <div class="row"><span>Temperature trend</span><span class="pill" id="tTrend">—</span></div>
      </div>
      <hr>
      <div class="rows">
        <div class="row"><span>dV/dt</span><b class="num" id="dV">—</b></div>
        <div class="row"><span>dI/dt</span><b class="num" id="dI">—</b></div>
        <div class="row"><span>dT/dt</span><b class="num" id="dT">—</b></div>
      </div>
    </section>

    <section class="panel">
      <h3>AI data collection</h3>
      <div class="row"><span id="aiCount" class="num">0 / 300 samples</span><span class="pill" id="aiPill">Idle</span></div>
      <div class="prog"><i id="aiBar"></i></div>
      <p class="hint" id="aiHint">Samples are recorded once per second.</p>
      <hr>
      <div class="hint">Health estimate (SOH)</div>
      <div class="soh num" id="soh">—</div>
      <p class="hint" id="sohNote">No estimate yet. Send <b>AITEST</b> over serial to run the on-device model demo.</p>
      <div class="btns">
        <a class="btn pri" id="aiDl" href="/api/ai.csv" download="voltsentinel_ai.csv" aria-disabled="true">⬇ Device CSV</a>
        <button class="btn" id="aiReset">↺ Restart</button>
      </div>
    </section>

    <section class="panel">
      <h3>Safety limits</h3>
      <table>
        <tr><td><i style="background:var(--warn)"></i>Low voltage warning</td><td class="num" id="lWarn">—</td></tr>
        <tr><td><i style="background:var(--crit)"></i>Low voltage critical</td><td class="num" id="lCrit">—</td></tr>
        <tr><td><i style="background:var(--crit)"></i>High voltage critical</td><td class="num" id="lHigh">—</td></tr>
        <tr><td><i style="background:var(--warn)"></i>Temperature warning</td><td class="num" id="tWarn">—</td></tr>
        <tr><td><i style="background:var(--crit)"></i>Temperature critical</td><td class="num" id="tCrit">—</td></tr>
      </table>
    </section>
  </aside>
</div>

<footer>
  <span id="net">—</span>
  <span class="sp"></span>
  <button class="btn" id="pauseBtn" aria-pressed="false">⏸ Pause view</button>
  <button class="btn" id="csvBtn">⬇ Session CSV</button>
</footer>
</div>
<div id="toasts" aria-live="polite"></div>

<script>
(()=>{
"use strict";
const $=s=>document.querySelector(s), $$=s=>[...document.querySelectorAll(s)];
const META={
  v:{name:"Voltage",unit:"V",dec:3,min:.05,cv:"--c-v"},
  i:{name:"Current",unit:"mA",dec:1,min:10,cv:"--c-i"},
  p:{name:"Power",unit:"mW",dec:0,min:50,cv:"--c-p"},
  c:{name:"Temperature",unit:"°C",dec:1,min:1,cv:"--c-c"}
};
const MAXPTS=7200;
let hist=[], last=null, lastTs=-1, selected="v", range=300, paused=false, frozenEnd=0, hover=-1;
let fails=0, offline=false, lim=null, prevSafety=null, colors={}, firstLoad=true;

/* ---------- helpers ---------- */
const fmt=(n,d)=>n==null||!isFinite(n)?"—":n.toFixed(d);
const sgn=(n,d)=>n==null?"—":(n>=0?"+":"−")+Math.abs(n).toFixed(d);
function dur(ms){let s=Math.floor(ms/1000),d=Math.floor(s/86400);s%=86400;const h=Math.floor(s/3600);s%=3600;const m=Math.floor(s/60);s%=60;
  return (d?d+"d ":"")+(h||d?h+"h ":"")+m+"m "+String(s).padStart(2,"0")+"s"}
function clsFor(st){return st==="CRITICAL"?"crit":st==="LOW"||st==="HIGH"||st==="WARNING"?"warn":st==="NORMAL"?"ok":""}
function toast(msg,kind){const t=document.createElement("div");t.className="toast "+(kind||"");t.textContent=msg;$("#toasts").appendChild(t);setTimeout(()=>t.remove(),5000)}
function readColors(){const cs=getComputedStyle(document.documentElement);
  for(const k of["--c-v","--c-i","--c-p","--c-c","--ok","--warn","--crit","--txt","--mut","--dim","--line","--card"])colors[k]=cs.getPropertyValue(k).trim()}

/* ---------- theme ---------- */
function applyTheme(t){document.documentElement.dataset.theme=t;try{localStorage.setItem("vs-theme",t)}catch(e){}readColors();renderAll()}
(function(){let t=null;try{t=localStorage.getItem("vs-theme")}catch(e){}
  if(!t)t=matchMedia("(prefers-color-scheme: light)").matches?"light":"dark";document.documentElement.dataset.theme=t;readColors()})();
$("#themeBtn").onclick=()=>applyTheme(document.documentElement.dataset.theme==="light"?"dark":"light");

/* ---------- charts ---------- */
function windowPts(key){
  if(!hist.length)return[];
  const end=paused?frozenEnd:hist[hist.length-1].t;
  const start=range?end-range*1000:hist[0].t;
  const out=[];for(const p of hist){if(p.t>=start&&p.t<=end&&p[key]!=null)out.push(p)}
  return out;
}
function setupCanvas(cv){
  const r=cv.getBoundingClientRect(),dpr=window.devicePixelRatio||1,w=Math.round(r.width*dpr),h=Math.round(r.height*dpr);
  if(cv.width!==w||cv.height!==h){cv.width=w;cv.height=h}
  const x=cv.getContext("2d");x.setTransform(dpr,0,0,dpr,0,0);x.clearRect(0,0,r.width,r.height);
  return{x,W:r.width,H:r.height};
}
function yRange(vals,key,extra){
  let lo=Infinity,hi=-Infinity;for(const v of vals){if(v<lo)lo=v;if(v>hi)hi=v}
  if(!isFinite(lo)){lo=0;hi=1}
  const span=Math.max(hi-lo,META[key].min),mid=(hi+lo)/2,pad=span*.18;
  lo=mid-span/2-pad;hi=mid+span/2+pad;
  return[lo,hi];
}
function niceTicks(lo,hi,n){
  const raw=(hi-lo)/n,mag=Math.pow(10,Math.floor(Math.log10(raw))),f=raw/mag,step=(f<1.5?1:f<3?2:f<7?5:10)*mag;
  const t=[];for(let v=Math.ceil(lo/step)*step;v<=hi+1e-9;v+=step)t.push(+v.toFixed(10));return{t,step};
}
function limitLines(key){
  if(!lim)return[];
  if(key==="v")return[[lim.lowWarn,"--warn"],[lim.lowCrit,"--crit"],[lim.highCrit,"--crit"]];
  if(key==="c")return[[lim.tWarn,"--warn"],[lim.tCrit,"--crit"]];
  return[];
}
function drawMain(){
  const cv=$("#chart"),{x,W,H}=setupCanvas(cv),key=selected,m=META[key],col=colors[m.cv];
  const pad={l:52,r:14,t:10,b:26},pw=W-pad.l-pad.r,ph=H-pad.t-pad.b;
  const pts=windowPts(key);
  if(pts.length<2){x.fillStyle=colors["--mut"];x.font="13px system-ui";x.textAlign="center";
    x.fillText(pts.length?"Collecting data…":"No data yet",W/2,H/2);updateStats(pts,key);return}
  const t1=pts[pts.length-1].t,t0=range?t1-range*1000:Math.min(pts[0].t,t1-10000);
  let[lo,hi]=yRange(pts.map(p=>p[key]),key);
  const X=t=>pad.l+(t-t0)/(t1-t0)*pw,Y=v=>pad.t+(1-(v-lo)/(hi-lo))*ph;
  // grid
  const{t:ticks,step}=niceTicks(lo,hi,4),dec=Math.max(0,Math.min(4,-Math.floor(Math.log10(step))));
  x.font="11.5px system-ui";x.textBaseline="middle";x.lineWidth=1;
  for(const v of ticks){const y=Math.round(Y(v))+.5;x.strokeStyle=colors["--line"];x.beginPath();x.moveTo(pad.l,y);x.lineTo(W-pad.r,y);x.stroke();
    x.fillStyle=colors["--dim"];x.textAlign="right";x.fillText(v.toFixed(dec),pad.l-8,y)}
  // x labels
  x.textBaseline="alphabetic";x.fillStyle=colors["--dim"];
  const sec=(t1-t0)/1000,labs=4;
  for(let k=0;k<=labs;k++){const s=sec*(1-k/labs);x.textAlign=k===0?"left":k===labs?"right":"center";
    x.fillText(k===labs?"now":"−"+(s>=60?Math.floor(s/60)+":"+String(Math.round(s%60)).padStart(2,"0"):Math.round(s)+" s"),pad.l+pw*k/labs,H-7)}
  // limit lines
  for(const[v,c]of limitLines(key)){if(v<lo||v>hi)continue;const y=Math.round(Y(v))+.5;
    x.save();x.setLineDash([5,5]);x.strokeStyle=colors[c];x.globalAlpha=.75;x.beginPath();x.moveTo(pad.l,y);x.lineTo(W-pad.r,y);x.stroke();x.restore()}
  // series (decimated)
  const stride=Math.max(1,Math.floor(pts.length/(pw*1.2)));
  const path=()=>{x.beginPath();let first=true;for(let k=0;k<pts.length;k+=stride){const p=pts[k];
    first?x.moveTo(X(p.t),Y(p[key])):x.lineTo(X(p.t),Y(p[key]));first=false}
    const q=pts[pts.length-1];x.lineTo(X(q.t),Y(q[key]))};
  path();x.lineTo(X(pts[pts.length-1].t),pad.t+ph);x.lineTo(X(pts[0].t),pad.t+ph);x.closePath();
  const g=x.createLinearGradient(0,pad.t,0,pad.t+ph);g.addColorStop(0,col+"55");g.addColorStop(1,col+"00");x.fillStyle=g;x.fill();
  path();x.strokeStyle=col;x.lineWidth=2.2;x.lineJoin="round";x.stroke();
  // live dot
  const q=pts[pts.length-1];x.fillStyle=col;x.beginPath();x.arc(X(q.t),Y(q[key]),4,0,7);x.fill();
  x.strokeStyle=colors["--card"];x.lineWidth=2;x.stroke();
  // hover
  const tip=$("#tip");
  if(hover>=0){
    const tt=t0+(hover-pad.l)/pw*(t1-t0);let bi=0,bd=1e18;
    for(let k=0;k<pts.length;k++){const d=Math.abs(pts[k].t-tt);if(d<bd){bd=d;bi=k}}
    const p=pts[bi],px=X(p.t),py=Y(p[key]);
    x.strokeStyle=colors["--mut"];x.lineWidth=1;x.setLineDash([3,3]);x.beginPath();x.moveTo(px+.5,pad.t);x.lineTo(px+.5,pad.t+ph);x.stroke();x.setLineDash([]);
    x.fillStyle=col;x.beginPath();x.arc(px,py,5,0,7);x.fill();x.strokeStyle=colors["--card"];x.lineWidth=2;x.stroke();
    const ago=Math.round((t1-p.t)/1000);
    tip.innerHTML=`<b class="num">${p[key].toFixed(m.dec)} ${m.unit}</b>${new Date(p.t).toLocaleTimeString()} · ${ago?ago+" s ago":"now"}`;
    tip.style.opacity=1;const tw=tip.offsetWidth;tip.style.left=Math.min(Math.max(px-tw/2,4),W-tw-4)+"px";tip.style.top=Math.max(py-58,2)+"px";
  }else tip.style.opacity=0;
  updateStats(pts,key);
}
function updateStats(pts,key){
  const m=META[key];let mn=Infinity,mx=-Infinity,sum=0;
  for(const p of pts){const v=p[key];if(v<mn)mn=v;if(v>mx)mx=v;sum+=v}
  const f=v=>pts.length?v.toFixed(m.dec)+" "+m.unit:"—";
  $("#sMin").textContent=f(mn);$("#sMax").textContent=f(mx);$("#sAvg").textContent=f(sum/pts.length);
  const L=limitLines(key);
  $("#legend").innerHTML=L.length?`<span class="leg" style="color:var(--warn)"><i></i>warning</span> &nbsp; <span class="leg" style="color:var(--crit)"><i></i>critical</span>`:"";
}
function drawSpark(cv){
  const key=cv.dataset.spark,{x,W,H}=setupCanvas(cv);
  const end=hist.length?hist[hist.length-1].t:0,pts=hist.filter(p=>p.t>end-60000&&p[key]!=null);
  if(pts.length<2)return;
  const[lo,hi]=yRange(pts.map(p=>p[key]),key),t0=end-60000;
  const X=t=>(t-t0)/60000*W,Y=v=>2+(1-(v-lo)/(hi-lo))*(H-4),col=colors[META[key].cv];
  x.beginPath();pts.forEach((p,k)=>k?x.lineTo(X(p.t),Y(p[key])):x.moveTo(X(p.t),Y(p[key])));
  x.strokeStyle=col;x.lineWidth=1.6;x.lineJoin="round";x.stroke();
  x.lineTo(X(pts[pts.length-1].t),H);x.lineTo(X(pts[0].t),H);x.closePath();
  const g=x.createLinearGradient(0,0,0,H);g.addColorStop(0,col+"40");g.addColorStop(1,col+"00");x.fillStyle=g;x.fill();
}
function renderCharts(){drawMain();$$("canvas[data-spark]").forEach(drawSpark)}

/* chart interaction */
const box=$("#chartBox");
box.addEventListener("pointermove",e=>{hover=e.clientX-box.getBoundingClientRect().left;drawMain()});
box.addEventListener("pointerleave",()=>{hover=-1;drawMain()});
new ResizeObserver(()=>renderCharts()).observe(box);
addEventListener("resize",()=>renderCharts());

/* ---------- zone bars ---------- */
function zoneBar(el,lo,hi,stops,tail,val){
  const pct=v=>Math.min(100,Math.max(0,(v-lo)/(hi-lo)*100));let g=[],prev=0;
  for(const[end,c]of stops){const e=pct(end);g.push(`${colors[c]} ${prev}% ${e}%`);prev=e}
  g.push(`${colors[tail]} ${prev}% 100%`);
  el.style.background=`linear-gradient(90deg,${g.join(",")})`;el.style.opacity=.9;
  el.querySelector(".mk").style.left=`calc(${pct(val)}% - 1.5px)`;
}

/* ---------- state ---------- */
const ICONS={
  NORMAL:'<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.2" stroke-linecap="round" stroke-linejoin="round"><path d="M12 3 4 6v6c0 4.5 3.2 8 8 9 4.8-1 8-4.5 8-9V6z"/><path d="m8.5 12 2.5 2.5L16 9.5"/></svg>',
  WARNING:'<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.2" stroke-linecap="round" stroke-linejoin="round"><path d="M10.3 3.9 1.8 18a2 2 0 0 0 1.7 3h17a2 2 0 0 0 1.7-3L13.7 3.9a2 2 0 0 0-3.4 0z"/><path d="M12 9v4M12 17h.01"/></svg>',
  CRITICAL:'<svg viewBox="0 0 24 24" fill="none" stroke="currentColor" stroke-width="2.2" stroke-linecap="round" stroke-linejoin="round"><circle cx="12" cy="12" r="9.5"/><path d="M12 7v6M12 16.5h.01"/></svg>'
};
const TITLES={NORMAL:"All systems normal",WARNING:"Warning",CRITICAL:"Critical"};
const TREND={RISING:"↗ Rising",FALLING:"↘ Falling",STABLE:"→ Stable",WAIT:"… Collecting"};
function setPill(id,txt,cls){const e=$(id);e.textContent=txt;e.className="pill "+(cls||"")}

function applyState(s){
  last=s;lim=s.lim;
  if(s.ready&&s.at!==lastTs){
    lastTs=s.at;
    hist.push({t:Date.now()-(s.up-s.at),v:s.v,i:s.i,p:s.p,c:s.c});
    if(hist.length>MAXPTS)hist.splice(0,hist.length-MAXPTS);
  }
  if(paused)return;
  renderAll();
}
function renderAll(){
  const s=last;if(!s){return}
  document.body.dataset.s=s.safety;
  // hero
  $("#heroIco").innerHTML=ICONS[s.safety]||ICONS.NORMAL;
  $("#heroTitle").textContent=s.ready?TITLES[s.safety]||s.safety:"Starting up…";
  let msg="All readings are within their limits.";
  if(!s.ready)msg="Waiting for the first sensor reading.";
  else if(s.safety!=="NORMAL"){const r=[];
    if(s.vs!=="NORMAL")r.push(`Voltage ${s.vs.toLowerCase()} (${s.v.toFixed(2)} V)`);
    if(s.c==null)r.push("Temperature sensor not detected");
    else if(s.tst!=="NORMAL")r.push(`Temperature ${s.tst.toLowerCase()} (${s.c.toFixed(1)} °C)`);
    msg=r.join(" · ")||"Check the battery."}
  $("#heroMsg").textContent=msg;
  $("#uptime").textContent=dur(s.up);
  $("#modeTxt").textContent=s.mode==="DATA"?"Data collection":"Monitor";
  $("#modeChip").classList.toggle("data",s.mode==="DATA");
  // metric cards
  $("#vVal").innerHTML=`${fmt(s.v,3)}<small>V</small>`;
  $("#iVal").innerHTML=`${fmt(s.i,1)}<small>mA</small>`;
  $("#pVal").innerHTML=Math.abs(s.p)>=1000?`${fmt(s.p/1000,2)}<small>W</small>`:`${fmt(s.p,0)}<small>mW</small>`;
  $("#cVal").innerHTML=s.c==null?`—<small>°C</small>`:`${fmt(s.c,1)}<small>°C</small>`;
  const vt=$("#vTag");vt.textContent=s.vs;vt.className=clsFor(s.vs);
  const ct=$("#cTag");ct.textContent=s.c==null?"NO SENSOR":s.tst;ct.className=s.c==null?"warn":clsFor(s.tst);
  $("#iTag").textContent=Math.abs(s.i)<5?"idle":s.i>0?"flow +":"flow −";
  if(lim){
    zoneBar($("#vZone"),2.8,4.4,[[lim.lowCrit,"--crit"],[lim.lowWarn,"--warn"],[lim.highCrit,"--ok"]],"--crit",Math.min(4.4,Math.max(2.8,s.v)));
    if(s.c!=null)zoneBar($("#cZone"),10,60,[[lim.tWarn,"--ok"],[lim.tCrit,"--warn"]],"--crit",s.c);
    $("#lWarn").textContent="< "+lim.lowWarn.toFixed(2)+" V";$("#lCrit").textContent="≤ "+lim.lowCrit.toFixed(2)+" V";$("#lHigh").textContent="> "+lim.highCrit.toFixed(2)+" V";
    $("#tWarn").textContent="≥ "+lim.tWarn.toFixed(0)+" °C";$("#tCrit").textContent="≥ "+lim.tCrit.toFixed(0)+" °C";
  }
  // behaviour
  setPill("#vStat",s.vs,clsFor(s.vs));setPill("#tStat",s.c==null?"NO SENSOR":s.tst,s.c==null?"warn":clsFor(s.tst));
  setPill("#vTrend",TREND[s.vt]||s.vt,"");setPill("#tTrend",TREND[s.tt]||s.tt,"");
  $("#dV").textContent=sgn(s.dv,5)+" V/s";$("#dI").textContent=sgn(s.di,3)+" mA/s";$("#dT").textContent=sgn(s.dt,4)+" °C/s";
  // AI
  const a=s.ai,pct=Math.min(100,a.n/a.max*100);
  $("#aiCount").textContent=`${a.n} / ${a.max} samples`;$("#aiBar").style.width=pct+"%";
  if(a.ready){setPill("#aiPill","Complete","ok");$("#aiHint").textContent="Dataset ready — download it as CSV."}
  else{setPill("#aiPill",a.n?"Collecting":"Idle",a.n?"info":"");const left=a.max-a.n;
    $("#aiHint").textContent=a.n?`About ${Math.ceil(left/60)} min left (1 sample / second).`:"Waiting for valid sensor data."}
  $("#aiDl").setAttribute("aria-disabled",a.n?"false":"true");
  $("#aiDl").textContent=`⬇ Device CSV${a.n?" ("+a.n+")":""}`;
  if(a.avail){$("#soh").textContent=a.soh.toFixed(1)+" %";
    $("#sohNote").textContent=a.demo?"Dataset demo: computed from a fixed NASA feature vector. This is NOT your connected battery.":"Model estimate from live data."}
  else{$("#soh").textContent="—";$("#sohNote").innerHTML="No estimate yet. Send <b>AITEST</b> over serial to run the on-device model demo."}
  // net
  $("#net").textContent=`${s.net.ap?"Access-point mode":"Wi‑Fi"} · ${s.net.ip}${s.net.ap?"":" · "+s.net.rssi+" dBm"}`;
  // title / favicon
  document.title=`${s.safety==="NORMAL"?"●":s.safety==="WARNING"?"▲":"■"} ${fmt(s.v,2)} V · VoltSentinel`;
  const c=colors[s.safety==="CRITICAL"?"--crit":s.safety==="WARNING"?"--warn":"--ok"];
  $("#fav").href="data:image/svg+xml,"+encodeURIComponent(`<svg xmlns='http://www.w3.org/2000/svg' viewBox='0 0 32 32'><circle cx='16' cy='16' r='14' fill='${c}'/><path d='M17 6 9 18h6l-1 8 8-12h-6z' fill='#fff'/></svg>`);
  // transitions
  if(prevSafety&&prevSafety!==s.safety&&s.ready){
    toast(`Status changed: ${prevSafety} → ${s.safety}`,s.safety==="CRITICAL"?"crit":s.safety==="WARNING"?"warn":"ok");
    if(navigator.vibrate&&s.safety==="CRITICAL")navigator.vibrate([200,100,200]);
  }
  prevSafety=s.safety;
  renderCharts();
}

/* ---------- polling ---------- */
async function getJSON(url,ms){
  const ac=new AbortController(),t=setTimeout(()=>ac.abort(),ms||2500);
  try{const r=await fetch(url,{cache:"no-store",signal:ac.signal});if(!r.ok)throw new Error(r.status);return await r.json()}finally{clearTimeout(t)}
}
function setLive(on){
  if(on===!offline)return;offline=!on;document.body.classList.toggle("offline",offline);
  $("#liveTxt").textContent=on?"Live":"Offline";
  if(on&&!firstLoad)toast("Reconnected to device","ok");
  if(!on)toast("Lost connection to the device","crit");
}
async function tick(){
  try{const s=await getJSON("/api/state");fails=0;firstLoad&&(firstLoad=false);setLive(true);$("#liveTxt").textContent="Live · 1 s";applyState(s)}
  catch(e){if(++fails>=3){setLive(false)}else if(firstLoad)$("#liveTxt").textContent="Connecting…"}
  setTimeout(tick,1000);
}
async function boot(){
  try{
    const h=await getJSON("/api/history",4000),now=Date.now(),n=h.v.length;
    for(let k=0;k<n;k++)hist.push({t:now-(n-1-k)*1000,v:h.v[k],i:h.i[k],p:h.p[k],c:h.c[k]});
  }catch(e){}
  tick();
}

/* ---------- controls ---------- */
$$(".metric").forEach(b=>b.onclick=()=>{selected=b.dataset.k;
  $$(".metric").forEach(o=>o.setAttribute("aria-pressed",o===b));$("#chartTitle").textContent=META[selected].name;hover=-1;renderCharts()});
$$("#rangeSeg button").forEach(b=>b.onclick=()=>{range=+b.dataset.r;
  $$("#rangeSeg button").forEach(o=>o.setAttribute("aria-pressed",o===b));renderCharts()});
$("#pauseBtn").onclick=function(){paused=!paused;this.setAttribute("aria-pressed",paused);
  this.textContent=paused?"▶ Resume view":"⏸ Pause view";if(paused)frozenEnd=hist.length?hist[hist.length-1].t:0;else renderAll()};
$("#csvBtn").onclick=()=>{
  if(!hist.length){toast("No data recorded yet");return}
  const rows=["time,voltage_V,current_mA,power_mW,temperature_C"];
  for(const p of hist)rows.push([new Date(p.t).toISOString(),p.v,p.i,p.p,p.c==null?"":p.c].join(","));
  const a=document.createElement("a");a.href=URL.createObjectURL(new Blob([rows.join("\n")],{type:"text/csv"}));
  a.download="voltsentinel_session.csv";a.click();setTimeout(()=>URL.revokeObjectURL(a.href),2000);
};
$("#aiReset").onclick=async()=>{
  if(!confirm("Discard the collected AI samples and start again?"))return;
  try{const r=await fetch("/api/ai/reset",{method:"POST"});if(!r.ok)throw 0;toast("AI collection restarted","ok")}catch(e){toast("Could not reach the device","crit")}
};
document.addEventListener("visibilitychange",()=>{if(!document.hidden)renderCharts()});

boot();
})();
</script>
</body>
</html>
)rawliteral";
