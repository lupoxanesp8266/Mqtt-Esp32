/* Library for update through OTA
   Created by Carlos Sanchez Prudencio
   Released into the public domain
   @since 2021/11/18
   @version 2
*/

#ifndef Ota_updater_h
#define Ota_updater_h

#include "Arduino.h"

class Ota_updater{

  public:
    Ota_updater(String);
    void myhandleClient();
    void begin();
};

#endif
