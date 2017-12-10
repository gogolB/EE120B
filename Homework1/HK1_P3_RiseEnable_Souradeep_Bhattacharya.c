/*
This code was automatically generated using the Riverside-Irvine State machine Builder tool
Version 2.7 --- 8/10/2017 18:0:29 PST
*/

#include "rims.h"

/*Define user variables and functions for this state machine here.*/

enum SM_States { SM_Init, SM_WaitRiseA0, SM_WaitFallA0, SM_Reset, SM_waitNoCount } SM_State;

TickFct_State_machine_1() {
   switch(SM_State) { // Transitions
      case -1:
         SM_State = SM_Init;
         break;
         case SM_Init: 
         if (A1 && !A2) {
            SM_State = SM_WaitRiseA0;
         }
         else if (!A1 && !A2) {
            SM_State = SM_waitNoCount;
         }
         else if (A2) {
            SM_State = SM_Reset;
         }
         break;
      case SM_WaitRiseA0: 
         if (A0 && A1 && !A2) {
            SM_State = SM_WaitFallA0;
            B++;
         }
         else if (A2) {
            SM_State = SM_Reset;
         }
         else if (!A1 && !A2) {
            SM_State = SM_waitNoCount;
         }
         break;
      case SM_WaitFallA0: 
         if (A2) {
            SM_State = SM_Reset;
         }
         else if (!A0) {
            SM_State = SM_WaitRiseA0;
         }
         break;
      case SM_Reset: 
         if (A1) {
            SM_State = SM_WaitRiseA0;
         }
         else if (!A1) {
            SM_State = SM_waitNoCount;
         }
         break;
      case SM_waitNoCount: 
         if (!A1 && !A2) {
            SM_State = SM_waitNoCount;
         }
         else if (A2) {
            SM_State = SM_Reset;
         }
         else if (A1 && !A2 && !A0) {
            SM_State = SM_WaitRiseA0;
         }
         else if (A1 && !A2 && A0) {
            SM_State = SM_WaitFallA0;
         }
         break;
      default:
         SM_State = SM_Init;
   } // Transitions

   switch(SM_State) { // State actions
      case SM_Init:
         B= 0;
         break;
      case SM_WaitRiseA0:
         break;
      case SM_WaitFallA0:
         break;
      case SM_Reset:
         B = 0;
         break;
      case SM_waitNoCount:
         break;
      default: // ADD default behaviour below
      break;
   } // State actions

}

int main() {

   
   SM_State = -1; // Initial state
   B = 0; // Init outputs

   while(1) {
      TickFct_State_machine_1();} // while (1)
} // Main