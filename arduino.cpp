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

const char* ssid = "Helpy Weather Config"; //nome da rede ap
const char* password = "12345678"; //senha da rede ap

ESP8266WebServer server(80); //server usa a porta padrao 80

void setup(){
  pinMode(ONBOARD_LED, OUTPUT);
  Serial.begin(115200);

  WiFi.softAP(ssid, password);
  Serial.print("Ponto de Acesso /");
  Serial.print(ssid);
  Serial.print("\" iniciado");

  Serial.print("Endereco de IP:\t");
  Serial.println(WiFi.softAPIP());

  //tratamentos das rotas do server

  server.on("/", handleRoot);
  server.on("/action_new_connection", handleForm);
  server.on("/action_previous_connection", connectEeprom);

  server.begin();
  Serial.println("Servidor HTTP iniciado");
}

void loop(){
    server.handleClient();
    if(WiFi.status() != WL_CONNECTED) {
    digitalWrite(ONBOARD_LED, HIGH); //Desativa o LED
    Serial.println("CONECTADO");
  }
}

void handleRoot(){
    String index = listSSID(); //le o html
    server.send(200, "text/html", index);
}

void handleForm(){
    String ssidWifi = server.arg("ssid"); //pega esses dados da pagina html
    String passwordWifi = server.arg("password");

    Serial.printf("SSID: %s\n", ssidWifi);
    Serial.printf("Password: %s\n", passwordWifi);

    if(!ssidWifi.equals("") && !passwordWifi.equals("")){
        connectToWifi(ssidWifi, passwordWifi);
    }
}

void connectToWifi(String ssidWifi, String passwordWifi){
    int count = 0;
    WiFi.begin(ssidWifi.c_str(), passwordWifi.c_str()); //conecta ao roteador
    Serial.print("");

    //espera por alguma conexao
    while (count < 15){
        delay(500);
        Serial.print(".");
        if(WiFi.status() == WL_CONNECTED) {
            Serial.println("");
            salvarEeprom(ssidWifi, passwordWifi);
            Serial.println("");
            //se a conexao acontecer com sucesso ele mostra o ip no monitor serial
            Serial.println("Conectado ao WiFi");
            Serial.print("Endereco IP: ")
            Serial.println(WiFi.localIP()); //endereco ip do esp
            digitalWrite(ONBOARD_LED, LOW); //acende o mano led
            String responsePage = (const __FlashStringHelper*) MAIN_page;
            responsePage.replace("<br><br>", "<p id='status'>Conectado!</p>");
            server.send(200, "text/html", responsePage);
            return;
        }
        else if(WiFi.status() == WL_CONNECT_FAILED){
            String responsePage = (const __FlashStringHelper*) MAIN_page;
            responsePage.replace("<br><br>", "<p id='status', style='color=red;'>Falha ao Conectar</p>");
            server.send(200, "text/html", responsePage);
        }
        count++;
    }
    Serial.println();
    Serial.println("Timed out.");
    String responsePage = (const __FlashStringHelper*) MAIN_page; 
    responsePage.replace("<br><br>", "<p id='status' style='color:red;'>Erro.</p>");
    server.send(200, "text/html", responsePage);
    return;
}

String listSSID(){
    String index = (const __FlashStringHelper*) MAIN_page; // le o conteudo html
    String networks = "";
    int n = WiFi.scanNetworks();
    Serial.println("Acabou o escaneamento");
    if(n == 0){
        Serial.printf("Nenhuma rede encontrada")
        index.replace("<select class='text-field' name='ssid'><option value'' disable select>Nenhuma Rede Encontrada</option></select>");
        index.replace("<br><br>", "<p id='status' style='color=red;'>Rede nao encontrada.</p>");
        return index;
    }
    else{
        Serial.printf("%d Redes Encontradas. \n", n);
        networks += "<select class='text-field' name=ssid><option value='' disable selected>SSID</option></select>";
        for (int i = 0; i < n; ++i){
            //mostra o ssid de cada rede encontrada
            networks += "option value='" + WiFi.SSID(i) + "'>" +WiFi.SSID(i) + "</option>";
        }
        networks += "</select>";
    }
    index.replace("<select class='text-field' name='ssid'></select>", networks);
    return index;
}

void salvarEeprom(String ssidWifi, String passwordWifi) {
  EEPROM.begin(98); // Tamanho da FLASH reservado para EEPROM. Pode ser de 4 a 4096 bytes
 
  if(!compareEeprom(ssidWifi, passwordWifi)) {
    Serial.println("Salvando:");
    EEPROM.write(0, ssidWifi.length());
    Serial.println(ssidWifi.length());
    
    for(int i = 2; i < 2+ssidWifi.length(); i++) {
      Serial.print(ssidWifi.charAt(i-2));
      EEPROM.write(i, ssidWifi.charAt(i-2));
    }
    Serial.println("");
    
    Serial.println("Salvando:");
    EEPROM.write(1, passwordWifi.length());
    Serial.println(passwordWifi.length());
    
    for(int j = 2+ssidWifi.length(); j < 2+ssidWifi.length()+passwordWifi.length(); j++) {
      Serial.print(passwordWifi.charAt(j-2-ssidWifi.length()));
      EEPROM.write(j, passwordWifi.charAt(j-2-ssidWifi.length()));
    }
    Serial.println("");
    
    EEPROM.commit(); // Salva alterações na FLASH
  }
  EEPROM.end(); // Apaga a cópia da EEPROM salva na RAM
}

boolean compareEeprom(String ssidWifi, String passwordWifi) {
    int idLength = int(EEPROM.read(0)); //tamanho do ssid armazenado
    int passLength = int(EEPROM.read(1)); //tamanho da senha armazenada
    String id = "";
    String pass = "";

    Serial.println("Lendo SSID: ");
    Serial.print("Tamanho: ");
    Serial.println(idLength);
    for(int i = 2; i < 2+idLength; i++) {
        Serial.print("Posição ");
        Serial.print(i);
        Serial.print(": ");
        id = id + char(EEPROM.read(i));
        Serial.println(id[i-2]);
    }
    Serial.println("");

    Serial.println("Lendo senha:");
    Serial.print("Tamanho:");
    Serial.println(passLength);
    for(int j = 2+idLength; j < 2+idLength+passLength; j++) {
        Serial.print("Posição ");
        Serial.print(j);
        Serial.print(": ");
        pass = pass + char(EEPROM.read(j));
        Serial.println(pass[j-2-idLength]);
        Serial.println(pass);
    }
    Serial.println("");

    Serial.print("SSID é igual: ");
    Serial.println(id.equals(ssidWifi));
    
    Serial.print("Senha é igual: ");
    Serial.println(pass.equals(passwordWifi));
    
    if(id.equals(ssidWifi) && pass.equals(passwordWifi))
    {
        Serial.println("Dados já presentes na memória.");
        return true;
    }
    else
    {
        return false;
    }
}

void connectEeprom() {
    EEPROM.begin(98); //denovo, e o tamanho da memoria usada no flash(aquele "e" tem acento, e pq meu teclado e ingles... mds que falta faz um teclado pt-br)

    int ssidSize = (int)EEPROM.read(0); // Tamanho do SSID armazenado (número de bytes)
    int passwordSize = (int)EEPROM.read(1); // Tamanho do Password armazenado (número de bytes)
    String ssidWifi = "";
    String passwordWifi = "";
    
    Serial.println("Lendo:");
    for(int i = 2; i < 2+ssidSize; i++) {
        Serial.print(char(EEPROM.read(i)));
        ssidWifi.concat(char(EEPROM.read(i)));
    }
    Serial.println("");
    
    Serial.println("Lendo:");
    for(int j = 2+ssidSize; j < 2+ssidSize+passwordSize; j++) {
        Serial.print(char(EEPROM.read(j)));
        passwordWifi.concat(char(EEPROM.read(j)));
    }
    Serial.println("");
    
    EEPROM.end(); // Apaga a cópia da EEPROM salva na RAM
    
    Serial.println("Leu:");
    Serial.println(ssidWifi);
    Serial.println(passwordWifi);
    
    connectToWiFi(ssidWifi, passwordWifi);
}
