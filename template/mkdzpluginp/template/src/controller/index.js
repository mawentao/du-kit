/* 默认controller, (c) 2016 mawentao */
define(function(require){
	var frame=require('frame');
    var o={};
	var control='index';

	o.conf = {
		controller: control,
		path: [
			'/'+control+'/index'
		]
	};

	// 默认action
	o.indexAction=function() {
		require('./login').check();
		frame.showpage('welcome');
	};

	return o;
});
