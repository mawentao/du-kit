define("er/template",function(require){function Stack(){this.container=[],this.index=-1}function ArrayBuffer(){this.raw=[],this.idx=0}function NodeIterator(e){this.stream=e,this.index=0}function Scope(e){this._store={},this.parent=e}function nodeAnalyse(e){function t(){g.push({type:TYPE.TEXT,text:y.join("")}),y=new ArrayBuffer}function r(e,t){throw e+" is invalid: "+t}function n(e){y.pushMore(d,e,v)}var o,i,a,s,u,c,l,p,f,h,d="<!--",v="-->",y=new ArrayBuffer,g=new ArrayBuffer,E=e.split(d);for(""===E[0]&&E.shift(),o=0,i=E.length;i>o;o++)if(a=E[o].split(v),s=a.length,2===s||o>0)if(2===s){if(u=a[0],COMMENT_RULE.test(u))if(t(),c=RegExp.$2.toLowerCase(),l=RegExp.$3,p={type:TYPE[c.toUpperCase()]},RegExp.$1)p.endTag=1,g.push(p);else{switch(c){case"content":case"contentplaceholder":case"master":case"import":case"target":if(TAG_RULE.test(l))for(p.id=RegExp.$1,f=RegExp.$2.split(/\s*,\s*/),h=f.length;h--;){var m=f[h];PROP_RULE.test(m)&&(p[RegExp.$1]=RegExp.$2)}else r(c,u);break;case"for":FOR_RULE.test(l)?(p.list=RegExp.$1,p.item=RegExp.$2,p.index=RegExp.$4):r(c,u);break;case"if":case"elif":IF_RULE.test(RegExp.$3)?p.expr=condExpr.parse(RegExp.$1):r(c,u);break;case"else":break;default:p=null,n(u)}p&&g.push(p)}else n(u);y.push(a[1])}else y.push(a[0]);return t(),g.getRaw()}function getVariableValue(e,t,r){t=t.split(/[\.\[]/);var n=e.get(t[0]);t.shift();for(var o=0,i=t.length;i>o&&null!=n;o++){var a=t[o].replace(/\]$/,""),s=a.length;/^(['"])/.test(a)&&a.lastIndexOf(RegExp.$1)===--s&&(a=a.slice(1,s)),n=n[a]}var u="";null!=n&&(u=n);var c=filterContainer[r||"html"];return c&&(u=c(u)),u}function getContent(e){var t,r,n,o=e.block,i=[];for(r=0,n=o.length;n>r;r++)t=o[r],t.block?i.push(getContent(t)):t.type===TYPE.IMPORT?i.push(getTargetContent(t.id)):i.push(t.text||"");return i.join("")}function getTargetContent(e){try{var t=getTarget(e);return getContent(t)||""}catch(r){return""}}function getTarget(e){var t=targetContainer[e];if(!t)throw'target "'+e+'" is not exist!';if(t.type!==TYPE.TARGET)throw'target "'+e+'" is invalid!';return t}function replaceVariable(e,t){return e.replace(/\$\{([.a-z0-9\[\]'"_]+)\s*(\|([a-z]+))?\s*\}/gi,function(e,r,n,o){return getVariableValue(t,r,o)})}function execImport(e,t){var r=getTarget(e.id);return exec(r,t)}function exec(e,t){var r,n,o,i,a,s,u,c,l,p,f=[],h=e.block;for(n=0,o=h.length;o>n;n++)switch(r=h[n],r.type){case TYPE.TEXT:f.push(replaceVariable(r.text,t));break;case TYPE.IMPORT:f.push(execImport(r,t));break;case TYPE.FOR:for(i=new Scope(t),a=t.get(r.list),c=r.item,l=r.index,s=0,u=a.length;u>s;s++)i.set(c,a[s]),l&&i.set(l,s),f.push(exec(r,i));break;case TYPE.IF:for(p=condExpr.exec(r.expr,t);!p&&(r=r["else"]);)p=!r.expr||condExpr.exec(r.expr,t);r&&f.push(exec(r,t))}return f.join("")}function parse(e){var t=nodeAnalyse(e);syntaxAnalyse(t)}function merge(e,t,r){var n,o="";try{n=getTarget(t),o=exec(n,new Scope(r))}catch(i){}return e&&(e.innerHTML=o),o}var util=require("./util");Stack.prototype={current:function(){return this.container[this.index]},push:function(e){this.container[++this.index]=e},pop:function(){if(this.index<0)return null;var e=this.container[this.index];return this.container.length=this.index--,e},bottom:function(){return this.container[0]}},ArrayBuffer.prototype={push:function(e){this.raw[this.idx++]=e},pushMore:function(){for(var e=0,t=arguments.length;t>e;e++)this.push(arguments[e])},join:function(e){return this.raw.join(e)},getRaw:function(){return this.raw}},NodeIterator.prototype={next:function(){return this.stream[++this.index]},prev:function(){return this.stream[--this.index]},current:function(){return this.stream[this.index]}},Scope.prototype={get:function(e){var t=this._store[e];return null==t&&this.parent?this.parent.get(e):null!=t?t:null},set:function(e,t){this._store[e]=t}};var TYPE={TEXT:1,TARGET:2,MASTER:3,IMPORT:4,CONTENT:5,CONTENTPLACEHOLDER:6,FOR:7,IF:8,ELIF:9,ELSE:10},COMMENT_RULE=/^\s*(\/)?([a-z]+)(.*)$/i,PROP_RULE=/^\s*([0-9a-z_]+)\s*=\s*([0-9a-z_]+)\s*$/i,FOR_RULE=/^\s*:\s*\$\{([0-9a-z_.\[\]]+)\}\s+as\s+\$\{([0-9a-z_]+)\}\s*(,\s*\$\{([0-9a-z_]+)\})?\s*$/i,IF_RULE=/^\s*:([>=<!0-9a-z$\{\}\[\]\(\):\s'"\.\|&_]+)\s*$/i,TAG_RULE=/^\s*:\s*([a-z0-9_]+)\s*(?:\(([^)]+)\))?\s*$/i,CONDEXPR_RULE=/\s*(\!=?=?|\|\||&&|>=?|<=?|===?|['"\(\)]|\$\{[^\}]+\}|\-?[0-9]+(\.[0-9]+)?)/,masterContainer={},targetContainer={},filterContainer={html:util.encodeHTML,url:encodeURIComponent},syntaxAnalyse=function(){function e(e){for(var t;(t=a.current())&&t.type!==e;)a.pop();return a.pop()}function t(e){a.push(e)}function r(){var e=a.bottom;return"[er template]"+e.type+" "+e.id+": unexpect "+s.current().type+" on "+a.current().type}function n(e){var t=u[e];t&&t()}var o,i,a,s,u={};return u[TYPE.TARGET]=function(){var i=s.current();for(i.block=[],i.content={},t(i),o[i.id]=i;i=s.next();)switch(i.type){case TYPE.TARGET:case TYPE.MASTER:return e(),i.endTag||s.prev(),void 0;case TYPE.CONTENTPLACEHOLDER:case TYPE.ELSE:case TYPE.ELIF:throw r();default:n(i.type)}},u[TYPE.MASTER]=function(){var o=s.current();for(o.block=[],t(o),i[o.id]=o;o=s.next();)switch(o.type){case TYPE.TARGET:case TYPE.MASTER:return e(),o.endTag||s.prev(),void 0;case TYPE.CONTENT:case TYPE.ELSE:case TYPE.ELIF:throw r();default:n(o.type)}},u[TYPE.TEXT]=u[TYPE.IMPORT]=u[TYPE.CONTENTPLACEHOLDER]=function(){a.current().block.push(s.current())},u[TYPE.CONTENT]=function(){var o=s.current();for(o.block=[],a.bottom().content[o.id]=o,t(o);o=s.next();){if(o.endTag)return o.type===TYPE.CONTENT?e(TYPE.CONTENT):s.prev(),void 0;switch(o.type){case TYPE.TARGET:case TYPE.MASTER:return e(),s.prev(),void 0;case TYPE.CONTENTPLACEHOLDER:case TYPE.ELSE:case TYPE.ELIF:throw r();case TYPE.CONTENT:return e(TYPE.CONTENT),s.prev(),void 0;default:n(o.type)}}},u[TYPE.FOR]=function(){var o=s.current();for(o.block=[],a.current().block.push(o),t(o);o=s.next();){if(o.endTag)return o.type===TYPE.FOR?e(TYPE.FOR):s.prev(),void 0;switch(o.type){case TYPE.TARGET:case TYPE.MASTER:return e(),s.prev(),void 0;case TYPE.CONTENTPLACEHOLDER:case TYPE.CONTENT:case TYPE.ELSE:case TYPE.ELIF:throw r();default:n(o.type)}}},u[TYPE.IF]=function(){var o=s.current();for(o.block=[],a.current().block.push(o),t(o);o=s.next();){if(o.endTag)return o.type===TYPE.IF?e(TYPE.IF):s.prev(),void 0;switch(o.type){case TYPE.TARGET:case TYPE.MASTER:return e(),s.prev(),void 0;case TYPE.CONTENTPLACEHOLDER:case TYPE.CONTENT:throw r();default:n(o.type)}}},u[TYPE.ELIF]=function(){var o=s.current();for(o.type=TYPE.IF,o.block=[],e(TYPE.IF)["else"]=o,t(o);o=s.next();){if(o.endTag)return s.prev(),void 0;switch(o.type){case TYPE.TARGET:case TYPE.MASTER:return e(),s.prev(),void 0;case TYPE.CONTENTPLACEHOLDER:case TYPE.CONTENT:throw r();case TYPE.ELIF:return s.prev(),void 0;default:n(o.type)}}},u[TYPE.ELSE]=function(){for(var o,i=s.current(),u=a.current();;){if(o=u.type,o===TYPE.IF||o===TYPE.ELIF){u={type:TYPE.ELSE,block:[]},a.current()["else"]=u;break}u=a.pop()}for(t(u);i=s.next();){if(i.endTag)return s.prev(),void 0;switch(i.type){case TYPE.TARGET:case TYPE.MASTER:return e(),s.prev(),void 0;case TYPE.CONTENTPLACEHOLDER:case TYPE.CONTENT:case TYPE.ELSE:case TYPE.ELIF:throw r();default:n(i.type)}}},function(e){var t,r,n,c,l,p,f,h,d,v;for(s=new NodeIterator(e),o={},i={},a=new Stack;t=s.current();)switch(t.type){case TYPE.TARGET:case TYPE.MASTER:u[t.type]();break;default:s.next()}for(r in i){if(masterContainer[r])throw'master "'+r+'" is exist!';masterContainer[r]=i[r]}for(r in o){if(targetContainer[r])throw'target "'+r+'" is exist!';if(n=o[r],c=n.master,targetContainer[r]=n,c){if(p=[],n.block=p,c=masterContainer[c],!c)continue;for(f=c.block,h=0,d=f.length;d>h;h++)v=f[h],v.type===TYPE.CONTENTPLACEHOLDER?(l=n.content[v.id],l&&p.push.apply(p,l.block)):p.push(v)}}o=null,i=null,s=null,a=null}}(),condExpr=function(){function andExpr(e){var t,r=relationExpr(e);return(t=e.current())&&"&&"===t.text&&(e.next(),r={type:EXPR_T.and,expr1:r,expr2:andExpr(e)}),r}function orExpr(e){var t,r=andExpr(e);return(t=e.current())&&"||"===t.text&&(e.next(),r={type:EXPR_T.or,expr1:r,expr2:orExpr(e)}),r}function primaryExpr(e){var t=e.current();return"("===t.text&&(e.next(),t=orExpr(e)),e.next(),t}function unaryExpr(e){return"!"===e.current().text?(e.next(),{type:EXPR_T.unary,oper:"!",expr:primaryExpr(e)}):primaryExpr(e)}function relationExpr(e){var t,r=unaryExpr(e);return(t=e.current())&&/^[><=]+$/.test(t.text)&&(e.next(),r={type:EXPR_T.relation,expr1:r,expr2:unaryExpr(e),oper:t.text}),r}function execRelationExpr(e,t){var r=execCondExpr(e.expr1,t),n=execCondExpr(e.expr2,t);switch(e.oper){case"==":return r==n;case"===":return r===n;case">":return r>n;case"<":return n>r;case">=":return r>=n;case"<=":return n>=r;case"!=":return r!=n;case"!==":return r!==n}}function execCondExpr(expr,scope){switch(expr.type){case EXPR_T.or:return execCondExpr(expr.expr1,scope)||execCondExpr(expr.expr2,scope);case EXPR_T.and:return execCondExpr(expr.expr1,scope)&&execCondExpr(expr.expr2,scope);case EXPR_T.unary:return!execCondExpr(expr.expr,scope);case EXPR_T.relation:return execRelationExpr(expr,scope);case EXPR_T.string:case EXPR_T.number:return eval(expr.text);case EXPR_T.variable:return getVariableValue(scope,expr.text,"raw")}}var EXPR_T={or:1,and:2,relation:3,unary:4,string:5,number:6,variable:7,punc:8};return{parse:function(e){e=util.trim(e);for(var t,r,n,o=e,i=[];e;){if(t=CONDEXPR_RULE.exec(e),!t)throw"conditional expression invalid: "+o;if(e=e.slice(t[0].length),r=t[1],0===r.indexOf("$"))i.push({type:EXPR_T.variable,text:r.slice(2,r.length-1)});else if(/^[-0-9]/.test(r))i.push({type:EXPR_T.number,text:r});else switch(r){case"'":case'"':for(var a,s=[r],u=0;;){if(a=e.charAt(u++),a===r){s.push(r),e=e.slice(u);break}s.push(a)}i.push({type:EXPR_T.string,text:s.join("")});break;default:i.push({type:EXPR_T.punc,text:r})}}return n=orExpr(new NodeIterator(i))},exec:execCondExpr}}(),template={addFilter:function(e,t){filterContainer[e]=t},get:getTargetContent,parse:parse,merge:merge,render:function(e,t){return merge(null,e,t)}};return template});