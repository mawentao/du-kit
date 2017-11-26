<?php

/***********************************************************
 * @file:   RedisClient.php
 * @author: mawentao(mawentao@baidu.com)
 * @create: 2013-11-17 20:49:05
 * @modify: 2013-11-17 20:49:05
 * @brief:  RedisClient.php
 ***********************************************************/


class RedisClient
{
	private $_hostarr;
	private $_rc;

	public function __construct(&$hostarr)
	{
		$this->_hostarr = $hostarr;
		$this->_rc = new Redis();
	}

	/* 根据key从Redis中获取value */
	public function get(&$keys, $key_prefix="")
	{
		$hostarr = $this->_hostarr;
		shuffle($hostarr);
		$connected = false;
		while(!empty($hostarr)){
			try{
				$host = array_pop($hostarr);
				$ip = $host["ip"];
				$port = $host["port"];
				$timeout = $host["timeout"];
				$this->_rc->connect($ip, $port, $timeout);
				$keyarr = array();
				foreach($keys as $key){
					$keyarr[] = $key_prefix.$key;
				}
				$valuearr = $this->_rc->mget($keyarr);
				return $valuearr;
			}catch(Exception $e){
				Env::getlog()->warning("could not connect to Redis server[$ip:$port], retrying...");
				//print("could not connect to Redis server[$ip:$port], retrying...");
				continue;
			}
		}
		return array();
	}


	/* 充Redis, 可带生命期 */
	public function set(&$kvmap, $key_prefix="", $lifeSeconds=0)
	{
		foreach($this->_hostarr as &$host)
		{
			try{
				$ip = $host["ip"];
				$port = $host["port"];
				$timeout = $host["timeout"];
				$this->_rc->connect($ip, $port, $timeout);

				$keys = array_keys($kvmap);
				foreach($keys as $k){
					$key = $key_prefix.$k;
					$value = $kvmap[$k];
					if($lifeSeconds > 0){
						$this->_rc->setex($key, $lifeSeconds, $value);
					}else{
						$this->_rc->set($key, $value);
					}
				}
			}catch(Exception $e){
				Env::getlog()->warning("could not connect to Redis server[$ip:$port], retrying...");
				//print("could not connect to Redis server[$ip:$port], retrying...");
				continue;
			}
		}
	}
}

// vim600: sw=4 ts=4 fdm=marker syn=php
?>
