#include <EtherCard.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(10, 9, 5, 4, 3, 2);

static byte mymac[] = {
  0xDD,0xDD,0xDD,0x00,0x00,0x01};
char website[] PROGMEM = "0.arduinotest.duapp.com";
String msg,oldMsg;
String authCode="sdefvcw4efr";
byte Ethernet::buffer[700];
static uint32_t timer;
static uint32_t timerCheckWan;
//connect to internet
boolean initInternet(){
  if (!ether.begin(sizeof Ethernet::buffer, mymac, 8)){
//    Serial.println( "Failed to access Ethernet controller");
    return false;
  }
//  else
//    Serial.println("Ethernet controller initialized");
//  Serial.println();

  if (!ether.dhcpSetup()){
//    Serial.println("Failed to get configuration from DHCP");
    return false;
  }
//  else
//    Serial.println("DHCP configuration done");
//  lcd.LCD_clear();
//  lcd.LCD_write_string(8, MENU_Y, "conneted to     router", MENU_NORMAL);
//  lcd.clear();
//  lcd.print("connet to router");
//  ether.printIp("IP Address:\t", ether.myip);
//  ether.printIp("Netmask:\t", ether.mymask);
//  ether.printIp("Gateway:\t", ether.gwip);
//  Serial.println();

  if (!ether.dnsLookup(website)){
//    Serial.println("DNS failed");
    //    lcd.LCD_clear();
    //    lcd.LCD_write_string(MENU_ZERO, 1, "no internet", MENU_NORMAL);
    lcd.clear();
    lcd.print("no internet");
    return false;
  }
  else 
//    Serial.println("DNS resolution done");  
//  ether.printIp("SRV IP:\t", ether.hisip);
//  Serial.println(); 
  return true;

}

//process the call result
static void response_callback (byte status, word off, word len) {
//  Serial.println("received");
//  Serial.println((const char*) Ethernet::buffer +248);

  if(*((char*)Ethernet::buffer+248)=='s')
    off=248;
  else if(*((char*)Ethernet::buffer+248)=='n')
    off=267;
  else {
    lcd.clear();
    lcd.print("Error");
    return;
  } 
  msg=(const char*) Ethernet::buffer +off;
//  Serial.println("msg");
//  Serial.println(msg);
//  Serial.println(msg.substring(0,10));
  if(!msg.substring(0,11).equals(authCode)){
    return;
  }
  if(!msg.equals(oldMsg)){
    lcd.clear();
    lcd.print(msg.substring(12));
    if (msg.substring(11).length()>16){
      lcd.setCursor(0, 1) ;
      lcd.print(msg.substring(28));
    }

    oldMsg=msg;
//    Serial.println("screen show");
//    Serial.println((const char*) Ethernet::buffer +off-1);
  }


} 

void setup () {
  //  lcd.LCD_init();
  lcd.begin(16, 2);
  //  lcd.LCD_clear();
  //  lcd.LCD_write_string(MENU_X+5, MENU_Y + 1, "Starting", MENU_NORMAL);
  lcd.clear();
  lcd.setCursor(4, 0) ;
  lcd.print("starting");
  lcd.setCursor(3, 1) ;
  lcd.print("for maomao");
//  Serial.begin(57600);
//  Serial.println("Client Demo");
//  Serial.println();
  while (!initInternet()){
    delay(5000);     
  }     
//  Serial.println("internet success");
}

void loop() {

  ether.packetLoop(ether.packetReceive());

  if (millis() > timer) {
     if(!ether.dnsLookup(website)){
      //lcd.LCD_clear();
      //lcd.LCD_write_string(MENU_X, MENU_Y + 1, "connecting", MENU_NORMAL);
      lcd.clear();
      lcd.print("connecting");
      while (!initInternet()){
//        Serial.println("Try Again in 5s");
        delay(5000);     
      }
//      Serial.println("internet success");
      oldMsg="";
    }
    timer = millis() + 5000;
    ether.browseUrl(PSTR("/AMSgetMessage.php"), "?id=maomao", website, response_callback);
  }

}





