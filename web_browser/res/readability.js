var readStyle="style-newspaper";var readSize="size-medium";var readMargin="margin-narrow";var dbg=function(a){};var readability={version:"1.5.0",iframeLoads:0,frameHack:false,bodyCache:null,flags:1|2,FLAG_STRIP_UNLIKELYS:1,FLAG_WEIGHT_CLASSES:2,regexps:{unlikelyCandidatesRe:/combx|comment|disqus|foot|header|menu|meta|rss|shoutbox|sidebar|sponsor/i,okMaybeItsACandidateRe:/and|article|body|column|main/i,positiveRe:/article|body|content|entry|hentry|page|pagination|post|text/i,negativeRe:/combx|comment|contact|foot|footer|footnote|link|media|meta|promo|related|scroll|shoutbox|sponsor|tags|widget/i,divToPElementsRe:/<(a|blockquote|dl|div|img|ol|p|pre|table|ul)/i,replaceBrsRe:/(<br[^>]*>[ \n\r\t]*){2,}/gi,replaceFontsRe:/<(\/?)font[^>]*>/gi,trimRe:/^\s+|\s+$/g,normalizeRe:/\s{2,}/g,killBreaksRe:/(<br\s*\/?>(\s|&nbsp;?)*){1,}/g,videoRe:/http:\/\/(www\.)?(youtube|vimeo)\.com/i},init:function(){document.body.style.display="none";if(document.body&&!readability.bodyCache){readability.bodyCache=document.body.innerHTML}readability.prepDocument();var c=document.createElement("DIV");var d=document.createElement("DIV");var e=readability.getArticleTitle();var b=readability.grabArticle();var g=readability.getArticleFooter();if(readability.getInnerText(b,false).length<250){if(readability.flagIsActive(readability.FLAG_STRIP_UNLIKELYS)){readability.removeFlag(readability.FLAG_STRIP_UNLIKELYS);document.body.innerHTML=readability.bodyCache;return readability.init()}else{if(readability.flagIsActive(readability.FLAG_WEIGHT_CLASSES)){readability.removeFlag(readability.FLAG_WEIGHT_CLASSES);document.body.innerHTML=readability.bodyCache;return readability.init()}else{b.innerHTML="<p>Sorry, Article Mode was unable to detect an article on this page. Please note that Article Mode was intended for use on individual article pages and not home pages.</p>"}}}c.id="readOverlay";d.id="readInner";document.body.className=readStyle;if(readStyle=="style-athelas"||readStyle=="style-apertura"){c.className=readStyle+" rdbTypekit"}else{c.className=readStyle}d.className=readMargin+" "+readSize;d.appendChild(e);d.appendChild(b);d.appendChild(g);c.appendChild(d);document.body.innerHTML="";document.body.insertBefore(c,document.body.firstChild);if(readability.frameHack){var f=document.getElementById("readOverlay");f.style.height="100%";f.style.overflow="auto"}if((window.location.protocol+"//"+window.location.host+"/")==window.location.href){b.style.display="none";var a=document.createElement("p");a.id="readability-warning";a.innerHTML='Article Mode is intended for use on individual article pages and not home pages. If you still want to turn on Article Mode, <a onClick=\'javascript:document.getElementById("readability-warning").style.display="none";document.getElementById("readability-content").style.display="block";\'>click here</a>.';d.insertBefore(a,b)}document.body.style.display="block";window.scrollTo(0,0)},getArticleTitle:function(){var f="",b="";try{f=b=document.title;if(typeof f!="string"){f=b=readability.getInnerText(document.getElementsByTagName("title")[0])}}catch(d){}if(f.match(/ [\|\-] /)){f=b.replace(/(.*)[\|\-] .*/gi,"$1");if(f.split(" ").length<3){f=b.replace(/[^\|\-]*[\|\-](.*)/gi,"$1")}}else{if(f.indexOf(": ")!==-1){f=b.replace(/.*:(.*)/gi,"$1");if(f.split(" ").length<3){f=b.replace(/[^:]*[:](.*)/gi,"$1")}}else{if(f.length>150||f.length<15){var a=document.getElementsByTagName("h1");if(a.length==1){f=readability.getInnerText(a[0])}}}}f=f.replace(readability.regexps.trimRe,"");if(f.split(" ").length<=4){f=b}var c=document.createElement("H1");c.innerHTML=f;return c},getArticleFooter:function(){var a=document.createElement("DIV");a.id="readFooter";return a},prepDocument:function(){if(document.body===null){var o=document.createElement("body");try{document.body=o}catch(p){document.documentElement.appendChild(o);dbg(p)}}var q=document.getElementsByTagName("frame");if(q.length>0){var f=null;var s=0;for(var d=0;d<q.length;d++){var l=q[d].offsetWidth+q[d].offsetHeight;var r=false;try{var a=q[d].contentWindow.document.body;r=true}catch(n){dbg(n)}if(r&&l>s){f=q[d];s=l}}if(f){var b=document.createElement("body");b.innerHTML=f.contentWindow.document.body.innerHTML;b.style.overflow="scroll";document.body=b;var j=document.getElementsByTagName("frameset")[0];if(j){j.parentNode.removeChild(j)}readability.frameHack=true}}var h=document.getElementsByTagName("script");for(var m=h.length-1;m>=0;m--){if(typeof(h[m].src)=="undefined"||(h[m].src.indexOf("readability")==-1&&h[m].src.indexOf("typekit")==-1)){h[m].parentNode.removeChild(h[m])}}for(var g=0;g<document.styleSheets.length;g++){if(document.styleSheets[g].href!==null&&document.styleSheets[g].href.lastIndexOf("readability")==-1){document.styleSheets[g].disabled=true}}var c=document.getElementsByTagName("style");for(var t=0;t<c.length;t++){if(navigator.appName!="Microsoft Internet Explorer"){c[t].textContent=""}}document.body.innerHTML=document.body.innerHTML.replace(readability.regexps.replaceBrsRe,"</p><p>").replace(readability.regexps.replaceFontsRe,"<$1span>")},prepArticle:function(b){readability.cleanStyles(b);readability.killBreaks(b);readability.clean(b,"form");readability.clean(b,"object");readability.clean(b,"h1");if(b.getElementsByTagName("h2").length==1){readability.clean(b,"h2")}readability.clean(b,"iframe");readability.cleanHeaders(b);readability.cleanConditionally(b,"table");readability.cleanConditionally(b,"ul");readability.cleanConditionally(b,"div");var g=b.getElementsByTagName("p");for(var d=g.length-1;d>=0;d--){var c=g[d].getElementsByTagName("img").length;var f=g[d].getElementsByTagName("embed").length;var a=g[d].getElementsByTagName("object").length;if(c===0&&f===0&&a===0&&readability.getInnerText(g[d],false)==""){g[d].parentNode.removeChild(g[d])}}try{b.innerHTML=b.innerHTML.replace(/<br[^>]*>\s*<p/gi,"<p")}catch(h){dbg("Cleaning innerHTML of breaks failed. This is an IE strict-block-elements bug. Ignoring.: "+h)}},initializeNode:function(a){a.readability={contentScore:0};switch(a.tagName){case"DIV":a.readability.contentScore+=5;break;case"PRE":case"TD":case"BLOCKQUOTE":a.readability.contentScore+=3;break;case"ADDRESS":case"OL":case"UL":case"DL":case"DD":case"DT":case"LI":case"FORM":a.readability.contentScore-=3;break;case"H1":case"H2":case"H3":case"H4":case"H5":case"H6":case"TH":a.readability.contentScore-=5;break}a.readability.contentScore+=readability.getClassWeight(a)},grabArticle:function(){var v=readability.flagIsActive(readability.FLAG_STRIP_UNLIKELYS);var A=null;var j=[];for(var E=0;(A=document.getElementsByTagName("*")[E]);E++){if(v){var C=A.className+A.id;if(C.search(readability.regexps.unlikelyCandidatesRe)!==-1&&C.search(readability.regexps.okMaybeItsACandidateRe)==-1&&A.tagName!=="BODY"){dbg("Removing unlikely candidate - "+C);A.parentNode.removeChild(A);E--;continue}}if(A.tagName==="P"||A.tagName==="TD"){j[j.length]=A}if(A.tagName==="DIV"){if(A.innerHTML.search(readability.regexps.divToPElementsRe)===-1){dbg("Altering div to p");var a=document.createElement("p");try{a.innerHTML=A.innerHTML;A.parentNode.replaceChild(a,A);E--}catch(H){dbg("Could not alter div to p, probably an IE restriction, reverting back to div.: "+H)}}else{for(var D=0,t=A.childNodes.length;D<t;D++){var b=A.childNodes[D];if(b.nodeType==3){dbg("replacing text node with a p tag with the same content.");var x=document.createElement("p");x.innerHTML=b.nodeValue;x.style.display="inline";x.className="readability-styled";b.parentNode.replaceChild(x,b)}}}}}var g=[];for(var y=0;y<j.length;y++){var n=j[y].parentNode;var J=n.parentNode;var k=readability.getInnerText(j[y]);if(k.length<25){continue}if(typeof n.readability=="undefined"){readability.initializeNode(n);g.push(n)}if(typeof J.readability=="undefined"){readability.initializeNode(J);g.push(J)}var B=0;B++;B+=k.split(",").length;B+=Math.min(Math.floor(k.length/100),3);n.readability.contentScore+=B;J.readability.contentScore+=B/2}var r=null;for(var I=0,q=g.length;I<q;I++){g[I].readability.contentScore=g[I].readability.contentScore*(1-readability.getLinkDensity(g[I]));dbg("Candidate: "+g[I]+" ("+g[I].className+":"+g[I].id+") with score "+g[I].readability.contentScore);if(!r||g[I].readability.contentScore>r.readability.contentScore){r=g[I]}}if(r===null||r.tagName=="BODY"){r=document.createElement("DIV");r.innerHTML=document.body.innerHTML;document.body.innerHTML="";document.body.appendChild(r);readability.initializeNode(r)}var z=document.createElement("DIV");z.id="readability-content";var o=Math.max(10,r.readability.contentScore*0.2);var h=r.parentNode.childNodes;for(var w=0,G=h.length;w<G;w++){var f=h[w];var F=false;dbg("Looking at sibling node: "+f+" ("+f.className+":"+f.id+")"+((typeof f.readability!="undefined")?(" with score "+f.readability.contentScore):""));dbg("Sibling has score "+(f.readability?f.readability.contentScore:"Unknown"));if(f===r){F=true}if(typeof f.readability!="undefined"&&f.readability.contentScore>=o){F=true}if(f.nodeName=="P"){var u=readability.getLinkDensity(f);var m=readability.getInnerText(f);var K=m.length;if(K>80&&u<0.25){F=true}else{if(K<80&&u===0&&m.search(/\.( |$)/)!==-1){F=true}}}if(F){dbg("Appending node: "+f);var d=null;if(f.nodeName!="DIV"&&f.nodeName!="P"){dbg("Altering siblingNode of "+f.nodeName+" to div.");d=document.createElement("div");try{d.id=f.id;d.innerHTML=f.innerHTML}catch(l){dbg("Could not alter siblingNode to div, probably an IE restriction, reverting back to original.");d=f;w--;G--}}else{d=f;w--;G--}d.className="";z.appendChild(d)}}readability.prepArticle(z);return z},getInnerText:function(c,a){var b="";a=(typeof a=="undefined")?true:a;if(navigator.appName=="Microsoft Internet Explorer"){b=c.innerText.replace(readability.regexps.trimRe,"")}else{b=c.textContent.replace(readability.regexps.trimRe,"")}if(a){return b.replace(readability.regexps.normalizeRe," ")}else{return b}},getCharCount:function(b,a){a=a||",";return readability.getInnerText(b).split(a).length-1},cleanStyles:function(a){a=a||document;var b=a.firstChild;if(!a){return}if(typeof a.removeAttribute=="function"&&a.className!="readability-styled"){a.removeAttribute("style")}while(b!==null){if(b.nodeType==1){if(b.className!="readability-styled"){b.removeAttribute("style")}readability.cleanStyles(b)}b=b.nextSibling}},getLinkDensity:function(g){var b=g.getElementsByTagName("a");var d=readability.getInnerText(g).length;var f=0;for(var c=0,a=b.length;c<a;c++){f+=readability.getInnerText(b[c]).length}return f/d},getClassWeight:function(b){if(!readability.flagIsActive(readability.FLAG_WEIGHT_CLASSES)){return 0}var a=0;if(b.className!=""){if(b.className.search(readability.regexps.negativeRe)!==-1){a-=25}if(b.className.search(readability.regexps.positiveRe)!==-1){a+=25}}if(typeof(b.id)=="string"&&b.id!=""){if(b.id.search(readability.regexps.negativeRe)!==-1){a-=25}if(b.id.search(readability.regexps.positiveRe)!==-1){a+=25}}return a},killBreaks:function(b){try{b.innerHTML=b.innerHTML.replace(readability.regexps.killBreaksRe,"<br />")}catch(a){dbg("KillBreaks failed - this is an IE bug. Ignoring.: "+a)}},clean:function(g,a){var h=g.getElementsByTagName(a);var b=(a=="object"||a=="embed");for(var j=h.length-1;j>=0;j--){if(b){var f="";for(var d=0,c=h[j].attributes.length;d<c;d++){f+=h[j].attributes[d].value+"|"}if(f.search(readability.regexps.videoRe)!==-1){continue}if(h[j].innerHTML.search(readability.regexps.videoRe)!==-1){continue}}h[j].parentNode.removeChild(h[j])}},cleanConditionally:function(s,t){var b=s.getElementsByTagName(t);var m=b.length;for(var r=m-1;r>=0;r--){var j=readability.getClassWeight(b[r]);var q=(typeof b[r].readability!="undefined")?b[r].readability.contentScore:0;dbg("Cleaning Conditionally "+b[r]+" ("+b[r].className+":"+b[r].id+")"+((typeof b[r].readability!="undefined")?(" with score "+b[r].readability.contentScore):""));if(j+q<0){b[r].parentNode.removeChild(b[r])}else{if(readability.getCharCount(b[r],",")<10){var l=b[r].getElementsByTagName("p").length;var u=b[r].getElementsByTagName("img").length;var k=b[r].getElementsByTagName("li").length-100;var g=b[r].getElementsByTagName("input").length;var f=0;var o=b[r].getElementsByTagName("embed");for(var a=0,d=o.length;a<d;a++){if(o[a].src.search(readability.regexps.videoRe)==-1){f++}}var h=readability.getLinkDensity(b[r]);var c=readability.getInnerText(b[r]).length;var n=false;if(u>l){n=true}else{if(k>l&&t!="ul"&&t!="ol"){n=true}else{if(g>Math.floor(l/3)){n=true}else{if(c<25&&(u===0||u>2)){n=true}else{if(j<25&&h>0.2){n=true}else{if(j>=25&&h>0.5){n=true}else{if((f==1&&c<75)||f>1){n=true}}}}}}}if(n){b[r].parentNode.removeChild(b[r])}}}}},cleanHeaders:function(c){for(var a=1;a<7;a++){var d=c.getElementsByTagName("h"+a);for(var b=d.length-1;b>=0;b--){if(readability.getClassWeight(d[b])<0||readability.getLinkDensity(d[b])>0.33){d[b].parentNode.removeChild(d[b])}}}},htmlspecialchars:function(a){if(typeof(a)=="string"){a=a.replace(/&/g,"&amp;");a=a.replace(/"/g,"&quot;");a=a.replace(/'/g,"&#039;");a=a.replace(/</g,"&lt;");a=a.replace(/>/g,"&gt;")}return a},flagIsActive:function(a){return(readability.flags&a)>0},addFlag:function(a){readability.flags=readability.flags|a},removeFlag:function(a){readability.flags=readability.flags&~a}};