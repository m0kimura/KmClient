//*******1*********2*********3*********4*********5*********6*********7*********8
/* KmClient.cpp */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>

#include "KmClient.h"

WiFiClient Client;

KmClient::KmClient(String ssid, String psw){
  ssid.toCharArray(Ssid, 32); psw.toCharArray(Psw, 16);
  Port=8088; Server="192.168.1.3";
}

//##### begin
//##### WiFiの初期化
bool KmClient::begin(){
  WiFi.begin(Ssid, Psw);
  while (WiFi.status() != WL_CONNECTED){delay(500); Serial.print(".");}
  return true;
}

//##### get
//##### GET リクエスト
bool KmClient::get(String host, int port, String url){
  char h[32]; host.toCharArray(h, 32);
  KmClient::encode(url); String u=Out;  
  if (!Client.connect(h, port)){return false;}
  Client.print(String("GET ") + u + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "Connection: close\r\n\r\n");
  Out=""; delay(10);
  while(Client.available()){
    String line=Client.readStringUntil('\r');
    Out.concat(line);
  }
  return true;
}

//##### post
//##### POST リクエスト
bool KmClient::post(String host, int port, String url, String data){
  char h[32]; host.toCharArray(h, 32); KmClient::encode(data); String d=Out;
  if (!Client.connect(h, port)){return false;}
  Client.print(String("POST ") + url + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n" + 
//               "Authorization: Basic " + unameenc + " \r\n" + 
               "Content-Length: "+String(d.length())+"\r\n" + 
               d+"\r\n");
  Out=""; delay(10);
  while(Client.available()){
    String line=Client.readStringUntil('\r');
    Out.concat(line);
  }
  return true;
}

bool KmClient::debug(String msg){
  return KmClient::get(Server, Port, "/log/"+msg);
}

void KmClient::encode(String data){
  const char *hex = "0123456789abcdef";
  char msg; int i; Out="";

  for(i=0; i<data.length(); i++){
    msg=data.charAt(i);
    if(('a'<=msg&&msg<='z')||('A'<=msg&&msg<='Z')||('0'<=msg&&msg<='9')||(msg=='/')){
      Out += msg;
    }else{
      Out += '%';
      Out += hex[msg >> 4];
      Out += hex[msg & 15];
    }
  }
}
