(function(){/*

 Copyright The Closure Library Authors.
 SPDX-License-Identifier: Apache-2.0
*/
'use strict';var aa=this||self;function g(a,b){a=a.split(".");var c=aa;a[0]in c||"undefined"==typeof c.execScript||c.execScript("var "+a[0]);for(var d;a.length&&(d=a.shift());)a.length||void 0===b?c[d]&&c[d]!==Object.prototype[d]?c=c[d]:c=c[d]={}:c[d]=b};var l={},m=null;var n="undefined"!==typeof Uint8Array;const p=Symbol(void 0);function q(a,b){Object.isFrozen(a)||(p?a[p]|=b:void 0!==a.m?a.m|=b:Object.defineProperties(a,{m:{value:b,configurable:!0,writable:!0,enumerable:!1}}))}function r(a){let b;p?b=a[p]:b=a.m;return null==b?0:b}function t(a){q(a,1);return a}function w(a){return Array.isArray(a)?!!(r(a)&2):!1};function x(a){return null!==a&&"object"===typeof a&&!Array.isArray(a)&&a.constructor===Object}var y=Object.freeze(t([]));function z(a){if(w(a.j))throw Error("Cannot mutate an immutable Message");}function A(a){return{value:a,configurable:!1,writable:!1,enumerable:!1}};function B(a,b=ba){return C(a,b)}function D(a,b){if(null!=a){if(Array.isArray(a))a=C(a,b);else if(x(a)){const c={};for(let d in a)c[d]=D(a[d],b);a=c}else a=b(a);return a}}function C(a,b){const c=a.slice();for(let d=0;d<c.length;d++)c[d]=D(c[d],b);Array.isArray(a)&&r(a)&1&&t(c);return c}
function E(a){if(a&&"object"==typeof a&&a.toJSON)return a.toJSON();a:switch(typeof a){case "number":a=isFinite(a)?a:String(a);break a;case "object":if(a&&!Array.isArray(a)&&n&&null!=a&&a instanceof Uint8Array){var b;void 0===b&&(b=0);if(!m){m={};for(var c="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789".split(""),d=["+/=","+/","-_=","-_.","-_"],e=0;5>e;e++){var f=c.concat(d[e].split(""));l[e]=f;for(var h=0;h<f.length;h++){var k=f[h];void 0===m[k]&&(m[k]=h)}}}b=l[b];c=Array(Math.floor(a.length/
3));d=b[64]||"";for(e=f=0;f<a.length-2;f+=3){var u=a[f],v=a[f+1];k=a[f+2];h=b[u>>2];u=b[(u&3)<<4|v>>4];v=b[(v&15)<<2|k>>6];k=b[k&63];c[e++]=h+u+v+k}h=0;k=d;switch(a.length-f){case 2:h=a[f+1],k=b[(h&15)<<2]||d;case 1:a=a[f],c[e]=b[a>>2]+b[(a&3)<<4|h>>4]+k+d}a=c.join("")}}return Array.isArray(a)?B(a,E):a}function ba(a){return n&&null!=a&&a instanceof Uint8Array?new Uint8Array(a):a};function F(a,b,c=!1){return-1===b?null:b>=a.i?a.g?a.g[b]:void 0:c&&a.g&&(c=a.g[b],null!=c)?c:a.j[b+a.h]}function G(a,b,c,d=!1,e=!1){e||z(a);b<a.i&&!d?a.j[b+a.h]=c:(a.g||(a.g=a.j[a.i+a.h]={}))[b]=c}function H(a,b){a=F(a,b);return null==a?a:!!a}function ca(a){a=F(a,3);return null==a?0:a}function da(a){a=H(a,1);return null==a?!1:a}function I(a,b,c,d){z(a);c!==d?G(a,b,c):G(a,b,void 0,!1,!1)}
function J(a,b,c){if(-1===c)return null;a.l||(a.l={});var d=a.l[c];if(d)return d;let e=F(a,c,!1);if(null==e)return d;b=new b(e);if(w(a.j)){d=b.j;if(!Array.isArray(d))throw Error("cannot mark non-array as immutable");q(d,2)}return a.l[c]=b};function K(a,b){if(null==b||""==b)return new a;b=JSON.parse(b);if(!Array.isArray(b))throw a=typeof b,Error("Expected to deserialize an Array but got "+("object"!=a?a:b?Array.isArray(b)?"array":a:"null")+": "+b);L=b;a=new a(b);L=null;return a}
var M=class{constructor(a,b,c){a||(a=L);L=null;var d=this.constructor.h;a||(a=d?[d]:[]);this.h=(d?0:-1)-(this.constructor.g||0);this.l=void 0;this.j=a;a:{d=this.j.length;a=d-1;if(d&&(d=this.j[a],x(d))){this.i=a-this.h;this.g=d;break a}void 0!==b&&-1<b?(this.i=Math.max(b,a+1-this.h),this.g=void 0):this.i=Number.MAX_VALUE}if(c)for(b=0;b<c.length;b++)if(a=c[b],a<this.i)a+=this.h,(d=this.j[a])?Array.isArray(d)&&t(d):this.j[a]=y;else{d=this.g||(this.g=this.j[this.i+this.h]={});let e=d[a];e?Array.isArray(e)&&
t(e):d[a]=y}}toJSON(){return B(this.j,E)}};M.prototype.toString=function(){return this.j.toString()};let L;var N=class extends M{};Object.defineProperties(N,{[Symbol.hasInstance]:A(()=>{throw Error("Cannot perform instanceof checks for MutableMessage");})});class O extends N{}Object.defineProperties(O,{[Symbol.hasInstance]:A(Object[Symbol.hasInstance])});var P=class extends O{constructor(a){super(a,-1,ea)}},ea=[8];var Q=class extends O{constructor(a){super(a,-1,fa)}},fa=[17];var ia=class extends O{constructor(a){super(a,-1,ha)}},ha=[27];var ja=class extends O{constructor(a){super(a)}};var la=class extends O{constructor(a){super(a,-1,ka)}},ka=[1,2,3,6];var na=class extends O{constructor(){super(void 0,-1,ma)}},ma=[12];/*

 SPDX-License-Identifier: Apache-2.0
*/
function R(a,b){a=a.getElementsByTagName("META");for(let c=0;c<a.length;++c)if(a[c].getAttribute("name")===b)return a[c].getAttribute("content")||"";return""};function S(a){if(!(0<a.g.length)){var b=R(a.body,"environment");for(const c of b.split("|"))c&&a.g.push(c)}}function T(a,b){a=a.body;var c={detail:void 0};let d;"function"===typeof window.CustomEvent?d=new CustomEvent(b,c):(d=document.createEvent("CustomEvent"),d.initCustomEvent(b,!!c.bubbles,!!c.cancelable,c.detail));a.dispatchEvent(d)}
var oa=class{constructor(a){this.body=a;this.g=[];this.h={};R(a,"sampling_mod");var b=R(a,"namespace");if(!b){b="ns-"+(0,Math.random)().toString(36).substr(2,5);a:{var c=a.getElementsByTagName("META");for(let d=0;d<c.length;++d)if("namespace"===c[d].getAttribute("name")&&c[d].getAttribute("index")===(0).toString()){c[d].setAttribute("content",b);break a}c=a.querySelector("#mys-meta");c||(c=document.createElement("div"),c.id="mys-meta",c.style.position="absolute",c.style.display="none",a.appendChild(c));
a=document.createElement("META");a.setAttribute("name","namespace");a.setAttribute("content",b);a.setAttribute("index",(0).toString());c.appendChild(a)}}S(this);!this.h.hasOwnProperty("META_LOCKED")&&(a=R(this.body,"custom_flags"))&&(this.h=JSON.parse(a))}addEventListener(a,b){this.body.addEventListener(a,b)}};var pa=class extends O{constructor(){super(void 0)}};function U(a){var b=document;a=String(a);"application/xhtml+xml"===b.contentType&&(a=a.toLowerCase());return b.createElement(a)};function V(a){if(1>=a.h.offsetWidth||1>=a.h.offsetHeight)return!1;a.g.remove();T(a.context,"spanReady");return!0}
var qa=class{constructor(a,b){this.context=a;this.i=b;this.h=U("SPAN");this.g=U("DIV");this.h.style.fontSize="6px";this.h.textContent="go";this.g.style.position="absolute";this.g.style.top="100%";this.g.style.left="100%";this.g.style.width="1px";this.g.style.height="0";this.g.style.overflow="hidden";this.g.style.visibility="hidden";this.g.appendChild(this.h)}wait(){if(!da(this.i)&&(T(this.context,"spanStart"),this.context.body.appendChild(this.g),!V(this)))return new Promise(a=>{const b=setInterval(()=>
{V(this)&&(clearInterval(b),a())},ca(this.i))})}};var ra=class{constructor(a,b){this.context=a;this.h=J(b,ia,1);this.g=J(b,P,12)||new P;J(this.h,Q,10)||new Q}};var sa=class{constructor(){this.channel=3;(this.g=!(!window.mys||!window.mys.pingback))&&this.setData(43,Date.now()-window.mys.pingback.getBaseTime())}setAttribute(a,b){this.g&&window.mys.pingback.setAttribute(a,b)}setData(a,b){this.g&&window.mys.pingback.setData(a,b,this.channel)}send(a){this.g&&window.mys.pingback.send(a)}};function W(a){a.g&=-31}function X(a,b){a.g|=b}var ta=class{constructor(a,b){this.context=a;this.l=b;new na;this.g=0;this.pingback=new sa}h(){}};function Y(a){K(la,R(a.context.body,"engine_msg")||"[]");return a.g.h()||Promise.resolve()}
var ua=class{constructor(a,b,c,d){this.g=d(a,b,c);this.context=this.g.context}init(){W(this.g);X(this.g,1);let a;null==(a=window.AFMA_Communicator)||a.addEventListener("onshow",()=>{X(this.g,32)});let b=0;const c=this.context.body;c.addEventListener("browserRender",()=>{++b;if(1===b)T(this.context,"overallStart"),Y(this).then(()=>{T(this.context,"overallQuiet")});else{var d=c.clientHeight;c.clientWidth&&d&&Y(this)}})}};function va(a){a.i.length=0;a.h=!0}function Z(a,b){a.g=!0;const c=()=>{a.h=!1;const d=a.i.shift();return void 0===d?(a.g=!1,Promise.resolve()):Z(a,d())};return b?b.then(c,()=>{if(a.h)return c();a.g=!1;return Promise.reject()}):c()}function wa(a,b){for(const c of b)a.i.push(c);if(!a.g)return Z(a)}var xa=class{constructor(){this.h=this.g=!1;this.i=[]}};var ya=class extends ta{constructor(){super(...arguments);this.i=new xa}h(){va(this.i);return wa(this.i,[()=>{},()=>{let a;var b=this.l;const c=new pa;I(c,3,100,0);I(c,4,1E4,0);I(c,5,!0,!1);H(b.g,3)&&I(c,2,!0,!1);b=b.context;S(b);0<=b.g.indexOf("amp")&&I(c,2,!0,!1);c&&(a=(new qa(this.context,c)).wait());T(this.context,"browserStart");T(this.context,"browserStartEnd");W(this);X(this,2);return a},()=>{T(this.context,"browserReady");T(this.context,"browserReadyEnd");X(this,4);T(this.context,"overallReady")},
()=>{T(this.context,"browserQuiet");T(this.context,"browserQuietEnd");X(this,8)}])}};(function(a){let b=null;g("mys.engine.init",(c,d)=>{const e=K(ja,R(d,"render_config")||"[]");b=new ua(c,d,e,a);b.init()});g("mys.engine.stage",()=>b?b.g.g:0)})(function(a,b,c){a=new oa(b);c=new ra(a,c);return new ya(a,c)});}).call(this);
