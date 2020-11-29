<!DOCTYPE html>
<html>
        <head>
                <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
                <title>Mostrar numero</title>
        </head>

        <body>


        <?php
                //echo "<p>" . $_POST['decimal'] "</p>"
                error_reporting(E_ALL);

                $address = 'localhost';
                $port = 2018;
                $mens_ok = "OK ......";
                $mens_er = "ERROR ..........";

                $s = socket_create(AF_INET,SOCK_DGRAM, getprotobyname('udp'));

                $Numero =  $_POST["decimal"];

                $msg = "LEARN:" . $Numero ;
                $len = strlen($msg);



                        if ($Numero > 1023) {

                        }else{


                        socket_sendto($s, $msg, $len, 0, $address, $port);


                        socket_recvfrom($s, $resp, $len, 0, $addresscli, $portcli);
                        }
                        if( substr( $resp,0,2 ) == "OK")
                                $respuesta = $mens_ok;
                        elseif (substr( $resp,0,2 ) == "ER")
                                $respuesta = $mens_er;




        ?>

                <center>
                <?php echo "<p>".$respuesta."</p>" ?>
                </center>

        </body>
</html>
