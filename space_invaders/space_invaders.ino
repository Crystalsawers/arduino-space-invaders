#include <MUIU8g2.h>
#include <U8g2lib.h>
#include <U8x8lib.h>

#include <MUIU8g2.h>
#include <U8g2lib.h>
#include <U8x8lib.h>

// U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* CS=*/ 10, /* reset=*/ U8X8_PIN_NONE);
U8G2_ST7920_128X64_F_SW_SPI u8g2(U8G2_R0, /* clock=*/ 13, /* data=*/ 11, /* CS=*/ 10, /* reset=*/ 8);
void setup() {
  // put your setup code here, to run once:
  u8g2.begin();
}

void loop() {
  // put your main code here, to run repeatedly:

}
