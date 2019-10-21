const char PAGINA_INICIO[] PROGMEM = R"=====(
<html style="width: 100%; height: 100%;">
    <head>
        <title>Cagece</title>
        <style>
            span {
                display: block;
                vertical-align: middle;
                background: #00F;
                height: 120px;
                border-radius: 4px 0px 60px 60px;
                background: linear-gradient(to bottom, rgb(0, 146, 213), rgb(115, 199, 246));
                border-top: 6px dashed rgb(0, 146, 213);
            }
            #topo {
                width: 299px;
                height: 0px;
                border-right: 20px solid transparent;
                border-left: 20px solid transparent;
                border-bottom: 50px solid transparent;
                border-top: 100px solid rgb(115, 199, 246);
                border-radius: 0px;
                position: absolute;
                top:65px;
            }
            #meio {
                width: 271px;
                height: 0px;
                border-right: 20px solid transparent;
                border-left: 20px solid transparent;
                border-bottom: 50px solid transparent;
                border-top: 100px solid rgb(110, 194, 241);
                border-radius: 40px;
                position: absolute;
                top: 135px;
            }
            #fundo {
                width: 243px;
                height: 0px;
                border-right: 20px solid transparent;
                border-left: 20px solid transparent;
                border-bottom: 50px solid transparent;
                border-top: 100px solid rgb(105, 199, 236);
                border-radius: 40px;
                position: absolute;
                top: 205px;
            }
            #tampa {
                width: 25px;
                height: 0px;
                border-right: 170px solid transparent;
                border-left: 170px solid transparent;
                border-top: 20px solid transparent;
                border-bottom: 49px solid rgb(0, 146, 213);
                border-radius: 20px;
                position: absolute;
                top: 0px;
            }
            .vazio {
                border-top-color: rgb(215, 235, 255) !important
            }
            button {
                width: 50%;
                display: inline-block;
                padding: 5px;
            }
        </style>
    </head>
    <body style="width: 100%; height: 100%;">
        <div  style="width: 100%; flex-direction: column; height: 100%; display: flex; justify-content: center; align-items: center; position: relative;">
            <span id="fundo">&nbsp;</span>
            <span id="meio">&nbsp;</span>
            <span id="topo">&nbsp;</span>
            <span id="tampa">&nbsp;</span>
            <hr/>
            <hr/>
            <div style="width: 80%; padding: 0; position: absolute; top: 320px;">
                <button onclick="desligar()">DESLIGAR</button><button onclick="ligar()">LIGAR</button>
            </div>
        </div>
        <script>
            window.onload = function() {
                setTimeout(check, 1000);
            }
            function ligar() {
                var xhttp = new XMLHttpRequest();
                xhttp.open("GET", "ledON?pino=22", true);
                xhttp.send();
            }

            function desligar() {
                var xhttp = new XMLHttpRequest();
                xhttp.open("GET", "ledOFF?pino=22", true);
                xhttp.send();
            }
            function check() {
                var xhttp = new XMLHttpRequest();
                xhttp.onreadystatechange = function() {
                    if (this.readyState == 4 && this.status == 200) {
                        if (this.responseText != '') {
                            var res = JSON.parse(this.responseText);
                            if (res.alto == 1) {
                                document.getElementById('topo').classList.remove('vazio');
                                document.getElementById('meio').classList.remove('vazio');
                                document.getElementById('fundo').classList.remove('vazio');
                            }
                            else if (res.baixo == 0) {
                                document.getElementById('topo').classList.add('vazio');
                                document.getElementById('meio').classList.remove('vazio');
                                document.getElementById('fundo').classList.remove('vazio');                                
                            }
                            else {
                                document.getElementById('topo').classList.add('vazio');
                                document.getElementById('meio').classList.add('vazio');
                                document.getElementById('fundo').classList.remove('vazio');
                            }
                        }
                        else {
                            document.getElementById('topo').classList.add('vazio');
                            document.getElementById('meio').classList.add('vazio');
                            document.getElementById('fundo').classList.add('vazio');
                        }
                        setTimeout(check, 2000);
                    }
                };
                xhttp.open("GET", "check", true);
                xhttp.send();
            }
        </script>
    </body>
</html>
)=====";
