#include <Arduino.h>
#include <TFT_eSPI.h> 
#include <SPI.h>

#define xPin 33
#define yPin 32
#define swPin 19

constexpr uint16_t background = TFT_BLACK;
TFT_eSPI tft = TFT_eSPI();

int xVal =0, yVal=0, screenType=0, dirY=1, dirX=1,ballSpeed =60, xPos, yPos, count, x;
  

void mainScreen();
void gameScreen();
void gameoverScreen();
void initGame();
void ball(int xVal);

void setup() {

  //pin settings
  pinMode(xPin, INPUT);
  pinMode(yPin, INPUT);
  pinMode(swPin, INPUT);

  Serial.begin(9600);
  //initialize screen
  tft.init();
  tft.setRotation(1); 
  tft.fillScreen(background);
  tft.setTextColor(TFT_BLUE, background); 
  tft.setTextSize(2);

}

void loop() {

  //changing screens
  switch (screenType)
  {
  case 1:
    gameScreen();
    break;

    case 2:
    gameoverScreen();
    break;

  default:
    mainScreen();
    break;
  }

}

void clearCursor(){
    tft.fillCircle(xVal,yVal,5, background);    //erasing previous cursor
  }

void drawCursor(){

    //mapping current jostick position to move the cursor
    xVal = xVal + map(analogRead(xPin)+300,0,4096,-5,5);
    yVal = yVal + map(analogRead(yPin)+300,0,4096,-5,5);

    //checking if the cursor is outside of the screen
    if(yVal > TFT_HEIGHT - 8){
      yVal =TFT_HEIGHT - 8;
    } else if(yVal < 10){
      yVal = 10;
    }

    if(xVal > TFT_WIDTH -10){
      xVal =TFT_WIDTH - 10;
    } else if(xVal < 10){
      xVal = 10;
    }

    tft.fillCircle(xVal,yVal,5, TFT_DARKGREEN);   //drawing cursor
  }

  void mainScreen(){

    x = digitalRead(swPin);
    
    clearCursor();

    //buttons
    tft.setTextColor(TFT_BLUE);
    tft.setCursor(20+(120-(4*12))/2,10); //(4)lenght of string
    tft.print("Game");

    //light the button if cursors is on it  
    if(yVal>5 && yVal < 30){

      tft.drawRoundRect(20,5,120,25,5,TFT_SKYBLUE);
      
    }else{

      tft.drawRoundRect(20,5,120,25,5,TFT_BLUE);
    }

    drawCursor();

    //making action when button is clicked and in  the cursor is in the right position
    if(!x && yVal < 30 && yVal > 5 ){
      initGame();
    }

    
    delay(70);
    
  }

  void gameScreen(){
    
    tft.fillRect(xVal,120,10,5,background);
    //mapping current jostick position to move the cursor
    xVal = xVal + map(analogRead(xPin)+300,0,4096,-7,7);

    if(xVal > 145){
      xVal = 145;
    }else if(xVal < 5){
      xVal = 5;
    }
    tft.fillRect(xVal,120,10,5,TFT_BROWN);
    ball(xVal);
    delay(70);

  }

  void ball(int xVal){

    tft.fillCircle(xPos,yPos,4,background);

    //making it a little harder 
    if(count >= 5){
      ballSpeed = 4;
    }else if(count >= 15){
      ballSpeed = 5;
    }

    // setting right ball speed and direction
    if(dirY ==1){
       yPos+=ballSpeed;
    }else if(dirY ==2){
      yPos-=ballSpeed;
    }

    if(dirX ==1){
       xPos+=ballSpeed;
    }else if(dirX ==2){
      xPos-=ballSpeed;
    }

    tft.fillCircle(xPos,yPos,4,TFT_GREEN);
   
   // comapre position of ball and platfrom and configure right action
    if(yPos>115){
      if(pow(xPos-(xVal+5),2) < 4){
       dirY = 2;
       count++;
      }else if(xPos-xVal < 5 && xPos - xVal >= 0){
        dirY = 2;
        dirX = 2;
        count++;
      }else if(xPos-xVal < 10 && xPos - xVal >= 0){
        dirY = 2;
        dirX =1;
        count++;
      }else{
        tft.fillScreen(background);
        screenType = 2;
      }
    }

    //configure to baunce back from walls
    if(yPos<10){
      dirY = 1;
    }
    if(xPos<10){
      dirX = 1;
    }else if(xPos >150){
      dirX = 2; 
    }

    delay(30);
  }

  void gameoverScreen(){

    x = digitalRead(swPin);
    
    clearCursor();

    //buttons
    tft.setTextColor(TFT_BLUE);
    tft.setCursor(20+(120-(7*12))/2,10); //(4)lenght of string
    tft.print("Restart");
    tft.drawCentreString("Score:",80,40,2);
    tft.setCursor(70,75);
    tft.print(count);

    //light the button if cursors is on it  
    if(yVal>5 && yVal < 30){

      tft.drawRoundRect(20,5,120,25,5,TFT_SKYBLUE);
      
    }else{

      tft.drawRoundRect(20,5,120,25,5,TFT_BLUE);
    }

    drawCursor();

    //making action when button is clicked and in  the cursor is in the right position
    if(!x && yVal < 30 && yVal > 5 ){
      
      initGame();
    }    
    delay(70);
  }

  void initGame(){

    count =0;
    ballSpeed = 3;
    xPos = random(10,140);
    yPos = random(10,60);
    screenType = 1;
    tft.fillScreen(background);
    delay(200);
  }