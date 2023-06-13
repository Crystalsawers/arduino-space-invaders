#include "Aliens.h"

byte alienPattern[] = {
  B00111100,  // Row 1: Pixels turned on: 3, 4, 5, 6
  B01111110,  // Row 2: Pixels turned on: 2, 3, 4, 5, 6, 7
  B11111111,  // Row 3: All pixels turned on
  B10101010   // Row 4: Pixels turned on: 1, 3, 5, 7
};

Aliens::Aliens(U8G2_ST7920_128X64_F_SW_SPI display, int alienSize, int spacing, int rows, int columns): _display(display){
  _alienSize = alienSize;
  _spacing = spacing;
  _rows = rows;
  _columns = columns;


_alienData = new byte*[_rows * _columns];
for (int i = 0; i < _rows * _columns; i++) {
  _alienData[i] = new byte[_alienSize];
  memcpy(_alienData[i], alienPattern, _alienSize);
}

}

Aliens::~Aliens() {
  // Free memory for the alien data array
  for (int i = 0; i < _rows * _columns; i++) {
    delete[] _alienData[i];
  }
  delete[] _alienData;
}

void Aliens::update() {
  _x += _speed;

  if (_x < -8 || _x > _display.getWidth()) {
    _speed *= -1;     // Reverse horizontal direction
    _y += 10;         // Move the alien downwards
  }
}

void Aliens::draw() {
  int xPos;
  int yPos;

  for (int row = 0; row < _rows; row++) {
    for (int col = 0; col < _columns; col++) {
      xPos = col * (_alienSize + _spacing);
      yPos = row * (_alienSize + _spacing);

      // Get the byte array representing the current alien
      byte* alien = _alienData[row * _columns + col];

      // Draw the alien using the byte array
      for (int y = 0; y < _alienSize; y++) {
        for (int x = 0; x < _alienSize; x++) {
          // Calculate the index of the current byte
          int byteIndex = y * _alienSize + x;

          // Calculate the index of the current bit within the byte
          int bitIndex = byteIndex % 8;

          // Calculate the index of the current byte within the byte array
          int arrayIndex = byteIndex / 8;

          // Calculate the mask for the current bit
          byte mask = 1 << bitIndex;

          // Check if the current bit is set in the byte
          bool isSet = alien[arrayIndex] & mask;

          // Calculate the position to draw the pixel
          int pixelX = xPos + x;
          int pixelY = yPos + y;

          // Draw the pixel if the bit is set
          if (isSet) {
            _display.drawPixel(pixelX, pixelY);
          }
        }
      }
    }
  }
}