define(function(require){
    /* _FILENAME_.js, (c) _YEAR_ _AUTHOR_ */
    var form,dialog,params,callback;

    function init_dialog() 
    {/*{{{*/
        //1. new form
        form = new MWT.Form();
        form.addField('name',new MWT.TextField({
            render      : 'fm-name',
            value       : '用户名',
            placeholder : '',
            empty       : false,
            errmsg      : "请输入用户名,不超过10个字符",
            checkfun    : function(v){return v.length<=10;}
        }));
        //2. new dialog
        dialog = new MWT.Dialog({
            title     : '对话框',
            top       : 50,
            width     : 550,
            form      : form,
            bodyStyle : 'padding:10px;',
            body : '<table class="mwt-formtab">'+
                '<tr><td width="60">名称</td><td><div id="fm-name"></div></td></tr>'+
            '</table>',
            buttons : [
                {label:"确定",cls:'mwt-btn-primary',handler:submitClick},
                {label:"取消",type:'close',cls:'mwt-btn-default'}
            ]
        });
        //3. dialog open event
        dialog.on('open',function(){
            form.reset();
            if (item.item_id) {
                dialog.setTitle("编辑记录");
                form.set(params);
            } else {
                dialog.setTitle("添加记录");
            }
        });
    }/*}}}*/

    var o={};
    o.open=function(_params,_callback){
        params   = _params;
        callback = _callback;
        if (!dialog) init_dialog();
        dialog.open();
    };

    /////////////////////////////////////
    // 提交按钮点击事件
    function submitClick() {
        var data = form.getData();
        try {
            //data.id = params.id;
            ajax.post('record&action=save',data,function(res){
                if (res.retcode!=0) mwt.notify(res.retmsg,1500,'danger');
                else {
                    dialog.close();
                    if (callback) callback();
                }
            });
        } catch (e) {
            mwt.notify(e,1500,'danger');
        }
    }

    return o;
});
