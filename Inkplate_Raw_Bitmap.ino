#include "Inkplate.h" //Include Inkplate library to the sketch
#include "SdFat.h" //Include library for SD card

Inkplate display(INKPLATE_3BIT); //Create an object on Inkplate library and also set library into 1 Bit mode (Monochrome)
SdFile file; //Create SdFile object used for accessing files on SD card

void loadPicture(char *fName, uint16_t offsetX, uint16_t offsetY) {
  if (!file.open(fName, O_RDONLY)) {
    //If it fails to open, send error message to display, otherwise read the file.
    Serial.println("File open error");
    return;
  }
  uint16_t myWidth, myHeight;
  int len = file.fileSize(), readLen = 0;
  myWidth = file.read() + file.read() * 256;
  myHeight = file.read() + file.read() * 256;
  Serial.println("len: " + String(len));
  Serial.println("w: " + String(myWidth) + ", h: " + String(myHeight));
  char myBuffer[myWidth];
  uint16_t ix = 0, jx = 0, px = 0, w2 = myWidth / 2;
  while (readLen < len) {
    file.read(myBuffer, w2);
    uint8_t v;
    for (ix = 0; ix < w2; ix++) {
      v = myBuffer[ix];
      // we cannot assume that both halves of the byte are for the same line --> width = odd number
      // so we must treat each half separately and check
      display.drawPixel(offsetX + px++, offsetY + jx, (v >> 4) >> 1);
      if (px == myWidth) {
        px = 0;
        jx++;
      }
      display.drawPixel(offsetX + px++, offsetY + jx, (v & 0x0F) >> 1);
      if (px == myWidth) {
        px = 0;
        jx++;
      }
    }
    readLen += w2;
  } Serial.println("Read: " + String(readLen));
  //display.display();
  file.close();
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  while (Serial.available()) char c = Serial.read();
  Serial.println("\n\nRaw Bitmap Test");
  display.begin(); //Init Inkplate library (you should call this function ONLY ONCE)
  display.clearDisplay(); //Clear frame buffer of display
  display.display(); //Put clear image on display
  display.setTextSize(2); //Scale text to be two times bigger then original (5x7 px)
  display.setTextColor(0, 7);
  if (display.sdCardInit()) {
    Serial.println("SD Card ok! Reading data...");
    showWanchai();
  } else {
    //If card init was not successful, display error on screen and stop the program (using infinite loop)
    Serial.println("SD Card error!");
    while (true);
  }
}

void showWanchai() {
  display.clearDisplay(); //Clear frame buffer of display
  display.display(); //Put clear image on display
  double t0, t1;
  t0 = millis();
  loadPicture("Wanchai.raw", 0, 120);
  t1 = millis() - t0;
  String render = "Wanchai rendered in " + String(t1) + " ms; ";
  t0 = millis();
  loadPicture("RAKLogo.raw", 0, 0);
  t1 = millis() - t0;
  display.setCursor(0, 580);
  render += "RAK rendered in " + String(t1) + " ms";
  display.print(render);
  display.display();
}

void showBastWAN_Antenna() {
  double t0, t1;
  display.clearDisplay(); //Clear frame buffer of display
  display.display(); //Put clear image on display
  t0 = millis();
  loadPicture("BastWAN_Antenna.raw", 0, 0);
  t1 = millis() - t0;
  display.setCursor(610, 20);
  display.print("Rendered in");
  display.setCursor(610, 40);
  display.print(String(t1) + " ms; ");
  display.display();
  delay(3000);
  display.clearDisplay(); //Clear frame buffer of display
  display.display(); //Put clear image on display
  t0 = millis();
  loadPicture("BastWAN_Antenna_Warranties.raw", 0, 0);
  t1 = millis() - t0;
  display.setCursor(610, 20);
  display.print("Rendered in");
  display.setCursor(610, 40);
  display.print(String(t1) + " ms; ");
  display.display();
}

void showSZBay() {
  display.clearDisplay(); //Clear frame buffer of display
  display.display(); //Put clear image on display
  double t0, t1;
  t0 = millis();
  loadPicture("Shenzhen_Bay.raw", 0, 120);
  t1 = millis() - t0;
  String render = "SZBay rendered in " + String(t1) + " ms; ";
  t0 = millis();
  loadPicture("RAKLogo.raw", 0, 0);
  t1 = millis() - t0;
  display.setCursor(0, 580);
  render += "RAK rendered in " + String(t1) + " ms";
  display.print(render);
  display.display();
}

// Shenzhen_Bay.raw 800x388
// BastWAN_Antenna.raw 600x600
// BastWAN_Antenna_Warranties.raw 600x600
// RAKLogo.raw 500x92
// Wanchai.raw 800x450
// Format is 0xAAAA 16 bits, width; 0xBBBB 16 bits, height; Then the bitmaps, 2 pixels / byte.

void loop() {
  if (display.readTouchpad(PAD1)) {
    showWanchai();
  } else if (display.readTouchpad(PAD2)) {
    showBastWAN_Antenna();
  } else if (display.readTouchpad(PAD3)) {
    showSZBay();
  }
  delay(100); //Wait a little bit between readings.
}
