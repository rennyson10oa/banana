#include <ESP8266WiFi.h>       
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <ESP8266HTTPClient.h>
#include <DHT.h>
#include <FirebaseArduino.h>

#define ONBOARD_LED D4 //pino do led de aviso wifi
#define DHTPIN 4 //Pino digital D2 (GPIO4) conectado ao DHT11

#define FIREBASE_HOST "helpyweather-b54ab-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "LwDhQynOI2BrXFBzxkSW8GSkUF9zAEqVL1nCK6Ck"

const int pinHigrometro = A0; //pino do higrometro

const char* ssid = "Helpy Weather Config"; //nome da rede ap
const char* password = "12345678"; //senha da rede ap

//pagina html do web server
const char MAIN_page[] PROGMEM = R"=====( 
<!DOCTYPE html>
<html>
<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <title>HTML Form ESP8266 - SmartKits</title>
    <style>
        body {color: #434343; font-family: "Helvetica Neue",Helvetica,Arial,sans-serif; font-size: 14px; background-color: #eeeeee; margin-top: 100px;}
        .container {margin: 0 auto; max-width: 400px; padding: 30px; box-shadow: 0 10px 20px rgba(0,0,0,0.19), 0 6px 6px rgba(0,0,0,0.23); background-color: #ffffff; border-radius: 10px;}
    h2 {text-align: center; margin-bottom: 20px; margin-top: 0px; color: #0ee6b1; font-size: 35px;}
    #titleGreen {color: #00E1AA;}
    #titleBlack {color: #000000;}
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
        if (ssid == "" && password == "") {
        status.innerHTML = "<p id='status' style='color:red;'>Insira SSID e senha.</p>";
        return false;
        }
        else if (ssid == "") {
        status.innerHTML = "<p id='status' style='color:red;'>Insira o SSID.</p>";
        return false;
        }
        else if (password == "") {
        status.innerHTML = "<p id='status' style='color:red;'>Insira a senha.</p>";
        return false;
        }
        else {
        status.innerHTML = "<p id='status'>Conectando...</p>";
        return true;
        }
    }
    </script>
    </head>
    <body>
    <div class="container">
    <h2><span id="titleGreen">Helpy</span><span id="titleBlack">Weather</span></h2>
    <h3>Conexão ESP8266</h3>
    <form name="myForm" action="/action_new_connection" onsubmit="return validateForm()" method="post">
        <div class="field-group">
    <select class='text-field' name='ssid'></select>
        </div>
        <br>
        <div class="field-group">
        <input class="text-field" type="password" name="password" length=64 placeholder="Password">
        </div>
    <br>
    <div id="statusDiv">
        <br><br>
    </div>
        <div class="button-container">
        <input class="button" type="submit" value="Conectar">
        </div>
    </form>
    <p>OU</p>
    <div class="button-container">
        <button class="button" type="button" onclick="window.location.href='/action_previous_connection'">Conectar à última rede utilizada</button>
    </div>
    </div>
    </body>
</html>
)=====";

String listSSID() {
  String index = (const __FlashStringHelper*) MAIN_page; //Lê a página HTML
  String networks = ""; //variavel de armazenamento da rede
  int n = WiFi.scanNetworks(); //escaneamento das redes proximas
  Serial.println("Escaneamento Terminado.");
  if (n == 0) {
    Serial.println("Nenhuma rede foi encontrada.");
    index.replace("<select class='text-field' name='ssid'></select>", "<select class='text-field' name='ssid'><option value='' disabled selected>Nenhuma rede encontrada</option></select>"); //modifica a pagina html
    index.replace("<br><br>", "<p id='status' style='color:red;'>Rede não encontrada.</p>");
    return index;
  }
  else {
    Serial.printf("%d networks found.\n", n); //printa o numero de redes achadas
    networks += "<select class='text-field' name='ssid'><option value='' disabled selected>SSID</option>";
    // Pega o SSID de cada rede encontrada e muda a pagina html

    for (int i = 0; i < n; ++i)
    {
      networks += "<option value='" + WiFi.SSID(i) + "'>" + WiFi.SSID(i) + "</option>";
    }
    networks += "</select>";
  }
  index.replace("<select class='text-field' name='ssid'></select>", networks);
  return index;
}


ESP8266WebServer server(80); //server usa a porta padrao 80

//declaracao de funcoes
void salvarEeprom(String ssidWifi, String passwordWifi);
void connectToWifi(String ssidWifi, String passwordWifi);
void handleRoot();
void handleForm();
void connectEeprom();

DHT dht(DHTPIN, DHT11); //Declaracao do dht(sensor de temperatura a umidade do ar)

void setup(){
    pinMode(ONBOARD_LED, OUTPUT);
    pinMode(pinHigrometro, INPUT);
    Serial.begin(115200);
    

    WiFi.softAP(ssid, password);
    Serial.print("Ponto de Acesso /");
    Serial.print(ssid);
    Serial.print("\" iniciado");

    Serial.print("Endereco de IP:\t");
    Serial.println(WiFi.softAPIP());

    connectEeprom(); //ver se ja existe uma rede salva

    //tratamentos das rotas do server

    server.on("/", handleRoot);
    server.on("/action_new_connection", handleForm);
    server.on("/action_previous_connection", connectEeprom);

    server.begin();
    Serial.println("Servidor HTTP iniciado");

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); //inicia o firebase
    Serial.println("Firebase iniciado");

}

void handleRoot() {
    String index = listSSID(); //Recebe página HTML editada
    server.send(200, "text/html", index); //Envia página web ao usuário
}

void handleForm() {

    String ssidWifi = server.arg("ssid"); //pega o argumento da pagina html
    String passwordWifi = server.arg("password");

    //chama a funcao de conectar wifi

    if(!ssidWifi.equals("") && !passwordWifi.equals("")) {
    connectToWifi(ssidWifi, passwordWifi);
    
    }
}

void connectToWifi(String ssidWifi, String passwordWifi) {
    printf("chamada do connect wifi");
    int count = 0;
    WiFi.begin(ssidWifi.c_str(), passwordWifi.c_str());     //Conecta ao seu roteador Wi-Fi
    
    // Espera pela conexão
    while ( count < 15 ) {
        delay(500);
        if (WiFi.status() == WL_CONNECTED) {
        salvarEeprom(ssidWifi, passwordWifi);
        //Se a conexão acontecer, mostra o endereço IP no monitor serial
        Serial.println("Conectado ao WiFi");
        Serial.print("Endereço IP: ");
        Serial.println(WiFi.localIP());  //Endereço IP atribuído ao ESP
        digitalWrite(ONBOARD_LED, LOW); //Desativa o led da placa

        String responsePage = (const __FlashStringHelper*) MAIN_page; //Lê conteúdo do HTML
        responsePage.replace("<br><br>", "<p id='status'>Conectado!</p>"); //Aciona mensagem de sucesso
        server.send(200, "text/html", responsePage); //Envia a página web
        return;
        }
        else if (WiFi.status() == WL_CONNECT_FAILED) {
        String responsePage = (const __FlashStringHelper*) MAIN_page; //Lê conteúdo do HTML
        responsePage.replace("<br><br>", "<p id='status' style='color:red;'>Falha na conexão.</p>"); //Aciona mensagem de erro
        server.send(200, "text/html", responsePage); //Envia a página web
        }
        count++;
    }
    //Caso ocorra algum erro não detectado dentro do while
    String responsePage = (const __FlashStringHelper*) MAIN_page; //Lê conteúdo do HTML
    responsePage.replace("<br><br>", "<p id='status' style='color:red;'>Erro.</p>");
    server.send(200, "text/html", responsePage); 

    return;
}

boolean compareEeprom(String ssidWifi, String passwordWifi) {
    //esse codigo serve para comparar os dados ja salvos no flash

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

void salvarEeprom(String ssidWifi, String passwordWifi) {

    //codigo para salvar na memoria flash

    EEPROM.begin(98); // Tamanho da FLASH reservado para EEPROM. Pode ser de 4 a 4096 bytes
    
    if(!compareEeprom(ssidWifi, passwordWifi)) {
        Serial.println("Salvando:");
        EEPROM.write(0, ssidWifi.length());
        Serial.println(ssidWifi.length());
        
        for(unsigned int i = 2; i < 2+ssidWifi.length(); i++) {
        Serial.print(ssidWifi.charAt(i-2));
        EEPROM.write(i, ssidWifi.charAt(i-2));
        }
        Serial.println("");
        
        Serial.println("Salvando:");
        EEPROM.write(1, passwordWifi.length());
        Serial.println(passwordWifi.length());
        
        for(unsigned int j = 2+ssidWifi.length(); j < 2+ssidWifi.length()+passwordWifi.length(); j++) {
        Serial.print(passwordWifi.charAt(j-2-ssidWifi.length()));
        EEPROM.write(j, passwordWifi.charAt(j-2-ssidWifi.length()));
        }
        Serial.println("");
        
        EEPROM.commit(); // Salva alterações na FLASH
    }
    EEPROM.end(); // Apaga a cópia da EEPROM salva na RAM
}

void connectEeprom() {
    //esse codigo e para conectar ao wifi ja armazenado na memoria flash
    
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
    
    connectToWifi(ssidWifi, passwordWifi);
}

// string que analisa os valores do solo, deve ser modificada dependendo do solo, sera algo adicionado mais tarde
String analiseUmidade(int valorHigrometro) {

    // Status: Solo úmido
    if (valorHigrometro >= 0 && valorHigrometro <= 400)
    {
        return "Úmido";
    }
    
    // Status: Umidade moderada
    if (valorHigrometro > 400 && valorHigrometro <= 800)
    {
        return "Média";
    }
    
    // Status: Solo seco
    if (valorHigrometro > 800 && valorHigrometro <= 1024)
    {
        return "Seco";
    }
    return "Erro";
}

void higroTimer() {
    int umidadeSolo = analogRead(pinHigrometro);
    String textUmi = analiseUmidade(umidadeSolo);
    
    Serial.print("Umidade do solo: ");
    Serial.print(umidadeSolo);
    Serial.print(" | Status: ");
    Serial.println(textUmi);
}

void dhtTimer(){
    // funcao do dht
    float umidadeAr = dht.readHumidity();
    float temperatura = dht.readTemperature();
    if (isnan(umidadeAr) || isnan(temperatura)) {
        Serial.println("Falha ao ler o sensor DHT11");
        return;
    }else{
        Serial.print("Umidade do ar: ");
        Serial.print(umidadeAr);
        Serial.print(" %\t |");
        Serial.print(" Temperatura: ");
        Serial.print(temperatura);
        Serial.println(" °C");
    }
    
}
void loop() {
    server.handleClient();
    higroTimer();
    dhtTimer();

    //Teste de um led
    String ledtest = Firebase.getString("led");

    if(ledtest == "ligar"){
        digitalWrite(ONBOARD_LED, HIGH);
    } else {
        digitalWrite(ONBOARD_LED, LOW);
    }

    if(WiFi.status() == WL_CONNECTED) {
        // digitalWrite(ONBOARD_LED, LOW); // Desativa o LED
        Serial.println("Conectado à rede Wi-Fi");

        HTTPClient http;

        // Endereço do site para ping (por exemplo, o Google)
        String targetServer = "http://www.google.com";

        // Inicia a conexão diretamente com o servidor alvo
        WiFiClient wifiClient;
        http.begin(wifiClient, targetServer);

        // Adiciona o endereço do servidor
        http.addHeader("Content-Type", "text/plain");

        // Envia uma requisição GET para o servidor
        int httpCode = http.GET();

        // Verifica se a requisição foi bem-sucedida
        if (httpCode > 0) {
        Serial.printf("[HTTP] GET... código: %d\n", httpCode);

        // Lê a resposta do servidor
        String payload = http.getString();
        Serial.println(payload);
        } else {
        Serial.printf("[HTTP] GET... falha, código: %d\n", httpCode);
        }

        http.end();
        WiFi.softAPdisconnect(true);

        delay(5000); // Espera por 5 segundos antes de fazer o próximo ping
    } else {
        // digitalWrite(ONBOARD_LED, HIGH); // Ativa o LED
        Serial.println("Não conectado à rede Wi-Fi");
        delay(5000); // Espera por 5 segundos antes de tentar novamente
    }
}
