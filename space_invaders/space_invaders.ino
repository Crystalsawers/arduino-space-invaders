#include <U8g2lib.h>

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* CS=*/10, /* reset=*/8);

const int joystickXPin = A0;      // Analog input pin for X-axis as im only using it to move mothership horizontally
const int fireButtonPin = 2;      // Digital input pin for fire button
const int joystickDeadZone = 20;  // Dead zone for joystick reading

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

// bytes for the layout of the aliens on screen for each alien
byte alienStatus[2][6] = {
  { 1, 1, 1, 1, 1, 1 },  // Row 1
  { 1, 1, 1, 1, 1, 1 }   // Row 2
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
int mothershipX = 55;
int mothershipY = 0;
int alienSpeed = 1;
int mothershipSpeed = 4;
const int missileSpeed = 16;

// all the possible missile states
enum State {
  Idle,
  Firing,
  Active,
  Collision
};

State missileState = Idle;
int missileY = -1;  // meaning no active missile

unsigned long previousUpdateTime = 0;
unsigned long updateInterval = 250;  // Update interval in milliseconds

void setup() {

  u8g2.begin();
  u8g2.setFlipMode(1);  // Set flip mode to double buffering
  pinMode(fireButtonPin, INPUT_PULLUP);
  // external interrupt for firing missile everytime the button is pressed
 attachInterrupt(digitalPinToInterrupt(fireButtonPin), fireButtonInterrupt, FALLING);
}

void loop() {

  unsigned long currentMillis = millis();
  // Check if it's time to update the display
  if (currentMillis - previousUpdateTime >= updateInterval) {
    previousUpdateTime = currentMillis;

    updateAliens();
    mothershipMove();
    fireMissile();
    drawScene();
  }
}

void updateAliens() {
  x += alienSpeed;

  if (x < 0 || x >= (u8g2.getWidth() - (columns * (alienSize + spacing)))) {
    alienSpeed *= -1;  // Reverse the direction

    // Move the aliens downwards
    y += alienSize + spacing;

    if (y >= (u8g2.getHeight() - (rows * (alienSize + spacing)))) {
      // Aliens have reached the bottom

      y = 0;
    }
  }
}

void fireMissile() {
  switch (missileState) {
    case Idle:
      if (digitalRead(fireButtonPin) == LOW) {
        missileState = Firing;
      }
      break;

    case Firing:
      missileY = u8g2.getHeight() - 9;  // Start missile from the bottom of the screen
      missileState = Active;
      break;

    case Active:
      missileY -= missileSpeed;
      if (missileY < 0) {
        missileState = Idle;
        missileY = -1;
      } else {
        checkCollision();
      }
      break;

    case Collision:
      // Handle collision state, e.g., remove the alien or update score
      missileState = Idle;
      missileY = -1;
      break;
  }
}

void checkCollision() {
  if (missileState != Active) {
    return;
  }

  int alienColumn = (mothershipX + (alienSize / 2)) / (alienSize + spacing);
  int alienRow = (missileY - y - alienSize) / (alienSize + spacing);

  if (alienRow >= 0 && alienRow < rows && alienColumn >= 0 && alienColumn < columns && alienStatus[alienRow][alienColumn] == 1) {
    // Collision with an alive alien
    missileState = Collision;
    alienStatus[alienRow][alienColumn] = 0;  // Set the alien as dead or gone
    // Perform any additional actions, such as updating the score
  }
}

void fireButtonInterrupt() {
  if (missileState == Idle) {
    missileY = u8g2.getHeight() - 9;  // Start missile from the bottom of the screen
    missileState = Active;
  }
}

void drawScene() {
  u8g2.firstPage();

  do {
    u8g2.clearBuffer();

    drawMothership();
    drawAliens();
    drawMissile();

  } while (u8g2.nextPage());
}

void drawAliens() {
  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < columns; col++) {
      int xPos = x + col * (alienSize + spacing);
      int yPos = y + row * (alienSize + spacing);

      if (alienStatus[row][col] == 1) {
        // Alien is alive, draw it
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
}

void drawMothership() {
  int xPos = mothershipX;  // 55 for the middle
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

void drawMissile() {
  if (missileState == Active) {
    int xPos = mothershipX + 3;  // X position of the missile, aligned with the mothership
    int yPos = missileY;

    // Draw a vertical line to represent the missile
    u8g2.drawVLine(xPos, yPos, 4);
  }
}

void mothershipMove() {
  int joystickValue = analogRead(joystickXPin);

  // Check if joystick value is within the dead zone
  if (abs(joystickValue - 512) < joystickDeadZone) {
    return;  // No movement within dead zone, return
  }

  // Move the mothership based on joystick value
  if (joystickValue < 512) {
    mothershipX -= mothershipSpeed;
  } else {
    mothershipX += mothershipSpeed;
  }

  // Limit the mothership movement within the screen width
  mothershipX = constrain(mothershipX, 0, u8g2.getWidth() - 8);
}
