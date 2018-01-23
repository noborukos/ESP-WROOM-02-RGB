/*
 * File:   APmode.cpp
 * Author: Noboru
 */

#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>


#define DBG_OUTPUT_PORT Serial

const char* APssid = "ESP8266-RGB";
const char* APpassword = "12345678";
const char* APhost = "esp8266.local";
const byte DNS_PORT = 53;

IPAddress apIP(192, 168, 0, 1);
DNSServer dnsServer;

void ApMode_init( void )
{
  WiFi.mode(WIFI_AP);
  WiFi.softAPConfig( apIP, apIP, IPAddress(255, 255, 255, 0) );
  WiFi.softAP( APssid, APpassword );

  dnsServer.setTTL(300);
  dnsServer.setErrorReplyCode( DNSReplyCode::ServerFailure );
  dnsServer.start( DNS_PORT, APhost, apIP );
}

void ApMode_task( void )
{
  dnsServer.processNextRequest();
}


