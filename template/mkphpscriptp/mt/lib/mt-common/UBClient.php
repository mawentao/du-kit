<?php

/***********************************************************
 * @file:    ubclient.php
 * @version: 1.0.0
 * @author:  mawentao(mawentao@baidu.com)
 * @brief:   nshead + mcpack 访问ubserver的封装类
 ***********************************************************/

require_once 'Packer.php';

define('NS_HEADER_LEN', 36);


/** 访问ubserver的封装类 */
class UBClient
{
	private $target_ips = array();
    private $target_port;
	private $timeout = 0.1;

	public function __construct($ips, $port, $timeout=0.1) {
        $this->target_ips  = $ips;
        $this->target_port = $port;
		$this->timeout = $timeout;
    }

    private function set_nshead_item(&$head, $k) {
        //if(array_key_exists(k, $head)){
            $this->_head[k] = $head[k];
        //}
    }
   
	public function send_and_recv($head, $arr) {
        //1. pack head
        $packer = new Packer();
        $packer->packInt16($head["ID"]);
        $packer->packInt16($head["VERSION"]);
        $packer->packInt32($head["LOG_ID"]);
        $packer->packNChar($head["PROVIDER"], 16);
        $packer->packInt32($head["MAGIC_NUM"]);
        $packer->packInt32(0);

        //2. pack body
        $pack = mc_pack_array2pack($arr);
        $packer->packInt32(strlen($pack));
        $packer->packNChar($pack, strlen($pack));

        //3. send
        $errno = 0;
        $errstr = '';
        $wait = $this->timeout;
        //3-1. Open Connection
		$hostArray = $this->target_ips;
		shuffle($hostArray);
		$connected = false;
		while(!empty($hostArray)){
			try{
				$ip    = array_pop($hostArray);
				$port  = $this->target_port;
				$sockp = fsockopen($ip, $port, $errno, $errstr, $wait);
				if ($sockp == NULL){
					Env::getlog()->warning('could not connect to UB server retrying ..' . "$ip:$port");
					continue;
				}else{
					$connected = true;
					break;
				}
			}catch(Exception $e){
				Env::getLogger()->warning('could not connect to AS server retrying ..' . "$ip:$port");
				continue;
			}
		}
		if(!$connected){
			Env::getlog()->warning("could not connect to UB server.");
			return -1;
		}
        //3-2. Write Nshead & Body
        $strData = $packer->getData();
        $ret = fwrite($sockp, $strData, strlen($strData));
		if ($ret == -1){    
			return -1;
		}
        //3-3. Read Nshead
        $res = fread($sockp, NS_HEADER_LEN);
		if (0 == strlen($res)){       
			return -1;
		} 

		//3-4. Parse Nshead
		$packer->clearData();
		$packer->setData($res);
        /*
        $objPacker->setPos(0);
		$id = $objPacker->unpackInt16();
		echo $id, "\n";
		$version = $objPacker->unpackInt16();
		echo $version, "\n";
		echo $objPacker->unpackInt32(), "\n";
		echo $objPacker->unpackNChar(16), "\n";
		echo $objPacker->unpackInt32(), "\n";
        */
		$packer->setPos(32);
		$intLen = $packer->unpackInt32();
		if($intLen <= 0){
			$res_arraydata = 0;
			return 0;
		}
		$tempread = fread($sockp, $intLen);
		$intReaded = strlen($tempread);
		while($intReaded < $intLen) {
			$temp = fread($sockp, $intLen - $intReaded);
			$intReaded += strlen($temp);
			$tempread .= $temp;
		}

		$resarr = mc_pack_pack2array($tempread);
		//var_dump($resarr);
		fclose($sockp); 
        return $resarr; //*/
    }

}

?>
