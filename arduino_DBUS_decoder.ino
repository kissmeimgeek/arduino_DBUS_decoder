#include "RemoteControl.h"

byte inputBuffer[18] = { 0 };
byte idleData[] = { 0x0, 0x4, 0x20, 0x0, 0x1, 0xD8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0 }; // [5] can be 0x98, 0xD8, or 0x58
int inputLen = 0;

RC_Ctl_t RC_CtrlData;
char* output = (char*)malloc(100 * sizeof(char));

void serialEvent() {
	if (Serial.available()) {
		inputLen = Serial.readBytes(inputBuffer, 18);
	}
}

void setup()
{
	Serial.begin(100000, SERIAL_8E1);
	while (!Serial.available()) {};
}

void loop()
{
	if (Serial.available()) {
		if (inputLen == 18) {
			//TODO - Verify data

      int breakable, numPop = 0
			while (numPop < 18) {
        breakable = 1;
        if (inputBuffer[0+numPop] != 0x0 || inputBuffer[(1+numPop)%18] != 0x4 || inputBuffer[(2+numPop)%18] != 0x20 || inputBuffer[(3+numPop)%18] != 0x0 || inputBuffer[(4+numPop)%18] != 0x1) {
          breakable = 0;
        }
        if (inputBuffer[(5+numPop)%18] != 0xD8 || inputBuffer[(5+numPop)%18] != 0x98 || inputBuffer[(5+numPop)%18] != 0x58) {
          breakable = 0;
        }
        for (int i = numPop+6; i < numPop+18; i++) {
          if (inputBuffer[(i)%18] != 0x0) {
            breakable = 0
          }
        }
        if (breakable) {
          break;
        }
        numPop++;
			}

     for (int i = 0; i < numPop; i++) {
      Serial.read();
     }

     

			RC_CtrlData.rc.ch0 = ((int16_t)inputBuffer[0] | ((int16_t)inputBuffer[1] << 8)) & 0x07FF;
			RC_CtrlData.rc.ch1 = (((int16_t)inputBuffer[1] >> 3) | ((int16_t)inputBuffer[2] << 5)) & 0x07FF;
			RC_CtrlData.rc.ch2 = (((int16_t)inputBuffer[2] >> 6) | ((int16_t)inputBuffer[3] << 2) |
				((int16_t)inputBuffer[4] << 10)) & 0x07FF;
			RC_CtrlData.rc.ch3 = (((int16_t)inputBuffer[4] >> 1) | ((int16_t)inputBuffer[5] << 7)) & 0x07FF;

			RC_CtrlData.rc.s1 = ((inputBuffer[5] >> 4) & 0x000C) >> 2;
			RC_CtrlData.rc.s2 = ((inputBuffer[5] >> 4) & 0x0003);

			RC_CtrlData.mouse.x = ((int16_t)inputBuffer[6]) | ((int16_t)inputBuffer[7] << 8);
			RC_CtrlData.mouse.y = ((int16_t)inputBuffer[8]) | ((int16_t)inputBuffer[9] << 8);
			RC_CtrlData.mouse.z = ((int16_t)inputBuffer[10]) | ((int16_t)inputBuffer[11] << 8);

			RC_CtrlData.mouse.press_l = inputBuffer[12];
			RC_CtrlData.mouse.press_r = inputBuffer[13];

			RC_CtrlData.key.v = ((uint16_t)inputBuffer[14]) | ((uint16_t)inputBuffer[15] << 8);

			for (int i = 0; i < 18; i++) {
				Serial.print(inputBuffer[i], HEX);
				Serial.print(" ");
			}
			sprintf(output, "C0: %d, C1: %d, C2: %d, C3: %d, S1: %d, S2: %d, X: %d, Y: %d, Z: %d, L: %d, R: %d, K: %d", RC_CtrlData.rc.ch0, RC_CtrlData.rc.ch1, RC_CtrlData.rc.ch2, RC_CtrlData.rc.ch3, RC_CtrlData.rc.s1, RC_CtrlData.rc.s2, RC_CtrlData.mouse.x, RC_CtrlData.mouse.y, RC_CtrlData.mouse.z, RC_CtrlData.mouse.press_l, RC_CtrlData.mouse.press_r, RC_CtrlData.key.v);
			Serial.println(output);
			inputLen = 0;
		}
	}
}
