#include <IRremoteESP8266.h>
#include <ESP8266WiFi.h>
#include <WiFiManager.h>
#include <ESP8266WebServer.h>
#include <stdlib.h>

/***************************************Recv*********************************************/ 
int RECV_PIN = 13; 
IRrecv irrecv(RECV_PIN);
decode_results results;

/***************************************Send********************************************/ 
IRsend irsend(5);

/***************************************IoT********************************************/ 
String deviceID="000000003f";
String apikey="89d20a92-6ecd-4f0a-a147-d785d0273ca5";
WiFiClient client;
const char* server = "www.linksprite.io";

/**************************This is for send_val and get_val**************************/
typedef struct
{
  unsigned long val=0;
  int lengt=0;
  int type_t=0;
} str;

/*****************************save the remote button value****************************/
typedef struct
{
  int type=0;
  int bits=0;
  unsigned long power=0;
  unsigned long menu=0;
  unsigned long up=0;
  unsigned long down=0;
  unsigned long left=0;
  unsigned long right=0;
  unsigned long a=0;
  unsigned long b=0;
  unsigned long c=0;
  unsigned long d=0; 
} Remote;
Remote remote[10];

/********************************for query to send value****************************/
typedef struct
{
  String power;
  String menu;
  String left;
  String right;
  String up;
  String down;
  String a;
  String b;
  String c;
  String d;
  int type;  
}Que;

/***************************************decline********************************************/ 
str dump(decode_results *results) ;
str get_val();
void send_val(str s3);
void config_wifi();
void update_status(String button,String val);
Que query();
int StrtoDec(String a);
void add_button(Que val);

/*************************************set up******************************************/ 
void setup()
{
  Serial.begin(9600);
  irsend.begin();
  irrecv.enableIRIn(); // Start the receiver
  config_wifi();
}

/***************************************loop********************************************/ 
void loop() 
{
  Que qq;
  qq=query();
  add_button(qq);
  Serial.println(remote[0].a,HEX);
  Serial.println(remote[0].b,HEX);
  delay(1000);
}

/***************************************dump********************************************/
str dump(decode_results *results) 
{
  str s1;
  s1.lengt = results->bits;
  s1.type_t = results->decode_type;
  s1.val = results->value;
  Serial.println(s1.type_t,DEC);
  Serial.print(s1.val,HEX);
  Serial.print(" (");
  Serial.print(s1.lengt, DEC);   
  Serial.println(" bits)");
  return s1;
}

/***************************************get_val********************************************/
str get_val()
{
  str s2;
  if (irrecv.decode(&results)) 
  {
    s2=dump(&results);
    irrecv.resume(); // Receive the next value
  }
  return s2;
}

/***************************************send_val********************************************/
void send_val(str s3)
{
  if(s3.type_t==SONY)
  {
     irsend.sendSony(s3.val,s3.lengt);
     delay(500);  
  }  
  if(s3.type_t==RC5)
  {
     irsend.sendRC5(s3.val,s3.lengt);
     delay(500);  
  }
  if(s3.type_t==RC6)
  {
     irsend.sendRC6(s3.val,s3.lengt);
     delay(500);  
  }
  if(s3.type_t==DISH)
  {
     irsend.sendDISH(s3.val,s3.lengt);
     delay(500);  
  }
  if(s3.type_t==NEC)
  {
     irsend.sendNEC(s3.val,s3.lengt);
     delay(500);  
  }
  if(s3.type_t==LG)
  {
     irsend.sendLG(s3.val,s3.lengt);
     delay(500);  
  }
  if(s3.type_t==SAMSUNG)
  {
     irsend.sendSAMSUNG(s3.val,s3.lengt);
     delay(500);  
  }
  Serial.println("this is send ");
}

/*************************************config_wifi******************************************/ 
void config_wifi()
{
   WiFiManager wifiManager;
   wifiManager.setAPStaticIPConfig(IPAddress(10,0,1,1), IPAddress(10,0,1,1), IPAddress(255,255,255,0));
   wifiManager.autoConnect("LinkNodeAP");
   Serial.print("WiFi Connected ...\n");
   Serial.println("WiFi connected");
}

/*************************************update status******************************************/
void update_status(String button,String val)
{
   String but = "\"";
   but += button;
   but += "\":"; 
   if (client.connect(server,80)) 
   {  
     String  postStr1 ="{";
           postStr1 +="\"action\":\"update\",";
           postStr1 +="\"apikey\":\"";
           postStr1 += apikey;
           postStr1 +="\",";
           postStr1 +="\"deviceid\":\"";
           postStr1 += deviceID;
           postStr1 +="\",";
           postStr1 +="\"params\":";
           postStr1 +="{";
           postStr1 +=but;
           postStr1 +="\"";
           postStr1 +=val;
           postStr1 +="\"\r\n";
           postStr1 +="}";
           postStr1 +="}";
    client.print("POST /api/http HTTP/1.1\n");
    client.print("Host: ");
    client.print(server);
    client.print("\nContent-Type: application/json\n");
    client.print("Content-Length: ");
    client.print(postStr1.length());
    client.print("\n\n");
    client.print(postStr1);
    Serial.println(postStr1);
  }
   delay(100);
   String request = "";
   while (client.available()) 
   {
     char c = client.read();
     request +=c;
   } 
   if (request!= NULL)
   {
     int index1 = request.indexOf(":{");
     int index2 = request.indexOf("},");
     String param = request.substring(index1, index2 + 1);
     Serial.print("The update param is:");
    // Serial.println(param);
     client.stop();   
   } 
}

/*************************************query******************************************/ 
Que query()
{
  Que query_val;
  if (client.connect(server,80)) 
   {  
   String  postStr ="{";
         postStr +="\"action\":\"query\",";
         postStr +="\"apikey\":\"";
         postStr += apikey;
         postStr +="\",";
         postStr +="\"deviceid\":\"";
         postStr += deviceID;
         postStr +="\",";
         postStr += "\"params\":";
         postStr += "[";
         postStr += "\"power\"";
         postStr +=",";
         postStr += "\"menu\"";
         postStr +=",";
         postStr += "\"right\"";
         postStr +=",";
         postStr += "\"left\"";
         postStr +=",";
         postStr += "\"up\"";
         postStr +=",";
         postStr += "\"down\"";
         postStr +=",";
         postStr += "\"type\"";
         postStr +=",";
         postStr += "\"def1\"";
         postStr +=",";
         postStr += "\"def2\"";
         postStr +=",";
         postStr += "\"def3\"";
         postStr +=",";
         postStr += "\"def4\"";
         postStr +="]";
         postStr +="}";
    client.print("POST /api/http HTTP/1.1\n");
    client.print("Host: ");
    client.print(server);
    client.print("\nContent-Type: application/json\n");
    client.print("Content-Length: ");
    client.print(postStr.length());
    client.print("\n\n");
    client.print(postStr);    
  }
 delay(100);
 String request = "";
 while (client.available()) 
 {
   char c = client.read();
   request +=c;
 } 
 if (request!= NULL)
 {
   int dat;
   int index1 = request.indexOf(":{");
   int index2 = request.indexOf("},");
   int index3 = request.indexOf("power");
   int index4 = request.indexOf("menu");
   int index5 = request.indexOf("left");
   int index6 = request.indexOf("right");
   int index7 = request.indexOf("up");
   int index8 = request.indexOf("down");
   int index9 = request.indexOf("type");
   int index10 = request.indexOf("def1");
   int index11 = request.indexOf("def2");
   int index12 = request.indexOf("def3");
   int index13 = request.indexOf("def4");
   String param = request.substring(index1, index2+1);
   query_val.power = request.substring(index3+8, index3+9);
   query_val.menu = request.substring(index4+7, index4+8);
   query_val.right = request.substring(index6+8, index6+9);
   query_val.left = request.substring(index5+7, index5+8);
   query_val.up = request.substring(index7+5, index7+6);
   query_val.down = request.substring(index8+7, index8+8);
   query_val.a = request.substring(index10+7, index10+8);
   query_val.b = request.substring(index11+7, index11+8);
   query_val.c = request.substring(index12+7, index12+8);
   query_val.d = request.substring(index13+7, index13+8);
   String type = request.substring(index9+7, index9+8);
   Serial.println(param);
   query_val.type = StrtoDec(type);
   client.stop();  
   return query_val;
 }
}

/*************************************StrtoDec******************************************/ 
int StrtoDec(String a)
{
  if(a == "0")
      return 0;
  if(a == "1")
     return 1;  
  if(a == "2")
      return 2;
  if(a == "3")
     return 3;  
  if(a == "4")
      return 4;
  if(a == "5")
     return 5;  
  if(a == "6")
      return 6;
  if(a == "7")
     return 7;  
  if(a == "8")
      return 8;
  if(a == "9")
     return 9;  
}

/*************************************add_button******************************************/
void add_button(Que val)
{
   str s4,s5;
   while(val.power == "2")
   {
        s4=get_val();
        if (s4.val != 0)
        {
          remote[val.type].type = s4.type_t;
          remote[val.type].power = s4.val;
          remote[val.type].bits = s4.lengt;
          update_status("power","0");  
          break; 
        }
        delay(100);  
   }
   while(val.menu == "2")
   {
        s4=get_val();
        if (s4.val != 0)
        {
          remote[val.type].type = s4.type_t;
          remote[val.type].menu = s4.val;
          remote[val.type].bits = s4.lengt;
          update_status("menu","0");  
          break; 
        }
        delay(100);  
   }
   while(val.right == "2")
   {
        s4=get_val();
        if (s4.val != 0)
        {
          remote[val.type].type = s4.type_t;
          remote[val.type].right = s4.val;
          remote[val.type].bits = s4.lengt;
          update_status("right","0");  
          break; 
        }
        delay(100);  
   }
   while(val.left == "2")
   {
        s4=get_val();
        if (s4.val != 0)
        {
          remote[val.type].type = s4.type_t;
          remote[val.type].left = s4.val;
          remote[val.type].bits = s4.lengt;
          update_status("left","0");  
          break; 
        }
        delay(100);  
   }
   while(val.up == "2")
   {
        s4=get_val();
        if (s4.val != 0)
        {
          remote[val.type].type = s4.type_t;
          remote[val.type].up = s4.val;
          remote[val.type].bits = s4.lengt;
          update_status("up","0");  
          break; 
        }
        delay(100);  
   }
   while(val.down == "2")
   {
        s4=get_val();
        if (s4.val != 0)
        {
          remote[val.type].type = s4.type_t;
          remote[val.type].down = s4.val;
          remote[val.type].bits = s4.lengt;
          update_status("down","0");  
          break; 
        }
        delay(100);  
   }  
   while(val.a == "2")
   {
        s4=get_val();
        if (s4.val != 0)
        {
          remote[val.type].type = s4.type_t;
          remote[val.type].a = s4.val;
          remote[val.type].bits = s4.lengt;
          update_status("def1","0");  
          break; 
        }
        delay(100);  
   }  
   while(val.b == "2")
   {
        s4=get_val();
        if (s4.val != 0)
        {
          remote[val.type].type = s4.type_t;
          remote[val.type].b = s4.val;
          remote[val.type].bits = s4.lengt;
          update_status("def2","0");  
          break; 
        }
        delay(100);  
   } 
   while(val.c == "2")
   {
        s4=get_val();
        if (s4.val != 0)
        {
          remote[val.type].type = s4.type_t;
          remote[val.type].c = s4.val;
          remote[val.type].bits = s4.lengt;
          update_status("def3","0");  
          break; 
        }
        delay(100);  
   } 
   while(val.d == "2")
   {
        s4=get_val();
        if (s4.val != 0)
        {
          remote[val.type].type = s4.type_t;
          remote[val.type].d = s4.val;
          remote[val.type].bits = s4.lengt;
          update_status("def4","0");  
          break; 
        }
        delay(100);  
   } 
   if(val.power=="1")
   {   
       if(remote[val.type].power!=0)
       {
          s5.val = remote[val.type].power;
          s5.lengt = remote[val.type].bits;
          s5.type_t = remote[val.type].type;
          send_val(s5);
          update_status("power","0"); 
       }
   }
   if(val.menu=="1")
   {   
       if(remote[val.type].menu!=0)
       {
          s5.val = remote[val.type].menu;
          s5.lengt = remote[val.type].bits;
          s5.type_t = remote[val.type].type;
          send_val(s5);
          update_status("menu","0"); 
       }
   }
   if(val.up=="1")
   {   
       if(remote[val.type].up!=0)
       {
          s5.val = remote[val.type].up;
          s5.lengt = remote[val.type].bits;
          s5.type_t = remote[val.type].type;
          send_val(s5);
          update_status("up","0"); 
       }
   }
   if(val.down=="1")
   {   
       if(remote[val.type].down!=0)
       {
          s5.val = remote[val.type].down;
          s5.lengt = remote[val.type].bits;
          s5.type_t = remote[val.type].type;
          send_val(s5);
          update_status("down","0"); 
       }
   }
   if(val.left=="1")
   {   
       if(remote[val.type].left!=0)
       {
          s5.val = remote[val.type].left;
          s5.lengt = remote[val.type].bits;
          s5.type_t = remote[val.type].type;
          send_val(s5);
          update_status("left","0"); 
       }
   }
   if(val.right=="1")
   {   
       if(remote[val.type].right!=0)
       {
          s5.val = remote[val.type].right;
          s5.lengt = remote[val.type].bits;
          s5.type_t = remote[val.type].type;
          send_val(s5);
          update_status("right","0"); 
       }
   }
   if(val.a=="1")
   {   
       if(remote[val.type].a!=0)
       {
          s5.val = remote[val.type].a;
          s5.lengt = remote[val.type].bits;
          s5.type_t = remote[val.type].type;
          send_val(s5);
          update_status("def1","0"); 
       }
   }
   if(val.b=="1")
   {   
       if(remote[val.type].b!=0)
       {
          s5.val = remote[val.type].b;
          s5.lengt = remote[val.type].bits;
          s5.type_t = remote[val.type].type;
          send_val(s5);
          update_status("def2","0"); 
       }
   }
   if(val.c=="1")
   {   
       if(remote[val.type].c!=0)
       {
          s5.val = remote[val.type].c;
          s5.lengt = remote[val.type].bits;
          s5.type_t = remote[val.type].type;
          send_val(s5);
          update_status("def3","0"); 
       }
   }
   if(val.d=="1")
   {   
       if(remote[val.type].d!=0)
       {
          s5.val = remote[val.type].d;
          s5.lengt = remote[val.type].bits;
          s5.type_t = remote[val.type].type;
          send_val(s5);
          update_status("def4","0"); 
       }
   }
}
