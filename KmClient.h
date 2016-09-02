//*******1*********2*********3*********4*********5*********6*********7*********8
/* KmClient.h */

#ifndef KMCLIENT_H
#define KMCLIENT_H

#include <Arduino.h>

class KmClient{
  public:
    KmClient(String ssid, String psw);
    bool begin();
    bool get(String host, int port, String url);
    bool post(String host, int port, String url, String dara);
    bool debug(String msg);
    void encode(String data);
    String Out;
    int Port;
    String Server;

  private:
    char Ssid[32];
    char Psw[16];

};


#endif
