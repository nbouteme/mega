#include <IRremote.h>
#include <boarddefs.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>

#include <LiquidCrystal.h>

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
IRrecv irrecv(7);

struct menu_item {
  const char *name;
  const char *cmd;
};

struct menu_decl {
  int cidx;
  menu_item items[3];
};

menu_decl menu = {0, {
    {"MPV", "mpv"},
    {"Chrome", "chromium"},
    {"Gedit", "gedit"}
  }
};

void setup() {
  Serial.begin(9600);
  irrecv.enableIRIn(); 
  //irrecv.blink13(true);
}

menu_item *getCurrentEntry() {
    return menu.items + menu.cidx;
}

void nextEntry() {
    menu.cidx += 1;
    menu.cidx %= 3;
}

void prevEntry() {
    menu.cidx -= 1;
    if (menu.cidx < 0)
      menu.cidx = 2;
}

void emitCmd() {
  const static unsigned char magic[] = {0xD1, 53, 0xAE, 53};
  String cmd = String(getCurrentEntry()->cmd);
  int s = cmd.length();
  Serial.write(magic, 4);
  Serial.write(s);
  Serial.write(cmd.c_str());
}

void loop() {
  lcd.setCursor(0, 0);
  lcd.clear();
  decode_results results;

  if (irrecv.decode(&results)){
    switch (results.value) {
      case 0xff22dd: // prev
        prevEntry();
        break;
      case 0xffc23d: // next
        nextEntry();
        break;
      case 0xff02fd: // next
        emitCmd();
        break;  
    }
    irrecv.resume(); 
  }
  lcd.print(String("<< ") + String(getCurrentEntry()->name) + String(" >>"));
        
  delay(100);             
  
}
