#include <Aliens.h>

#include <U8g2lib.h>

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* CS=*/10, /* reset=*/8);
Aliens aliens(u8g2, 8, 4, 2, 6);


void setup() {
  // put your setup code here, to run once:
  u8g2.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  alienFunction();
  Mothership();
  delay(1000);
}

void alienFunction() {

  u8g2.firstPage();
  do {
    u8g2.clearBuffer();

    aliens.update();
    aliens.draw();

  } while (u8g2.nextPage());

  delay(100);
}

void Mothership() {

byte mothershipBytes[] = {
  B00010000,  // Row 1
  B00010000,  // Row 2
  B00111000,  // Row 3
  B00111000,  // Row 4
  B01111100,  // Row 5
  B01111110,  // Row 6
  B11011011,  // Row 7
  B11011011   // Row 8
};

  u8g2.firstPage();

  do {
    u8g2.clearBuffer();
  
        // Draw the mothership pattern
     for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        byte row = mothershipBytes[i];
        if (row & (1 << (7 - j))) {
          u8g2.drawPixel(j, u8g2.getHeight() - 8 + i);  // Adjust the position to bottom left
        }
      }
    }
  } while (u8g2.nextPage());
}
