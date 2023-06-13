#include <Aliens.h>

#include <U8g2lib.h>

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* CS=*/10, /* reset=*/8);
Aliens aliens(u8g2, 8, 4, 2, 6);

void setup() {
  // put your setup code here, to run once:
  u8g2.begin();
}

void loop() {

  drawAliensAndMothership();
  delay(500);
}

void drawAliensAndMothership() {
  u8g2.firstPage();

  do {
    u8g2.clearBuffer();

    // Draw the aliens
    aliens.update();
    aliens.draw();

    // Draw the mothership
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