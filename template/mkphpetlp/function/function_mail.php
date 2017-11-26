<?php
require_once dirname(__FILE__).'/../lib/PHPMailerAutoload.php';

/*
$tomails = array(
    'js_mawentao@163.com',
);
sendMail('js_mawentao@163.com','hello',$tomails,'test','休息休息');
*/


/***********************************************************
    $from : 发送邮箱
    $from : 发送邮箱显示在邮件的名称
    $toList : 接收邮箱列表，以数组格式传进来
    $title : 标题
    $data : 邮件内容，字符串
    $attachList : 附件列表，不支持中文文件名
    注：所有中文都需要进行utf-8编码，不然是乱码
***********************************************************/
function sendMail($from, $fromName, $toList, $title, $data, $attachList = array()){
	error_reporting(E_ALL);

	$mail = new PHPMailer;
	$mail->CharSet ="UTF-8";
	$mail->Encoding = 'base64';

	
	$mail->setFrom($from, $fromName);
	foreach($toList as $to){
		$mail->addAddress($to);
	}

	//发消息
	$mail->Subject = $title;
	$mail->msgHTML($data, dirname(__FILE__));
	//$mail->AltBody = 'This is a plain-text message body';
	foreach($attachList as $attach){
		$mail->addAttachment($attach);
	}

	//send the message, check for errors	
	if (!$mail->send()) {
    echo "Mailer Error: " . $mail->ErrorInfo;
	} else {
    echo "Message sent!";
	}
}
