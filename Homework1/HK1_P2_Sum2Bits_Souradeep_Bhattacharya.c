#include "rims.h"


void main()
{
   while (1) {
      
      char sum = 0;
      char A1 = A & 0x03;
      char A2 = (A & (0x03 << 2)) >> 2;
      char A3 = (A & (0x03 << 4)) >> 4;
      char A4 = (A & (0x03 << 6)) >> 6;
      sum = A1 + A2 + A3 + A4;
      B = 0xF0 | sum;
   }
}
