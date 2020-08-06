/*
Control pileta V1.0
CLIENTE: Analia Conesa
COMIENZO DEL PROYECTO : 11/03/2020


Programa funcionando para controlar el encendido de un motor de filtrado
Cuenta con un reloj y una programacion por DIA, HORA INI, HORA FIN
FORMATO PARA ENVIAR LA PROGRAMACION (2,15,30,16,30)(dia,hora_ini,min_ini,hora_fin,min_fin)
Topico programacion: /placa_0100/programa1
/////////////////////////////PLACA3/////////////////////////////////////
-------------BORRADO DE EEPROM---------------
for (int i = 0 ; i < EEPROM.length() ; i++) {
   EEPROM.write(i, 0);
 }
------------------------------------------------
*/
#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <EEPROM.h>
#include <PubSubClient.h>
 #include <Wire.h>
 #include <TimeLib.h>
 #include <WiFiUdp.h>

 //---------------------//
#include <Wire.h>
#include "RTClib.h"
 RTC_DS1307 rtc;
 int n_dia;
 int r_dia;
 int n_mes;
 int t_mes;
 int n_anno;
 int d_anno;
 int t_siglo=6;
 int r_diaSemana;
 int hora;
 int minutos;

 int AA;
 int MM;
 int DD;
 byte HH;
 byte mm;
 byte ss;

 byte proteccion = 0;

 byte fecha_str = 0;
 byte minutos_p1_ini;
 byte minutos_p2_ini;
 byte minutos_p3_ini;
 byte minutos_p4_ini;
 byte minutos_p5_ini;
 byte minutos_todos_ini;
 byte minutos_p1_fin;
 byte minutos_p2_fin;
 byte minutos_p3_fin;
 byte minutos_p4_fin;
 byte minutos_p5_fin;
 byte minutos_todos_fin;
 byte hora_p1_ini;
 byte hora_p2_ini;
 byte hora_p3_ini;
 byte hora_p4_ini;
 byte hora_p5_ini;
 byte hora_todos_ini;
 byte hora_p1_fin;
 byte hora_p2_fin;
 byte hora_p3_fin;
 byte hora_p4_fin;
 byte hora_p5_fin;
 byte hora_todos_fin;
  String dia;
  String stprog;
  String stprog2;
  String stprog3;
  String stprog4;
  String stprog5;
  /*String dia_p1;
  String dia_p2;
  String dia_p3;
  String dia_p4;
  String dia_p5;
  String dia_todos;*/

 int dia_p1;
 int dia_p2;
 int dia_p3;
 int dia_p4;
 int dia_p5;
 int dia_todos;
String dia_prog1;
String dia_prog2;
String dia_prog3;
String dia_prog4;
String dia_prog5;
String dia_prog_todos;
String programa;


  //---------------------//
  String enc_p1 = "no";
  String enc_p22 = "no";
  String enc_p3 = "no";
  String enc_p4 = "no";
  String enc_p5 = "no";
  String enc_todos = "no";
  String str_p1 ;
  String str_p1_leido;
  String str_p2 ;
  String str_p2_leido;
  String str_p3 ;
  String str_p3_leido;
  String str_p4 ;
  String str_p4_leido;
  String str_p5 ;
  String str_p5_leido;
  String str_todos ;
  String str_todos_leido;
  const char separator = ',';
  const int dataLength = 6;
  int data[dataLength];
  int data_2[dataLength];
  int data_3[dataLength];
  int data_4[dataLength];
  int data_5[dataLength];
  int data_6[dataLength];
  int prg_p1;
  int prg_p2;
  int prg_p3;
  int prg_p4;
  int prg_p5;

  byte ee=0;


  //---------------------//
 // NTP Servers:

 IPAddress timeServer(132, 163, 4, 101);
 const int timeZone = -3;
 WiFiUDP Udp;
 unsigned int localPort = 8888;  // local port to listen for UDP packets
 time_t prevDisplay = 0; // when the digital clock was displayed
 /*/-------------------------------sensor 18b20-------------------------//
 #include <OneWire.h>
 #include <DallasTemperature.h>
 #define ONE_WIRE_BUS 12
 OneWire oneWire(ONE_WIRE_BUS);
 DallasTemperature sensor (&oneWire);
 float temperatura;
//--------------------------------------------------------------------//

*/
//#define luz 4 //D1
#define OUT_1 D7
#define PULSADOR D5   //MANUAL

byte val;
byte valor_Pulsador;
byte valEE;
byte valpul=0;
byte valpul_ultimo=0;
byte valpul_p=0;
byte valpul_ultimo_p=0;
byte valpul_2=0;
byte val_apagado=0;
byte val2EE;
int contconexion = 0;
int str_debug =1;
byte programa1_nuevo=0;
byte programa2_nuevo=0;
byte programa3_nuevo=0;
byte programa4_nuevo=0;
byte programa5_nuevo=0;
byte programatodos_nuevo=0;
byte falla = 1;
//////////////////////////////////////////////////////////////////////////////
long lastMsg = 0;
char msg[50];                              // es un char para el mensaje
int value =0;
String str_manual;
String str_automatico;
char charPulsador [15];
String strmotor;
char valueStrmotor[15];
String stRreloj;
char valueStrreloj[15];
char valuestrprog[15];
byte strPulsador;
String strPulsadorUltimo;
char valueStr[15];
String strluz = "";
String strluzUltimo;
unsigned long previousMillis = 0;
unsigned long previousMillis2 = 0;
unsigned long previousMillis3 = 0;
long diferencia_disparo;
long tiempo_anterior_disparo;

char SALIDADIGITAL[50];
char PLACA[50];
char NOMBRE[50];
char MOTOR[50];
char TEMPERATURA[50];
char DEBUG[50];
char ACTUALIZACION[50];
char RESET[50];
char AUTOMATICO[50];
char MANUAL[50];
char PROGRAMA1[50];
char PROGRAMA2[50];
char PROGRAMA3[50];
char RELOJ[50];
char PROGRAMACION[20];
char INFORMACION[20];
char TODOS[20];

const char *mqtt_server = "serviciosiot.com.ar";
const int mqtt_port = 1883;

String USERNAME = "placa_0100";
char   PASSWORD[50] = "12345678";
String NAME = "pileta";
String ARCHIVO = "pileta_web_11062020.ino";
String VER_ARCHIVO = "Ver.:1.11062020";

WiFiClient espClient;
PubSubClient client(espClient);

//-------------------------FUNCIONES-------------------------------//
void(* resetFunc) (void) = 0; //declare reset function @ address 0
void enciende_luz();
void apaga_luz();
void enciende_luz_2();
void apaga_luz_2();
void wifi_conf();
void intento_conexion();
void reconnect();
void digitalClockDisplay();
int dia_de_semana();
void encendido();
void encendido_prog();
void apagado();
void estado_motor();
void estado_programacion();
void programa_1();
void programa_2();
void programa_3();
void programa_todos();
void programa1_guardado();
void programa2_guardado();
void programa3_guardado();
void web();
void topicos();
void printDigits(int digits);
void reloj();


//----------------------CALLBACK------------------------------//
void callback(char* topic, byte* payload, unsigned int lenght){

  char PAYLOAD[50] = "    ";

  Serial.print("Mensaje recibido bajo el topico-> ");
  Serial.println(topic);
  Serial.print("\n");                                              //deja un espacio en blanco

  for (int i = 0; i < lenght; i++) {
    PAYLOAD[i] = (char)payload[i];
   }
   Serial.print("Mensaje : ");
  Serial.println(PAYLOAD);
   Serial.print("\n");

    if (String(topic) ==  PROGRAMA1) {
      str_p1 = PAYLOAD;
      Serial.println("(str_p1)=>"+ String(str_p1)+"<--");
      str_p1.trim();
      Serial.println("(str_p1)=>"+ String(str_p1)+"<--");
      graba(90, str_p1);
      programa1_nuevo=1; //variable control
      Serial.println("");
      Serial.print("Mensaje recibido programa1-> ");
      Serial.println(str_p1);
      Serial.print("");
           }

    if (String(topic) ==  PROGRAMA2) {
       str_p2 = PAYLOAD;
       graba(120, str_p2);
       programa2_nuevo=1; //variable control
       Serial.println("");
       Serial.print("Mensaje recibido programa2-> ");
       Serial.println(str_p2);
       Serial.print("");
     }

     if (String(topic) ==  PROGRAMA3) {
        str_p3 = PAYLOAD;
        graba(150, str_p3);
        programa3_nuevo=1; //variable control
        Serial.println("");
        Serial.print("Mensaje recibido programa3-> ");
        Serial.println(str_p3);
        Serial.print("");
      }
/*
      if (String(topic) ==  PROGRAMA4) {
         str_p4 = PAYLOAD;
         graba(87, str_p4);
         programa4_nuevo=1; //variable control
         Serial.println("");
         Serial.print("Mensaje recibido programa4-> ");
             Serial.println(str_p4);
         Serial.print("");
       }


        if (String(topic) ==  PROGRAMA5) {
           str_p5 = PAYLOAD;
           graba(102, str_p5);
           programa5_nuevo=1; //variable control
           Serial.println("");
           Serial.print("Mensaje recibido programa5-> ");
           Serial.println(str_p5);
           Serial.print("");
         }
         */
         if (String(topic) ==  TODOS) {
            str_todos = PAYLOAD;
            graba(180, str_todos);
            programatodos_nuevo=1; //variable control
            Serial.println("");
            Serial.print("Mensaje recibido programa todos-> ");
            Serial.println(str_todos);
            Serial.print("");
          }


      if (String(topic) ==  INFORMACION) {
        Serial.println("");
        Serial.print("Mensaje recibido bajo el topico->");
        Serial.println(topic);
        Serial.print("\n");
       for (int i = 0; i < lenght; i++) {
         PAYLOAD[i] = (char)payload[i];
       }        //programa1
           if (PAYLOAD[8] == '1'){   // debug si
              programa1_guardado();
              stprog = String(dia_prog1) + " " + hora_p1_ini + ":" + minutos_p1_ini + " " + hora_p1_fin + ":" + minutos_p1_fin;
              stprog.toCharArray(valuestrprog, 50);
              client.publish(PROGRAMACION, valuestrprog);
            }
            if (PAYLOAD[8] == '2'){   // debug si
              programa2_guardado();
              stprog = String(dia_prog2) + " " + hora_p2_ini + ":" + minutos_p2_ini + " " + hora_p2_fin + ":" + minutos_p2_fin;
              stprog.toCharArray(valuestrprog, 50);
              client.publish(PROGRAMACION, valuestrprog);
             }
             if (PAYLOAD[8] == '3'){   // debug si
               programa3_guardado();
               stprog = String(dia_prog3) + " " + hora_p3_ini + ":" + minutos_p3_ini + " " + hora_p3_fin + ":" + minutos_p3_fin;
               stprog.toCharArray(valuestrprog, 50);
               client.publish(PROGRAMACION, valuestrprog);
              }
              /*if (PAYLOAD[8] == '4'){   // debug si
                programa4_guardado();
                stprog = String(dia_prog4) + " " + hora_p4_ini + ":" + minutos_p4_ini + " " + hora_p4_fin + ":" + minutos_p4_fin;
                stprog.toCharArray(valuestrprog, 50);
                client.publish(PROGRAMACION, valuestrprog);
               }
               if (PAYLOAD[8] == '5'){   // debug si
                 programa5_guardado();
                 stprog = String(dia_prog5) + " " + hora_p5_ini + ":" + minutos_p5_ini + " " + hora_p5_fin + ":" + minutos_p5_fin;
                 stprog.toCharArray(valuestrprog, 50);
                 client.publish(PROGRAMACION, valuestrprog);
               }*/
                if (PAYLOAD[8] == 't'){   // debug si
                  programa_todos_guardado();
                  stprog = String(dia_todos)+ " " + hora_todos_ini + ":" + minutos_todos_ini + " " + hora_todos_fin + ":" + minutos_todos_fin;
                  stprog.toCharArray(valuestrprog, 50);
                  client.publish(PROGRAMACION, valuestrprog);
                }
       }


    if (String(topic) ==  MANUAL) {
      Serial.println("");
      Serial.print("Mensaje recibido bajo el topico->");
      Serial.println(topic);
      Serial.print("\n");
     for (int i = 0; i < lenght; i++) {
       PAYLOAD[i] = (char)payload[i];
       }
         if (PAYLOAD[0] == 'S'){   // debug si
           encendido();
           client.publish(MOTOR,"Modo Manual");
          }
          if (PAYLOAD[0] == 'N'){   // debug si
            proteccion=1;
            apagado();
           }
     }
   if (String(topic) ==  DEBUG) {
     Serial.println("");
     Serial.print("Mensaje recibido bajo el topico->");
     Serial.println(topic);
     Serial.print("\n");
    for (int i = 0; i < lenght; i++) {
      PAYLOAD[i] = (char)payload[i];
      }
        if (PAYLOAD[0] == 'S'){   // debug si
          str_debug= 1;
          Serial.println("str_debug=> " +String(str_debug));

         }
         if (PAYLOAD[0] == 'N'){   // debug si
           str_debug= 0;
           Serial.println("str_debug=> " +String(str_debug));
          }
          if (PAYLOAD[0] == 'O'){   // debug todos los programas apagados

           }
          if (PAYLOAD[0] == 'n'){   // debug ntc
            //------------------=NtpTime-------------------//
             Serial.print("IP number assigned by DHCP is ");
             Serial.println(WiFi.localIP());
             Serial.println("Starting UDP");
             Udp.begin(localPort);
             Serial.print("Local port: ");
             Serial.println(Udp.localPort());
             Serial.println("waiting for sync");
             setSyncProvider(getNtpTime);

           }
           if (PAYLOAD[0] == 'r'){   // debug Rtc
              AA=year();
              MM=month();
              DD=day();
              HH=hour();
              mm=minute();
              ss=second();
             digitalClockDisplay();
             Serial.println("------------- ");
            rtc.adjust(DateTime(AA, MM, DD, HH, mm,  ss));
            digitalClockDisplay2();
            Serial.println(" ");
            }
    }
/*

*/

    if (String(topic) == RESET) {
      Serial.println("");
      Serial.print("Mensaje recibido bajo el topico->");
      Serial.println(topic);
      Serial.print("\n");
     for (int i = 0; i < lenght; i++) {
       PAYLOAD[i] = (char)payload[i];
       }
       if (PAYLOAD[0] == 'S'){   // debug si

         resetFunc();  //call reset
          }
          if (PAYLOAD[0] == 'E'){   // debug si

            ee=1;  //Borrado de memoria
             }

         }



}

//------------------------------------------------------------//

//----------------------PAGINA WEB---------------------///
String pral = "<html>"
"<meta http-equiv='Content-Type' content='text/html; charset=utf-8'/>"
"<title>WIFI CONFIG by Fer@Fra</title> <style type='text/css'> body,td,th { color: #036; } body { background-color: #bbbd13; } </style> </head>"
"<body> "
"<h1>WIFI CONF</h1><br>"
"<form action='config' method='get' target='pantalla'>"
"<fieldset align='left' style='border-style:solid; border-color:#336666; width:400px; height:180px; padding:10px; margin: 5px;'>"
"<legend><strong>Configurar WI-FI</strong></legend>"
"Programar (si / no):  <input name='prog' type='text' size='15'/> <br><br>"
"SSID:  <input name='ssid' type='text' size='15'/> <br><br>"
"PASSWORD:  <input name='pass' type='password' size='15'/> <br><br>"
"<input type='submit' value='Comprobar conexion' /><br><br>"
"</fieldset>"
"<fieldset align='left' style='border-style:solid; border-color:#336666; width:400px; height:120px; padding:10px; margin: 5px;'>"
"<legend><strong>Config PROGRAMA 1</strong></legend>"
"PROGRAMA1:  <input name='prog1' type='text' size='15'/> <br><br>"
"<input type='submit' value='Guardar configuracion' />"
"</fieldset><br><br>"
"<fieldset align='left' style='border-style:solid; border-color:#336666; width:400px; height:100px; padding:10px; margin: 5px;'>"
"<legend><strong>MANUAL / AUTOMATICO</strong></legend>"
"MANUAL (si) AUTOMATICO (no):  <input name='auto' type='text' size='15'/> <br><br>"
"<input type='submit' value='Activar' />"
"</fieldset>"
"</form>"
"<iframe id='pantalla' name='pantalla' src='' width=900px height=400px frameborder='0' scrolling='no'></iframe>"
"</body>"
"</html>";

String pral2= "<html>"
"<meta http-equiv='Content-Type' content='text/html; charset=utf-8'/>"
"<title>WIFI CONFIG by Fer@Fra</title> <style type='text/css'> body,td,th { color: #036; } body { background-color: gren; } </style> </head>"
"<body>"
"<h1>WIFI PRUEBA</h1><br>"
"<form action='config' method='get' target='pantalla'>"
"<fieldset align='left' style='border-style:solid; border-color:#336666; width:200px; height:165px; padding:10px; margin: 5px;'>"
"<legend><strong>Configurar WI-FI</strong></legend>"
"Programar (si / no): <br> <input name='prog' type='text' size='15'/> <br><br>"
"SSID: <br> <input name='ssid' type='text' size='15'/> <br><br>"
"PASSWORD: <br> <input name='pass' type='password' size='15'/> <br><br>"
"<input type='submit' value='Comprobar conexion' /><br><br><br>"
"</fieldset><br><br><br>"
"<fieldset align='left' style='border-style:solid; border-color:#336666; width:200px; height:120px; padding:10px; margin: 5px;'>"
"<legend><strong>Config PROGRAMA 1</strong></legend>"
"PROGRAMA1: <br> <input name='prog1' type='text' size='15'/> <br><br>"
"<input type='submit' value='Guardar configuracion' />"
"</fieldset><br><br>"
"<fieldset align='left' style='border-style:solid; border-color:#336666; width:200px; height:140px; padding:10px; margin: 5px;'>"
"<legend><strong>MANUAL / AUTOMATICO</strong></legend>"
"MANUAL (si) AUTOMATICO (no): <br> <input name='auto' type='text' size='15'/> <br><br>"
"<input type='submit' value='Activar' />"
"</fieldset>"
"</form>"
"<iframe id='pantalla' name='pantalla' src='' width=900px height=400px frameborder='0' scrolling='no'></iframe>"
"</body>"
"</html>";
//-------------------//------------------------------------------------//
ESP8266WebServer server(80);

char ssid[20];
char pass[20];
String ssid_leido;
String pass_leido;
int ssid_tamano = 0;
int pass_tamano = 0;

//------------------------------------------------------------------//
void web(){
String progweb = server.arg("prog1"); //Recibimos los valores que envia por GET el formulario web
  Serial.println(progweb);     //para depuracion

}

void wifi_conf() {
String prog = server.arg("prog"); //Recibimos los valores que envia por GET el formulario web
if (prog=="si") {                  //bloquea el cambio de datos de wifi
  String progweb = server.arg("prog1"); //Recibimos los valores que envia por GET el formulario web
    Serial.println("configuracion recibida via Web: "+String(progweb));     //para depuracion
    str_p1 = progweb;
    programa1_guardado();
      }

if (prog=="auto") {
  String str_auto = server.arg("auto"); //Recibimos los valores que envia por GET el formulario web
    Serial.println("Activacion Manual: "+ String(str_auto));     //para depuracion
    if (str_auto=="si") {
      encendido();
      client.publish(MOTOR,"Modo Manual");
     }
     if (str_auto=="no") {
       apagado();
       client.publish(MOTOR,"Modo Manual");
      }
  }
if (prog=="no") {
 int cuenta = 0;

 String getssid = server.arg("ssid"); //Recibimos los valores que envia por GET el formulario web
 String getpass = server.arg("pass");

 getssid = arregla_simbolos(getssid); //Reemplazamos los simbolos que aparecen cun UTF8 por el simbolo correcto
 getpass = arregla_simbolos(getpass);

 ssid_tamano = getssid.length() + 1;  //Calculamos la cantidad de caracteres que tiene el ssid y la clave
 pass_tamano = getpass.length() + 1;

 getssid.toCharArray(ssid, ssid_tamano); //Transformamos el string en un char array ya que es lo que nos pide WIFI.begin()
 getpass.toCharArray(pass, pass_tamano);

 Serial.println(ssid);     //para depuracion
 Serial.println(pass);

 WiFi.begin(ssid, pass);     //Intentamos conectar
 while (WiFi.status() != WL_CONNECTED)
 {
  delay(500);
  Serial.print(".");
  cuenta++;
  if (cuenta > 20) {
   graba(50, "noconfigurado");
   server.send(200, "text/html", String("<h2>No se pudo realizar la conexion<br>no se guardaron los datos.</h2>"));
   digitalWrite(BUILTIN_LED, LOW);
   return;

  }
 }
 Serial.println(WiFi.localIP());
 graba(50, "configurado");
 graba(1, getssid);
 graba(30, getpass);
 server.send(200, "text/html", String("<h2>Conexion exitosa a: "
  + getssid + "<br> El pass ingresado es: " + getpass + "<br>Datos correctamente guardados."));
digitalWrite(BUILTIN_LED, LOW);
}
}

 //*********  INTENTO DE CONEXION   *********************
 void intento_conexion() {
  if (lee(50).equals("configurado")) {
   ssid_leido = lee(1);      //leemos ssid y password
   pass_leido = lee(30);

   Serial.println(ssid_leido);  //Para depuracion
   Serial.println(pass_leido);

   ssid_tamano = ssid_leido.length() + 1;  //Calculamos la cantidad de caracteres que tiene el ssid y la clave
   pass_tamano = pass_leido.length() + 1;

   ssid_leido.toCharArray(ssid, ssid_tamano); //Transf. el String en un char array ya que es lo que nos pide WiFi.begin()
   pass_leido.toCharArray(pass, pass_tamano);

   int cuenta = 0;
   WiFi.begin(ssid, pass);      //Intentamos conectar
   while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    cuenta++;
    if (cuenta > 20) {
     Serial.println("Fallo al conectar");
     return;
    }
   }
  }
  if (WiFi.status() == WL_CONNECTED) {
   Serial.print("Conexion exitosa a: ");
   Serial.println(ssid);
   Serial.println(WiFi.localIP());
  }
 }
//FUNCION PARA RECONECTAR AL SERVIDOR
 void reconnect(){
   if (!client.connected()) {
     Serial.println("Intentando Conexion MQTT");

     String clientId = "pile_iot_1_";                                        //SOLUCION DE DAR DE BAJA LA ANTERIOR CONEXION
     clientId = clientId + String(random(0xffff), HEX);                //

     USERNAME.toCharArray(PLACA, 50);
     if (client.connect("", PLACA, PASSWORD)) {
     //if (client.connect(clientId.c_str(),mqtt_user,mqtt_pass)) {      // DATOS PARA LA CONEXION
       Serial.println("Conexion MQTT exitosa!!");
       String str_NAME = NAME + USERNAME;
       str_NAME.toCharArray(NOMBRE, 50);
       client.publish("alertas",NOMBRE);
       client.subscribe(ACTUALIZACION);
       client.subscribe(MANUAL);
       client.subscribe(AUTOMATICO);
       client.subscribe(DEBUG);
       client.subscribe(RESET);
       client.subscribe(PROGRAMA1);
       client.subscribe(PROGRAMA2);
       client.subscribe(PROGRAMA3);
       client.subscribe(INFORMACION);
       client.subscribe(TODOS);


       Serial.println("subscribe to /"+USERNAME+"/debug");
       Serial.println("subscribe to /"+USERNAME+"/actualizacion");
       Serial.println("subscribe to actualizar");
       Serial.println("subscribe to /"+USERNAME+"/reset");
       Serial.println("");

       //------------------=NtpTime-------------------//
        Serial.print("IP number assigned by DHCP is ");
        Serial.println(WiFi.localIP());
        Serial.println("Starting UDP");
        Udp.begin(localPort);
        Serial.print("Local port: ");
        Serial.println(Udp.localPort());
        Serial.println("waiting for sync");
        setSyncProvider(getNtpTime);
        //-------------------------------------------//


     }
     else{
        Serial.println("Fallo la  Conexion ");
        Serial.println(client.state());                                      // QUIERO SABER EL ESTADO DEL CLIENTE CON CODIGO DE ERROR
        Serial.println("Se intentara de nuevo en 5 segundos");


     }
   }
 }



 //////-----------------------------------------------------------------//////

 String arregla_simbolos(String a) {
  a.replace("%C3%A1", "á");
  a.replace("%C3%A9", "é");
  a.replace("%C3%A", "i");
  a.replace("%C3%B3", "ó");
  a.replace("%C3%BA", "ú");
  a.replace("%21", "!");
  a.replace("%23", "#");
  a.replace("%24", "$");
  a.replace("%25", "%");
  a.replace("%26", "&");
  a.replace("%27", "/");
  a.replace("%28", "(");
  a.replace("%29", ")");
  a.replace("%3D", "=");
  a.replace("%3F", "?");
  a.replace("%27", "'");
  a.replace("%C2%BF", "¿");
  a.replace("%C2%A1", "¡");
  a.replace("%C3%B1", "ñ");
  a.replace("%C3%91", "Ñ");
  a.replace("+", " ");
  a.replace("%2B", "+");
  a.replace("%22", "\"");
  return a;
 }

 //*******  G R A B A R  EN LA  E E P R O M  ***********
 void graba(int addr, String a) {
  int tamano = (a.length() + 1);
  Serial.print(tamano);
  char inchar[30];    //'30' Tamaño maximo del string
  a.toCharArray(inchar, tamano);
  EEPROM.write(addr, tamano);
  for (int i = 0; i < tamano; i++) {
   addr++;
   EEPROM.write(addr, inchar[i]);
  }
  EEPROM.commit();
 }

 //*******  L E E R   EN LA  E E P R O M    **************
 String lee(int addr) {
  String nuevoString;
  int valor;
  int tamano = EEPROM.read(addr);
  for (int i = 0;i < tamano; i++) {
   addr++;
   valor = EEPROM.read(addr);
   nuevoString += (char)valor;
  }
  return nuevoString;
 }

 void topicos(){
   String programacion = "/" + USERNAME + "/" + "programacion";
   programacion.toCharArray(PROGRAMACION, 50);
   String reloj = "/" + USERNAME + "/" + "reloj";
   reloj.toCharArray(RELOJ, 50);
   //------------programas------------------------//
   String programa1 = "/" + USERNAME + "/" + "programa1";
   programa1.toCharArray(PROGRAMA1, 50);
   String programa2 = "/" + USERNAME + "/" + "programa2";
   programa2.toCharArray(PROGRAMA2, 50);
   String programa3 = "/" + USERNAME + "/" + "programa3";
   programa3.toCharArray(PROGRAMA3, 50);
   /*String programa4 = "/" + USERNAME + "/" + "programa4";
   programa4.toCharArray(PROGRAMA4, 50);
   String programa5 = "/" + USERNAME + "/" + "programa5";
   programa5.toCharArray(PROGRAMA5, 50);
   */String todos = "/" + USERNAME + "/" + "todos";
   todos.toCharArray(TODOS, 50);

   String informacion = "/" + USERNAME + "/" + "informacion";
   informacion.toCharArray(INFORMACION, 50);
  String temperatura = "/" + USERNAME + "/" + "temperatura";
  temperatura.toCharArray(TEMPERATURA, 50);
  String salidaDigital = "/" + USERNAME + "/" + "salida_digital";
  salidaDigital.toCharArray(SALIDADIGITAL, 50);
  String Automatico = "/" + USERNAME + "/" + "Automatico";
  Automatico.toCharArray(AUTOMATICO, 50);
  String manual = "/" + USERNAME + "/" + "manual";
  manual.toCharArray(MANUAL, 50);
  String motor = "/" + USERNAME + "/" + "motor";
  motor.toCharArray(MOTOR, 50);
  String actualizacion = "/" + USERNAME + "/" + "actualizacion";
  actualizacion.toCharArray(ACTUALIZACION, 50);
  String debug = "/" + USERNAME + "/" + "debug";
  debug.toCharArray(DEBUG, 50);
  String reset = "/" + USERNAME + "/" + "reset";
  reset.toCharArray(RESET, 50);
 }

//------------------------------------------------------------------//
//*****  S E T U P  **************
void setup() {
 Serial.begin(115200);
 EEPROM.begin(4096);

 pinMode(BUILTIN_LED, OUTPUT);
 pinMode(OUT_1, OUTPUT);
 pinMode(PULSADOR, INPUT_PULLUP);

 rtc.begin();

 // following line sets the RTC to the date & time this sketch was compiled
     rtc.adjust(DateTime(__DATE__, __TIME__));


if (!rtc.begin()) {
      Serial.println(F("Couldn't find RTC"));
      while (1);
   }
 //attachInterrupt(digitalPinToInterrupt(pulsador),salida_1,RISING);
 WiFi.softAP("PILETA");      //Nombre que se mostrara en las redes wifi

 server.on("/", []() {
    server.send(200, "text/html", pral);
  });
 server.on("/config", wifi_conf);
 //server.on("/config", web);
 server.begin();
 Serial.println("Webserver iniciado...");
 intento_conexion();
 client.setServer(mqtt_server, mqtt_port);
 client.setCallback(callback);


 topicos(); // generacion de topicos

 //----------------- PROGRAMACION GUARDADA-----------------------//
 programa1_guardado();
 programa2_guardado();
 programa3_guardado();
 //programa4_guardado();
 //programa5_guardado();
 programa_todos_guardado();

enc_p1 =    lee(210);    // estado de encendido
enc_p22 =   lee(220);    // estado de encendido
enc_p3 =    lee(230);    // estado de encendido
enc_todos = lee(240);    // estado de encendido

//apagado();



/*enc_p4 = lee(133);    // estado de encendido
enc_p5 = lee(137);    // estado de encendido
*/


/*enc_p1 = lee(121);    // estado de encendido
enc_p22 = lee(125);    // estado de encendido
enc_p3 = lee(129);    // estado de encendido
enc_p4 = lee(133);    // estado de encendido
enc_p5 = lee(137);    // estado de encendido
enc_todos = lee(161);    // estado de encendido
 */


/////--------------------------------------------------------------///////
                 //*****  TOPIC  **************
Serial.println("****ESTADO PLACA****");
Serial.println("");
Serial.println("****TEMPERATURA AJUSTE****");
Serial.println("");
Serial.println("****TOPIC CREADOS****");
Serial.println("/"+USERNAME + "/salida_digital");
Serial.println("/"+USERNAME + "/temperatura");
Serial.println("/"+USERNAME + "/motor");
Serial.println("/"+USERNAME + "/automatico");
Serial.println("/"+USERNAME + "/manual");
Serial.println("/"+USERNAME + "/actualizacion");
Serial.println("/"+USERNAME + "/debug");
Serial.println("/"+USERNAME + "/reset");
Serial.println("");
Serial.println("Archivo: " + String(ARCHIVO));
Serial.println("****SERVIDOR****");
Serial.println("iotargentina.ga");
Serial.println("");
Serial.println("****Ver.-***");
Serial.println(String (VER_ARCHIVO));
Serial.println("");
Serial.println("Conexion Conesa a: PB3 2.4Ghz");
Serial.println("El pass : 0142556741");

}




//*****   L O O P   **************
void loop() {
server.handleClient();
//Serial.println("n_dia: "+String(n_dia));
//Serial.println("AA: "+String(AA));

  delay(2000);
  client.loop();
if (client.connected()==false){
  reconnect();
   }
   Serial.println("fecha_str=> " +String(fecha_str));


   DateTime fecha = rtc.now(); //fecha y hora del RTC
   AA=year();
   if (fecha.month()==165 || fecha.year()>2100){
     fecha_str=1;
   }else{
     fecha_str=0;
    }

   if (fecha_str==0) {

     reloj();
     dia_de_semana();
     }


  //-------------BORRADO DE EEPROM---------------
if (ee==1) {

  for (int i = 0 ; i < EEPROM.length() ; i++) {
     EEPROM.write(i, 0);
   }
   Serial.println(F("EEPROM borrada!!!"));
   ee=0;

 }
//  ------------------------------------------------

if (programa1_nuevo==1) {
   enc_p1 = "no";
   graba(210, enc_p1);
    programa1_nuevo=0;
    programa1_guardado();
  }

if (programa2_nuevo==1) {
  enc_p22 = "no";
  graba(220, enc_p22);
    programa2_nuevo=0;
    programa2_guardado();
   }
if (programa3_nuevo==1) {
  enc_p3 = "no";
  graba(230, enc_p3);
    programa3_nuevo=0;
    programa3_guardado();
      }

if (programatodos_nuevo==1) {
      enc_todos = "no";
      graba(240, enc_todos);
        programatodos_nuevo=0;
        programa_todos_guardado();
        }
//-----------MEMORIAS------------//
str_p1 = lee(90);
str_p2 = lee(120);
str_p3 = lee(150);
str_todos=lee(180);
enc_p1 = lee(210);
enc_p22 = lee(220);
enc_p3 = lee(230);
enc_todos = lee(240);
//--------------------------------//

  programa_1();
  programa_2();
  programa_3();
  programa_todos();

 //if (digitalRead(PULSADOR) == 0 & proteccion==0) {  //TECLA
  if (digitalRead(PULSADOR) == 0 ) {  //TECLA
         strPulsador = 1; //variable control MANUAL
         client.publish(MOTOR,"Modo Manual ACTIVO!");
         valpul=1;
         programa="manual";
         encendido();

       }

  if (digitalRead(PULSADOR) == 1 & valpul_2==1) {  //TECLA AUTOMATICO
    strPulsador = 0; //variable control MANUAL
    valpul_2=0;
    proteccion=1;
    apagado();
    }


if (fecha_str==0) {
 unsigned long currentMillis = millis();
 if (currentMillis - previousMillis >= 10000) { //envia la temperatura cada 10 segundos
   previousMillis = currentMillis;

         DateTime fecha = rtc.now(); //fecha y hora del RTC
         n_anno=(fecha.year()-2000);
         n_dia=fecha.day();
         n_mes=fecha.month();
         hora=fecha.hour();
         minutos=fecha.minute();
         stRreloj = String(n_dia) + "/" + n_mes + "/" + n_anno + " " + hora + ":" + minutos;
         stRreloj.toCharArray(valueStrreloj, 50);
         client.publish(RELOJ, valueStrreloj);



         //digitalClockDisplay();
         //----------------------DEBUG----------------------------//
         if (str_debug==1) {
         

         Serial.println("Falla:-> "+String(falla));
         stprog = String(dia_prog1) + " " + hora_p1_ini + ":" + minutos_p1_ini + " " + hora_p1_fin + ":" + minutos_p1_fin;
         Serial.println("Mensaje grabado en str_p1:-> "+String(stprog));
         stprog2 = String(dia_prog2) + " " + hora_p2_ini + ":" + minutos_p2_ini + " " + hora_p2_fin + ":" + minutos_p2_fin;
         Serial.println("Mensaje grabado en str_p2:-> "+String(stprog2));

         ssid_tamano = ssid_leido.length() + 1;
         stprog3 = String(dia_prog3) + " " + hora_p3_ini + ":" + minutos_p3_ini + " " + hora_p3_fin + ":" + minutos_p3_fin;
         Serial.println("Mensaje grabado en str_p3:-> "+String(stprog3));
         String stprogtodos = String(dia_prog_todos) + " " + hora_todos_ini + ":" + minutos_todos_ini + " " + hora_todos_fin + ":" + minutos_todos_fin;
         Serial.println("Mensaje grabado en str_todos:-> "+String(stprogtodos));

         Serial.println("enc_p1  en loop: " +String(enc_p1) );
         Serial.println("enc_p2 en loop: " +String(enc_p22) );
         Serial.println("enc_p3  en loop: " +String(enc_p3) );
         Serial.println("enc_todos  en loop: " +String(enc_todos) );
         Serial.print("Fecha TimeNTP: ");

         if (timeStatus() != timeNotSet) {
           if (now() != prevDisplay) { //update the display only if time has changed
             prevDisplay = now();
             digitalClockDisplay();
           }
         }
         Serial.println(" ");
         Serial.print("Fecha RTC: ");
         digitalClockDisplay2();
         Serial.println(" ");

         }
         estado_motor();
         //estado_programacion();
         }

}
 unsigned long currentMillis3 = millis();

 if (currentMillis3 - previousMillis3 >= 200000) { //envia la temperatura cada 10 segundos
   previousMillis3 = currentMillis3;
   client.publish(MOTOR, valueStrmotor);
   Serial.println("str_debug=> " +String(str_debug));
   //client.publish(PLACA3_LUZ_2, valueStrluz2);
 }
}

//---------------Encendido del motor------------------//

 void encendido(){
 digitalWrite(OUT_1, HIGH);
 if (valpul != valpul_ultimo){
 valpul_2=1;
 valpul_ultimo=valpul;
 strmotor = "enc.por-> "+programa;
 strmotor.toCharArray(valueStrmotor, 15);
 client.publish(MOTOR, valueStrmotor);
 if (str_debug==1) {
   Serial.println("Enviando: [" +  String(MOTOR) + "] " + strluz);
  }
 }
 }
 void encendido_prog(){
    digitalWrite(OUT_1, HIGH);
    if (valpul_p != valpul_ultimo_p){
    valpul_ultimo_p=valpul_p;
    estado_motor();
    if (str_debug==1) {
    Serial.println("Enviando: [" +  String(MOTOR) + "] " + strluz);
  }
 }
 }
 void apagado(){
     digitalWrite(OUT_1, LOW);
     valpul=!valpul_ultimo;
     valpul_ultimo=valpul;
     estado_motor();
     if (str_debug==1) {
       Serial.println("Enviando: [" +  String(MOTOR) + "] " + strluz);
      }
     }






//---------programacion por web------------//
void reloj(){
  if (AA>2020 & AA<2022 ) {
  Udp.begin(localPort);
  setSyncProvider(getNtpTime);
  DateTime fecha = rtc.now(); //fecha y hora del RTC
  AA=year();
  MM=month();
  DD=day();
  HH=hour();
  mm=minute();
  ss=second();

  if (AA<2020 || fecha.month()==165 ) {
    digitalClockDisplay();
    rtc.adjust(DateTime(AA, MM, DD, HH, mm,  ss));
    Serial.println(" ");
    Serial.print("Fecha RTC Actualizada: ");
    digitalClockDisplay2();
    Serial.println(" ");
  }
}
}

//----------------------------PROGRAMA 1 GUARDADO---------------------//
void programa1_guardado(){
    str_p1 = lee(90);
    Serial.println("Mensaje grabado en str_p1:->"+String(str_p1));
    for (int i = 0; i < dataLength ; i++){
       int index = str_p1.indexOf(separator);
       data[i] = str_p1.substring(0, index).toInt();
       str_p1 = str_p1.substring(index + 1);

        if (i==0) {
          dia_p1=data[0];
          if (dia_p1==1) {
            dia_prog1="todos";
          }
          if (str_debug==1) {
           Serial.println("Mensaje grabado en dia_todos:->"+String(dia_prog1));
          }
        }
        if (i==1) {
      hora_p1_ini=data[1];
      if (str_debug==1) {
       Serial.println("Mensaje grabado en hora_p1:->"+String(hora_p1_ini));
       }
     }
        if (i==2) {
      minutos_p1_ini=data[2];
      if (str_debug==1) {
        Serial.println("Mensaje grabado en minutos_p1:->"+String(minutos_p1_ini));
       }
     }
        if (i==3) {
       hora_p1_fin=data[3];
       if (str_debug==1) {
         Serial.println("Mensaje grabado en hora_p1_fin:->"+String(hora_p1_fin));
        }
      }
        if (i==4) {
        minutos_p1_fin=data[4];
        if (str_debug==1) {
          Serial.println("Mensaje grabado en minutos_p1_fin:->"+String(minutos_p1_fin));
         }
       }
      }

}
void programa2_guardado(){
  str_p2 = lee(120);
  Serial.println("Mensaje grabado en str_p2:->"+String(str_p2));
  for (int i = 0; i < dataLength ; i++){
     int index = str_p2.indexOf(separator);
     data_2[i] = str_p2.substring(0, index).toInt();
     str_p2 = str_p2.substring(index + 1);
     if (i==0) {
       dia_p2=data_2[0];
          if (dia_p2==1) {
            dia_prog2="todos";
          }
          if (str_debug==1) {
           Serial.println("Mensaje grabado en dia_todos:->"+String(dia_prog2));
          }
        }
        if (i==1) {
          hora_p2_ini=data_2[1];
          if (str_debug==1) {
           Serial.println("Mensaje grabado en hora_p2:->"+String(hora_p2_ini));
           }
         }
        if (i==2) {
          minutos_p2_ini=data_2[2];
          if (str_debug==1) {
            Serial.println("Mensaje grabado en minutos_p2:->"+String(minutos_p2_ini));
           }
         }
        if (i==3) {
           hora_p2_fin=data_2[3];
           if (str_debug==1) {
             Serial.println("Mensaje grabado en minutos_p2:->"+String(hora_p2_fin));
            }
          }
        if (i==4) {
            minutos_p2_fin=data_2[4];
            if (str_debug==1) {
              Serial.println("Mensaje grabado en minutos_p2:->"+String(minutos_p2_fin));
             }
           }
      }

}
void programa3_guardado(){

      str_p3 = lee(150);
        for (int i = 0; i < dataLength ; i++){
            int index = str_p3.indexOf(separator);
            data_3[i] = str_p3.substring(0, index).toInt();
            str_p3 = str_p3.substring(index + 1);
            if (i==0) {
              dia_p3=data_3[0];
              switch (dia_p3) {
                case 1:
                  dia_prog3="lunes";
                  break;
                case 2:
                  dia_prog3="martes";
                  break;
                case 3:
                  dia_prog3="miercoles";
                  break;
                case 4:
                  dia_prog3="jueves";
                  break;
                case 5:
                  dia_prog3="viernes";
                  break;
                case 6:
                  dia_prog3="sabado";
                  break;
                case 7:
                  dia_prog3="domingo";
                  break;
              }
              if (str_debug==1) {
                Serial.println("Mensaje grabado en dia_prog3:->"+String(dia_prog3));
              }
            }
            if (i==1) {
              hora_p3_ini=data_3[1];
              if (str_debug==1) {
              Serial.println("Mensaje grabado en hora_p3:->"+String(hora_p3_ini));
              }
            }
            if (i==2) {
              minutos_p3_ini=data_3[2];
              if (str_debug==1) {
                Serial.println("Mensaje grabado en minutos_p3:->"+String(minutos_p3_ini));
              }
            }
            if (i==3) {
              hora_p3_fin=data_3[3];
              if (str_debug==1) {
                Serial.println("Mensaje grabado en minutos_p3:->"+String(hora_p3_fin));
                }
              }
              if (i==4) {
                minutos_p3_fin=data_3[4];
                if (str_debug==1) {
                  Serial.println("Mensaje grabado en minutos_p3:->"+String(minutos_p3_fin));
                }
              }
          }
                       }

void programa_todos_guardado(){
    str_todos = lee(180);
    for (int i = 0; i < dataLength ; i++){
      //for (int i = 0; i < dataLength ; i++){
       int index = str_todos.indexOf(separator);
       data_6[i] = str_todos.substring(0, index).toInt();
       str_todos = str_todos.substring(index + 1);

        if (i==0) {
          dia_todos=data_6[0];
          if (dia_todos==1) {
            dia_prog_todos="todos";
          }
          if (str_debug==1) {
           Serial.println("Mensaje grabado en dia_todos:->"+String(dia_prog_todos));
          }
        }
        if (i==1) {
          hora_todos_ini=data_6[1];
          if (str_debug==1) {
          Serial.println("Mensaje grabado en hora_todos:->"+String(hora_todos_ini));
          }
        }
        if (i==2) {
         minutos_todos_ini=data_6[2];
          if (str_debug==1) {
           Serial.println("Mensaje grabado en minutos_todos:->"+String(minutos_todos_ini));
          }
        }
        if (i==3) {
          hora_todos_fin=data_6[3];
          if (str_debug==1) {
            Serial.println("Mensaje grabado en minutos_todos:->"+String(hora_todos_fin));
            }
          }
          if (i==4) {
           minutos_todos_fin=data_6[4];
            if (str_debug==1) {
             Serial.println("Mensaje grabado en minutos_todos:->"+String(minutos_todos_fin));
            }
          }
      }

}

void programa_1(){    //hora inicio progamacion 1
   if (hora_p1_ini==0 & minutos_p1_ini==0 & falla==1) {
     enc_p1 = "no";
     falla = 0;
     programa = "P1";
     apagado();
    if (str_debug==1) {
         Serial.println("apagado por Stop P1" );
      }
    }
    else if (enc_p1=="si" ) {
      valpul_p=1;
      val_apagado=1;
      programa = "P1";
      encendido_prog();
     if (str_debug==1) {
        Serial.println("encendido por enc_p1 " );
        Serial.println("enc_p1: " +String(enc_p1) );
      }
       if (dia_prog1=="todos"  & hora==hora_p1_fin & minutos>=minutos_p1_fin & val_apagado==1) {
         valpul_2=0;
         val_apagado=0;
         enc_p1="no";
         programa = "P1";
         graba(210, enc_p1);   // estado de encendido
         apagado();
        if (str_debug==1) {
             Serial.println("apagado" );
             Serial.println("enc_p1: " +String(enc_p1) );
          }
       }

     }
     if (enc_p1=="no") {
      if (dia_prog1=="todos"  & hora_p1_ini==hora & minutos_p1_ini==minutos) {
         enc_p1="si"; //variable control de encendido_prog
         graba(210, enc_p1);   // estado de encendido
         valpul_p=1;
         val_apagado=1;
         programa = "P1";
         encendido_prog();
      if (str_debug==1) {
         Serial.println("primer inicio prog1 " );
         Serial.println("enc_p1: " +String(enc_p1) );
        }
      }
  }
}

void programa_2(){    //hora inicio progamacion 1
  if (hora_p2_ini==0 & minutos_p2_ini==0 & falla==1) {
    enc_p22 = "no";
    falla = 0;
    programa = "P2";
    apagado();
   if (str_debug==1) {
        Serial.println("apagado por Stop P2" );
     }
   }
   else if (enc_p22=="si") {
      valpul_p=1;
      val_apagado=1;
      programa = "P2";
      encendido_prog();
     if (str_debug==1) {
        Serial.println("encendido por enc_p22 " );
        Serial.println("enc_p22: " +String(enc_p22) );

      } //if (r_dia==dia_p1  & hora==hora_p1_fin & minutos>=minutos_p1_fin & val_apagado==1) {

       if (dia_prog2=="todos"  & hora==hora_p2_fin & minutos>=minutos_p2_fin & val_apagado==1) {
         valpul_2=0;
         val_apagado=0;
         enc_p22="no";
         programa = "P2";
         graba(220, enc_p22);   // estado de encendido
         apagado();
        if (str_debug==1) {
          if (str_debug==1) {
             Serial.println("apagado" );
             Serial.println("enc_p22: " +String(enc_p22) );
            }
          }
       }
    }
    if (enc_p22=="no") {
      if (dia_prog2=="todos"  & hora_p2_ini==hora & minutos_p2_ini==minutos) {
         enc_p22="si"; //variable control de encendido_prog
         graba(220, enc_p22);   // estado de encendido
         valpul_p=1;
         val_apagado=1;
         programa = "P2";
         encendido_prog();
      if (str_debug==1) {
         Serial.println("primer inicio prog1 " );
         Serial.println("enc_p22: " +String(enc_p22) );
        }
      }
  }
}
void programa_3(){    //hora inicio progamacion 1
  if (hora_p3_ini==0 & minutos_p3_ini==0 & falla==1) {
    enc_p3 = "no";
    falla = 0;
    programa = "P3";
    apagado();
   if (str_debug==1) {
        Serial.println("apagado por Stop P3" );
     }
   }
    else if (enc_p3=="si") {
    valpul_p=1;
    val_apagado=1;
    programa = "P3";
    encendido_prog();
   if (str_debug==1) {
      Serial.println("encendido por enc_p3 " );
      Serial.println("enc_p3: " +String(enc_p3) );
    }

       if (dia==dia_prog3  & hora==hora_p3_fin & minutos>=minutos_p3_fin & val_apagado==1) {
         valpul_2=0;
         val_apagado=0;
         enc_p3="no";
         programa = "P3";
         graba(230, enc_p3);   // estado de encendido
         apagado();
        if (str_debug==1) {
          if (str_debug==1) {
             Serial.println("apagado" );
             Serial.println("enc_p3: " +String(enc_p3) );
            }
          }
       }
}
      if (enc_p3=="no") {
      if (dia==dia_prog3  & hora_p3_ini==hora & minutos_p3_ini==minutos) {
         enc_p3="si"; //variable control de encendido_prog
         graba(230, enc_p3);   // estado de encendido
         valpul_p=1;
         val_apagado=1;
         programa = "P3";
         encendido_prog();
      if (str_debug==1) {
         Serial.println("primer inicio prog3 " );
         Serial.println("enc_p3: " +String(enc_p3) );
        }
      }

}
}

void programa_todos(){    //hora inicio progamacion 1
  if (hora_todos_ini==0 & hora_todos_ini==0 & falla==1) {
    enc_todos ="no";
    falla=0;
    programa = "TODOS";
    apagado();
   if (str_debug==1) {
        Serial.println("apagado por Stop Todos" );
     }
   }
   else if (enc_todos=="si") {
      valpul_p=1;
      val_apagado=1;
      programa = "TODOS";
      encendido_prog();
     if (str_debug==1) {
        Serial.println("encendido por enc_todos " );
        Serial.println("enc_todos: " +String(enc_todos) );
       }
       if (dia_prog_todos=="todos"  & hora==hora_todos_fin & minutos>=minutos_todos_fin & val_apagado==1) {
         valpul_2=0;
         val_apagado=0;
         enc_todos="no";
         programa = "TODOS";
         graba(240, enc_todos);   // estado de encendido
         apagado();
        if (str_debug==1) {
          if (str_debug==1) {
             Serial.println("apagado" );
             Serial.println("enc_todos: " +String(enc_todos) );
            }
          }
       }
    }
    if (enc_todos=="no") {
      if (dia_prog_todos=="todos"  & hora_todos_ini==hora & minutos_todos_ini==minutos) {
         enc_todos="si"; //variable control de encendido_prog
         graba(240, enc_todos);   // estado de encendido
         valpul_p=1;
         val_apagado=1;
         programa = "TODOS";
         encendido_prog();
      if (str_debug==1) {
         Serial.println("primer inicio programa todos " );
         Serial.println("enc_todos: " +String(enc_todos) );
        }
      }
  }

}

void estado_motor(){
  byte est_motor = digitalRead(OUT_1);
  if (est_motor==0) {
    strmotor = "apag.por-> "+programa;
    }
  if (est_motor==1) {
     strmotor = "enc.por->"+programa;
    }
  strmotor.toCharArray(valueStrmotor, 15);
  client.publish(MOTOR, valueStrmotor);
}
void estado_programacion(){
  String stprog = String(dia_p1) + " " + hora_p1_ini + ":" + minutos_p1_ini + " " + hora_p1_fin + ":" + minutos_p1_fin;
  stprog.toCharArray(valuestrprog, 50);
  client.publish(PROGRAMACION, valuestrprog);
 }



//-----------------RTC-----------------------------//
int dia_de_semana(){
 DateTime fecha = rtc.now(); //fecha y hora del RTC
 n_anno=(fecha.year()-2000);
 d_anno=n_anno/4;
 n_dia=fecha.day();
 n_mes=fecha.month();
 hora=fecha.hour();
 minutos=fecha.minute();

 switch (n_mes) {
    case 1:
      t_mes=0;
      break;
    case 2:
      t_mes=3;
      break;
    case 3:
      t_mes=3;
      break;
    case 4:
      t_mes=6;
      break;
    case 5:
      t_mes=1;
      break;
    case 6:
      t_mes=4;
      break;
    case 7:
      t_mes=6;
      break;
    case 8:
      t_mes=2;
      break;
    case 9:
      t_mes=5;
      break;
    case 10:
      t_mes=0;
      break;
    case 11:
      t_mes=3;
      break;
    case 12:
      t_mes=5;
      break;
    default:
      t_mes=t_mes;
    break;
 }

 r_dia=n_dia+t_mes+n_anno+d_anno+t_siglo;
 r_dia = r_dia % 7;

 switch (r_dia) {
    case 1:
        dia="lunes";
      break;
      case 2:
       dia="martes";
      break;
      case 3:
       dia="miercoles";
      break;
      case 4:
       dia="jueves";
      break;
      case 5:
       dia="viernes";
      break;
      case 6:
       dia="sabado";
      break;
      case 0:
      dia="domingo";
      break;
      default:
       Serial.print(" ---");
      break;
 }
 return r_dia;
}
void digitalClockDisplay2(){
DateTime fecha = rtc.now();			// funcion que devuelve fecha y horario en formato
           // DateTime y asigna a variable fecha
Serial.print(String(dia));
Serial.print(" ");
Serial.print(fecha.year());			// funcion que obtiene el año de la fecha completa
Serial.print("/");				// caracter barra como separador
Serial.print(fecha.month());			// funcion que obtiene el mes de la fecha completa
Serial.print("/");				// caracter barra como separador
Serial.print(fecha.day());			// funcion que obtiene el dia de la fecha completa
Serial.print(" ");				// caracter espacio en blanco como separador
Serial.print(fecha.hour());			// funcion que obtiene la hora de la fecha completa
Serial.print(":");				// caracter dos puntos como separador
Serial.print(fecha.minute());			// funcion que obtiene los minutos de la fecha completa
Serial.print(":");				// caracter dos puntos como separador
Serial.println(fecha.second());

}
void digitalClockDisplay(){
  // digital clock display of the time
  Serial.print(year());
  Serial.print("/");
  Serial.print(month());
  Serial.print("/");
  Serial.print(day());
  Serial.print(" ");
  Serial.print(hour());
  printDigits(minute());
  printDigits(second());
}


    ///-------- NTP code ----------//
    void printDigits(int digits){
      // utility for digital clock display: prints preceding colon and leading 0
      Serial.print(":");
      if(digits < 10)
        Serial.print('0');
      Serial.print(digits);
    }
    const int NTP_PACKET_SIZE = 48; // NTP time is in the first 48 bytes of message
    byte packetBuffer[NTP_PACKET_SIZE]; //buffer to hold incoming & outgoing packets

    time_t getNtpTime()
    {
      while (Udp.parsePacket() > 0) ; // discard any previously received packets
      Serial.println("Transmit NTP Request");
      sendNTPpacket(timeServer);
      uint32_t beginWait = millis();
      while (millis() - beginWait < 1500) {
        int size = Udp.parsePacket();
        if (size >= NTP_PACKET_SIZE) {
          Serial.println("Receive NTP Response");
          Udp.read(packetBuffer, NTP_PACKET_SIZE);  // read packet into the buffer
          unsigned long secsSince1900;
          // convert four bytes starting at location 40 to a long integer
          secsSince1900 =  (unsigned long)packetBuffer[40] << 24;
          secsSince1900 |= (unsigned long)packetBuffer[41] << 16;
          secsSince1900 |= (unsigned long)packetBuffer[42] << 8;
          secsSince1900 |= (unsigned long)packetBuffer[43];
          return secsSince1900 - 2208988800UL + timeZone * SECS_PER_HOUR;
        }
      }
      Serial.println("No NTP Response :-(");
      return 0; // return 0 if unable to get the time
    }

    // send an NTP request to the time server at the given address
    void sendNTPpacket(IPAddress &address)
    {
      // set all bytes in the buffer to 0
      memset(packetBuffer, 0, NTP_PACKET_SIZE);
      // Initialize values needed to form NTP request
      // (see URL above for details on the packets)
      packetBuffer[0] = 0b11100011;   // LI, Version, Mode
      packetBuffer[1] = 0;     // Stratum, or type of clock
      packetBuffer[2] = 6;     // Polling Interval
      packetBuffer[3] = 0xEC;  // Peer Clock Precision
      // 8 bytes of zero for Root Delay & Root Dispersion
      packetBuffer[12]  = 49;
      packetBuffer[13]  = 0x4E;
      packetBuffer[14]  = 49;
      packetBuffer[15]  = 52;
      // all NTP fields have been given values, now
      // you can send a packet requesting a timestamp:
      Udp.beginPacket("3.ar.pool.ntp.org", 123); //NTP requests are to port 123
      Udp.write(packetBuffer, NTP_PACKET_SIZE);
      Udp.endPacket();
    }
    //--------------------------------------------------------------------//
