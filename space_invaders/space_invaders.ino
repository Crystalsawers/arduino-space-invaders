#include <U8g2lib.h>

U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* CS=*/10, /* reset=*/8);

const int joystickXPin = A0;      // Analog input pin for X-axis as im only using it to move mothership horizontally
const int fireButtonPin = 2;      // Digital input pin for fire button
const int soundPin = 3;           // Digital pin for sound
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
  { 1, 1, 1, 1, 1, 1 },  // top row of each alien
  { 1, 1, 1, 1, 1, 1 }   // bottom row of each alien
};


// Draw the mothership
byte mothershipBytes[] = {
  B00010000,
  B00010000,
  B00111000,
  B00111000,
  B01111100,
  B01111110,
  B11011011,
  B11011011
};

int alienSize = 8;
int spacing = 4;
int rows = 2;
int columns = 6;
int x = 0;
int y = 0;
int mothershipX = 55;
int mothershipY = 0;
int mothershipHeight = 8;
int alienSpeed = 1;
int mothershipSpeed = 4;
const int missileSpeed = 16;

const int debounceDelay = 50;  // Debounce delay in milliseconds

bool isButtonPressed = false;
bool wasButtonReleased = true;

// all the possible missile states
enum MissileState {
  Idle,
  Firing,
  Active,
  Collision
};


MissileState missileState = Idle;
int missileY = -1;  // meaning no active missile

unsigned long previousUpdateTime = 0;
unsigned long updateInterval = 250;  // Update interval in milliseconds

bool gameOver = false;
bool playAgain = false;

void resetGame() {
  // Reset all game variables and states to their initial values
  x = 0;
  y = 0;
  mothershipX = 55;
  mothershipY = 0;
  alienSpeed = 1;
  mothershipSpeed = 4;
  missileState = Idle;
  missileY = -1;
  previousUpdateTime = 0;
  gameOver = false;

  // Reset the alien status
  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < columns; col++) {
      alienStatus[row][col] = 1;
    }
  }
}

void setup() {

  u8g2.begin();
  u8g2.setFlipMode(1);  // Set flip mode to double buffering
  pinMode(fireButtonPin, INPUT_PULLUP);
  pinMode(soundPin, OUTPUT);
  // external interrupt for firing missile everytime the button is pressed
  attachInterrupt(digitalPinToInterrupt(fireButtonPin), fireButtonInterrupt, FALLING);
  Serial.begin(9600);
}

void loop() {
  while (!gameOver) {
    unsigned long currentMillis = millis();

    // Update the display
    if (currentMillis - previousUpdateTime >= updateInterval) {
      previousUpdateTime = currentMillis;

      updateAliens();
      mothershipMove();
      checkButtonState();
      fireMissile();
      drawScene();

      if (gameOver) {
        // Display game over message
        if (allAliensDead()) {
          u8g2.firstPage();
          do {
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_helvB08_tr);
            u8g2.drawStr(35, 20, "You win!");
            u8g2.drawStr(10, 40, "Restarting in 5 sec...");
          } while (u8g2.nextPage());
          winningSound();
        } else {
          u8g2.firstPage();
          do {
            u8g2.clearBuffer();
            u8g2.setFont(u8g2_font_helvB08_tr);
            u8g2.drawStr(35, 20, "You lose!");
            u8g2.drawStr(10, 40, "Restarting in 5 sec...");
          } while (u8g2.nextPage());
          losingSound();
        }

        if (digitalRead(fireButtonPin) == HIGH) {
          resetGame();
          playAgain = true;
        }
        
      }

      break;  // Exit the loop and end the game
    }
  }
}



bool allAliensDead() {
  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < columns; col++) {
      if (alienStatus[row][col] == 1) {
        return false;  // At least one alien is still alive
      }
    }
  }
  return true;  // All aliens are dead
}


void checkButtonState() {
  int buttonValue = digitalRead(fireButtonPin);

  if (buttonValue == LOW && wasButtonReleased) {
    isButtonPressed = true;
    wasButtonReleased = false;
  } else if (buttonValue == HIGH && !wasButtonReleased) {
    wasButtonReleased = true;
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
      gameOver = true;  // Set game over state
    }
  }

  if (allAliensDead()) {
    gameOver = true;  // Set game over state
  }
}

void fireMissile() {
  switch (missileState) {
    case Idle:
      if (isButtonPressed) {
        missileState = Firing;
        isButtonPressed = false;  // Reset button state after firing
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

  int missileColumn = (mothershipX - x) / (alienSize + spacing);
  int missileRow = (missileY - y) / (alienSize + spacing);

  // Check if any alive alien intersects with the missile's path
  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < columns; col++) {
      if (alienStatus[row][col] == 1) {
        int alienX = x + col * (alienSize + spacing);
        int alienY = y + row * (alienSize + spacing);

        // Check if the missile intersects with the alien
        if (missileColumn >= col && missileColumn < col + 1 && missileRow >= row && missileRow < row + 1) {
          // Collision with an alive alien
          missileState = Collision;
          alienStatus[row][col] = 0;  // Set the alien as dead or gone
          tone(soundPin, 130, 200);
          return;  // Exit the function early since collision detected
        }
      }
    }
  }
}


void fireButtonInterrupt() {
  isButtonPressed = true;
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

void winningSound() {

  int numNotes = 11;

  int frequencies[] = { 554, 622, 415, 622, 698, 830, 783, 698, 622, 554, 622 };

  int rhythm[] = { 16, 16, 8, 16, 16, 2, 2, 2, 2, 16, 16 };

  for (int i = 0; i < numNotes; i++) {
    tone(soundPin, frequencies[i], 150 * rhythm[i]);
    delay(60 * rhythm[i]);
  }
  noTone(soundPin);
  delay(5000);
}

void losingSound() {

  int numNotes = 4;

  int frequencies[] = { 311, 293, 277, 261 };

  int rhythm[] = { 8, 8, 8, 16 };

  for (int i = 0; i < numNotes; i++) {
    tone(soundPin, frequencies[i], 150 * rhythm[i]);
    delay(60 * rhythm[i]);
  }
  noTone(soundPin);
  delay(5000);
}
