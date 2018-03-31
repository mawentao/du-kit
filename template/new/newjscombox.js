define(function(require){
    /* _FILENAME_.js, (c) _YEAR_ _AUTHOR_ */
    var store,grid,gridid,fd;

    function query() {
        store.baseParams = { 
            //cityid: mwt.get_select_value('city-sel-'+gridid),
            key: mwt.get_value('sokey-'+gridid)
        };  
        grid.load();
    }

    // 初始化Grid
    function init_grid(domid) 
    {/*{{{*/
        gridid = domid;
        store = new mwt.Store({
            proxy: new mwt.HttpProxy({
                url: ajax.getAjaxUrl("cpcompany&action=queryList")
            })
        }); 
        grid = new MWT.Grid({
            render      : gridid,
            store       : store,
            pagebar     : true,     //!< 分页
            pageSize    : 20,       //!< 每页大小
            pagebarSimple: true,    //!< 简单分页样式
            multiSelect : false,    //!< 多行选择
            bordered    : false,    //!< 单元格边框
            striped     : true,     //!< 斑马纹
            noheader    : false,    //!< 隐藏列头
            notoolbox   : true,     //!< 隐藏工具箱(刷新,斑马纹,导出Excel)
            position    : 'fixed',  //!< 位置(relative:相对位置,其他:固定头部和尾部)
            bodyStyle   : '',
            tbarStyle   : 'margin-bottom:-1px;',
            tbar: [
                {type:'search',id:'sokey-'+gridid,width:120,handler:o.query,placeholder:'查询关键词'}
            ],
            cm: new MWT.Grid.ColumnModel([
                {head:"请选择",dataIndex:"name",sort:true}
            ]),
            rowclick: function(im) {
                fd.setText(im.name);
                fd.setValue(im);
            }
        });
        grid.create();
    }/*}}}*/

    var o={};

    o.create=function(domid) {
        fd = new MWT.ComboxField({
            render   : domid,
            //cls      : 'radius',
            //style    : 'width:100px;', 
            popWidth : 500,
            popHeight: 300,
            value    : '',
            empty    : false,
            errmsg   : '请选择...'
        });
        grid = null;
        fd.on('pop',function(){
            if (!grid) init_grid(fd.getPopDivId());
            if (store.size()==0) {
                query();
            }
        });
        fd.create();
        return fd;
    };

    return o;
});
