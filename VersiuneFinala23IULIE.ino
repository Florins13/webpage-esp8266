#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <DHT.h>

#define D0 16
#define D1 5 
#define D2 4 
#define D3 0
#define DHTTYPE   DHT11      
#define DHTPIN    D4
#define D5 14 
#define D6 12  
#define D7 13 
#define D8 15 
//#define D9 3 
//#define D10 1 

const char* ssid = "........"; 
const char* password = "........"; 

ESP8266WebServer server (80); // start the server on port 80
DHT dht(DHTPIN, DHTTYPE); // declare the temp pin and type with DHT library

float   t = 0; // -- Store the dates of temperature sensor
float   h = 0; // -- Store the dates of humidity sensor
String text = "";
String data;
String text1 = "";
String data1;
String text2 = "";
String data2;
String Argument_Name; // neimportant
String Colectaredate; // colectare date de la FORM-INPUT


int nivel = A0;
int stocarenivel = 0;

int leduri = D0;
int pompamare = D1;
int senzorPIR = D2;
int pompamica = D3;
int DOIsenzori = D5;
int incalzitor = D6;
int ventilator = D7;
int ledrosu = D8;
int ledverde = D9;


int stocareDOIsenzori = 0;  // stocare input doisenzori
int lastState=0; // stare doisenzori spate USA

int valoarePIR = 0; // stocare input PIR
int pirState = LOW; // stare senzor PIR

bool start = false; // false/true pompe
bool pornireINCALZITOR = false; // false/true sistem incalzire
bool pornireVENTILATOR = false; // false/true sistem ventilatie
bool ledState = LOW; // Stare iluminare/toggle

const char* www_username = "Sesiunecomunicari";
const char* www_password = "parolagrea";


const char *paginaLicenta = {
  "<!DOCTYPE html>"
  "<html>"
  "<head>"
  "<title>Proiect DIPLOMĂ</title>" 
  "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
  "<link rel=\"stylesheet\" href=\"https://www.w3schools.com/w3css/4/w3.css\">"
  "<link rel=\"stylesheet\" href=\"https://cdnjs.cloudflare.com/ajax/libs/font-awesome/4.7.0/css/font-awesome.min.css\">" // introdus acuma
  "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.3.1/jquery.min.js\"></script>"  
  "<script src=\"https://cdnjs.cloudflare.com/ajax/libs/popper.js/1.14.0/umd/popper.min.js\"></script>"
  
  
  "</head>"
  "<body>"
  
  "<style>"
  ".w3-button {width:25%;height:20%;margin:3px}"
  ".w3-panel {width:60%}" 
  ".w3-card-4 {width:400px}" 
  "p.solid {border-style: solid; width: 50px}"
  "body {"
  "background-image: url(\"https://www.tuv.com/media/romania/parteneri/Universitatea_Aurel_Vlaicu_Arad_TextwithImageflexible.jpg\");"
  "background-repeat: no-repeat;"
  "background-position:92% 23%;"
  "background-blend-mode: lighten;"
  "background-size: 170px 200px;"
  "}"
  "#toggleButton{width:40%}"
  "#randu1{display:inline-block;margin-right:10px;}" 
  "#randu2{display:inline-block;margin-right:10px;}"
  "#randu3{display:inline-block;margin-right:10px;}"
  "th{background-color:rgba(14, 85, 131,0.90);}"
  "</style>"

  "<header class=\"w3-container\" align=\"center\" style=\"height:61px;width:100%;float:right;background-color:rgba(14, 85, 131,0.93);\">"
  "<i id=\"randu1\" class=\"fa fa-cloud\" style=\"font-size:47px;color:lightblue;\"></i>"
  "<h2 id=\"randu2\" style=\"font-family:arial;color:White;text-align:center\">Sistem de control si supraveghere</h2>"
  "<i id=\"randu3\" class=\"fa fa-car\" style=\"font-size:40px;color:lightblue;\"></i>"
  "</header>"


"<div class=\"w3-cell-row\">"

  "<div class=\"w3-container w3-cell w3-quarter\" style=\"width:50%\">" 
    "<h3 style=\"font-family:arial;font-style: italic;\"><b>Control</b></h3>"
    
    "<h4>Sistem de irigare</h4>"
    "<div class=\"w3-bar\">"
    "<button type =\"button\" class=\"w3-button w3-green w3-ripple\" id=\"buttonON\">ON</button>"  
    "<button type =\"button\" class=\"w3-button w3-red w3-ripple\" id=\"buttonOFF\">OFF</button>"
    //"<button type =\"button\" class=\"w3-button\" style=\"background-color:gray;\" id="">AUTO OFF</button>"
    "</div>"
    "<br>"
    
    "<h4>Iluminare casa</h4>"
    "<div class=\"w3-bar\">"
    "<button type=\"button\" class=\"w3-button\" style=\"background-color:gray;\" id=\"toggleButton\">Led OFF</button>"       
    "</div>"
    "<br>"
    
    "<h4>Sistem incalzire</h4>"
    "<div class=\"w3-bar\">"
    "<button class=\"w3-button w3-ripple\" style=\"background-color:gray;\" id=\"butonincalzire1\">Pornire caldura</button>"  
    "<button class=\"w3-button w3-ripple\" style=\"background-color:gray;\" id=\"butonincalzire2\">Oprire caldura</button>"
    "</div>"
    "<br>"

    "<h4>Sistem ventilatie</h4>"
    "<div class=\"w3-bar\">"
    "<button class=\"w3-button w3-ripple\" style=\"background-color:gray;\" id=\"butonracire1\">Pornire Cooler</button>"  
    "<button class=\"w3-button w3-ripple\" style=\"background-color:gray;\" id=\"butonracire2\">Oprire Cooler</button>"
    "</div>"
    "<br>"
   
    "<br>"
    "<br>"
     
       "<div>"
          "<form id=\"setaretemp\" class=\"w3-container w3-card-4 w3-light-grey\" method=\"POST\" target=\"_self\" action="">" 
          "<p>"
          "<h5><b>Setare Temperatura: </b></h5>"
          "<p>"
          "<label>Valori maxime admise: 18&deg;C - 33&deg;C </label></p>" 
          "<input type=\"number\" min=\"18\" max=\"33\" class=\"w3-input w3-border w3-round w3-animate-input\" name=\"nume_utilizator\" style=\"width:22%;height:25%\">"     
          "<p id=\"paragrafinput\">Se asteapta introducerea unei valori...</p>" 
          "<button class=\"w3-btn w3-green\" style=\"width:22%;height:20%\">OK</button></p>" 
          "</form>"
          "<br>"
        "</div>"

  "</div>"
  
  "<div class=\"w3-container w3-cell\" style=\"width:50%\">"
  "<h3 style=\"font-family:arial;font-style: italic;\"><b>Monitorizare</b></h3>"
  "<table class=\"w3-table w3-striped w3-border\" style=\"width:50%\">"
    "<thead>"
    "<tr class=\"w3-blue\">"
      "<th>Senzor        &      Valoare</th>"
      "<th></th>" 
    "</tr>"
    "</thead>"    
    "<tr>"
      "<td id=\"data\">""</td>"
      "<td>""</td>"
    "</tr>"
    "<tr>"
      "<td id=\"data1\">""</td>"
      "<td>""</td>"
    "</tr>"
    "<tr>"
      "<td id=\"data2\">""</td>"
      "<td>""</td>"
    "</tr>"

 "</table>"
    "<br>"
    "<br>"
 
  "<h3 style=\"font-family:arial;font-style: italic;\"><b>Securitate</b></h3>"
       " <div class=\"w3-cell-row\">"        
          "<div class=\"w3-container w3-cell w3-quarter\" style=\"width:50%\">"
          "<h5>Nivel 1</h5>"
          "<img id=\"tare1\" src=\"https://media1.tenor.com/images/5968cc6bdc04ac9826db5c7a49b7208b/tenor.gif?itemid=5248772\" alt=\"Italian Trulli\" style=\"width:280px;height:222px;\">"
          "</div>"
                    
          "<div class=\"w3-container w3-cell\" style=\"width:50%\">"
          "<h5>Nivel 2</h5>"  
          "<img id=\"tare2\" src=\"https://media.giphy.com/media/Tb2i75AlI926I/giphy.gif\" alt=\"Steal\" style=\"width:300px;height:222px;\">"         
          "</div>"
                        
        "</div>"
    "</div>"
"</div>"

"<script>"
"$(document).ready(function(){"
    "$(\"#buttonON\").click(function(){"
        "$.get(\"/on\", function(){"
            "if(document.getElementById(\"buttonON\").innerHTML==\"ON\")"
            "{"
             "alert(\"Sistemul de irigatie a fost PORNIT cu succes!\");"
             "document.getElementById(\"buttonON\").innerHTML=\"ON\";"
            "}"
       " });"
    "});"
"});"

"$(document).ready(function(){"
    "$(\"#buttonOFF\").click(function(){"
        "$.get(\"/off\", function(){"
            "if(document.getElementById(\"buttonOFF\").innerHTML==\"OFF\")"
            "{"
            "alert(\"Sistemul de irigatie a fost OPRIT cu succes!\");"
            "document.getElementById(\"buttonOFF\").innerHTML=\"OFF\";"
            "}"
       " });"
    "});"
"});"




"var x = setInterval(function() {loadData(\"data.txt\", updateData)}, 2500);"
"var y = setInterval(function() {loadData1(\"data1.txt\", updateData1)}, 3250);"
"var z = setInterval(function() {loadData2(\"data2.txt\", updateData2)}, 5000);"
"function loadData(url, callback){" 
  "var xhttp = new XMLHttpRequest();"
  "xhttp.onreadystatechange = function(){"
    "if(this.readyState == 4 && this.status == 200){"
      "callback.apply(xhttp);"
     "}"
   "};"
   "xhttp.open(\"GET\", url, true);"
   "xhttp.send();"
"}"

"function loadData1(url, callback){" 
  "var xhttp = new XMLHttpRequest();"
  "xhttp.onreadystatechange = function(){"
    "if(this.readyState == 4 && this.status == 200){"
      "callback.apply(xhttp);"
     "}"
   "};"
   "xhttp.open(\"GET\", url, true);"
   "xhttp.send();"       
"}"

"function loadData2(url, callback){" 
  "var xhttp = new XMLHttpRequest();"
  "xhttp.onreadystatechange = function(){"
    "if(this.readyState == 4 && this.status == 200){"
      "callback.apply(xhttp);"
     "}"
   "};"
   "xhttp.open(\"GET\", url, true);"
   "xhttp.send();"    
"}"
  
"function updateData(){"
  "document.getElementById(\"data\").innerHTML = this.responseText;"
       "if(document.getElementById(\"data1\").innerHTML==\"1\")"
      "{"
      "document.getElementById(\"tare1\").src=\"https://media.tenor.com/images/1badf93589e8d6b5a520238d792b68ab/tenor.gif\";"
      "}"   
      "else {"     
      "document.getElementById(\"tare1\").src=\"https://cdn.dribbble.com/users/330915/screenshots/3112152/maison_1_anim.gif\";"
      "}" 
"}"
"function updateData1(){"
  "document.getElementById(\"data1\").innerHTML = this.responseText;"
  "if(document.getElementById(\"data2\").innerHTML==\"0\")"
      "{"
      "document.getElementById(\"tare2\").src=\"http://frontface.se/magazine/wp-content/uploads/2015/06/thug_stealing_money_w_a_ha.gif\";"
      "}"
"}"
"function updateData2(){"
  "document.getElementById(\"data2\").innerHTML = this.responseText;"
"}"

 
"$(document).ready(function(){"
    "$(\"#toggleButton\").click(function(){"
        "$.get(\"/toggle\", function(){"
            "if(document.getElementById(\"toggleButton\").innerHTML==\"Led OFF\")"
            "{"
            "document.getElementById(\"toggleButton\").innerHTML=\"Led ON\";"
            "document.getElementById(\"toggleButton\").style.backgroundColor=\"green\";"
            "}"
            "else"
            "{"
            "document.getElementById(\"toggleButton\").innerHTML=\"Led OFF\";"
            "document.getElementById(\"toggleButton\").style.backgroundColor=\"red\";"
            "}"
       " });"
    "});"
"});"

"$(document).ready(function(){"
    "$(\"#butonincalzire1\").click(function(){"
        "$.get(\"/incalzirePORNIT\", function(){"
            "if(document.getElementById(\"butonincalzire1\").innerHTML==\"Pornire caldura\")"
            "{"
            "document.getElementById(\"butonincalzire1\").innerHTML=\"Caldura pornita\";"
            "document.getElementById(\"butonincalzire1\").style.backgroundColor=\"green\";"
            "document.getElementById(\"butonincalzire2\").innerHTML=\"Oprire caldura\";"
            "document.getElementById(\"butonincalzire2\").style.backgroundColor=\"gray\";"
            "}"
            "else"
            "{"
            "document.getElementById(\"butonincalzire1\").innerHTML=\"Pornire caldura\";"
            "document.getElementById(\"butonincalzire1\").style.backgroundColor=\"gray\";"
            "}"
       " });"
    "});"
"});"

"$(document).ready(function(){"
    "$(\"#butonincalzire2\").click(function(){"
        "$.get(\"/incalzireOPRIT\", function(){"
            "if(document.getElementById(\"butonincalzire2\").innerHTML==\"Oprire caldura\")"
            "{"
            "document.getElementById(\"butonincalzire2\").innerHTML=\"Caldura oprita\";"
            "document.getElementById(\"butonincalzire2\").style.backgroundColor=\"red\";"
            "document.getElementById(\"butonincalzire1\").innerHTML=\"Pornire caldura\";"
            "document.getElementById(\"butonincalzire1\").style.backgroundColor=\"gray\";"
            "}"
            "else"
            "{"
            "document.getElementById(\"butonincalzire2\").innerHTML=\"Oprire caldura\";"
            "document.getElementById(\"butonincalzire2\").style.backgroundColor=\"gray\";"
            "}"
       " });"
    "});"
"});"

"$(document).ready(function(){"
    "$(\"#butonracire1\").click(function(){"
        "$.get(\"/ventilarePORNIT\", function(){"
            "if(document.getElementById(\"butonracire1\").innerHTML==\"Pornire Cooler\")"
            "{"
            "document.getElementById(\"butonracire1\").innerHTML=\"Cooler pornit\";"
            "document.getElementById(\"butonracire1\").style.backgroundColor=\"green\";"
            "document.getElementById(\"butonracire2\").innerHTML=\"Oprire Cooler\";"
            "document.getElementById(\"butonracire2\").style.backgroundColor=\"gray\";"
            "}"
            "else"
            "{"
            "document.getElementById(\"butonracire1\").innerHTML=\"Pornire Cooler\";"
            "document.getElementById(\"butonracire1\").style.backgroundColor=\"gray\";"
            "}"
       " });"
    "});"
"});"

"$(document).ready(function(){"
    "$(\"#butonracire2\").click(function(){"
        "$.get(\"/ventilareOPRIT\", function(){"
            "if(document.getElementById(\"butonracire2\").innerHTML==\"Oprire Cooler\")"
            "{"
            "document.getElementById(\"butonracire2\").innerHTML=\"Cooler Oprit\";"
            "document.getElementById(\"butonracire2\").style.backgroundColor=\"red\";"
            "document.getElementById(\"butonracire1\").innerHTML=\"Pornire Cooler\";"
            "document.getElementById(\"butonracire1\").style.backgroundColor=\"gray\";"
            "}"
            "else"
            "{"
            "document.getElementById(\"butonracire2\").innerHTML=\"Oprire Cooler\";"
            "document.getElementById(\"butonracire2\").style.backgroundColor=\"gray\";"
            "}"
       " });"
    "});"
"});"


"$(document).ready(function(){"
   "$(\"form\").submit(function(e){"
        "alert(\"Valoarea introdusa a fost acceptata!\");"
        "e.preventDefault();" //prevent the click event and stay on the same webpage
        "var formData = $(this).serialize();" //save the data of form introduced
      "$.post(\"/input\", formData).complete(function() {"
      "console.log(\"Succes!\");"
    "});"
  "});"
"});"

"$(document).ready(function(){"
    "var inpObj = document.getElementById(\"setaretemp\");"
    "$(document).on(\"submit\",\"#setaretemp\", function(){ "    
    "if (!inpObj.checkValidity()) {"
    "document.getElementById(\"paragrafinput\").innerHTML = inpObj.validationMessage;"
    "}"
    "else {"
    "document.getElementById(\"paragrafinput\").innerHTML = \"In curs de procesare...\";"
    "console.log(valoriinput)"
    "}"  
    "});"
"});"


"</script>"

"</body>"
"</html>" 
};

void setup() {
digitalWrite(pompamare, HIGH);
pinMode(nivel, INPUT);
pinMode(leduri, OUTPUT);
pinMode(pompamare, OUTPUT);
pinMode(pompamica, OUTPUT);
pinMode(DOIsenzori, INPUT_PULLUP);
pinMode(senzorPIR, INPUT);
pinMode(incalzitor, OUTPUT);
pinMode(ventilator, OUTPUT);
pinMode(ledrosu, OUTPUT);
pinMode(ledverde, OUTPUT);

delay(2000); // modificat la 2
Serial.begin(115200);
WiFi.begin("AndroidAP","11223344");

while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.println("Waiting to connect...");  
}

Serial.print("IP address: ");
Serial.println(WiFi.localIP());

server.on("/other", []() {   //Define the handling function for the path 
server.send(200, "text/plain", "Other URL");
  });

server.on("/",[](){
   if (!server.authenticate(www_username, www_password)) {
      return server.requestAuthentication();
   }
  server.send(200, "text/html", paginaLicenta);
});

server.on("/data.txt",[](){
  text = data;
  server.send(200, "text/html", text);
});
server.on("/data1.txt",[](){
  text1 = data1;
  server.send(200, "text/html", text1);
});
server.on("/data2.txt",[](){
  text2 = data2;
  server.send(200, "text/html", text2);
});


server.on("/toggle", toggle);
server.on("/on", turnOn);
server.on("/off", turnOff);
server.on("/input", inputdate);
server.on("/incalzirePORNIT", pornireREZISTENTA);
server.on("/incalzireOPRIT", oprireREZISTENTA);
server.on("/ventilarePORNIT", pornireCOOLER);
server.on("/ventilareOPRIT", oprireCOOLER);


server.begin();
Serial.println("server listening");
}


void loop() {
server.handleClient();
t = dht.readTemperature();
h = dht.readHumidity();

data ="Temperatura= <p class=\"solid\" id=\"temperatura\">" + (String)t + "</p> Umiditate= <p class=\"solid\" id=\"umiditate\">" + (String)h + "</p> Nivelapa= <p class=\"solid\" id=\"Senzornivel\">" + (String)stocarenivel + "</p>";
data1 = "" + (String)valoarePIR;
data2 = "" + (String)stocareDOIsenzori;

stocarenivel = analogRead(nivel);
valoarePIR = digitalRead(senzorPIR);
stocareDOIsenzori = digitalRead(DOIsenzori);


Serial.print("Senzor = ");
Serial.print(stocareDOIsenzori);
miscarePIR();
senzoriusa(); 
sistemAC();
sistemudareGAZON();
  
  
delay(150); 
}


void toggle() {
  ledState = !ledState;
  digitalWrite(leduri, ledState);
  server.send(200, "text/plain", "LED toggled");  
}
void turnOn() {
  start = true;
  server.send(200, "text/plain", "Pornire sistem pompe");
 // digitalWrite(led, HIGH);
}

void turnOff() {
  start = false;
  server.send(200, "text/plain", "Oprire sistem pompe!"); 
  digitalWrite(leduri, LOW); 
  digitalWrite(ledrosu, LOW); 
  digitalWrite(pompamare,1);
  digitalWrite(pompamica,0);
}

void startpompamare() {
  digitalWrite(pompamare, 0);
  digitalWrite(leduri, HIGH);
   
}

void startpompamica() {
  digitalWrite(pompamica, 1);
  digitalWrite(ledrosu, HIGH);
}

void sistemudareGAZON(){                                
    if (stocarenivel >= 500 && start == true){
    digitalWrite(pompamica, 0);
    digitalWrite(ledrosu, LOW);
    startpompamare();
  }
  /*
  if (stocarenivel <= 860 && start == true){
    // digitalWrite(pompamare, 1 );
    startpompamica();
    digitalWrite(ledrosu, HIGH);          
  }
  */
  
  if (stocarenivel <= 200 && start == true){
    digitalWrite(pompamare, 1 );
    digitalWrite(ledverde, LOW);
    startpompamica();          
  }
  
  if (stocarenivel <= 100 && start == true) {
    digitalWrite(pompamare, 1);
    digitalWrite(pompamica, 0);
    digitalWrite(ledrosu, LOW);  
    
  }
  
}







void pornireREZISTENTA(){
  pornireINCALZITOR = true;
  server.send(200, "text/plain", "Sistem incalzire pornit");  
}

void oprireREZISTENTA(){
  pornireINCALZITOR = false;
  digitalWrite(incalzitor, LOW);
  server.send(200, "text/plain", "Sistem incalzire oprit");
}

void pornireCOOLER(){
  pornireVENTILATOR = true;
  server.send(200, "text/plain", "Sistem racire pornit");
}
void oprireCOOLER(){
  pornireVENTILATOR = false;
  digitalWrite(ventilator, LOW);
  server.send(200, "text/plain", "Sistem racire oprit");
}

void sistemAC(){
  if (Colectaredate.toInt() < t && pornireINCALZITOR == true){
    digitalWrite(incalzitor, LOW);
  }
  if (Colectaredate.toInt() > t && pornireINCALZITOR == true){
    digitalWrite(incalzitor, HIGH);
  }
  if (Colectaredate.toInt() == t && pornireINCALZITOR == true){
    digitalWrite(incalzitor, LOW);
  }
    
  if (Colectaredate.toInt() > t && pornireVENTILATOR == true){
    digitalWrite(ventilator, LOW);
  }
  if (Colectaredate.toInt() < t && pornireVENTILATOR == true){
    digitalWrite(ventilator, HIGH);
  }
  if (Colectaredate.toInt() == t && pornireVENTILATOR == true){
    digitalWrite(ventilator, LOW);
  }
}





void inputdate(){  
    if( server.args() > 0 ){
    for ( uint8_t i = 0; i <server.args(); i++ ){
      Serial.print(server.argName(i));
      Argument_Name = server.argName(i);     
      if (server.argName(i) == "nume_utilizator"){
        Serial.print(" Datele primite sunt: ");
        Serial.println(server.arg(i));
        Colectaredate = server.arg(i);       
      }
    }
  }
  server.send(200, "text/plain", "Merge?");
}

void miscarePIR(){
if (valoarePIR == HIGH) {            // check if the input is HIGH
    digitalWrite(ledrosu, HIGH);  // turn LED ON  
    if (pirState == LOW) {
      // we have just turned on
      Serial.println("Miscare detectata!");
      // We only want to print on the output change, not state
      pirState = HIGH;
    }
  }  
  else {
    digitalWrite(ledrosu, LOW); // turn LED OFF  
    if (pirState == HIGH){
      // we have just turned of
      Serial.println("A plecat miscarea!");
      // We only want to print on the output change, not state
      pirState = LOW;
    }
  }   
}

void senzoriusa(){
 
  // check if the sensor beam is broken
  // if it is, the sensorState is LOW:
  if (stocareDOIsenzori == LOW) {     
    // turn LED on:
    digitalWrite(ledrosu, HIGH);  
  } 
  else {
    // turn LED off:
    digitalWrite(ledrosu, LOW); 
  }
  
  if (stocareDOIsenzori && !lastState) {
    Serial.println("Unbroken");
  } 
  if (!stocareDOIsenzori && lastState) {
    Serial.println("Broken");
  }
  lastState = stocareDOIsenzori;  
}







  

