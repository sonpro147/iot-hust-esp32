#include <WiFi.h>
#include <WebServer.h>

//day la cai giao dien web
#include "index.h"

//de dung cam bien dh11
#include "DHT.h"


//info wifi nha minh
const char* ssid = "</>";
const char* password = "tynuanoi";


//thoi gian bom (ms)
int TGbom = 2000;


//gan cac chan cho cac chuc nang
#define VAN1 2
#define VAN2 23
#define BOM 22
#define CBdoamkk 17
#define CBdoam1 18
#define CBdoam2 19
#define CBnhietdo 21

//#define Nut1 5
//#define Nut2 6
//#define Nut3 8


//khai bao cho con dh11
#define DHTPIN 13
const int DHTTYPE = DHT11;
DHT dht(DHTPIN, DHTTYPE);

//khai bao cho con sr04
#define Trig 16
#define Echo 14
unsigned long duration;   //luu thoi gian
int distance;             //luu khoang cach

WebServer server(80);


//cac ham xu ly va cai dat
void ConnectFiWi(); //ket noi voi mang wifi nha minh
void SendWebPage(); //hien thi web
void tuoiCay1();
void tuoiCay2();
void tuoiTatCa();   //tuoi all cay
void camBien();     //doc gia tri cac cam bien
void setDoam();     //duoi bao nhieu % thi auto tuoi
void setTG();       //thoi gian giua cac lan tuoi trong che do auto
void setTGbom();    //thoi gian tuoi
void setLuongnuoc();//luong nuoc toi thieu ma van tuoi
void dataJson(String ND, String DAkk, String DA1, String DA2, String LN, String TTVAN1, String TTVAN2, String TTBOM, String TTauto, String TTauto1, String TTauto2);
void sendData();



//cac bien trung gian
int TTVAN1 = 0;
int TTVAN2 = 0;
int TTBOM = 0;
int TTauto = 0;
int TTauto1 = 0;
int TTauto2 = 0;
long bien1 = 0;
long bien2 = 0;
long bien3 = 0;
long bien4 = 0;
long last = 0;
long nhietDo = 0;
long doAmkk = 0;
long doAm1 = 0;
long doAm2 = 0;
long luongNuoc = 0;
String ChuoiSendWebJson = "";
long doCaobinh = 30;
int sum = 0;


void setup() {

  Serial.begin(115200); //hien thi => de check loi cho de
   
  //khoi tao cac chan I/O
  //input
  pinMode(VAN1, OUTPUT);
  pinMode(VAN2, OUTPUT);
  pinMode(BOM, OUTPUT);
  pinMode(Trig, OUTPUT);

  //output
  pinMode(DHTPIN, INPUT);
  pinMode(CBdoamkk, INPUT);
  pinMode(CBdoam1, INPUT);
  pinMode(CBdoam2, INPUT);
  pinMode(CBnhietdo, INPUT);
  pinMode(Echo, INPUT);
//  pinMode(Nut1, INPUT);
//  pinMode(Nut2, INPUT);
//  pinMode(Nut3, INPUT);

  //mac dinh khi khoi dong
  digitalWrite(VAN1, LOW);  //van1 khong hoat dong
  digitalWrite(VAN2, LOW);  //van2 khong hoat dong
  digitalWrite(BOM, LOW);   //bom khong hoat dong

  ConnectFiWi();

  server.on("/", [] {
    SendWebPage();
  });
  server.on("/tuoiCay1", [] {
    tuoiCay1();
  });
  server.on("/tuoiCay2", [] {
    tuoiCay2();
  });
  server.on("/x1", [] {
    setDoam();
  });
  server.on("/x2", [] {
    setTG();
  });
  server.on("/x3", [] {
    setTGbom();
  });
  server.on("/x4", [] {
    setLuongnuoc();
  });
  server.on("/loadData", [] {
    sendData();
  });


  server.begin();
  Serial.println("Server start!");

  last = millis();
}



void loop() {
  
  server.handleClient();  //duy tri server
  
  if(millis()-last >=1000){
      camBien();
      dataJson(String(nhietDo), String(doAmkk), String(doAm1), String(doAm2), String(luongNuoc), String(TTVAN1), String(TTVAN2), String(TTBOM), String(TTauto), String(TTauto1), String(TTauto2));
      last = millis();
  }

}




void ConnectFiWi(){
   WiFi.begin(ssid, password);
   int count = 0;
   while(WiFi.status() != WL_CONNECTED){
      delay(1000);
      Serial.print(".");
      count++;
      
      if(WiFi.status() == WL_CONNECTED){
          //in ra ip local
          Serial.print("\nDa ket noi WiFi! \nLocal IP: ");
          Serial.println(WiFi.localIP());
          break;
      }
      
      //neu bi lag thi ket noi lai
      
      if(count>10){
        Serial.print("\nWiFi nhu loz. Dang ket noi lai!\n");
        ConnectFiWi();
      }      
   }
}

void SendWebPage(){
  server.send(200, "text/html", webpage);
}


//tuoi cay so 1
void tuoiCay1(){
  TTVAN1 = !TTVAN1;
  TTBOM = !TTBOM;
  digitalWrite(VAN1, HIGH);
  digitalWrite(BOM, HIGH);
  delay(TGbom);
  digitalWrite(BOM, LOW);
  TTBOM = !TTBOM;
  delay(50);
  digitalWrite(VAN1, LOW);
  TTVAN1 = !TTVAN1;
  
  Serial.println("Da tuoi Cay 1");
  
  server.send(200, "text/html", webpage);
}


//tuoi cay so 2
void tuoiCay2(){
  TTVAN2 = !TTVAN2;
  TTBOM = !TTBOM;
  digitalWrite(VAN2, HIGH);
  digitalWrite(BOM, HIGH);
  delay(TGbom);
  digitalWrite(BOM, LOW);
  TTBOM = !TTBOM;
  delay(50);
  digitalWrite(VAN2, LOW);
  TTVAN2 = !TTVAN2;
  
  Serial.println("Da tuoi Cay 2");
  
  server.send(200, "text/html", webpage);
}


//tuoi tat ca cac cay
void tuoiTatCa(){
  tuoiCay1();
  tuoiCay2();
  
  Serial.println("Da tuoi all cay!");
  
  server.send(200, "text/html", webpage);
}



//doc gia tri cac cam bien
void camBien(){ 
  nhietDo = dht.readTemperature(); 
  doAmkk = dht.readHumidity();

  //tinh toan do am dat
  for(int i = 0; i<10; i++){
    sum += analogRead(CBdoam1);
  }
  doAm1 = sum*3.3/(10*4096);
  sum = 0;
  for(int i = 0; i<10; i++){
    sum += analogRead(CBdoam2);
  }
  doAm2 = sum*3.3/(10*4096);

  //tinh toan luong nuoc
  digitalWrite(Trig,0);   // tắt chân trig
  delayMicroseconds(2);
  digitalWrite(Trig,1);   // phát xung từ chân trig
  delayMicroseconds(5);   // xung có độ dài 5 microSecond
  digitalWrite(Trig,0);   // tắt chân trig

  duration = pulseIn(Echo,HIGH);  //do do rong xung o chan Echo
  distance = int(duration/2/29.412);
  luongNuoc = doCaobinh - distance;
  
  server.send(200, "text/html", webpage);

  Serial.println("Da doc gia tri cam bien");
}


//set do am de tuoi auto
void setDoam(){
  String text1 = server.arg("namex1");
  bien1 = text1.toInt();
  
  server.send(200, "text/html", webpage);
  
  Serial.println("Da set do am thanh cong!\nDo am = ");
  Serial.println(server.arg("namex1"));
}


//thoi gian giua cac lan tuoi trong che do auto
void setTG(){
  String text2 = server.arg("namex2");
  bien2 = text2.toInt();

  server.send(200, "text/html", webpage);
  
  Serial.println("Da set thoi gian thanh cong!\nThoi gian giua cac lan tuoi = ");
  Serial.println(server.arg("namex2"));
}


//set thoi gian bom
void setTGbom(){
  String text3 = server.arg("namex3");
  bien3 = text3.toInt();
  
  server.send(200, "text/html", webpage);
  
  Serial.println("Set thoi gian bom thanh cong!\nMoi lan bom trong: ");
  Serial.println(server.arg("namex3"));
}


//set luong nuoc toi thieu
void setLuongnuoc(){
  String text4 = server.arg("namex4");
  bien4 = text4.toInt();

  server.send(200, "text/html", webpage);
  
  Serial.println("Set luong nuoc thanh cong!\nLuong nuoc toi thieu: ");
  Serial.println(server.arg("namex4"));
}


//ham data json
void dataJson(String ND, String DAkk, String DA1, String DA2, String LN, String TTVAN1, String TTVAN2, String TTBOM, String TTauto, String TTauto1, String TTauto2){
  ChuoiSendWebJson = "{\"ND\":\"" + String(ND) + "\"," +
                     "\"DAkk\":\"" + String(DAkk) + "\"," +
                     "\"DA1\":\"" + String(DA1) + "\"," +
                     "\"DA2\":\"" + String(DA2) + "\"," +
                     "\"LN\":\"" + String(LN) + "\"," +
                     "\"TTVAN1\":\"" + String(TTVAN1) + "\"," +
                     "\"TTVAN2\":\"" + String(TTVAN2) + "\"," +
                     "\"TTBOM\":\"" + String(TTBOM) + "\"," +
                     "\"TTauto\":\"" + String(TTauto) + "\"," +
                     "\"TTauto1\":\"" + String(TTauto1) + "\"," +
                     "\"TTauto2\":\"" + String(TTauto2) + "\"}";
  Serial.print("ChuoiSendWebJson: ");
  Serial.println(ChuoiSendWebJson);
}


//ham gui data
void sendData(){
  server.send(200, "text/html", String(ChuoiSendWebJson));
}
