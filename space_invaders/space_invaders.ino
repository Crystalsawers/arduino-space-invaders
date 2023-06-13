#include <U8g2lib.h>
#include <U8g2lib.h>

// Custom libraries
#include <Aliens.h>


U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/13, /* data=*/11, /* CS=*/10, /* reset=*/8);
Aliens aliens(u8g2, 8, 4, 2, 6);

void setup() {
  // put your setup code here, to run once:
  u8g2.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  alienFunction();

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