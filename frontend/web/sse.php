<?php

$data = $_GET["data"]; 

header("Content-Type: text/event-stream");
header("Cache-Control: no-cache");
header("Connection: keep-alive");

function sse_logging(){
	
}

function sse_angle(){
	
}

switch($data){
	case "logs":
	sse_logging();
	break;
	case "angle":
	sse_angle();
	break;
}
?>
