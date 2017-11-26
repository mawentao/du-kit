<?php

/***********************************************************
 * @file:   Validate.php
 * @author: mawentao(mawentao@baidu.com)
 * @create: 2014-02-25 16:23:35
 * @modify: 2014-02-25 16:23:35
 * @brief:  数据验证模块
 ***********************************************************/

class Validate
{
	/**
	 * 攻击请求检查
	 **/
	public static function checkAttackRequest()
	{
		$keys = array_keys($_REQUEST);
        foreach($keys as $key){
            $value = "".$_REQUEST[$key];
			if ($value=="") {continue;}
            if(preg_match("/<.*>/", $value)){
                Env::getlog()->warning("AttackRequest\t".$_SERVER["REQUEST_URI"]);
                return false;
            }
            if(preg_match("/>.*</", $value)){
                Env::getlog()->warning("AttackRequest\t".$_SERVER["REQUEST_URI"]);
                return false;
            }
        }

		return true;
	}

	/**
     * @brief 验证 REQUEST 中的必填字段
     * @param[in] $key : 字段key
     * @param[in] $name : 字段的含义
     * @param[in] $valueType :
	 *			字段值类型: integer, number, string, url, email
	 *          非以上值时，当成正则表达式处理
     * 
     **/
	public static function checkNCParameter($key, $name, $valueType='string', $maxlen=1024)
	{
		//1. 检查字段是否设置
		if (!isset($_REQUEST[$key])) {
			$msg = $key." is not set.";
			throw new Exception($msg);
			return;
		}

		//2. 去首尾空格
		$value = trim($_REQUEST[$key]);
		$_REQUEST[$key] = $value;

		//3. 根据数据类型检查
		$res = true;
		switch($valueType) {
			case "string"  : $res = self::checkString($value, $maxlen); break;
			case "number"  : $res = self::checkNumber($value); break;
			case "integer" : $res = self::checkInteger($value); break;
			case "url"     : $res = self::checkUrl($key, $maxlen); break;
			case "email"   : $res = self::checkEmail($key); break;
			case "rebate"  : $res = self::checkRebate($value, $maxlen); break;
			default:
				if (preg_match($valueType, $value)) {
					$res = true;
				} else {
					$res = "格式不正确";
				}
				break;
		}

		//4. 检查失败抛异常
		if ($res !== true) {
			$msg = $name.$res;
			throw new Exception($msg);
		}
	}


	/**
     * @brief 验证 REQUEST 中的可选字段
     * @param[in] $key : 字段key
     * @param[in] 
     **/
	public static function checkOPParameter($key, $name, $valueType='string', $maxlen=1024, $dafaultValue="")
	{
		if (!isset($_REQUEST[$key]) || $_REQUEST[$key]==="") {
			$_REQUEST[$key] = $dafaultValue;
			return;
		}
		self::checkNCParameter($key, $name, $valueType, $maxlen);
	}


	// 验证字符串
	private static function checkString($str_utf8, $maxlen)
	{/*{{{*/
		if (mb_strlen($str_utf8, "UTF-8") > $maxlen) {
			return "不能超过".$maxlen."个字";
		}
		$illegalCharacters = array('delete', 'null', '||');
		foreach ($illegalCharacters as &$wd) {
			if (stristr($str_utf8, $wd)) {
				return "不能包含字符 $wd";
			}
		}
		return true;
	}/*}}}*/

	// 验证数字
	private static function checkNumber($value)
	{/*{{{*/
		if (!is_numeric($value)) {
			return "必须是数字";
		}
		return true;
	}/*}}}*/

	// 验证整数
	private static function checkInteger($value)
	{/*{{{*/
		$regex = "/^-?\d+$/";
		if (!preg_match($regex, $value)) {
			return "必须是整数";			
		}
		return true;
	}/*}}}*/

	// 验证url
	private static function checkUrl($key, $maxlen)
	{/*{{{*/
		$value = trim($_REQUEST[$key]);
		$res = self::checkString($value, $maxlen);
		if ($res !== true) {
			return $res;
		}
		$regex = "/^(https?:\/\/)?(([0-9a-z_!~*'().&=+$%-]+: )?[0-9a-z_!~*'().&=+$%-]+@)?(([0-9]{1,3}\.){3}[0-9]{1,3}|([0-9a-z_!~*'()-]+\.)*([0-9a-z][0-9a-z-]{0,61})?[0-9a-z]\.[a-z]{2,6})(:[0-9]{1,4})?((\/?)|(\/[^\s]+)+\/?)$/i";
		if (!preg_match($regex, $value)) {
			return "不符合标准URL格式";
		}
		$initial = substr($value, 0, 4);
        if (strcmp($initial, "http") != 0) {
            $_REQUEST[$key] = "http://" . $value;
		}
		return true;
	}/*}}}*/

	// 验证email
	private static function checkEmail($value, $maxlen)
	{/*{{{*/
		$regex = "/^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$/";
		if (!preg_match($regex, $value)) {
			return "必须是Email";
		}
		return true;
	}/*}}}*/

	// 验证rebate(折扣)
	private static function checkRebate($value, $maxlen) 
	{/*{{{*/
		if (mb_strlen($value, "UTF-8") > $maxlen) {
			return "不能超过".$maxlen."个字";
		}
		
		$str = $value;
		$z_utf8   = iconv("GBK", "UTF-8", "折");
        $zq_utf8  = iconv("GBK", "UTF-8", "折起");
        $zfd_utf8 = iconv("GBK", "UTF-8", "折封顶");
        $zqs_utf8 = iconv("GBK", "UTF-8", "折起售");
        $regex = "/(^\d+(\.\d+)?)($zq_utf8|$zfd_utf8)$/i";
        $regex1 = "/(^\d+(\.\d+)?)($zq_utf8|$zqs_utf8)$/i";
        $matches = array();
        if (!preg_match($regex, $str, $matches) && !preg_match($regex1, $str, $matches)) {
            $regex = "/(^\d+(\.\d+)?)-(\d+(\.\d+)?)$z_utf8$/i";
            $matches = array();
            if (!preg_match($regex, $str, $matches)) {
                $regex = "/(^\d+(\.\d+)?)$z_utf8$/i";
                if(!preg_match($regex, $str, $matches)){
                    $regex = "/(^\d+(\.\d+)?)-(\d+(\.\d+)?)$/i";
                    if(!preg_match($regex, $str, $matches)){
                        return "不是合法的折扣格式";
                    }else{
                        $str .= $z_utf8;
                        if(isset($matches[1]) && 0 >= $matches[1] || isset($matches[3]) && 0 >= $matches[3]){
                            $str = '1-5' . $z_utf8;
                            return true;
                        }   
                    }   
                }else{
                    if(isset($matches[1]) && 0 >= $matches[1]){
                        $str = rand(1, 5) . $zq_utf8;
                        return true;
                    }   
                    $str .= iconv('GBK', 'UTF-8//ignore', '起');
                }   
            }else{
                if(isset($matches[1]) && 0 >= $matches[1] || isset($matches[3]) && 0 >= $matches[3]){
                    $str = '1-5' . $z_utf8;
                    return true;
                }
            }
        }else{
            if(isset($matches[1]) && 0 >= $matches[1]){
                $str = rand(1, 5) . $zfd_utf8;
                return true;
            }
        }

        return true;
	}/*}}}*/
}


if (__FILE__ == realpath($_SERVER['SCRIPT_FILENAME']))
{

	$_REQUEST["a1"] = iconv("GBK","UTF-8//ignore","2折起");
	
	if (($res=Validate::checkNCParameter("a1","a1", "rebate"))!==true) {
		echo $_REQUEST["a1"]." : ".$res."\n";
	} else {
		echo $_REQUEST["a1"]." : OK\n";
	}

	echo "!!!THE END!!!\n";
}



// vim600: sw=4 ts=4 fdm=marker syn=php
?>
