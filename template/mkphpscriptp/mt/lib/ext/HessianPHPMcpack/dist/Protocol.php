<?php
/*
-------------------------------------------------------------
HessianPHP - Binary Web Services for PHP

Copyright (C) 2004-2005  by Manolo G髆ez
http://www.hessianphp.org

Hessian Binary Web Service Protocol by Caucho(www.caucho.com) 

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

You can find the GNU General Public License here
http://www.gnu.org/licenses/lgpl.html
or in the license.txt file in your source directory.

If you have any questions or comments, please email:
vegeta.ec@gmail.com

支持mcpack2.0
*/

include_once 'Hessian.php';

/**
 * Base class for Hessian protocol handling objects. Contains methods to handle streams, references, mapping of classes 
 * and datetimes
 * 
 * @package HessianPHP.Protocol
 * @author Manolo G髆ez
 * @copyright Copyright (c) 2005
 * @version 1.0
 * @access public
 **/
class HessianProtocolHandler{
	var $stream;
	var $pos;
	var $len;
	var $refs = array();
	var $error;
	var $dateProvider;
	var $typemap;

	function HessianProtocolHandler($stream=null){
		if($stream)
			$this->setStream($stream);
		// Recover default configuration data
		$config = &HessianConfig::globalConfig();
		$this->setTypeMap($config->typeMap);
		$this->setDateProvider($config->dateProvider);
	}
	
	/**
	 * Sets the stream of bytes to parse
	 *  
	 * @param string stream Incoming stream
	 **/
	function setStream($stream){
		$this->stream = $stream;
		//$this->len = strlen($stream);
		//$this->pos =0;
	}

	function setDateProvider(&$provider){
		$this->dateProvider = $provider;
	}

	function setTypeMap(&$map){
		$this->typeMap = &$map;
	}

}

/**
 * <BLOCKQUOTE>
 * Hessian protocol parser, inspired and partially based on hessianlib.py by Caucho.
 * <BR>
 * TODO:
 * <UL>
 *	<LI>Handling of 'headers'</LI>
 * </UL>
 * </BLOCKQUOTE>
 * @package HessianPHP.Protocol
 * @author Manolo G髆ez
 * @copyright Copyright (c) 2004
 * @version 1.0
 * @access public
 **/
class HessianParser extends HessianProtocolHandler{
	var $lastCode = false;
	//var $refs = array();

	// Series of parsing method for the different elements in the Hessian spec

	function parseCall(){
        /*
		if($this->read(1) != 'c') {
			return new HessianError('Hessian Parser, Malformed call: Expected: c',HESSIAN_PARSER_ERROR,0,$this->stream);
		}
		$minor = $this->read(1);
		$major = $this->read(1);

		if($this->read(1) != 'm') {
			return new HessianError('Hessian Parser, Malformed call: Expected m',HESSIAN_PARSER_ERROR,0,$this->stream); 
		}
		return $this->parseObject('S');
        */
        //解析请求
        $arr = mc_pack_pack2array($this->stream);
        if ($arr) {
           if (!isset($arr[JSONRPC_HEAD]) || !isset($arr['method']) 
                       || !array_key_exists('params', $arr) || !isset($arr['id'])) {
            return new HessianError('Invalid JSON-RPC call',
                JSONRPC_INVALID_ERROR, 0, $this->stream);
           }
           if ($arr[JSONRPC_HEAD] != JSONRPC_VERSION) {
                return new HessianError('Invalid JSON-RPC call', 
                        JSONRPC_INVALID_ERROR, 0, $this->stream);
           }
        } else {
            return new HessianError('Parse error.',JSONRPC_PARSER_ERROR,0,$this->stream); 
        }
        return $arr;
	}

	function parseReply(){
        /*
		if($this->read(1) != 'r') {
			return new HessianError('Hessian Parser, Malformed reply: expected r',HESSIAN_PARSER_ERROR,0,$this->stream);
		}
		$minor = $this->read(1);
		$major = $this->read(1);
		$value = $this->parseObject($this->read(1));
		if($this->read(1) == 'z')
			return $value;
        */
        //解析返回结果,同时检查数据格式
        $arr = mc_pack_pack2array($this->stream);
        if ($arr) {
            if ($arr[JSONRPC_HEAD] != JSONRPC_VERSION || !array_key_exists('id', $arr)) {
                return new HessianError('Internal JSON-RPC error.', JSONRPC_INTERNAL_ERROR, 0, $this->stream);
            }
            if (array_key_exists('result', $arr) &&
                !array_key_exists('error', $arr)) {
                return $arr['result'];
            }
            if (array_key_exists('error', $arr) 
                && array_key_exists('code', $arr['error'])
                && array_key_exists('message', $arr['error'])) {
               return new HessianError($arr['error']['message'],
                    $arr['error']['code'],
                    $arr['error']['data'],
                    $this->stream);
            }
            return new HessianError('Invalid JSON-RPC call', JSONRPC_INVALID_ERROR, 0, $this->stream); 
        }
        return new HessianError('Hessian Parser, Malformed reply: expected r',HESSIAN_PARSER_ERROR,0,$this->stream);
	}

	function &parseFault(){
        //Hessian 用专门的格式来存放错误结果,在json rpc中都在parseReply中判断
        /*
		$code = $this->read(1);
		$fault = array();
		// OJO: que quise hacer aqui?
		$map = array();
		$this->refs[] = &$map;
		while($code != 'z'){
			$key = &$this->parseObject($code);
			$value = &$this->parseObject();
			$map[$key] = $value;
			$code = $this->read(1);
		}*/
        $map = mc_pack_pack2array($this->stream);
		$faultMessage = 'Service fault';
		if(isset($map['code']) && isset($map['message'])) {
			$faultMessage .= ': '.$map['message'];
			unset($map['message']);
		} 
		return new HessianError("Hessian Fault: $faultMessage",HESSIAN_FAULT,$map,$this->stream);

	}
}

/**
 * <BLOCKQUOTE>
 * Hessian protocol writer, inspired and partially based on hessianlib.py by Caucho.
 * <BR>
 * TODO:
 * <UL>
 *	<LI>Handling of _hessian_write function</LI>
 * </UL>
 * </BLOCKQUOTE>
 * @package HessianPHP.Protocol
 * @author Manolo G髆ez
 * @copyright Copyright (c) 2004
 * @version 1.0
 * @access public
 **/
class HessianWriter extends HessianProtocolHandler{
	//var $stream;
	var $fault = false;
	//var $refs = array();

	/**
	 * Sets the reply as a fault, following Hessian spec
	 *  
	 * @param string code Code number of the fault
	 * @param string message Descriptive message of the fault
	 * @param mixed detail Optional argument with detail of the fault, usually a stack trace
	 * @access public
	 **/
	function setFault($code,$message,$id, $detail=null){
		$this->fault = array('code' => $code, 'message' => $message, 'data' => $detail, "$id");
	}

    function object2array(&$object) {
        if (is_object($object)) {
            $arr = (array)($object);
        } else {
            $arr = &$object;
        }       
        if (is_array($arr)) {
            foreach($arr as $varName => $varValue){
                $arr[$varName] = $this->object2array($varValue);
            }       
        }       
        return $arr;
    }
	/**
	 * Writes a Hessian reply with a return object. If a fault has been set, it writes the fault instead
	 *  
	 * @param mixed object Object to be returned in the reply
	 * @return string Hessian reply
	 **/
	function writeReply($object, $version = PHP_MC_PACK_V2){
        
		$stream = &$this->stream;
		//$stream = "r\x01\x00";
        
        //写返回结果
		if(!$this->fault) {
            //运行结果正常
			//$this->writeObject($object);
            $stream = mc_pack_array2pack($this->object2array($object), $version); 
		} else {
            //运行错误
			$this->writeFault($this->fault['code'],
				$this->fault['message'],
				$this->fault['data'],
                "{$this->fault['id']}",
                $version
            );
		}
		//$stream .= "z";
		return $stream;
	}

	/**
	 * Writes a Hessian method call and serializes arguments.
	 *  
	 * @param string method Method to be called
	 * @param array params Arguments of the method
	 * @return string Hessian call
	 **/
	function writeCall($method,&$params, $id, $version = PHP_MC_PACK_V2){
        /*
		$stream = &$this->stream;
		$stream = "c\x01\x00m";
		$this->writeStringData($method);
		foreach($params as $param){
			$this->writeObject($param);
		}
		$stream .= "z";
        */
        #print_r($params);
        #print_r($this->object2array($params));
        //组装调用数据
        $stream = mc_pack_array2pack(array(
            JSONRPC_HEAD => JSONRPC_VERSION,
            'method'=>$method, 
            'params'=>$this->object2array($params), //object 转化成array
            'id' => "$id"), $version);
        #print($stream);
        
		return $stream;
	}

    //出错
	function writeFault($code,$message,$detail,$id, $version = PHP_MC_PACK_V2){
        /*
		$this->stream .= 'f';
		$this->writeString('code');
		$this->writeString($code);
		$this->writeString('message');
		$this->writeString($message);
		// OJO puede ser false o null o lo que sea, por lo pronto no es null
		if(!is_null($detail)){
			$this->writeString('detail');
			$this->writeObject($detail);
		}
		$this->stream .= 'z';*/
        //按照jsonrpc返回
        $this->stream = mc_pack_array2pack(
            array(JSONRPC_HEAD=>JSONRPC_VERSION,  //json rpc的协议头
                'error' => array(                 //错误信息
                    'code'=>$code,                //错误号
                    'message'=>$message,          //错误信息
                    'data'=>$detail               //错误数据,一般情况下是null
                ),
                'id' => "$id"                       //id号
            ),
            $version
        );
	}
}


?>
