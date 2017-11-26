<?php

/***********************************************************
 * @file:   HttpClient.php
 * @author: mawentao(mawentao@baidu.com)
 * @create: 2013-10-31 09:08:58
 * @modify: 2013-10-31 09:08:58
 * @brief:  HttpClient
 ***********************************************************/


// Http�����
class HttpRequest {
	public $url;    // �����ַ  
    public $method; // ���󷽷�  
    public $scheme; // Э�飬http  
    public $host;   // ����  
    public $port;   // �˿�  
    public $path;   // ·��  
  
    public $header;         // ����ͷ  
    public $body;           // ��������  

	function __construct() { 
		$this->method = "get";
		$this->scheme = "http";
		$this->host   = "127.0.0.1";
		$this->port   = 80;
		$this->path   = "/";
		$this->header = array();
	}

    // ����ͷ  
    function setHeader($k, $v) { $this->header[$k] = $v; }
	// ����Method
	function setMethod($method) { $this->method = $method; }
	// ����url
	function setUrl($url) 
	{/*{{{*/
		$this->url = $url;
		$matches = parse_url($this->url);  
        !isset ($matches['host'])  && $matches['host'] = '';
        !isset ($matches['path'])  && $matches['path'] = '';
        !isset ($matches['query']) && $matches['query'] = '';
        !isset ($matches['port'])  && $matches['port'] = '';
  
        $host = $matches['host'];  
        $path = $matches['path'] ? $matches['path'] . ($matches['query'] ? '?' . $matches['query'] : '') : '/';  
        $port = !empty ($matches['port']) ? $matches['port'] : 80;  
        $scheme = $matches['scheme'] ? $matches['scheme'] : 'http';  
  
        $this->host   = $host;  
        $this->path   = $path;  
        $this->scheme = $scheme;  
        $this->port   = $port;	
	}/*}}}*/

	// ��ȡ�����ַ���  
    // ����ͷ����������  
    // ��ȡ֮��ֱ��дsocket����  
    function toString() {  
        $method = strtoupper($this->method);
		$host   = $this->host;
		$path   = $this->path;
        $res = "$method $path HTTP/1.0\r\n";  
        $res .= "Host: $host\r\n";
		foreach ($this->header as $k => $v) {
			$res .= "$k: $v\r\n";
		}
        $res .= "\r\n";  
        if ($this->body) {  
            $res .= $this->body;  
            $res .= "\r\n\r\n";  
        }  
        return $res;  
    }
}

// Http��Ӧ��
class HttpResponse {
	public $scheme; // Э��
	public $status; // ״̬�룬�ɹ���ʱ����200
	public $code;   // ״̬���ɹ���ʱ����ok
	public $header; // ��Ӧͷ
	public $body;   // ��Ӧ����

	function __construct() { 
		$this->header = array();
		$this->body = "";
	}
	function setHeader($key, $val) {
		$this->header[$key] = $val;
	}
	public function toString() {
		$arr = array($this->scheme, $this->status, $this->code);
		$str = implode(" ", $arr);
		$str.= "\n";
		foreach ($this->header as $k => $v) {
			$str.= "$k: $v\n";
		}
		$str.= "\n";
		$str.= $this->body;
		return $str;
	}
}

 
class HttpClient
{
	// ���շ��ذ�
	private static function recv_response($fp)
	{
		$response = new HttpResponse();

		//1. ��ȡ��Ӧͷ
		while (!feof($fp)) {
			$h = trim(fgets($fp));
			// ��Ӧͷ����
			if ($h=='') {
				break;
			}
			$pos = strpos($h, ":");
			// ��Ӧͷ��һ�д���
			if (!$pos) {
				$preg = '/^(\S*) (\S*) (.*)$/';
				preg_match_all($preg, $h, $arr);
				isset ($arr[1][0]) & $response->scheme = trim($arr[1][0]);  
				isset ($arr[2][0]) & $response->status = trim($arr[2][0]);  
				isset ($arr[3][0]) & $response->code = trim($arr[3][0]);  
			} 
			// ��Ӧͷ����
			else {
				$k = strtolower(trim(substr($h, 0, $pos)));
				$v = trim(substr($h, $pos +1));  
				$response->setHeader($k, $v);
			}
		}

		//2. ��ȡ��Ӧ����
		$len = (int) $response->header['content-length'];  
		$response->body = '';  
		while (!feof($fp) && $len > 0) {  
			$c = fread($fp, $len);
			$response->body .= $c;
			$len -= strlen($c);  
		}  

		fclose($fp);

		return $response;	
	}

	// ���Ͳ����ܷ��ذ�
	private function send_and_recv($url, $method, $timeout)
	{
		//1. ����url�����������
		$request = new HttpRequest();
		$request->setUrl($url);
		$request->setMethod($method);

		//if (isset($_SERVER['HTTP_X_FORWARDED_FOR']))
			$request->setHeader("X_FORWARDED_FOR", $_SERVER['REMOTE_ADDR']);


		//2. ��������
		$host = $request->host;
		$port = $request->port;
		$fp = stream_socket_client("tcp://$host:$port", $errno, $errstr, $timeout);
		$pack = $request->toString();
		//echo $pack."<hr>";

		//3. ��������
		fputs($fp, $request->toString());

		return self::recv_response($fp);
	}

	// post����
	public function post($url, $timeout=100)
	{
		return self::send_and_recv($url, "post", $timeout);
	}

	// get����
	public function get($url, $timeout=100)
	{
		return self::send_and_recv($url, "get", $timeout);
	}
}



if (__FILE__ == realpath($_SERVER['SCRIPT_FILENAME']))
{
	/*
	$res = HttpClient::post('http://10.48.22.12:8008/request.php?s=dd');
	echo $res->toString();

	$res = HttpClient::get('http://10.48.22.12:8008/request.php?s=dd');
	echo $res->body;
	*/

	//$res = HttpClient::post('http://10.48.22.12:8008/tmp/rpc.php/email');
	
//	$res = HttpClient::get('http://10.81.7.30:8008/info.php');
//	echo $res->toString();

	$url = "http://fc-hag.baidu.com/getRolesOfEntity.action?entity=6389372";
	$res = HttpClient::get($url);
	echo $res->toString();
	$data = json_decode($res->body, true);
	print_r($data);
	

	echo "\n";
	echo "!!!THE END!!!\n";
}


// vim600: sw=4 ts=4 fdm=marker syn=php
?>
