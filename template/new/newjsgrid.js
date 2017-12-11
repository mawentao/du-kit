define(function(require){
    /* _FILENAME_.js, (c) _YEAR_ _AUTHOR_ */
    var store,grid,gridid;
    var o={};
    
    o.init = function(domid){
        gridid = domid;
        store = new mwt.Store({
            proxy: new mwt.HttpProxy({
                //beforeLoad : store_before_load,
                //afterLoad  : store_after_load,
                url : ajax.getAjaxUrl("admin&action=authQuery")
            })
        });
        grid = new MWT.Grid({
            render      : gridid,
            store       : store,
            pagebar     : true,     //!< 分页
            pageSize    : 20,       //!< 每页大小
            multiSelect : false,    //!< 多行选择
            bordered    : false,    //!< 单元格边框
            striped     : true,     //!< 斑马纹
            noheader    : false,    //!< 隐藏列头
            notoolbox   : false,    //!< 隐藏工具箱(刷新,斑马纹,导出Excel)
            position    : 'fixed',  //!< 位置(relative:相对位置,其他:固定头部和尾部)
            bodyStyle   : '', 
            tbarStyle   : 'margin-bottom:10px;',
            tbar: [
                {type:'search',id:'so-key-'+gridid,width:300,handler:o.query,placeholder:'查询用户名'},
                '->',
                {label:'添加用户 <i class="sicon-arrow-right"></i>',handler:function(){
                    alert('TODO');
                }}  
            ],
            cm: new MWT.Grid.ColumnModel([
                {head:"用户ID",   dataIndex:"uid",      width:100, sort:true},
                {head:"用户名",   dataIndex:"username", width:120, sort:true},
                {head:"用户邮箱", dataIndex:"email",    width:200, sort:true},
                {head:"权限", dataIndex:"auth",width:140,sort:true,render:function(v,item){
                    return dict.get_auth(v);
                }},
                {head:'',dataIndex:"uid",align:'right',render:function(v,item){
                    var editbtn = '<a name="editbtn-'+gridid+'" data-id="'+v+'" href="javascript:;">编辑</a>';
                    var delbtn = '<a name="delbtn-'+gridid+'" data-id="'+v+'" href="javascript:;">删除</a>';
                    var btns = [setbtn,delbtn];
                    return btns.join("&nbsp;&nbsp;");
                }}
            ])
        });
        store.on('load',function(){
            mwt.popinit();
            // 编辑按钮
            jQuery('[name=editbtn-'+gridid+']').unbind('click').click(editbtnClick);
            // 删除按钮
            jQuery('[name=delbtn-'+gridid+']').unbind('click').click(delbtnClick);
        });
        grid.create();
    };

    o.query = function() {
        store.baseParams = { 
            key: mwt.get_value("so-key-"+gridid)
        };  
        grid.load();
    };

    //////////////////////////////////////////////////////

    // 编辑按钮点击事件
    function editbtnClick() {
        var id = jQuery(this).data('id');
        alert("TODO: edit "+id);
    }

    // 删除按钮点击事件
    function delbtnClick() {
        var id = jQuery(this).data('id');
        alert("TODO: delete "+id);
    }

    //////////////////////////////////////////////////////

    return o;
});
