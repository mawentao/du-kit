<?php
/*
$filename = '示例文件';
$headers = array('学号','姓名','性别','年龄','班级');
$list = array (
	array('1','小王','男','20','100'),
	array('2','小李','男','20','101'),
	array('3','小张','女','20','102'),
	array('4','小赵','女','20','103')
);
export_excel($filename, $headers, $list);
*/

/**
 * 数据导出到Excel（直接网页输出,不生成文件）
 *     $filename: 文件名
 *     $headers: 表头 如: array('学号','姓名','性别','年龄','班级');
 *     $list: 表格内容, 如:
 *     array (
 *		   array('1','小王','男','20','100'),
 *		   array('2','小李','男','20','101'),
 *		   array('3','小张','女','20','102'),
 *		   array('4','小赵','女','20','103')
 *     );
 **/
function export_excel($filename,$headers,$list)
{
	require_once dirname(__FILE__)."/phpexcel-1.8/PHPExcel.php";
	$cols = array('A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z');
	//1. 编写excel
	$excel = new PHPExcel();
	$sheet = $excel->getActiveSheet();
	//1-1. 表头
	$i=0;
	foreach ($headers as $v) {
		$sheet->setCellValue($cols[$i].'1', $v);
		++$i;
	}
	//1-2. 内容
	$ri = 2;
	foreach ($list as &$row) {
		$ci = 0;
		foreach ($row as $v) {
			$sheet->setCellValue($cols[$ci].$ri, $v);
			++$ci;
		}
		++$ri;
	}
	//2. 输出
	$write = new PHPExcel_Writer_Excel5($excel);
	$write->save($filename);
}
