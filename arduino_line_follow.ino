#include "UNObot.h"

void setup() {
  // put your setup code here, to run once:
  UNObot_init();
  int checkPoint = 0;

}
void stop() {
  while (true) {
    set_motors(0, 0);

  }
}

bool scan(int sensors[3]) { 
  if ((sensors[0] > 750 || sensors[2] > 750) ||  sensors[1] > 750) {
    return true; // POKUD TO NAJDE CERNOU
  } else {
    return false;
  }
}

bool noLineFound(int sensors[3], int radius, int del) {
  for (int i = 0; i < radius + 1; i++) { 
    set_motors(100, -100);
    delay(del);
    if (scan(sensors)) {
      return true;
    }
    if (i == radius) { // pokud jsem na konci prvniho otaceni, tak se  zacnu otacet opacne
      for (int i = 0; i < radius + 1; i++) { // VRACENI DO ZAKLADNI POLOHY
        set_motors(100, -100);
        delay(del);
      }
      for (int i = 0; i < 10; i++) {
        set_motors(-100, 100);
        delay(del);
        if (scan(sensors)) {
          return true;
        }
        if (i == radius)  {
          for (int i = 0; i < 10; i++) {
            set_motors(-100, 100);
            delay(3);
          }
          for (int i = 0; i < 10; i++) {
            set_motors(100, 100);
            delay(20);
            scan(sensors);
          }
          set_motors(0, 0);
        }
      }
    }
  }
}


int isOnLine(int sensors[3]) {
  if (sensors[0] > 750 && sensors[2] > 750) { // pokud je vycentrovaný
    set_motors(200, 200);
  }
  else if ((sensors[0] > 750 && sensors[1] > 750) || (sensors[2] > 750 && sensors[1] > 750)) { // pokud jsou alespoň dva vycentrované
    if (sensors[0] > 750 && sensors[1] > 750) {
      set_motors(-50, 90);
    } else {
      set_motors(90, -50);
    }
  } else if (sensors[1] > 750) { // popř ověřit jestli nejsou aktivní postranní
    set_motors(200, 200);
  } else if ((sensors[0] > 750 && sensors[2] > 750) && sensors[1] < 750) {
    if (sensors[0] > 750) {
      set_motors(-50, 90);
    } else {
      set_motors(90, -50);
    }
  } else { // NENASEL NIC
    for(int i; i < 6; i++) {
      clear();
      print(i);
      if (noLineFound(sensors, 5, 25)) { // funguje 1 a 30
        break;
      }
      if (i == 5) {
        clear();
        print("HAH");
        stop();
      }
    }
  }
  delay(30);
}
void loop() {

  int values[3]; // Array für die 3 Sensoren
  read_line_sensors(values); // Funktionsaufruf mit Parameterübergabe

  int B_0 = 11;
  int B_1 = 10;
  int B_2 = 9;


  /* Print all of the 3 sensors to the screen *deprecated*
    lcd_goto_xy(0, 0); // Funktionsaufruf mit Parameterübergabe
    print(values[0]);
    lcd_goto_xy(2, 1); // Funktionsaufruf mit Parameterübergabe
    print(values[1]);// Funktionsaufruf mit Parameterübergabe
    lcd_goto_xy(5, 0); // Funktionsaufruf mit Parameterübergabe
    print(values[2]);
  */
  isOnLine(values);

  clear();




  pinMode(B_0, INPUT_PULLUP);
  if (digitalRead(B_2) != HIGH) {
    stop();
  }
}
