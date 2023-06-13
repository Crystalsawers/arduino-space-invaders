#ifndef Aliens_h
#define Aliens_h

#include <U8g2lib.h>

class Aliens {
  private:
    U8G2_ST7920_128X64_F_SW_SPI _display;
    int _alienSize;
    int _spacing;
    int _rows;
    int _columns;
    byte** _alienData;  // Array of byte arrays to store the alien data
    int _x;            // X-coordinate for the aliens' position
    int _y;            // Y-coordinate for the aliens' position
    int _speed;        // Speed of the aliens' movement

  public:
    Aliens(U8G2_ST7920_128X64_F_SW_SPI display, int alienSize, int spacing, int rows, int columns);
    ~Aliens();  // Destructor to free memory
    void update();
    void draw();
};

#endif
