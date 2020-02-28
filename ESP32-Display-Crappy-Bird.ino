/*
 * Crappy - Bird
 * 
 * This is a simple clone of the once popular Flappy Bird running on an ESP32 with a 128x32 px display
 * This program is free to use and modify.
 * Have Fun!
 * 
 * Author: Johannes Marqaurt
 */
#include <Wire.h>
#include <Adafruit_GFX.h> //
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4 // not used nicht genutzt bei diesem Display
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

//Adafruit_SSD1306 display(OLED_RESET); Treiber vor Version1.2
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  // put your setup code here, to run once:
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 128x32)

  display.display();  //showing the Adafruit logo
  //do not forget display.display(); otherwise the picture will not be visible
  delay(100); //waiting 10ms

  display.clearDisplay();
  
  //loading screen
  display.setTextColor(WHITE);
  
  char title[] = "CrappyBird";

  display.setCursor(0,24);
  display.print("by J.M.");
  display.setTextSize(2);
  display.setCursor(0,5);
  
  for(int i = 0; i < 11; i++){
    display.print(title[i]);
    display.display();
    delay(50);
  }
  display.setTextSize(1);
  delay(2000);
  
  const int buttonPin = 14;     //the number of the pushbutton pin
  pinMode(buttonPin, INPUT_PULLUP);
  
  int score = 0;
  int gapPos = 10;  //upper ycoord of the obstacle gap size, can be randomized for increased game difficulty
  int gapSize = 10; //obstacle gap size, can be randomized for increased game difficulty
  int xcoord=display.width(); //Position of the obstacle (Init: 128)
  int ycoord=display.height()/2; //Altitude of the Bird (Init: 16 = 32/2)
  
  while(true){  //could as well be put in void loop(){}... see explanation below
    //drawing the picture
    display.clearDisplay();
    display.setCursor(0,0); //Score position
    display.println(score); //Score
    display.drawRect(15, ycoord, 10, 5, WHITE);  //Bird
    display.fillRect(xcoord, 0, 5, gapPos, WHITE);  //Upper obstacle
    display.fillRect(xcoord, gapPos + gapSize, 5, display.height() - (gapPos + gapSize), WHITE);  //Lower obstacle
    
    display.display();

    //calculating obstacle hit
    if((xcoord > 15) && (xcoord < 25))  //Bird and obstacles have the same x-position
    {
      if((ycoord<gapPos)||(ycoord+5)>(gapPos+gapSize))  //Bird is not in the gap
      {
        display.setCursor(30,12);
        display.print("Game Over");
        display.display();
        delay(1000);
        score = 0;
        //Resetting the Birds initial position
        xcoord = display.width();
        ycoord = display.height()/2;
      }
    }

    //calculating obstacle
    xcoord--;
    if(xcoord<0)
    {
      xcoord = 128; //resetting the obstacles' position to the far right
      score++;  //increasing score, when obstacle passed
    }

    //calculating Bird
    bool buttonState = digitalRead(buttonPin);  //reading input
    if (buttonState == true) {
      ycoord--;
    } else {
      ycoord++;
    }

    //preventing Bird from escaping
    if(ycoord<1){ycoord = 1;}
    if(ycoord>27){ycoord = 27;}
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  //Personally, i do not really like the concept of separated setup and loop (JM)

}
