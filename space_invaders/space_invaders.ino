
#include <U8g2lib.h>

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* CS=*/10, /* reset=*/8);


byte alienPattern[] = {
  B01000010,  // Row 1
  B00100100,  // Row 2
  B11111111,  // Row 3
  B10100101,  // Row 4
  B11111111,  // Row 5
  B11000011,  // Row 6
  B01111110,  // Row 7
  B00111100   // Row 8
};

int alienSize = 8;
int spacing = 4;
int rows = 2;
int columns = 6;
int x = 0;
int y = 0;
int speed = 2;

void setup() {
  // put your setup code here, to run once:
  u8g2.begin();
}

void loop() {

  updateAliens();
  drawAliens();
  //drawAliensAndMothership();
  delay(500);
}

void updateAliens() {
  x += speed;

  if (x < 0 || x >= (u8g2.getWidth() - (columns * (alienSize + spacing)))) {
    speed *= -1;  // Reverse the direction

    // Move the aliens downwards
    y += alienSize + spacing;

    if (y >= (u8g2.getHeight() - (rows * (alienSize + spacing)))) {
      // Aliens have reached the bottom

      y = 0;
    }
  }
}

void drawAliens() {
  u8g2.firstPage();
  do {
    u8g2.clearBuffer();

    for (int row = 0; row < rows; row++) {
      for (int col = 0; col < columns; col++) {
        int xPos = x + col * (alienSize + spacing);
        int yPos = y + row * (alienSize + spacing);

        for (int yy = 0; yy < alienSize; yy++) {
          for (int xx = 0; xx < alienSize; xx++) {
            int byteIndex = yy * alienSize + xx;
            int bitIndex = byteIndex % 8;
            int arrayIndex = byteIndex / 8;
            byte mask = 1 << bitIndex;
            bool isSet = alienPattern[arrayIndex] & mask;

            int pixelX = xPos + xx;
            int pixelY = yPos + yy;

            if (isSet) {
              u8g2.drawPixel(pixelX, pixelY);
            }
          }
        }
      }
    }
  } while (u8g2.nextPage());
}


void mothershipMove() {
  // this is where the mothership moves with the joystick
}
