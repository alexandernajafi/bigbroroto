<?php
header("Content-Type: text/event-stream");
header("Cache-Control: no-cache");
header("Connection: keep-alive");

/*for($i=0; $i<10; $i++){
	echo "heJ";
	ob_flush();
	flush();
	sleep(1);
}*/

error_reporting(E_ALL);
set_time_limit(0);
echo "HeJ";
$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
if($socket == false){
	die("Could not create socket.. wtf?");
}

$adr = "localhost";
$port = 8008;
$result = socket_connect($socket, $adr, $port);
if($result == false){
 	die("Could not bind socket.. wtf?");
}

for($i=0; $i<5;$i++){
	echo "data: "+socket_read($socket, 4)+"\n\n";
	ob_flush();
	flush();
	sleep(1);
}

socket_close($socket);
?>
