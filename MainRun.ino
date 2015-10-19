
#include <SPI.h>
#include <WiFi.h>
#include <JsonParser.h>
#include <Stepper.h>

char ssid[] = "XXXX";      //  your network SSID (name) 
char pass[] = "XXXX";   // your network password
int keyIndex = 0; 
int start = 0;
int end_ =0;
String nodeId = String(100);// your network key Index number (needed only for WEP)
String response = "";
int status = WL_IDLE_STATUS;

// Initialize the Wifi client library
WiFiClient client;

// server address:
char server[] = "graph.facebook.com";
//IPAddress server(64,131,82,241);

unsigned long lastConnectionTime = 0;           // last time you connected to the server, in milliseconds
boolean lastConnected = false;                  // state of the connection last time through the main loop
const unsigned long postingInterval = 10*1000;  // delay between updates, in milliseconds


const int step_360 = 205;                               
Stepper myStepper1(step_360,14,16,15,17);
Stepper myStepper2(step_360,18,20,19,21);
Stepper myStepper3(step_360,28,32,30,34);
Stepper myStepper4(step_360,36,40,38,42);
Stepper myStepper5(step_360,2,4,3,5);
int count ;

int _new[5];
int _old[5];
int diff[5];
int ndiff=0;

int countBuffer[5];

void setup() {

  Serial.begin(9600); 
  myStepper1.setSpeed(80);
  myStepper2.setSpeed(80);
  myStepper3.setSpeed(80);
  myStepper4.setSpeed(80);
  myStepper5.setSpeed(80);
  pinMode(22, INPUT_PULLUP);
  pinMode(23, INPUT_PULLUP);
  pinMode(24, INPUT_PULLUP);
  pinMode(25, INPUT_PULLUP);
  pinMode(26, INPUT_PULLUP);
  switch1();
  switch2();
  switch3();
  switch4();
  switch5();
  

  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 

    while(true);
  } 
  
 while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(ssid);
  
    status = WiFi.begin(ssid, pass);


    delay(10000);
  } 

  printWifiStatus();
}

void loop() {
     
  response = "";    
bool begin_ = false;
  while (client.available()) {
    char c = client.read();
    char count = c;
    if (c == '{') {
        begin_ = true;
    }
    if (begin_) response += (c);
    if (c == '}') {
        break;
        
    }
    
    
delay(1);
    

   
  }
  start = response.indexOf("likes\":\"") + 10;

end_ = start + 5;

nodeId = response.substring(start, end_);

    
    
  if (!client.connected() && lastConnected) {
    Serial.println();
    Serial.println("disconnecting....");
//Serial.println(nodeId);
 long int new1 = nodeId.toInt();
 Serial.println(new1);
 start_motors(new1);
    client.stop();
    delay(10);
      movement(diff);
  }
  if(!client.connected() && (millis() - lastConnectionTime > postingInterval)) {
    httpRequest();
  }
  lastConnected = client.connected();
   nodeId= "";
   Serial.flush();
}

// this method makes a HTTP connection to the server:
void httpRequest() {
  // if there's a successful connection:
  if (client.connect(server, 80)) {
    Serial.println("connecting...");
    // send the HTTP PUT request:
    client.println("GET /DMTIIndia?fields=likes HTTP/1.1");
    client.println("Host: graph.facebook.com");
    client.println("User-Agent: arduino-ethernet");
    client.println("Connection: close");
    client.println();

    // note the time that the connection was made:
    lastConnectionTime = millis();
  } 
  else {
    // if you couldn't make a connection:
    Serial.println("connection failed");
    Serial.println("disconnecting.");
    client.stop();
   
    
  }
}


void printWifiStatus() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}


void movement(int diff[]) {
 for(int s=0; s<step_360; s++) {
  myStepper1.step(-diff[0]);
 
 }
 for(int s=0; s<step_360; s++) {
  myStepper2.step(-diff[1]);
 
 }
 for(int s=0; s<step_360; s++) {
  myStepper3.step(-diff[2]);
 
 }
 for(int s=0; s<step_360; s++) {
  myStepper4.step(-diff[3]);
 
 }
 for(int s=0; s<step_360; s++) {
  myStepper5.step(-diff[4]);
 
 }
 
}


void normal(int pos){
diff[pos] = _new[pos] - countBuffer[pos];
countBuffer[pos]= countBuffer[pos] + diff[pos];

}

void reverse(int pos){
  ndiff = countBuffer[pos] - _new[pos];
diff[pos] = 10 - ndiff;
countBuffer[pos]=_new[pos];

}

void start_motors(int test) {
   count = test;
  if(count == 0){
  
 }else{
   
  _new[4]=(count/10000)%10;
  _new[3]=(count/1000)%10;
  _new[2]=(count/100)%10;
  _new[1]=(count/10)%10;
  _new[0]=(count/1)%10;
  
  for(int i =0 ;i<5;i++)
  {
  if(_new[i]>_old[i])
  {
    Serial.println("NORMAL");
    normal(i);
  }
    else if(_new[i]<_old[i])
    {
          Serial.println("Reverse");
      reverse(i);
    }
  else
  {    Serial.println("No Diff");
  diff[i] =0;
  
  }
  Serial.println("diff");
  Serial.println(i); 
   Serial.println(diff[i]); 
   _old[i]=_new[i];
   //Serial.flush();
  }
  //}
}
}

void switch1(){
    for(int i=0; i<100; i++){
  int sensorVal1 = digitalRead(26);
  
 if(sensorVal1 == HIGH)
 {
   
    myStepper5.step(-i);
  }
   
   else 
   {
  }
    }
  }

 void switch2(){
    for(int i=0; i<100; i++){
  int sensorVal2 = digitalRead(25);
  
 if(sensorVal2 == HIGH)
 {
   
    myStepper4.step(-i);
  }
   else 
   {
    
  }
    }
  }
  
   void switch3(){
    for(int i=0; i<100; i++){
  int sensorVal3 = digitalRead(24);
  if(sensorVal3 == HIGH)
 {
   
    myStepper3.step(-i);
  }

   else 
   {
    
  }
    }
  }
  
  void switch4(){
    for(int i=0; i<100; i++){
   int sensorVal4 = digitalRead(23);
  
 if(sensorVal4 == HIGH)
 {
   
    myStepper2.step(-i);
  }

   else 
   {
    }
    }
  }  
  void switch5(){
    for(int i=0; i<100; i++){
  int sensorVal5 = digitalRead(22);
  
 if(sensorVal5 == HIGH)
 {
   
    myStepper1.step(-i);
  }

   else 
   {
    
  }
    }
  }
