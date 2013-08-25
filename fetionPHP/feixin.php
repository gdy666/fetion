
<?php
/*
	飞信PHP接口源码来自于：http://blog.quanhz.com/archives/246
	本代码有修改，增加了非必选的repeat选项
	支持GET和POST两种访问方式
	user:飞信账号
	passwd:飞信密码
	msg:飞信内容
	to:收件人号码
	repeat:飞信的重发次数(非必须，缺省值为1)
	返回结果为JSON格式；
	发送错误：{state: "error"}
	发送成功：
	{state: "success_MSG"}		//对方飞信客户端在线时
	{state: "success_SMS"}		//对方飞信客户端离线，以短信的形式发送时
	*/
require './PHPFetion.php';

if($_GET["user"]){
	$fetion = new PHPFetion($_GET["user"], $_GET["passwd"]);	
	$result = $fetion->send($_GET["to"], $_GET["msg"],$_GET["repeat"]);	
}
else{
	$fetion = new PHPFetion($_POST["user"], $_POST["passwd"]);	
	$result = $fetion->send($_POST["to"], $_POST["msg"], $_POST["repeat"]);	
}

if (strstr($result,"发送消息成功")){
	echo '{state: "success_MSG"}';
}
else if(strstr($result,"短信发送成功")){
	echo '{state: "success_SMS"}';
}
else{
	echo '{state: "error"}';
}