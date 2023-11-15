#include <ESP8266WiFi.h>       
#include <ESP8266WebServer.h>
#include <EEPROM.h>

#define ONBOARD_LED D4 //ledzin que ajuda

const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Configuration page - Helpy Weather</title>
    <style>
    body {color: #434343; font-family: "Helvetica Neue",Helvetica,Arial,sans-serif; font-size: 14px; background-color: #eeeeee; margin-top: 100px;}
    .container {margin: 0 auto; max-width: 400px; padding: 30px; box-shadow: 0 10px 20px rgba(0,0,0,0.19), 0 6px 6px rgba(0,0,0,0.23); background-color: #ffffff; border-radius: 10px;}
  h2 {text-align: center; margin-bottom: 20px; margin-top: 0px; color: #0ee6b1; font-size: 35px;}
  #titleGreen {color: #00E1AA;}
  #titleBlackG {color: #001211;}
    h3 {text-align: center; margin-bottom: 40px; margin-top: 0px; color: #336859; font-size: 35px;}
    form .field-group {box-sizing: border-box; clear: both; padding: 4px 0; position: relative; margin: 1px 0; width: 100%;}
    .text-field {font-size: 15px; margin-bottom: 4%; -webkit-appearance: none; display: block; background: #fafafa; color: #636363; width: 100%; padding: 15px 0px 15px 0px; text-indent: 10px; border-radius: 5px; border: 1px solid #e6e6e6; background-color: transparent;}
    .text-field:focus {border-color: #00bcd4; outline: 0;}
    .button-container {box-sizing: border-box; clear: both; margin: 1px 0 0; padding: 4px 0; position: relative; width: 100%;}
    .button {background: #00E1AA; border: none; border-radius: 5px; color: #ffffff; cursor: pointer; display: block; font-weight: bold; font-size: 16px; padding: 15px 0; text-align: center; text-transform: uppercase; width: 100%; -webkit-transition: background 250ms ease; -moz-transition: background 250ms ease; -o-transition: background 250ms ease; transition: background 250ms ease;}
    p {text-align: center; text-decoration: none; color: #87c1d3; font-size: 18px;}
    a {text-decoration: none; color: #ffffff; margin-top: 0%;}
    #status {text-align: center; text-decoration: none; color: #336859; font-size: 14px;}
    </style>
    <script>
    function validateForm() {
        var ssid = document.forms["myForm"]["ssid"].value;
        var password = document.forms["myForm"]["password"].value;
        var status = document.getElementById("statusDiv");
        if (ssid == "" & password == "") {
            status.innerHTML == "<p id='status' style='color:red;'>Insira SSID e senha.</p>";
            return false;
        }
        else if(ssid == "") {
            status.innerHTML = "<p id='status' style='color:red;'>Insira SSID.</p>";
            return false;
        }
        else if(password == "") {
            status.innerHTML = "<p id='status' style='color:red;'>Insira Senha.</p>";
            return false;
        }
        else {
            status.innerHTML = "<p id='status'>Conectando...</p>";
            return false;
        }
    }
    </script>
</head>
<body>
    <div class="Container">
        <h2><span id="titleGreen">Helpy</span><span id="titleBlackG">Weather</span></h2>
        <h3>Configuracao WiFi</h3>
        <form name="myForm" action="/action_new_connection" onsubmit="return validateForm()" method="post">
            <div class="field-group">
                <select class="text-field" name="ssid"></select>
            </div>
            <br>
            <div>
                <input class="text-field type="password" name="password" lenght=64 placeholder="Senha">                
            </div>
            <br>
            <div  id="statusDiv">
                <br><br>
            </div>
            <div class="button-container">
                <input class="button" type="submit" value="Conectar">
            </div>
        </form>
            <p>OU</p>
            <div>
                <div class="button-container">
                    <button class="button" type="button" onclick="window.location.href='/action_preview_conection'">Conectar a ulitma rede utilizada</button>
                </div>
            </div>
    </div>
</body>
</html>
)=====";

const char* ssid = "Helpy Weather Config" //nome da rede ap
const char* password = "12345678" //senha da rede ap

ESP8266WebServer server(80); //server usa a porta padrao 80

void setup(){
  pinMode(ONBOARD_LED, OUTPUT);
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  serial.print()
}
