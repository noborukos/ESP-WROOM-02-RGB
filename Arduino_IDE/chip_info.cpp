#include <ESP8266WiFi.h>
void disp_chip_info( void )
{
  Serial.println();
  
  Serial.println("-----ESP-WROOM-02 ( ESP8266 ) Chip Infomation -----");
  Serial.println();
  
  Serial.print("Core Version = ");
  Serial.println(ESP.getCoreVersion());
  
  Serial.print("CPU Frequency = ");
  Serial.print(ESP.getCpuFreqMHz());
  Serial.println(" MHz");
  
  Serial.print("ChipID = ");
  Serial.println(ESP.getChipId(), HEX); //MACアドレスの下位３バイト
  
  Serial.print("Flash ID = ");
  Serial.println(ESP.getFlashChipId(), HEX);
  
  Serial.print("SDK version = ");
  Serial.println(ESP.getSdkVersion());
  
  Serial.print("Boot version = ");
  Serial.println(ESP.getBootVersion());
  
  Serial.print("Boot Mode = ");
  Serial.println(ESP.getBootMode());
  
  Serial.print("Flash Chip IDE Size = ");
  Serial.print(ESP.getFlashChipSize()); //Arduino IDE 設定の Flash Size になる
  Serial.println(" byte");
 
  Serial.print("Flash Chip Real Size = ");
  Serial.print(ESP.getFlashChipRealSize()); //ESP-WROOM-32 内蔵最大 Flash Size になる
  Serial.println(" byte");
  
  Serial.print("Flash Frequency = ");
  Serial.print(ESP.getFlashChipSpeed());
  Serial.println(" Hz");
    
  String mode_str;
  switch( ESP.getFlashChipMode() ){
    case 0: mode_str = "QIO"; break;
    case 1: mode_str = "QOUT"; break;
    case 2: mode_str = "DIO"; break;
    case 3: mode_str = "DOUT"; break;
    case 4: mode_str = "Unknown"; break;
    default: break;
  }
  Serial.println("Flash Chip Mode = " + mode_str);
  
  Serial.print("Free Heap Size = ");
  Serial.println(ESP.getFreeHeap());
  
  Serial.print("Free Sketch Size = ");
  Serial.println(ESP.getFreeSketchSpace());
  
  Serial.print("Sketch Size = ");
  Serial.println(ESP.getSketchSize());
  
  Serial.println();
  }
