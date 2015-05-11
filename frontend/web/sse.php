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
$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
if($socket == false){
	echo "Could not create socket.. wtf?";
}
$adr = "localhost";
$port = 8008;
$result = socket_connect($socket, $adr, $port);
if($result == false){
	echo "Could not connect to socket";
}

#echo socket_read($socket, 4);
#echo socket_read($socket, 13);
#echo socket_read($socket, 1);
#echo socket_read($socket, 1);
#echo socket_read($socket, 1);

for(;;){
	#echo "data: ".socket_read($socket, 1)+"\n\n";
	$length = socket_read($socket, 4);
	$data = socket_read($socket, $length);
	echo "data: ";
	echo $data;
	echo "\n\n";
	ob_flush();
	flush();
	sleep(1);
}

socket_close($socket);
?>
