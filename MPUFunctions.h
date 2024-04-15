// D0 = Pin GPIO0
// D1 = Pin GPIO1
// D2 = Pin GPIO2
// D3 = Pin GPIO3
// D4 = Pin GPIO4
// D5 = Pin GPIO5
// D6 = Pin GPIO6
// D7 = Pin GPIO7
// A0 = Pin GPIO8
// A1 = Pin GPIO9
// A2 = Pin GPIO10
// A3 = Pin GPIO11
// A4 = Pin GPIO12
// A5 = Pin GPIO13
// A6 = Pin GPIO14
// RESET = Pin GPIO15
// HALT = Pin GPIO16 
// A7 = Pin GPIO19
// A8 = Pin GPIO20
// A9 = Pin GPIO21
// IRQ = PIN GPIO26
// A10 = Pin GPIO33
// A11 = Pin GPIO34
// A12 = Pin GPIO35
// A13 = Pin GPIO36
// CLOCK = Pin GPIO37
// VMA = Pin GPIO38
// R/W = Pin GPIO39
// A14 = Pin GPIO40

// CLOCK Functions - pin GPIO37
void SetClockPinDirection(boolean output) {
  if (output) GPIO.enable1_w1ts.val = 0x00000020;
  else GPIO.enable1_w1tc.val = 0x00000020;
}

boolean ReadClock() {
  if (GPIO.in1.val&0x0020) return true;
  return false;
}

void SetClock(boolean val) {
  GPIO.enable1_w1ts.val = 0x00000020;
  if (val) {
    GPIO.out1_w1ts.val = 0x00000020;
  } else {
    GPIO.out1_w1tc.val = 0x00000020;    
  }
}


// RESET Functions - pin GPIO15
void SetRESETPinDirection(boolean output) {
  if (output) GPIO.enable_w1ts = 0x0008000;
  else GPIO.enable_w1tc = 0x0008000;
}

boolean ReadRESET() {
  if (GPIO.in & 0x8000) return true;
  return false;
}

void SetRESET(boolean val) {
  if (val) {
    GPIO.out_w1ts = 0x0008000;
  } else {
    GPIO.out_w1tc = 0x0008000;    
  }
}


// HALT Functions - pin GPIO16
void SetHALTPinDirection(boolean output) {
  // Set HALT line to output
  if (output) GPIO.enable_w1ts = 0x00010000;  
}

void SetHALT(boolean val) {  
  if (val) {
    GPIO.out_w1ts = 0x00010000;
  } else {
    GPIO.out_w1tc = 0x00010000;    
  }
}




// VMA Functions - pin GPIO38
void SetVMAPinDirection(boolean output) {
  if (output) GPIO.enable1_w1ts.val = 0x00000040;
  else GPIO.enable1_w1tc.val = 0x00000040;
}

boolean ReadVMA() {  
  if (GPIO.in1.val&0x0040) return true;
  return false;
}

void SetVMA(boolean val) {  
  if (val) {
    GPIO.out1_w1ts.val = 0x00000040;
  } else {
    GPIO.out1_w1tc.val = 0x00000040;    
  }
}


// IRQ Functions - pin GPIO26
void SetIRQPinDirection(boolean output) {
  if (output) GPIO.enable_w1ts = 0x04000000;  
}

// RW Functions - pin GPIO39
void SetRWPinDirection(boolean output) {
  if (output) GPIO.enable1_w1ts.val = 0x00000080;
  else GPIO.enable1_w1tc.val = 0x00000080;
}

boolean ReadRW() {  
  if (GPIO.in1.val & 0x00000080) return true;
  return false;
}

void SetRW(boolean val) {
  if (val) {
    GPIO.out1_w1ts.val = 0x00000080;
  } else {
    GPIO.out1_w1tc.val = 0x00000080;    
  }
}


// Address Functions
void SetAddressPinsDirection(boolean output) {
  if (output) {
    // Set up address lines as output lines
    GPIO.enable_w1ts = 0x00387F00;
    GPIO.enable1_w1ts.val = 0x0000011E;  
// A0 = Pin GPIO8
// A1 = Pin GPIO9
// A2 = Pin GPIO10
// A3 = Pin GPIO11
// A4 = Pin GPIO12
// A5 = Pin GPIO13
// A6 = Pin GPIO14
// A7 = Pin GPIO19
// A8 = Pin GPIO20
// A9 = Pin GPIO21
// A10 = Pin GPIO33
// A11 = Pin GPIO34
// A12 = Pin GPIO35
// A13 = Pin GPIO36
// A14 = Pin GPIO40

    pinMode(8, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(14, OUTPUT);
    pinMode(19, OUTPUT);
    pinMode(20, OUTPUT);
    pinMode(21, OUTPUT);
    pinMode(33, OUTPUT);
    pinMode(35, OUTPUT);
    pinMode(36, OUTPUT);
    pinMode(40, OUTPUT);
  } else {
    // Set up address lines as input
    GPIO.enable_w1tc = 0x00387F00;
    GPIO.enable1_w1tc.val = 0x0000011E;
  }
}

unsigned short ReadAddressLines() {
  unsigned long curLowByte, curHighByte;
  curLowByte = GPIO.in;
  curHighByte = GPIO.in1.val;
  
  unsigned short addressShort;

  // Read current address
  addressShort = ((curLowByte & 0x00007F00)>>8) | ((curLowByte>>12) & 0x0380) | ((curHighByte&0x1E)<<9) | ((curHighByte&0x100)<<6);

  addressShort |= (GPIO.in & 0x10000)>>2;

  return addressShort;
}


void SetAddressLines(unsigned short currentAddress) {
  unsigned long setByte1, setByte2;
  unsigned long clearByte1, clearByte2;

  setByte1 = ((currentAddress & 0x0000007F)<<8) | ((currentAddress & 0x00000380)<<12);
  setByte2 = ((currentAddress & 0x00003C00)>>9) | ((currentAddress & 0x00004000)>>6);
  clearByte1 = ~(0xFFC780FF | setByte1);
  clearByte2 = ~(0xFFFFFEE1 | setByte2);

  // Output the data by setting the 1s and clearing the 0s
  GPIO.out_w1ts = setByte1;
  GPIO.out1_w1ts.val = setByte2;
  GPIO.out_w1tc = clearByte1;
  GPIO.out1_w1tc.val = clearByte2;

}


// Data Functions
void SetDataPinsDirection(boolean output) {
  if (output) {
    // Set data lines to output
    GPIO.enable_w1ts = 0x000000FF;
  } else {
    // Set data lines to input
    GPIO.enable_w1tc = 0x000000FF;
  }
}

byte ReadDataLines() {
  // Read data
  byte dataByte = (GPIO.in & 0x000000FF);

  return dataByte;
}

void SetDataLines(byte outputData) {  
  // Set data
  GPIO.out_w1ts = (unsigned long)outputData;
  GPIO.out_w1tc = ~(0xFFFFFF00 | (unsigned long)outputData);
}

void SetAllLinesToInput() {
  
  pinMode(0, INPUT);
  pinMode(1, INPUT);
  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  pinMode(10, INPUT);
  pinMode(11, INPUT);
  pinMode(12, INPUT);
  pinMode(13, INPUT);
  pinMode(14, INPUT);
  pinMode(15, INPUT);

  
  pinMode(16, INPUT);
  pinMode(19, INPUT);
  pinMode(20, INPUT);
  
  pinMode(21, INPUT);
//  pinMode(26, INPUT);

  pinMode(33, INPUT);
  pinMode(34, INPUT);
  pinMode(35, INPUT);
  pinMode(36, INPUT);
  pinMode(37, INPUT);
  pinMode(38, INPUT);
  pinMode(39, INPUT);  
  
}


void WaitOneClockCycle(unsigned long num=1) {
  for (unsigned long count=0; count<num; count++) {
    while (ReadClock());
    while (!ReadClock());
  }
}


byte BSOS_DataRead(int address) {
  // Wait for low clock
  while ((GPIO.in1.val&0x0020));
  while (!(GPIO.in1.val&0x0020));
  while ((GPIO.in1.val&0x0020));
  
  // Set address lines
  SetAddressLines(address);

  // Make sure we see a rising & then falling edge
//  delayMicroseconds(3);
  while ((GPIO.in1.val&0x0020));

  // Make sure R/W and VMA are high
  GPIO.out1_w1ts.val = 0x00000080;    
  GPIO.out1_w1ts.val = 0x00000040;
  
  while (!(GPIO.in1.val&0x0020));

  for (byte count=0; count<20; count++) {
    __asm__ __volatile__ ("nop\n\t");
  }

  // Collect data
  byte inputData = (GPIO.in & 0x000000FF);

  GPIO.out1_w1tc.val = 0x00000040;
  GPIO.out1_w1tc.val = 0x00000080;

  SetAddressLines(0);

  return inputData;
}

//#define NOP __asm__ __volatile__ ("nop\n\t")

void BurstRead(int address, byte *dataArray) {

  // Wait for low clock
  while ((GPIO.in1.val&0x0020));
  while (!(GPIO.in1.val&0x0020));
  while ((GPIO.in1.val&0x0020));
  
  // Set address lines
  SetAddressLines(address);

  // Make sure we see a rising & then falling edge
//  delayMicroseconds(3);
  while ((GPIO.in1.val&0x0020));

  // Make sure R/W and VMA are high
  GPIO.out1_w1ts.val = 0x00000080;    
  GPIO.out1_w1ts.val = 0x00000040;
  
  while (!(GPIO.in1.val&0x0020));
//  while ((GPIO.in1.val&0x0020));
//  while (!(GPIO.in1.val&0x0020));
/*
  while ((GPIO.in1.val&0x0020));
  while (!(GPIO.in1.val&0x0020));
  while ((GPIO.in1.val&0x0020));
  while (!(GPIO.in1.val&0x0020));
  while ((GPIO.in1.val&0x0020));
  while (!(GPIO.in1.val&0x0020));
  while ((GPIO.in1.val&0x0020));
  while (!(GPIO.in1.val&0x0020));  
  while ((GPIO.in1.val&0x0020));
  while (!(GPIO.in1.val&0x0020));
  while ((GPIO.in1.val&0x0020));
  while (!(GPIO.in1.val&0x0020));
  while ((GPIO.in1.val&0x0020));
*/  

//  NOP; NOP; NOP; NOP; NOP; 
//  NOP; NOP; NOP; NOP; NOP; 
  for (byte count=0; count<20; count++) {
    __asm__ __volatile__ ("nop\n\t");
  }

  // Collect data
  dataArray[0] = (GPIO.in & 0x000000FF);
  dataArray[1] = (GPIO.in & 0x000000FF);
  dataArray[2] = (GPIO.in & 0x000000FF);
  dataArray[3] = (GPIO.in & 0x000000FF);
  dataArray[4] = (GPIO.in & 0x000000FF);
  dataArray[5] = (GPIO.in & 0x000000FF);
  dataArray[6] = (GPIO.in & 0x000000FF);
  dataArray[7] = (GPIO.in & 0x000000FF);
  dataArray[8] = (GPIO.in & 0x000000FF);
  dataArray[9] = (GPIO.in & 0x000000FF);
  dataArray[10] = (GPIO.in & 0x000000FF);
  dataArray[11] = (GPIO.in & 0x000000FF);
  dataArray[12] = (GPIO.in & 0x000000FF);
  dataArray[13] = (GPIO.in & 0x000000FF);
  dataArray[14] = (GPIO.in & 0x000000FF);
  dataArray[15] = (GPIO.in & 0x000000FF);
  dataArray[16] = (GPIO.in & 0x000000FF);
  dataArray[17] = (GPIO.in & 0x000000FF);
  dataArray[18] = (GPIO.in & 0x000000FF);
  dataArray[19] = (GPIO.in & 0x000000FF);
  dataArray[20] = (GPIO.in & 0x000000FF);
  dataArray[21] = (GPIO.in & 0x000000FF);
  dataArray[22] = (GPIO.in & 0x000000FF);
  dataArray[23] = (GPIO.in & 0x000000FF);
  dataArray[24] = (GPIO.in & 0x000000FF);
  dataArray[25] = (GPIO.in & 0x000000FF);
  dataArray[26] = (GPIO.in & 0x000000FF);
  dataArray[27] = (GPIO.in & 0x000000FF);
  dataArray[28] = (GPIO.in & 0x000000FF);
  dataArray[29] = (GPIO.in & 0x000000FF);
  dataArray[30] = (GPIO.in & 0x000000FF);
  dataArray[31] = (GPIO.in & 0x000000FF);
  dataArray[32] = (GPIO.in & 0x000000FF);
  dataArray[33] = (GPIO.in & 0x000000FF);
  dataArray[34] = (GPIO.in & 0x000000FF);
  dataArray[35] = (GPIO.in & 0x000000FF);
  dataArray[36] = (GPIO.in & 0x000000FF);
  dataArray[37] = (GPIO.in & 0x000000FF);
  dataArray[38] = (GPIO.in & 0x000000FF);
  dataArray[39] = (GPIO.in & 0x000000FF);

  GPIO.out1_w1tc.val = 0x00000040;
  GPIO.out1_w1tc.val = 0x00000080;

  SetAddressLines(0);
}

void BSOS_DataWrite(unsigned int address, byte dataByte) {

  // Wait for low clock
  while ((GPIO.in1.val&0x0020));
  while (!(GPIO.in1.val&0x0020));
  while ((GPIO.in1.val&0x0020));
  
  // Set VMA off & set up address lines
  GPIO.out1_w1tc.val = 0x00000040;
  SetAddressLines(address);
  
  // Wait a full clock cycle (low & high)
  while (!(GPIO.in1.val&0x0020));
  while ((GPIO.in1.val&0x0020));

  // Turn off R/W and turn on VMA
  GPIO.out1_w1tc.val = 0x00000080;    
  GPIO.out1_w1ts.val = 0x00000040;

  // Set Data lines to output
  SetDataPinsDirection(true);

  // Set Data lines
  SetDataLines(dataByte);
  
  // Wait a full clock cycle (low & high)
  while (!(GPIO.in1.val&0x0020));
  while ((GPIO.in1.val&0x0020));

  // Turn R/W back on
  GPIO.out1_w1ts.val = 0x00000080;    

  // Set Data lines to output
  SetDataPinsDirection(false);

  // Wait
  WaitOneClockCycle();
}

#define ADDRESS_U10_A           0x88
#define ADDRESS_U10_A_CONTROL   0x89
#define ADDRESS_U10_B           0x8A
#define ADDRESS_U10_B_CONTROL   0x8B
#define ADDRESS_U11_A           0x90
#define ADDRESS_U11_A_CONTROL   0x91
#define ADDRESS_U11_B           0x92
#define ADDRESS_U11_B_CONTROL   0x93


void InitializeU10PIA() {
  // CA1 - Self Test Switch
  // CB1 - zero crossing detector
  // CA2 - NOR'd with display latch strobe
  // CB2 - lamp strobe 1
  // PA0-7 - output for switch bank, lamps, and BCD
  // PB0-7 - switch returns
  Serial.write("Writing 0x38 to 10A CONTROL\n");
  BSOS_DataWrite(ADDRESS_U10_A_CONTROL, 0x38);
  byte U10AControl = BSOS_DataRead(ADDRESS_U10_A_CONTROL);
  char buf[256];
  sprintf(buf, "Got back 0x%02X from 10A CONTROL\n", U10AControl);
  Serial.write(buf);
  // Set up U10A as output
  BSOS_DataWrite(ADDRESS_U10_A, 0xFF);
  // Set bit 3 to write data
  BSOS_DataWrite(ADDRESS_U10_A_CONTROL, BSOS_DataRead(ADDRESS_U10_A_CONTROL)|0x04);
  // Store F0 in U10A Output
  BSOS_DataWrite(ADDRESS_U10_A, 0xF0);
  
  BSOS_DataWrite(ADDRESS_U10_B_CONTROL, 0x33);
  // Set up U10B as input
  BSOS_DataWrite(ADDRESS_U10_B, 0x00);
  // Set bit 3 so future reads will read data
  BSOS_DataWrite(ADDRESS_U10_B_CONTROL, BSOS_DataRead(ADDRESS_U10_B_CONTROL)|0x04);

}

byte DipSwitches[4];


void ReadDipSwitches() {
  byte backupU10A = BSOS_DataRead(ADDRESS_U10_A);
  byte backupU10BControl = BSOS_DataRead(ADDRESS_U10_B_CONTROL);

  // Turn on Switch strobe 5 & Read Switches
  BSOS_DataWrite(ADDRESS_U10_A, 0x20);
  BSOS_DataWrite(ADDRESS_U10_B_CONTROL, backupU10BControl & 0xF7);
  // Wait for switch capacitors to charge
  for (int count=0; count<100; count++) WaitOneClockCycle();
  DipSwitches[0] = BSOS_DataRead(ADDRESS_U10_B);
 
  // Turn on Switch strobe 6 & Read Switches
  BSOS_DataWrite(ADDRESS_U10_A, 0x40);
  BSOS_DataWrite(ADDRESS_U10_B_CONTROL, backupU10BControl & 0xF7);
  // Wait for switch capacitors to charge
  for (int count=0; count<100; count++) WaitOneClockCycle();
  DipSwitches[1] = BSOS_DataRead(ADDRESS_U10_B);

  // Turn on Switch strobe 7 & Read Switches
  BSOS_DataWrite(ADDRESS_U10_A, 0x80);
  BSOS_DataWrite(ADDRESS_U10_B_CONTROL, backupU10BControl & 0xF7);
  // Wait for switch capacitors to charge
  for (int count=0; count<100; count++) WaitOneClockCycle();
  DipSwitches[2] = BSOS_DataRead(ADDRESS_U10_B);

  // Turn on U10 CB2 (strobe 8) and read switches
  BSOS_DataWrite(ADDRESS_U10_A, 0x00);
  BSOS_DataWrite(ADDRESS_U10_B_CONTROL, backupU10BControl | 0x08);
  // Wait for switch capacitors to charge
  for (int count=0; count<100; count++) WaitOneClockCycle();
  DipSwitches[3] = BSOS_DataRead(ADDRESS_U10_B);

  BSOS_DataWrite(ADDRESS_U10_B_CONTROL, backupU10BControl);
  BSOS_DataWrite(ADDRESS_U10_A, backupU10A);
}

byte CurrentSolenoidByte = 0;

void InitializeU11PIA() {
  // CA1 - Display interrupt generator
  // CB1 - test connector pin 32
  // CA2 - lamp strobe 2
  // CB2 - solenoid bank select
  // PA0-7 - display digit enable
  // PB0-7 - solenoid data

  BSOS_DataWrite(ADDRESS_U11_A_CONTROL, 0x31);
  // Set up U11A as output
  BSOS_DataWrite(ADDRESS_U11_A, 0xFF);
  // Set bit 3 to write data
  BSOS_DataWrite(ADDRESS_U11_A_CONTROL, BSOS_DataRead(ADDRESS_U11_A_CONTROL)|0x04);
  // Store 00 in U11A Output
  BSOS_DataWrite(ADDRESS_U11_A, 0x00);
  
  BSOS_DataWrite(ADDRESS_U11_B_CONTROL, 0x30);
  // Set up U11B as output
  BSOS_DataWrite(ADDRESS_U11_B, 0xFF);
  // Set bit 3 so future reads will read data
  BSOS_DataWrite(ADDRESS_U11_B_CONTROL, BSOS_DataRead(ADDRESS_U11_B_CONTROL)|0x04);
  // Store 9F in U11B Output
  BSOS_DataWrite(ADDRESS_U11_B, 0x9F);
  CurrentSolenoidByte = 0x9F;
  
}

void TestLightOn() {
  char buf[128];
  byte oldval = BSOS_DataRead(ADDRESS_U11_A_CONTROL);
  BSOS_DataWrite(ADDRESS_U11_A_CONTROL, 0x35);
  byte newval = BSOS_DataRead(ADDRESS_U11_A_CONTROL);
  sprintf(buf, "U11A Control was 0x%02X and now 0x%02X\n", oldval, newval);
  Serial.write(buf);
}

void TestLightOff() {
  char buf[128];
  byte oldval = BSOS_DataRead(ADDRESS_U11_A_CONTROL);
  BSOS_DataWrite(ADDRESS_U11_A_CONTROL, 0x3D);
  byte newval = BSOS_DataRead(ADDRESS_U11_A_CONTROL);
  sprintf(buf, "U11A Control was 0x%02X and now 0x%02X\n", oldval, newval);
  Serial.write(buf);
}



boolean TestDataLinesForFault() {
  return false;
}


void SetAllAddressLinesToInput() {
  // Set up address lines as input
  GPIO.enable_w1tc = 0x00387F00;
  GPIO.enable1_w1tc.val = 0x0000001E;  
}

void SetAddressLineToOutput(byte lineNum) {

  unsigned short currentAddress = 0x01<<lineNum;
  unsigned long setByte1, setByte2;
  unsigned long clearByte1, clearByte2;

  setByte1 = ((currentAddress & 0x0000007F)<<8) | ((currentAddress & 0x00000380)<<12);
  setByte2 = ((currentAddress & 0x00003C00)>>9);
  clearByte1 = ~(0xFFC780FF | setByte1);
  clearByte2 = ~(0xFFFFFFE1 | setByte2);

  // Set up address lines as output lines
  GPIO.enable_w1ts = setByte1;
  GPIO.enable1_w1ts.val = setByte2;

  // Output the data by setting the 1s and clearing the 0s
  GPIO.out_w1ts = setByte1;
  GPIO.out1_w1ts.val = setByte2;
  GPIO.out_w1tc = clearByte1;
  GPIO.out1_w1tc.val = clearByte2;  
}

boolean ReadSingleAddressLine(byte lineNum) {
  unsigned long curLowByte, curHighByte;
  curLowByte = GPIO.in;
  curHighByte = GPIO.in1.val;
  
  unsigned short addressShort;

  // Read current address
  addressShort = ((curLowByte & 0x00007F00)>>8) | ((curLowByte>>12) & 0x0380) | ((curHighByte&0x1E)<<9);
  addressShort = (addressShort) & (0x01<<lineNum);

  return (addressShort)?true:false;
}


boolean TestAddressLinesForFault() {

  boolean faultFound = false;
  Serial.write("Starting Address Line Check\n");

  for (byte lineNum=0; lineNum<14; lineNum++) {
    SetAllAddressLinesToInput();
    SetAddressLineToOutput(lineNum);
    for (byte readLine=0; readLine<14; readLine++) {
      if (readLine!=lineNum) {
        if (ReadSingleAddressLine(readLine)) {
          char buf[128];
          sprintf(buf, "FAIL: Short between Address line %d and %d\n", lineNum, readLine);
          Serial.write(buf);
          faultFound = true;
        }
      }
    }
  }
  return faultFound;
}
