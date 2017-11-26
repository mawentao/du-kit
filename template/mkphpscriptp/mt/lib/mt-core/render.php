<?php

/***********************************************************
 * @file:   render.php
 * @author: mawentao(mawentao@baidu.com)
 * @create: 2014-08-12 10:13:59
 * @modify: 2014-08-12 10:13:59
 * @brief:  数据输出渲染模块
 ***********************************************************/

final class Render
{
	public static function renderResult($method="php", $template='')
	{
		switch ($method) {
		case 'resource':
		case 'location':
			header("Location: ".Env::$resData);
			exit(0);
		case 'smarty':
			self::smartyRender($template);
			break;
		case 'email':
			self::emailRender($template);
			break;
		case 'csv':
		case 'xls':
			self::csvRender($method);
			break;
		case 'ajax':
		case 'json':
			self::jsonRender();
			break;
		case 'php':
		default:
			break;
		}
	}

	// smarty模板渲染
	private static function smartyRender($template, $display=true)
	{
		require_once(MT_EXT_PATH."/smarty/Smarty.class.php");
		$s = new Smarty();
		$s->template_dir = VIEW_PATH;
		$s->compile_dir  = VIEW_PATH.'/smarty/templates_c';
		$s->cache_dir    = VIEW_PATH.'/smarty/cache';
		$s->config_dir   = VIEW_PATH.'/smarty/config';
		$s->left_delimiter  = '{%';
		$s->right_delimiter = '%}';
		$s->assign("tplData", Env::$resData);
		if ($display) {
			$s->display($template);
		} else {
			return $s->fetch($template);
		}
	}

	// email模板渲染
	private static function emailRender($template)
	{
		require_once(MT_EXT_PATH."/PHPMailer_v5.1/class.phpmailer.php");

		//1. 检查必填字段
		$data = &Env::$resData;
		$errhead = "Email发送失败：";
		if (!isset($data["from"])) throw new Exception("$errhead 未设置发信人");
		$from = $data["from"];
		if (!isset($data["to"])) throw new Exception("$errhead 未设置收信人");
		$to = $data["to"];
		if (!isset($data["subject"])) throw new Exception("$errhead 未填写邮件标题");
		$subject = toUTF8($data["subject"]);

		//2. 渲染邮件内容
		$content = self::smartyRender($template, false);

		//3. 发送邮件
		$mail = new PHPMailer();
		$mail->SetFrom($from, '');
		$addrs = explode(",", $to);
		foreach ($addrs as $addr) {
			$mail->AddAddress($addr, '');
		}
		if (isset($data["cc"])) {
			$ccs = explode(",", $data["cc"]);
			foreach ($ccs as $cc) {
				$mail->AddCC($cc, '');
			}
		}
		$mail->Subject = $subject;  //!< 设置邮件标题
		$mail->MsgHTML($content);   //!< 设置邮件内容
		$mail->AltBody = "To view the message, please use an HTML compatible email viewer!";
		$mail->Charset='UTF-8';
		$mail->SetLanguage("zh_cn");
		if(!$mail->Send()) {
			throw new Exception("$errhead 网络异常");
		}

		echo "邮件发送成功！<hr>";
		echo "From: $from<br>";
		echo "To: $to<br>";
		if (isset($data["cc"]))
			echo "Cc: ".$data["cc"]."<br>";
		echo "Subject: ".iconv("UTF-8","GBK//ignore",$subject);
	}
	

	// csv,xls文件渲染（下载）
	private static function csvRender($suffix)
	{
		if (!isset(Env::$resData["title"])) {
			throw new Exception("Env::\$resData[\"title\"] is not set");
		}
		$file = Env::$resData["title"].".".$suffix;
		header("Content-type: text/plain");
		header('Content-Disposition: attachment; filename="'.$file.'"');
		$sp = ($suffix=="xls") ? "\t" : ",";
		if (isset(Env::$resData["head"])) 
			echo implode($sp, Env::$resData["head"])."\n";
		if (isset(Env::$resData["root"]))
			foreach (Env::$resData["root"] as $item)
				echo implode($sp, $item)."\n";
		//exit(0);
	}

	// json,ajax格式渲染（ajax）
	private static function jsonRender()
	{
		if (!isset(Env::$resData["retcode"])) {
			Env::$resData["retcode"] = 0;
		}
		if (!isset(Env::$resData["retmsg"])) {
			Env::$resData["retmsg"] = "";
		}
		Env::$resData["retmsg"] = toUTF8(Env::$resData["retmsg"]);
		echo json_encode(Env::$resData);
	}
}



// vim600: sw=4 ts=4 fdm=marker syn=php
?>
