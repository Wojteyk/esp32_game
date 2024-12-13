#include <Arduino.h>
#include "header.h"
#include <TFT_eSPI.h> 
#include <SPI.h>

#define xPin 33
#define yPin 32
#define swPin 19


enum direction{
    dirUp,
    dirDown,
    dirLeft,
    dirRight
};

enum screenType{
    main,
    game,
    gameover
};

struct ball{
    int xPos, yPos;
    int speed, size;
    direction dirX;
    direction dirY;
};

struct cursor
{
    int xVal, yVal;
    int buttonPressed;
};


cursor joystick;
ball pong1;
constexpr uint16_t background = TFT_BLACK;
TFT_eSPI tft = TFT_eSPI();
screenType currentScreen = main;
int count, hScore;

void screen(){
    
    switch (currentScreen)
    {
    case game:
        gameScreen();
        break;

    case main:
        mainScreen();
        break;
    case gameover:
        gameoverScreen();
        break;
    
    }
}

void gameoverScreen(){

    joystick.buttonPressed = digitalRead(swPin);
    
    clearCursor();

    //buttons
    tft.setTextColor(TFT_BLUE);
    tft.setCursor(20+(120-(7*12))/2,10); //(4)lenght of string
    tft.print("Restart");
    tft.drawCentreString("Score:",80,40,1);
    tft.setCursor(80,62);
    tft.print(count);
    tft.drawCentreString("High score:",80,80,1);
    tft.setCursor(80,100);
    tft.print(hScore);

    //light the button if cursors is on it  
    if(joystick.yVal > 5 && joystick.yVal < 30){

      tft.drawRoundRect(20,5,120,25,5,TFT_SKYBLUE);
      
    }else{

      tft.drawRoundRect(20,5,120,25,5,TFT_BLUE);
    }

    drawCursor();

    //making action when button is clicked and in  the cursor is in the right position
    if(!joystick.buttonPressed && joystick.yVal < 30 && joystick.yVal > 5 ){
        initGame();
    }    
    delay(70);
  }

void gameScreen(){

    tft.fillRect(joystick.xVal,120,10,5,background);
    //mapping current jostick position to move the cursor
    joystick.xVal = joystick.xVal + map(analogRead(xPin)+300,0,4096,-7,7);

    if(joystick.xVal > 145){
      joystick.xVal = 145;
    }else if(joystick.xVal < 5){
      joystick.xVal = 5;
    }
    tft.fillRect(joystick.xVal,120,10,5,TFT_BROWN);
    drawBall(joystick.xVal);
    delay(70);

}

void mainScreen(){

    joystick.buttonPressed = digitalRead(swPin);

    clearCursor();

    //buttons
    tft.setTextColor(TFT_BLUE);
    tft.setCursor(20+(120-(4*12))/2,10); //(4)lenght of string
    tft.print("Game");

    //light the button if cursors is on it  
    if(joystick.yVal>5 && joystick.yVal < 30){

      tft.drawRoundRect(20,5,120,25,5,TFT_SKYBLUE);
      
    }else{

      tft.drawRoundRect(20,5,120,25,5,TFT_BLUE);
    }


    drawCursor();

    //making action when button is clicked and in  the cursor is in the right position
    if(!joystick.buttonPressed && joystick.yVal < 30 && joystick.yVal > 5 ){
      initGame();
    }
    
    delay(70);
}

void clearCursor(){
    tft.fillCircle(joystick.xVal,joystick.yVal,5, background);    //erasing previous cursor
}

void drawCursor(){

    //mapping current jostick position to move the cursor
    joystick.xVal = joystick.xVal + map(analogRead(xPin)+300,0,4096,-5,5);
    joystick.yVal = joystick.yVal + map(analogRead(yPin)+300,0,4096,-5,5);

    //checking if the cursor is outside of the screen
    if(joystick.yVal > TFT_HEIGHT - 8){
      joystick.yVal =TFT_HEIGHT - 8;
    } else if(joystick.yVal < 10){
      joystick.yVal = 10;
    }

    if(joystick.xVal > TFT_WIDTH -10){
      joystick.xVal =TFT_WIDTH - 10;
    } else if(joystick.xVal < 10){
      joystick.xVal = 10;
    }

    tft.fillCircle(joystick.xVal,joystick.yVal,5, TFT_DARKGREEN);   //drawing cursor
  }

void drawBall(int xVal){

    tft.fillCircle(pong1.xPos,pong1.yPos,4,background);

    //making it a little harder 
    if(count >= 5){
      pong1.speed = 4;
    }else if(count >= 15){
      pong1.speed = 5;
    }

    // setting right ball speed and direction
    if(pong1.dirY == dirDown){
       pong1.yPos += pong1.speed;
    }else if(pong1.dirY == dirUp){
      pong1.yPos -= pong1.speed;
    }

    if(pong1.dirX == dirRight){
       pong1.xPos += pong1.speed;
    }else if(pong1.dirX == dirLeft){
      pong1.xPos -= pong1.speed;
    }

    tft.fillCircle(pong1.xPos,pong1.yPos,4,TFT_GREEN);
   
   // comapre position of ball and platfrom and configure right action
    if(pong1.yPos>115){
      if(pow(pong1.xPos-(xVal+5),2) < 4){
       pong1.dirY = dirUp;
       count++;
      }else if(pong1.xPos - xVal < 5 && pong1.xPos - xVal >= 0){
        pong1.dirY = dirUp;
        pong1.dirX = dirLeft;
        count++;
      }else if(pong1.xPos - xVal < 10 && pong1.xPos - xVal >= 0){
        pong1.dirY = dirUp;
        pong1.dirX =dirRight;
        count++;
      }else{
        if(hScore < count){
            hScore = count;
        }
        tft.fillScreen(background);
        currentScreen = gameover;
      }
    }

    //configure to baunce back from walls
    if(pong1.yPos<10){
      pong1.dirY = dirDown;
    }
    if(pong1.xPos<10){
      pong1.dirX = dirRight;
    }else if(pong1.xPos >150){
      pong1.dirX = dirLeft; 
    }

    delay(30);
  }

int initGame() {

  pong1.xPos = random(10,140);
  pong1.yPos = random(10,60);
  pong1.dirX = dirLeft;
  pong1.dirY = dirDown;
  pong1.speed = 3;
  pong1.size=4;
  count=0;
  tft.fillScreen(background);
  currentScreen = game;
  return count;
}

void initScreen(){
  //initialize screen
  tft.init();
  tft.setRotation(1); 
  tft.fillScreen(background);
  tft.setTextColor(TFT_BLUE, background); 
  tft.setTextSize(2);
}