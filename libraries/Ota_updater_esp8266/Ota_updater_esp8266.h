/* Library for update through OTA
   Created by Carlos Sanchez Prudencio
   Released into the public domain
   @since 2021/11/18
   @version 2
*/

#ifndef Ota_updater_esp8266_h
#define Ota_updater_esp8266_h

#include "Arduino.h"

class Ota_updater_esp8266{

  public:
    Ota_updater_esp8266();
    void myhandleClient();
    void begin();
};

#endif
