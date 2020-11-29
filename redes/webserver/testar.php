
<!DOCTYPE html>
<html>
        <head>
                <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
                <title>Testear</title>
        </head>

        <body>

        <?php

                error_reporting(E_ALL);

                $address = 'localhost';
                $port = 2018;
                $mens_ok = "OK ......";
                $mens_er = "ERROR ..........";
                $test = "TESTS:";
                        $s = socket_create(AF_INET,SOCK_DGRAM, getprotobyname('udp'));

                        $Rango =  $_POST["naleat"];
                        $coma = ",";

                        $Numero9 = isset($_POST["bitcode9"]) ? "1":"0";
                        $Numero8 = isset($_POST["bitcode8"]) ? "1":"0";
                        $Numero7 = isset($_POST["bitcode7"]) ? "1":"0";
                        $Numero6 = isset($_POST["bitcode6"]) ? "1":"0";
                        $Numero5 = isset($_POST["bitcode5"]) ? "1":"0";
                        $Numero4 = isset($_POST["bitcode4"]) ? "1":"0";
                        $Numero3 = isset($_POST["bitcode3"]) ? "1":"0";
                        $Numero2 = isset($_POST["bitcode2"]) ? "1":"0";
                        $Numero1 = isset($_POST["bitcode1"]) ? "1":"0";
                        $Numero0 = isset($_POST["bitcode0"]) ? "1":"0";


                        $Number = bindec( $Numero9. $Numero8 . $Numero7  . $Numero6 . $Numero5 . $Numero4. $Numero3 . $Numero2 . $Numero1 . $Numero0);
                        print $Number;



                        $msg = $test . $Rango . $coma . $Number;
                        $len = strlen($msg);


                        socket_sendto($s, $msg, $len, 0, $address, $port);


                        socket_recvfrom($s, $resp, $len, 0, $addresscli, $portcli);

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
