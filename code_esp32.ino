#include <WiFi.h>
#include <WebServer.h>
#include "ArduinoJson.h"

//day la cai giao dien web
#include "index.h"

//de dung cam bien dht11
#include "DHT.h"

//info wifi nha minh
const char* ssid = "</>";
const char* password = "tynuanoi";

//thoi gian bom (ms)
int TGbom = 2000;

//gan cac chan cho cac chuc nang
#define BOM 33
#define CBdoam1 34
//#define CBdoam2 19

//khai bao cho con dh11
#define DHTPIN 32
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

WebServer server(80);

//cac ham xu ly va cai dat
void ConnectFiWi(); //ket noi voi mang wifi nha minh
void SendWebPage(); //hien thi web
void tuoiCay1();
//void tuoiCay2();
void tuoiTatCa();   //tuoi all cay
void camBien();     //doc gia tri cac cam bien
void setDoam();     //duoi bao nhieu % thi auto tuoi
void dataJson(String ND, String DAkk, String DA1, String limit);
void sendData();


//cac bien trung gian
int TTBOM = 0;
long bien1 = 0;
long bien2 = 0;
long bien3 = 0;
long bien4 = 0;
long last = 0;
float nhietDo = 0.0;
float doAmkk = 0.0;
long doAm1 = 0;
//long doAm2 = 0;
float limit = 60;
String ChuoiSendWebJson = "";


void setup() {

  Serial.begin(115200);
   
  //output
  pinMode(BOM, OUTPUT);

  //input
  pinMode(DHTPIN, INPUT);
  pinMode(CBdoam1, INPUT);
//  pinMode(CBdoam2, INPUT);

  //mac dinh khi khoi dong
  digitalWrite(BOM, LOW);   //bom khong hoat dong

  dht.begin();
  
  ConnectFiWi();

  server.on("/", [] {
    SendWebPage();
  });
  server.on("/tuoiCay1", [] {
    tuoiCay1();
  });
//  server.on("/tuoiCay2", [] {
//    tuoiCay2();
//  });
  server.on("/x1", [] {
    setDoam();
  });
  server.on("/loadData", [] {
    sendData();
  });


  server.begin();
  last = millis();
}



void loop() {
  
  server.handleClient();  //duy tri server
  
  if(millis()-last >=500){
      camBien();
      dataJson(String(nhietDo), String(doAmkk), String(doAm1), String(limit));
      last = millis();
  }

  if(doAm1 < limit) {
    tuoiCay1();
  }
  
}


void ConnectFiWi(){
   WiFi.begin(ssid, password);
   int count = 0;
   while(WiFi.status() != WL_CONNECTED){
      delay(1000);
      count++;
      
      if(WiFi.status() == WL_CONNECTED){
          //in ra ip local
          Serial.print("\nDa ket noi WiFi! \nLocal IP: ");
          Serial.println(WiFi.localIP());
          break;
      }      
      if(count>10){
        ConnectFiWi();
      }      
   }
}

void SendWebPage(){
  server.send(200, "text/html", webpage);
}


void tuoiCay1(){
  TTBOM = 1;
  digitalWrite(BOM, HIGH);
  delay(TGbom);
  digitalWrite(BOM, LOW);
  TTBOM = 0;
  server.send(200, "text/html", webpage);
}

void tuoiTatCa(){
  tuoiCay1();
//  tuoiCay2();
}


void camBien(){ 
  nhietDo = dht.readTemperature(); 
  doAmkk = dht.readHumidity();
  int sum = 0;
  for(int i = 0; i<10; i++){
    sum += analogRead(CBdoam1);
  }
  doAm1 = 100 - map(sum/10, 0, 4095, 0, 100);
  sum = 0;
  server.send(200, "text/html", webpage);
}


void setDoam(){
  String text1 = server.arg("namex1");
  bien1 = text1.toInt();
  limit = bien1;
  server.send(200, "text/html", webpage);
}


void dataJson(String ND, String DAkk, String DA1, String limit){
  ChuoiSendWebJson = "{\"ND\":\"" + String(ND) + "\"," +
                     "\"DAkk\":\"" + String(DAkk) + "\"," +
                     "\"DA1\":\"" + String(DA1) + "\"," +
                     "\"limit\":\"" + String(limit) + "\"}";
  Serial.println(ChuoiSendWebJson);
}


void sendData(){
  server.send(200, "text/html", String(ChuoiSendWebJson));
}
