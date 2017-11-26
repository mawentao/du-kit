<?php
if(!defined('IN_DISCUZ') || !defined('IN_ADMINCP')) {
    exit('Access Denied');
}
require_once dirname(__FILE__).'/class/env.class.php';

// 插件设置
$params = C::m('#_FILENAME_#_FILENAME__nav_setting')->get();

// 保存设置
if (isset($_POST["reset"])) {
	if ($_POST["reset"]==1) {
		C::m('#_FILENAME_#_FILENAME__nav_setting')->reset();
	} else {
        $menumap = array();
        foreach ($_POST['displayorder'] as $k => $displayorder) {
            $menuid = $_POST['menuid'][$k];
            $parentid = $_POST['parentid'][$k];
            $item = array (
                'displayorder' => $displayorder,
                'text' => $_POST['text'][$k],
                'icon' => $_POST['icon'][$k],
                'href' => $_POST['href'][$k],
                'newtab' => $_POST['newtab'][$k],
                'enable' => $_POST['enable'][$k],
            );  
            if ($parentid==0) {
                $item['subitems'] = array();  //!< 子菜单
                $menumap[$menuid] = $item; 
            } else {
                $menumap[$parentid]['subitems'][] = $item;
            }   
        }   
        // 排序
        _FILENAME__utils::array_sort_by($menumap,'displayorder','ASC');
        foreach ($menumap as $k => &$v) {
            if (!empty($v['subitems'])) {
                _FILENAME__utils::array_sort_by($v['subitems'],'displayorder','ASC');
            }   
        }   
        $params['navmenu'] = array_values($menumap);
        //die(json_encode($params));
        C::m('#_FILENAME_#_FILENAME__nav_setting')->set($params);
	}
    $landurl = 'action=plugins&operation=config&do='.$pluginid.'&identifier=_FILENAME_&pmod=z_nav';
	cpmsg('plugins_edit_succeed', $landurl, 'succeed');
}

$params['ajaxapi'] = _FILENAME__env::get_plugin_path()."/index.php?version=4&module=";
$tplVars = array(
    'siteurl' => _FILENAME__env::get_siteurl(),
    'plugin_path' => _FILENAME__env::get_plugin_path(),
);
_FILENAME__utils::loadtpl(dirname(__FILE__).'/template/views/z_nav.tpl', $params, $tplVars);
_FILENAME__env::getlog()->trace("show admin page [z_nav] success");
