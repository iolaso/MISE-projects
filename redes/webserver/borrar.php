
<!DOCTYPE html>
<html>
        <head>
                <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
                <title>Resetear placa</title>
        </head>

        <body>
        <?php

                error_reporting(E_ALL);

                $address = 'localhost';
                $port = 2018;
                $mens_ok = "OK ......";
                $mens_er = "ERROR ..........";

                $s = socket_create(AF_INET,SOCK_DGRAM, getprotobyname('udp'));



                $msg = "CLEAR" ;
                $len = strlen($msg);

                socket_sendto($s, $msg, $len, 0, $address, $port);

                socket_recvfrom($s, $resp, $len, 0, $addresscli,$portcli);

                if( substr( $resp,0,2 ) == "OK")
                        $respuesta = $mens_ok;
                else
                        $respuesta = $mens_er;


        ?>

                <center>
                <?php echo "<p>".$respuesta."</p>" ?>
                </center>

        </body>
</html>

