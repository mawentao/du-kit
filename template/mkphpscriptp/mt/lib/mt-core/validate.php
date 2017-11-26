<?php

/***********************************************************
 * @file:   Validate.php
 * @author: mawentao(mawentao@baidu.com)
 * @create: 2014-02-25 16:23:35
 * @modify: 2014-02-25 16:23:35
 * @brief:  ������֤ģ��
 ***********************************************************/

class Validate
{
	/**
	 * ����������
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
     * @brief ��֤ REQUEST �еı����ֶ�
     * @param[in] $key : �ֶ�key
     * @param[in] $name : �ֶεĺ���
     * @param[in] $valueType :
	 *			�ֶ�ֵ����: integer, number, string, url, email
	 *          ������ֵʱ������������ʽ����
     * 
     **/
	public static function checkNCParameter($key, $name, $valueType='string', $maxlen=1024)
	{
		//1. ����ֶ��Ƿ�����
		if (!isset($_REQUEST[$key])) {
			$msg = $key." is not set.";
			throw new Exception($msg);
			return;
		}

		//2. ȥ��β�ո�
		$value = trim($_REQUEST[$key]);
		$_REQUEST[$key] = $value;

		//3. �����������ͼ��
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
					$res = "��ʽ����ȷ";
				}
				break;
		}

		//4. ���ʧ�����쳣
		if ($res !== true) {
			$msg = $name.$res;
			throw new Exception($msg);
		}
	}


	/**
     * @brief ��֤ REQUEST �еĿ�ѡ�ֶ�
     * @param[in] $key : �ֶ�key
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


	// ��֤�ַ���
	private static function checkString($str_utf8, $maxlen)
	{/*{{{*/
		if (mb_strlen($str_utf8, "UTF-8") > $maxlen) {
			return "���ܳ���".$maxlen."����";
		}
		$illegalCharacters = array('delete', 'null', '||');
		foreach ($illegalCharacters as &$wd) {
			if (stristr($str_utf8, $wd)) {
				return "���ܰ����ַ� $wd";
			}
		}
		return true;
	}/*}}}*/

	// ��֤����
	private static function checkNumber($value)
	{/*{{{*/
		if (!is_numeric($value)) {
			return "����������";
		}
		return true;
	}/*}}}*/

	// ��֤����
	private static function checkInteger($value)
	{/*{{{*/
		$regex = "/^-?\d+$/";
		if (!preg_match($regex, $value)) {
			return "����������";			
		}
		return true;
	}/*}}}*/

	// ��֤url
	private static function checkUrl($key, $maxlen)
	{/*{{{*/
		$value = trim($_REQUEST[$key]);
		$res = self::checkString($value, $maxlen);
		if ($res !== true) {
			return $res;
		}
		$regex = "/^(https?:\/\/)?(([0-9a-z_!~*'().&=+$%-]+: )?[0-9a-z_!~*'().&=+$%-]+@)?(([0-9]{1,3}\.){3}[0-9]{1,3}|([0-9a-z_!~*'()-]+\.)*([0-9a-z][0-9a-z-]{0,61})?[0-9a-z]\.[a-z]{2,6})(:[0-9]{1,4})?((\/?)|(\/[^\s]+)+\/?)$/i";
		if (!preg_match($regex, $value)) {
			return "�����ϱ�׼URL��ʽ";
		}
		$initial = substr($value, 0, 4);
        if (strcmp($initial, "http") != 0) {
            $_REQUEST[$key] = "http://" . $value;
		}
		return true;
	}/*}}}*/

	// ��֤email
	private static function checkEmail($value, $maxlen)
	{/*{{{*/
		$regex = "/^\w+([-+.]\w+)*@\w+([-.]\w+)*\.\w+([-.]\w+)*$/";
		if (!preg_match($regex, $value)) {
			return "������Email";
		}
		return true;
	}/*}}}*/

	// ��֤rebate(�ۿ�)
	private static function checkRebate($value, $maxlen) 
	{/*{{{*/
		if (mb_strlen($value, "UTF-8") > $maxlen) {
			return "���ܳ���".$maxlen."����";
		}
		
		$str = $value;
		$z_utf8   = iconv("GBK", "UTF-8", "��");
        $zq_utf8  = iconv("GBK", "UTF-8", "����");
        $zfd_utf8 = iconv("GBK", "UTF-8", "�۷ⶥ");
        $zqs_utf8 = iconv("GBK", "UTF-8", "������");
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
                        return "���ǺϷ����ۿ۸�ʽ";
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
                    $str .= iconv('GBK', 'UTF-8//ignore', '��');
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

	$_REQUEST["a1"] = iconv("GBK","UTF-8//ignore","2����");
	
	if (($res=Validate::checkNCParameter("a1","a1", "rebate"))!==true) {
		echo $_REQUEST["a1"]." : ".$res."\n";
	} else {
		echo $_REQUEST["a1"]." : OK\n";
	}

	echo "!!!THE END!!!\n";
}



// vim600: sw=4 ts=4 fdm=marker syn=php
?>
