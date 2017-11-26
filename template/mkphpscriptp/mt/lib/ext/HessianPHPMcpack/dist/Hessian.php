<?php
/*
-------------------------------------------------------------
HessianPHP - Binary Web Services for PHP

Copyright (C) 2004-2005  by Manolo G�mez
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

*/

define('HESSIAN_PHP_VERSION','1.0.5 RC2');

define('HESSIAN_FAULT',1);
define('HESSIAN_PARSER_ERROR',2);
define('HESSIAN_WRITER_ERROR',3);
define('HESSIAN_HTTP_ERROR',4);

define('HESSIAN_SILENT',1);
define('HESSIAN_EXCEPTION',2);
define('HESSIAN_TRIGGER_NOTICE',3);
define('HESSIAN_TRIGGER_ERROR',4);

define('JSONRPC_HEAD', 'jsonrpc');
define('JSONRPC_VERSION', '2.0');

define('JSONRPC_INVALID_ERROR', -32700);
define('JSONRPC_PARSER_ERROR', -32600);

include_once 'Filter.php';

if( !class_exists('Exception')){
	class Exception{}
}

/**
 * Represents an error generated by the Parser or Writer classes and Faults
 * 
 * @package HessianPHP
 * @author Manolo G�mez
 * @copyright Copyright (c) 2004
 * @version 1.0
 * @access public
 **/
class HessianError extends Exception{
	var $error;
	var $message;
	var $code;
	var $stream;
	var $callingContext;
	var $time;

	function HessianError($message='', $code=0, $error=null,$stream='') {
		$this->message = $message;
		$this->code = $code;
		$this->error = $error;
		$this->stream = $stream;
		$this->time = date("Y-m-d H:i:s");
	}

	function getError(){
		return $this->error;
	}

	function isError($obj){
		if( is_a($obj, 'HessianError') || is_a($obj, 'HttpError') )
			return true;
		return false;
	}

	function __toString(){
		if(phpversion() >= 5)
			return parent::__toString();
		$msg = "Message: ".$this->message."\n";
		$msg = "Error body: ".$this->error."\n";
		$msg .= "Time: ".$this->time."\n";
		$msg .= "Stream: ".$this->stream;
		return $msg;
	}
}

/**
 * Holds the mapping between remote and local object types
 * 
 * @package HessianPHP.Protocol
 * @author Manolo G�mez
 * @copyright Copyright (c) 2004
 * @version 1.0
 * @access public
 **/
class TypeMap {
	var $detectedTypes = array();
	var $types = array();

	function TypeMap(){
		$this->resetTypes();
	}

	function mapRemoteType($remoteType,$localType){
		$this->types[$remoteType] = strtolower($localType); // solo por que es php 4 toca hacerle minusculas
	}

	function getLocalType($remoteType) {
		if(!in_array($remoteType, $this->detectedTypes))
			$this->detectedTypes[] = $remoteType;
		if(class_exists($remoteType))
			return $remoteType;
		if(isset($this->types[$remoteType]))
			return $this->types[$remoteType];
		return false;
	}

	function getRemoteType($localType) {
		$val = array_search(strtolower($localType), $this->types); 
		if($val !== false) {
			return $val;
		}
		return false;
	}

	function resetTypes() {
		$this->types = array();
		$this->types['stdClass'] = 'stdClass';
	}
}

/**
 * Global library consiguration class. It holds objects shared by all client instances
 * and by the protocl handler classes.
 * It can be instanced but it's main use it through the singleton globalConfig() function
 * 
 * @package HessianPHP
 * @author Vegeta
 * @copyright Copyright (c) 2005
 * @version 1.0
 * @access public
 **/

class HessianConfig extends FilterContainer{
	var $typeMap;
	var $dateProvider;
	var $errorLog;
	var $remoteMethods = array();

	function HessianConfig(){
		$this->typeMap = &new TypeMap();
		$this->dateProvider = &new DefaultDateProvider();
		$this->errorLog = &new HessianErrorLog();
	}

	function remoteMethodUrl($url,$name){
		$phpmethod = strtolower($name);
		$this->remoteMethods[$url][$phpmethod] = $name;
	}

	/**
	 * Returns the singleton instance of the class and sets default configuration.
	 *  
	 * @return HessianConfig singleton
	 * @access protected
	 * @static
	 **/
	function &globalConfig(){
		static $config = null;
		if(!$config){
			$config = new HessianConfig();
		}
		return $config;
	}
}

/**
 * Main client configuration class. Use it's static methods to configure several options for the connection to a url
 * and mapping of remote types to php types via deserializers.
 * See HessianPHP tests for examples of usage.
 * 
 * @package HessianPHP
 * @author Vegeta
 * @copyright Copyright (c) 2005
 * @version 1.0
 * @access public
 **/
class Hessian{

	/**
	 * Returns an object representing information on a remote Hessian url.
	 * Optionally, it can create a default object if needed
	 *  
	 * @param string url Remote url
	 * @param boolean create Shall it create the object?
 	 * @access public 
	 * @static
	 **/
	function &getHessianProxy($url,$options=false){
		/*$config = &HessianConfig::globalConfig();
		if(!isset($config->proxies[$url])) {
			$proxy = &new HessianProxy($url,$options);
			$config->proxies[$url] = &$proxy;
		}
		$proxy = &$config->proxies[$url];
		$proxy->options = $options;
		return $proxy;*/
		$proxy = new HessianProxy($url,$options);
		return $proxy;
	}

	/**
	 * Registers a remote method name for a service url. 
	 *  
	 * @param string url Url of the service
	 * @param string name Name of the remote service method 
 	 * @access public 
	 * @static
	 **/
	function remoteMethod($url,$name){
		$config = &HessianConfig::globalConfig();
		$config->remoteMethodUrl($url,$name);
	}

	/**
	 * Sets a mapping between a remote object type to a php object type.
	 * It maps the class name received in a service to a local php class to be created
	 * and deserialized by the default object deserializer.
	 * Usefull to match types from different platforms to custrom php classes in a simple way.  
	 *
	 * @param string remoteType Name of the remote Class
	 * @param string phptype Name of the PHP class to create
 	 * @access public 
	 * @static
	 **/
	function mapRemoteType($remoteType,$phptype){
		$config = &HessianConfig::globalConfig();
		$config->typeMap->mapRemoteType($remoteType,$phptype);
	}

	function setDateProvider(&$provider){
		$config = &HessianConfig::globalConfig();
		$config->dateProvider = &$provider;
	}

	function &getDateProvider(){
		$config = &HessianConfig::globalConfig();
		return $config->dateProvider;
	}

	function errorReporting($level=null){
		$config = &HessianConfig::globalConfig();
		if($level!=null)
			$config->errorLog->errorLevel = $level;
		return $config->errorLog->errorLevel;
	}

	function error(){
		$config = &HessianConfig::globalConfig();
		return $config->errorLog->getLastError();
	}

	function getErrorStack(){
		$config = &HessianConfig::globalConfig();
		return $config->errorLog->errorStack;
	}

	function isError($obj){
		if( is_a($obj, 'HessianError') || is_a($obj, 'HttpError') )
			return true;
		return false;
	}

	function addFilter(&$obj,$name=''){
		$config = &HessianConfig::globalConfig();
		$config->addFilter($obj,$name);
	}

	function removeFilter($name){
		$config = &HessianConfig::globalConfig();
		$config->removeFilter($name);
	}
}


/**
 * This class gives HessianParser and HessianWriter a way to extract timestamps from Datetime objects
 * and to return new Datetime objects from timestamps that are deserialized from a Hessian reply.
 *
 * @package HessianPHP.Protocol
 * @author Manolo G�mez
 * @copyright Copyright (c) 2005
 * @version 1.0
 * @access public
 **/

class DefaultDateProvider{

	function DefaultDateProvider(){
		include_once('HessianDateTime.php');
	}

	function isDateObject(&$object){
		return is_a($object,'HessianDateTime');
	}

	function readDate($timestamp){
		return new HessianDateTime($timestamp / 1000);
	}

	function writeDate(&$dateobj){
		return $dateobj->getTimestamp() * 1000; // +0.0;
	}

}

/**
 * This is an error handling class that receives notifications from the clients
 * when an error occurs and notifies the user code. Implements the observer pattern.
 *
 * @package HessianPHP
 * @author Manolo G�mez
 * @copyright Copyright (c) 2005
 * @version 1.0
 * @access public
 **/

class HessianErrorLog{
	var $errorStack= array();
	var $errorLevel;

	function HessianErrorLog(){
		$this->clear();
		if (phpversion() < 5)
			$this->errorLevel = HESSIAN_SILENT;
		else
			$this->errorLevel = HESSIAN_EXCEPTION;
	}

	function notifyError($error){
		$this->errorStack[] = $error;
		switch($this->errorLevel){
			// Here's one of those ugly things you have to do in life sometimes
			case HESSIAN_EXCEPTION: 
				if( phpversion() >= 5 )	
					eval('throw $error;'); 
				break; 
			case HESSIAN_TRIGGER_NOTICE: trigger_error($error->message); break;
			case HESSIAN_TRIGGER_ERROR: trigger_error($error->message,E_USER_ERROR); break;
			default: break; // silent
		}
	}

	function clear(){
		$this->errorStack = array();
	}

	function getLastError(){
		if(!empty($this->errorStack)){
			$last = count($this->errorStack) -1;
			return $this->errorStack[$last];
		}
		return false;
	}
}

// static initialization
HessianConfig::globalConfig();
// uncomment this line if you want to control error_reporting() yourself
Hessian::addFilter(new PHPErrorReportingFilter());
?>
