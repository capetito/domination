#include <Adafruit_NeoPixel.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2);

char* menu1[]={"Configurar", "Jugar"};
char* menu2[]={"Tiempo de Juego", "Probar Relay"};

const int PIN = 8;
#define NUM_LEDS 16
const int AD_PIN = A0;

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);

 //leds

const int REDLED = 4;
const int BLUELED = 3;
int adValue = analogRead(AD_PIN);
int GAMEMINUTES = 3;

boolean cancelando;
boolean defusing;
boolean doStatus = false; //for DEmolition
char var;

int tonepin = 6; // Pin for the sound
const int RELAYPIN = 7;
int ACTIVATESECONDS = 3;

boolean endGame = false;
unsigned long redTime;
unsigned long blueTime;
unsigned long iZoneTime;//initial time for zone
byte team=0; // 0 = neutral, 1 = blue team, 2 = red team
unsigned long timeCalcVar;
boolean soundEnable = true;
int activeTone = 1330;
int alarmTone1 = 700;
int alarmTone2 = 2600;
boolean refresh=true;   //1 refresh one time...
int keyPressed = 0;

void setup(){
  //PinModes
  pinMode(BLUELED, OUTPUT);     
  pinMode(REDLED, OUTPUT); 
  pinMode(RELAYPIN, OUTPUT);  
  pinMode(tonepin, OUTPUT);
  
  lcd.begin();
  lcd.backlight();
  Serial.begin(9600);
  strip.begin();
  strip.show();
  lcd.setCursor(2,0);
  tone(tonepin,800,300);
  lcd.print("Programmable");// you can add your team name or someting cool
  lcd.setCursor(1,1);
  lcd.print("Airsoft Device");// you can add your team name or someting cool
  delay(1500);
//  keypad.addEventListener(keypadEvent);  //==================need to change this ============

  digitalWrite(RELAYPIN, HIGH);   //turn off power to relay channels
    
  // CONFIGURE THE BARS OF PROGRESS BAR
  byte bar1[8] = {
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
    B10000,
  };
  byte bar2[8] = {
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
    B11000,
  };
  byte bar3[8] = {
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
    B11100,
  };
  byte bar4[8] = {
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
    B11110,
  };
  byte bar5[8] = {
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
    B11111,
  };
  byte up[8] = {
    B00000,
    B00100,
    B01110,
    B11111,
    B11111,
    B00000,
    B00000,
  };
  byte down[8] = {
    B00000,
    B00000,
    B11111,
    B11111,
    B01110,
    B00100,
    B00000,
  };
  lcd.createChar(0,bar1);
  lcd.createChar(1,bar2);
  lcd.createChar(2,bar3);
  lcd.createChar(3,bar4);
  lcd.createChar(4,bar5);
  lcd.createChar(5,up);
  lcd.createChar(6,down);
}

void loop(){
   
  menuPrincipal();
}

//##################MENUS###############################

void menuPrincipal(){   //MAIN MENU
  
  digitalWrite(BLUELED, LOW); 
  digitalWrite(REDLED, LOW); 

  //   if whe start a new game from another we need to restart propertly this variables
  doStatus=false;
  strip.show();
  
  //Draw menu
  lcd.clear();   
  lcd.setCursor(0,0);//clear lcd and set cursor to 0,0
  int i=0;
    
 // HERE YOU CAN ADD MORE ITEMS ON THE MAIN MENU
  lcd.setCursor(3,0); //agregado desde aqui
  tone(tonepin,2400,30); 
  lcd.print("Objetivo");// you can add your team name or someting cool
  lcd.setCursor(1,1);
  lcd.print("Dominable V2.0");// you can add your team name or someting cool
  delay(1500);
  lcd.clear();   
  lcd.setCursor(0,0); //agregado hasta aqui para nombre al inicio
  lcd.print(menu1[i]);
  lcd.setCursor(15,1);
  checkArrows(i,2);
  while(1){

      if(var == 1 && i>0){
      tone(tonepin,2400,30);
      i--;
      lcd.clear();   
      lcd.setCursor(0,0);
      lcd.print(menu1[i]);
      checkArrows(i,1);
      delay(50);
    }
    if(var == 2 && i<1){ // cambiado a i<3 de orig i<2
      tone(tonepin,2400,30);
      i++;
      lcd.clear();   
      lcd.setCursor(0,0); 
      lcd.print(menu1[i]);    
      checkArrows(i,1); // cambiado de checkarrows i,2
      delay(50);
    }

    if(var == 3){
      tone(tonepin,2400,30);
      lcd.clear();   
      lcd.setCursor(0,0);
      switch (i){

      case 0:
        config();
        break;
      case 1:   
        doStatus=true;
        domination();
        break;
      }
    }
  }
}

void whatButton() {     //code for 1 pin keypad starts here:
  int adValue = analogRead(AD_PIN);
  //Serial.println(buttonFromValue(adValue));     // Returns the button number based on the analog value

  //byte buttonFromValue(int adValue); 

    if (adValue > 300 && adValue < 500) {
      keyPressed = 1;  //up
    }
    if (adValue > 500 && adValue < 700) {
      keyPressed = 2;  //down
    }
    if (adValue > 700 && adValue < 900) {
      keyPressed = 3;   //sel
    }
    if (adValue > 900) {
      keyPressed = 4;   //esc
    }
    keyPressed = 0;
  
//  int var = keyPresed;
}


// fix this function =========================
/*
void keypadEvent(KeypadEvent key){
  switch (keypad.getState()){
    case RELEASED:
      switch (var){
         case 3: defusing=false;
         break;
         case 4: cancelando=false;
         break;
      }
    break;
    case HOLD:
      switch (var){
        case 3: defusing= true;
        break;
        case 4: cancelando=true;
        break;
      }
    break;
  }
}
*/

void config(){
  //Draw menu
  lcd.clear();
  lcd.setCursor(0, 0);
  int i=0;
  
  delay(500);
  lcd.print(menu2[i]);
  checkArrows(i,2);

  while(1){
   int var=keyPressed;
    if(var == 1 && i>0){
      tone(tonepin,2400,30);
      i--;
      lcd.clear();  
      lcd.print(menu2[i]);
      checkArrows(i,1);
      delay(50);

    }
    if(var == 2 && i<1){
      tone(tonepin,2400,30);
      i++;
      lcd.clear();  
      lcd.print(menu2[i]);
      checkArrows(i,1);
      delay(50);
    }
    if(var == 4){     //cancelled
      tone(tonepin,2400,30);
      menuPrincipal();
    }
    if(var == 3){
      tone(tonepin,2400,30);
      lcd.clear();
      switch (i){

      case 0:
        gameTimeMenu();
        break;

      case 1:
        lcd.clear();   
        lcd.setCursor(0,0);
        lcd.print("RELAYPIN ON!");
        digitalWrite(RELAYPIN, LOW);   // turn the LED on (RELAY IS LOW TRIGGER)
        delay(1500);   // wait for 1.5 second
        lcd.clear();   
        lcd.setCursor(0,0);
        lcd.print("RELAYPIN OFF!");
        digitalWrite(RELAYPIN, HIGH);
        delay(1500);
        config();
        break;        

      }
    }
  }
}

void checkArrows(byte i,byte maxx ){

  if(i==0){
    lcd.setCursor(15,1);
    lcd.write(6); 
  }
  if(i==maxx){  
    lcd.setCursor(15,0);
    lcd.write(5);
  }
 // if(i>0 && i<maxx){
   // lcd.setCursor(15,1);
    //lcd.write(6);
    //lcd.setCursor(15,0);
    //lcd.write(5);  
  //}
}

void gameTimeMenu () {

   lcd.setCursor(1,0);
   lcd.print("Tiempo de Juego");
   lcd.setCursor(5,1);
   lcd.print(GAMEMINUTES);  
   lcd.println(" minutos");
   var = analogRead(AD_PIN);
   if(var == 3)  {    //sel
   domination();
   }
     else {
   if(var == 1) {
      tone(tonepin,2400,30);
      GAMEMINUTES++;
      //cls();
      //lcd.print("Tiempo de Juego");
      lcd.setCursor(5,1);
      lcd.print(GAMEMINUTES); 
      //lcd.println(" mins  up  ");
      delay(50);
      gameTimeMenu();
   }
    if(var == 2){ // cambiado a i<3 de orig i<2
      tone(tonepin,2400,30);
      GAMEMINUTES--;
      //cls(); 
      //lcd.print("Tiempo de Juego");
      lcd.setCursor(5,1);
      lcd.print(GAMEMINUTES); 
      //lcd.println(" mins  dn  ");
      delay(50);
      gameTimeMenu();
   }
    if(var == 4){     //cancelled
      tone(tonepin,2400,30);
      config();
     }
   }
}   

void printTimeDom(unsigned long aTiempo, boolean showMillis){
  //minutes
  if((aTiempo/60000)<10)
  {
    lcd.print("0");
    lcd.print(aTiempo/60000);
  }
  else
  {
    lcd.print(aTiempo/60000);
  }
  lcd.print(":");
  //seconds
  if(((aTiempo/1000)%60)<10)
  {
    lcd.print("0");
    lcd.print((aTiempo/1000)%60);
  }
  else
  {
    lcd.print((aTiempo/1000)%60);
  }
  if(showMillis){
    lcd.print(":");
    //this not mach with real time, is just a effect, it says 999 because millis%1000 sometimes give 0 LOL
      lcd.print(999-millis()%1000);
  }
}

void gameOver(){

  if(team==1)blueTime+=millis()-iZoneTime;
  if(team==2)redTime+=millis()-iZoneTime;
  digitalWrite(BLUELED, LOW);
  digitalWrite(REDLED, LOW);
  strip.show();
  while(!defusing){
    analogRead(AD_PIN);
    if(defusing){
      analogRead(AD_PIN);
      break;
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("TIEMPO ACABADO!");
    lcd.setCursor(1,1);

    //check who team win the base
    if(blueTime>redTime){
      //blueteam wins
      lcd.print("AZULES GANAN");
      digitalWrite(BLUELED, HIGH);
      RunningLights(0,0,0xff, 50);
    }
    else{
      //redteam wins 
      lcd.print(" ROJOS GANAN");
      digitalWrite(REDLED, HIGH);
      RunningLights(0xff,0,0, 50);
    }
    delay(2000);
    analogRead(AD_PIN);
    if(defusing){
      analogRead(AD_PIN);
      break;
    }
    lcd.clear();   
    lcd.setCursor(0,0);
    lcd.print("Tiempo ROJO:");
    lcd.setCursor(5,1);
    printTimeDom(redTime,false);
    delay(3000);
    analogRead(AD_PIN);
    if(defusing){
      analogRead(AD_PIN);
      break;
    }
    lcd.clear();   
    lcd.setCursor(0,0);
    lcd.print("Tiempo AZUL:");
    lcd.setCursor(5,1);
    printTimeDom(blueTime,false);
    delay(2000);
    analogRead(AD_PIN);
    if(defusing){
      analogRead(AD_PIN);
      break;
    }
  }
  lcd.clear();   
  lcd.setCursor(0,0);
  delay(100);
  lcd.print("Jugar Otra Vez?");
  lcd.setCursor(0,1);
  lcd.print("SEL: Si   CAN: No");
  while(1)
  {
    whatButton();
    if(var == 1 ){
      tone(tonepin,2400,30);
      lcd.clear();   
      lcd.setCursor(0,0);
      domination();
      break;
    }  
    if(var == 2 ){
      tone(tonepin,2400,30);
      menuPrincipal();
      break;
    }  
  } 
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
   strip.setPixelColor(Pixel, strip.Color(red, green, blue));
}

void RunningLights(byte red, byte green, byte blue, int WaveDelay) {
  int Position=0;
  for(int j=0; j<NUM_LEDS*2; j++)
  {
      Position++; // = 0; //Position + Rate;
      for(int i=0; i<NUM_LEDS; i++) {
   setPixel(i,((sin(i+Position) * 127 + 128)/255)*red,
      ((sin(i+Position) * 127 + 128)/255)*green,
                  ((sin(i+Position) * 127 + 128)/255)*blue);
      }
     
      strip.show();
      delay(WaveDelay);
  }
}

void domination(){

  //SETUP INITIAL TIME 
  int minutos = GAMEMINUTES-1;
  boolean showGameTime=true;
  unsigned long a;
  unsigned long iTime=millis(); //  initialTime in millisec 
  unsigned long aTime;
 
  team=0;
  iZoneTime=0;
  aTime=0;
  redTime=0;
  blueTime=0;

  int largoTono = 50;
  // 0 = neutral, 1 = blue team, 2 = red team
  a=millis();
  //Starting Game Code
  while(1)  // this is the important code, is a little messy but works good.
  {
    if(endGame){
      gameOver();
    }
    
    analogRead(AD_PIN);
    aTime=millis()- iTime;
    //Code for led blinking
    timeCalcVar=(millis()- iTime)%1000;
    if(timeCalcVar >= 0 && timeCalcVar <= 40)
    {
      if(team==1)
        digitalWrite(BLUELED, HIGH);  
        RunningLights(0,0,0xff, 50);
      }
      if(team==2)
        digitalWrite(REDLED, HIGH); 
        RunningLights(0xff,0,0, 50);  
      } 
    }
    
    if(timeCalcVar >= 50 && timeCalcVar <= 100)
    {    
      if(team==1)digitalWrite(BLUELED, LOW);  
      if(team==2)digitalWrite(REDLED, LOW);
      strip.show();
    }
    // Sound!!! same as Destroy 
    if(timeCalcVar >= 0 && timeCalcVar <= 40 && soundEnable)tone(tonepin,activeTone,largoTono);

    if(timeCalcVar >= 245 && timeCalcVar <= 255 && minutos-aTime/60000<2 && soundEnable)tone(tonepin,activeTone,largoTono);
    if(timeCalcVar >= 495 && timeCalcVar <= 510 && minutos-aTime/60000<4 && soundEnable)tone(tonepin,activeTone,largoTono);
    if(timeCalcVar >= 745 && timeCalcVar <= 760 && minutos-aTime/60000<2 && soundEnable)tone(tonepin,activeTone,largoTono);
    //Help to count 3 secs
    if(a+2000<millis()){
      a=millis();   
      showGameTime=!showGameTime;
      lcd.clear();   
      lcd.setCursor(0,0);
    }
    //THE NEXT TO METHODS SHOW "GAME TIME" AND "CONTROLED ZONE TIME" IT SHOWS 2 AND 2 SEC EACH

    if(showGameTime){ //THE SECOND IS /2
      lcd.setCursor(0,0);
      lcd.print("TIEMPO DE JUEGO");
      lcd.setCursor(3,1);
      printTime(minutos, aTime);
    }
    else if (!showGameTime){

      lcd.setCursor(2,0);
      if(team == 0)lcd.print("Zona Neutral");
      if(team == 1)lcd.print(" Zona AZUL");
      if(team == 2)lcd.print(" Zona ROJA");

      if(team>0){
        lcd.setCursor(3,1);
        printTimeDom(millis()-iZoneTime,true);
      }
    }

    //###########################CHECKINGS##################

    //Check If Game End
    if(minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0)
    {
      gameOver();
    }

    //Check If IS neutral
    while((defusing || cancelando) && team > 0)
    {
      lcd.clear();   
      lcd.setCursor(0,0);
      if(team>0)lcd.print("Neutralizando...");
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis(); //start disabling time
      while(defusing || cancelando)
      {
        //check if game time runs out during the disabling
        aTime= millis()- iTime;
        if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) || minutos-aTime/60000>4000000000){ 
          endGame = true;
        }
        
        analogRead(AD_PIN);
        timeCalcVar = (millis()- xTime)%1000;

        if( timeCalcVar >= 0 && timeCalcVar <= 20)
        {
          if(soundEnable)tone(tonepin,alarmTone1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 500)
        {
          if(soundEnable)tone(tonepin,alarmTone2,200);
          digitalWrite(REDLED, LOW);
          strip.show();
        }

        unsigned long seconds= millis() - xTime;
        percent = (seconds)/(ACTIVATESECONDS*10);
        drawBar(percent);

        if(percent >= 100)
        {
          delay(1000);

          if(team==1){ 
            blueTime+=millis()-iZoneTime;
            iZoneTime=0; 

          }
          if(team==2){ 
            redTime+=millis()-iZoneTime;
            iZoneTime=0; 
          }
          team=0;
          break;
        }
      }
      lcd.clear();   
      lcd.setCursor(0,0);
    }

    //Capturing red

    while(defusing && team == 0 )
    {
      lcd.clear();   
      lcd.setCursor(0,0);
      if(team==0)lcd.print("Capturando Zona");
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis(); //start disabling time
      while(defusing)
      {
        analogRead(AD_PIN);
        //check if game time runs out during the disabling
        aTime= millis()- iTime;
        if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) || minutos-aTime/60000>4000000000){ 
          endGame = true;
        }
        timeCalcVar = (millis()- xTime)%1000;

        if( timeCalcVar >= 0 && timeCalcVar <= 20)
        {
          digitalWrite(REDLED, HIGH);  
          RunningLights(0xff,0,0, 50);
          if(soundEnable)tone(tonepin,alarmTone1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 500)
        {
          if(soundEnable)tone(tonepin,alarmTone2,200);
          digitalWrite(REDLED, LOW);
          strip.show();
        }

        unsigned long seconds= millis() - xTime;
        percent = (seconds)/(ACTIVATESECONDS*10);
        drawBar(percent);

        if(percent >= 100)
        {
          digitalWrite(BLUELED, LOW);
          strip.show();
          team=2;
          iZoneTime=millis();
          delay(1000);
          break;
        }
      }
      lcd.clear();   
      lcd.setCursor(0,0);
      digitalWrite(REDLED, LOW);
      strip.show();
    }

    //getting to blue zone
    while(cancelando && team == 0 )
    {
      lcd.clear();   
      lcd.setCursor(0,0);
      if(team==0)lcd.print("Capturando Zona");
      lcd.setCursor(0,1);
      unsigned int percent=0;
      unsigned long xTime=millis(); //start disabling time
      while(cancelando)
      {
        analogRead(AD_PIN);
        //check if game time runs out during the disabling
        aTime= millis()- iTime;
        if((minutos-aTime/60000==0 && 59-((aTime/1000)%60)==0) || minutos-aTime/60000>4000000000){ 
          endGame = true;
        }
        timeCalcVar = (millis()- xTime)%1000;

        if( timeCalcVar >= 0 && timeCalcVar <= 20)
        {
          digitalWrite(BLUELED, HIGH);  
          RunningLights(0,0,0xff, 50);
          if(soundEnable)tone(tonepin,alarmTone1,200);
        }
        if(timeCalcVar >= 480 && timeCalcVar <= 500)
        {
          if(soundEnable)tone(tonepin,alarmTone2,200);
          digitalWrite(BLUELED, LOW);
          strip.show();
        }

        unsigned long seconds= millis() - xTime;
        percent = (seconds)/(ACTIVATESECONDS*10);
        drawBar(percent);

        if(percent >= 100)
        {
          digitalWrite(BLUELED, LOW);
          strip.show();
          team=1;
          iZoneTime=millis();
          delay(1000);
          break;
        }
      }
      lcd.clear();   
      lcd.setCursor(0,0);
      digitalWrite(BLUELED, LOW);
      strip.show();  
    }
  }
}

void printTime(unsigned long minutos, unsigned long aTiempo){

  timeCalcVar=minutos-aTiempo/60000;
  //Hours
  
  if(timeCalcVar/60==0 && refresh){
      lcd.clear();
      refresh=false;
      //delay(100);
      lcd.setCursor(3,1);
      Serial.println("!!!!");
  }
   
  if(timeCalcVar/60>=1){
    
    if(timeCalcVar/60<10)
  {
    lcd.setCursor(2,1);
    lcd.print("0");
    lcd.print(timeCalcVar/60);
  }
  else
  {
    lcd.print(timeCalcVar/60);
  }
  
  lcd.print(":");
  
  }
  //minutes
  if(timeCalcVar%60<10)
  {
    lcd.print("0");
    lcd.print(timeCalcVar%60);
  }
  else
  {
    lcd.print(timeCalcVar%60);
  }
  lcd.print(":");
  //seconds
  timeCalcVar=aTiempo/1000;
  if(59-(timeCalcVar%60)<10)
  {
    lcd.print("0");
    lcd.print(59-(timeCalcVar%60));
  }
  else
  {
    lcd.print(59-(timeCalcVar%60));
  }
  lcd.print(":");
  //this not mach with real time, is just a effect, it says 999 because millis%1000 sometimes give 0 LOL
  lcd.print(999-(millis()%1000));
}

void drawBar(byte porcent){
  //TODO: Optimize this code 
  int box=(8*porcent)/10;
  lcd.setCursor(0,1);
  while(box>=5){
    if(box>=5)
    {
      lcd.write(4);
      box-=5;
    }
  }
    switch(box){
    case 0:
      break;
    case 1:
      lcd.write((uint8_t)0);
      break;
    case 2:
      lcd.write(1);
      break;
    case 3:
      lcd.write(2);
      break;
    case 4:
      lcd.write(3);
      break;
    }
}
/*
void cls(){
  lcd.clear();
  lcd.setCursor(0,0);
}*/
