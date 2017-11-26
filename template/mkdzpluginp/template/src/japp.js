/* japp.js, (c) 2016 mawentao */
var conf = {
    // 日志级别 0:关闭;>=1:WARN;>=2:INFO;>=3DEBUG;
    loglevel: 3
};
/* JApp定义 */
var JApp=function(baseUrl)
{
    this.init = function() {
		require.config({
			baseUrl: baseUrl,
    		packages: [
				{name:'frame', location:'frame', main:'main'}
    		]
		});
        require(['jappengine'], function(jappengine,corelog,coreajax,mwt){
			jappengine.start();
        });
    };
};
