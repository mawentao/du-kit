<html>
<head>
  <meta http-equiv="X-UA-Compatible" content="IE=edge">
  <meta http-equiv="Content-Type" content="text/html; charset=gb2312">
  <title>����ҳ��</title>
  <style>
    body {padding:0;margin:0;background-color:#efefef;}
    div.head {position:absolute;top:0;left:0;right:0;height:40px;padding:10px 0 0 100px;
            font:bold 24px Arial,"΢���ź�";color:#fff;background-color:#000;}
	div.body {position:absolute;top:70px;left:0;right:0;padding-left:100px;
			font:normal 16px Arial,"΢���ź�";color:darkred;}
    .mwt-btn { padding: 6px 12px; font:normal 14px Arial,"΢���ź�";line-height: 1.428571429; cursor: pointer;
			background-image: none; border: 1px solid transparent;
			-webkit-user-select: none;
			-moz-user-select: none;
			-ms-user-select: none;
			-o-user-select: none;
			user-select: none;
	}
	.mwt-btn:active, .mwt-btn.active {
		background-image: none;
		outline: 0;
		-webkit-box-shadow: inset 0 3px 5px rgba(0,0,0,0.125);
		box-shadow: inset 0 3px 5px rgba(0,0,0,0.125);
	}
	.mwt-btn-default { color: #333; background-color: #fff; border-color: #ccc; }
	.mwt-btn-default:hover, .mwt-btn-default:focus, .mwt-btn-default:active {
		color: #333; background-color: #efefef;border-color: #ccc;}
  </style>
<head>
<body>
  <div class='head'>Exception</div>
  <div class='body'>
	<?php if (isset(Env::$resData["errmsg"])) echo Env::$resData["errmsg"]; else echo "δ֪�쳣"; ?><br><br>
	<button class='mwt-btn mwt-btn-default' onclick='window.location.reload();'>ˢ��ҳ������</button>
  </div>
</body>
</html>
