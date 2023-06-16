
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

int alienSize = 8;
int spacing = 4;
int rows = 2;
int columns = 6;
int x = 0;
int y = 0;
int speed = 1;

unsigned long previousUpdateTime = 0;
unsigned long updateInterval = 500;  // Update interval in milliseconds

void setup() {

  u8g2.begin();
  u8g2.setFlipMode(1);  // Set flip mode to double buffering
}

void loop() {
  unsigned long currentMillis = millis();
  // Check if it's time to update the display
  if (currentMillis - previousUpdateTime >= updateInterval) {
    previousUpdateTime = currentMillis;

    updateAliens();
    drawScene();
  }
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

void drawScene() {
  u8g2.firstPage();

  do {
    u8g2.clearBuffer();

    // Draw the aliens
    drawAliens();

    // Draw the mothership
    drawMothership();

  } while (u8g2.nextPage());
}

void drawAliens() {
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
}

void drawMothership() {
  int xPos = 0;
  int yPos = u8g2.getHeight() - 8;

  for (int i = 0; i < 8; i++) {
    for (int j = 0; j < 8; j++) {
      byte row = mothershipBytes[i];
      if (row & (1 << (7 - j))) {
        int pixelX = xPos + j;
        int pixelY = yPos + i;
        u8g2.drawPixel(pixelX, pixelY);
      }
    }
  }
}

void mothershipMove() {
  // this is where the mothership moves with the joystick
}
