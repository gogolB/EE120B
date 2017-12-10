#include "rims.h"

void main()
{
   while (1) 
   { 
      char input = A;
      char count = 0;
      char maxCount = 0;
      char bitMask = 1;
      char i = 0;
      for(; i < 8; i++)
      {
          if((input & bitMask) == bitMask)
          {
              count++;
          }
          else
          {
              if (count > maxCount)
              {
                  maxCount = count;
              }
              count = 0;
          }
          bitMask = bitMask << 1;
      }
      if(count > maxCount)
      {
          maxCount = count;
      }
      B = maxCount;
   }
}
