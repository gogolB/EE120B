#include "rims.h"

enum SM_States { SM_Init, SM_WaitLeft, SM_WaitRight, SM_WaitBoth_Right, SM_WaitBoth_Left, SM_WaitRight_Left, SM_WaitLeft_Right, SM_EndState_Left, SM_EndState_Right, SM_State_Hold } SM_State;

TickFct_State_machine_1() {
   switch(SM_State) { // Transitions
      case -1:
         SM_State = SM_Init;
         break;
         case SM_Init: 
         if (A == 0x02) {
            SM_State = SM_WaitLeft;
         }
         else if (A == 0x01) {
            SM_State = SM_WaitRight;
         }
         break;
      case SM_WaitLeft: 
         if (A == 0x03) {
            SM_State = SM_WaitBoth_Left;
         }
         else if (A != 0x02 && A != 0x03) {
            SM_State = SM_Init;
         }
         break;
      case SM_WaitRight: 
         if (A == 0x03) {
            SM_State = SM_WaitBoth_Right;
         }
         else if (A != 0x01 && A != 0x03) {
            SM_State = SM_Init;
         }
         break;
      case SM_WaitBoth_Right: 
         if (A == 0x02) {
            SM_State = SM_WaitLeft_Right;
         }
         else if ( A != 0x03 && A != 0x02) {
            SM_State = SM_Init;
         }
         break;
      case SM_WaitBoth_Left: 
         if (A == 0x01) {
            SM_State = SM_WaitRight_Left;
         }
         else if (A != 0x03 && A != 0x01) {
            SM_State = SM_Init;
         }
         break;
      case SM_WaitRight_Left: 
         if (A == 0x00) {
            SM_State = SM_EndState_Left;
         }
         else if (A != 0x01 && A != 0x00) {
            SM_State = SM_Init;
         }
         break;
      case SM_WaitLeft_Right: 
         if (A == 0x00) {
            SM_State = SM_EndState_Right;
         }
         else if (A != 0x02 && A != 0x00) {
            SM_State = SM_Init;
         }
         break;
      case SM_EndState_Left: 
         if (0) {
            SM_State = SM_Init;
         }
         else if (1) {
            SM_State = SM_State_Hold;
         }
         break;
      case SM_EndState_Right: 
         if (0) {
            SM_State = SM_Init;
         }
         else if (1) {
            SM_State = SM_State_Hold;
         }
         break;
      case SM_State_Hold: 
         if (A == 0x01) {
            SM_State = SM_WaitRight;
         }
         else if (A == 0x02) {
            SM_State = SM_WaitLeft;
         }
         else if (A != 0x01 || A != 0x02) {
            SM_State = SM_Init;
         }
         break;
      default:
         SM_State = SM_Init;
   } // Transitions

   switch(SM_State) { // State actions
      case SM_Init:
         B=0;
         break;
      case SM_WaitLeft:
         break;
      case SM_WaitRight:
         break;
      case SM_WaitBoth_Right:
         break;
      case SM_WaitBoth_Left:
         break;
      case SM_WaitRight_Left:
         break;
      case SM_WaitLeft_Right:
         break;
      case SM_EndState_Left:
         B = 0x80;
         break;
      case SM_EndState_Right:
         B = 0x01;
         break;
      case SM_State_Hold:
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {
   
   SM_State = -1; // Initial state
   B = 0; // Init outputs

   while(1) {
      TickFct_State_machine_1();
   } // while (1)
} // Main