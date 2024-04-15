#include "MPUFunctions.h"
//#include <WiFi.h>

#define NR_OF_LEDS   1
#define NR_OF_ALL_BITS 24
rmt_data_t led_data[NR_OF_ALL_BITS];
rmt_obj_t* rmt_send = NULL;

char buf[128];

void InitLED() {
  if ((rmt_send = rmtInit(18, true, RMT_MEM_64)) != NULL) {
    float realTick = rmtSetTick(rmt_send, 100);
  }
}

void WriteOnBoardPixel(byte red, byte green, byte blue) {
  byte bit, i = 0;
  for (byte color = 0; color < 3; color++) {
    byte curColor;
    if (color == 0) curColor = green;
    else if (color == 1) curColor = red;
    else if (color == 2) curColor = blue;

    byte bitMask = 0x80;
    for (bitMask = 0x80; bitMask; bitMask /= 2) {
      led_data[i].level0 = 1;
      led_data[i].duration0 = (curColor & bitMask) ? 8 : 4;
      led_data[i].level1 = 0;
      led_data[i].duration1 = 8;
      i += 1;
    }
  }

  // Send the data
  rmtWrite(rmt_send, led_data, NR_OF_ALL_BITS);
}





unsigned long lastReport = 0;
boolean memReported = false;
unsigned long outDataLong = 0x00;
byte outDataByte = 0x00;

unsigned short readAddress = 0x1000;


#define DATA_ARRAY_SIZE   8000
unsigned long dataArray0[DATA_ARRAY_SIZE];
unsigned long dataArray1[DATA_ARRAY_SIZE];
unsigned long DataCollected = 0, DataShown = 0;


void setup() {

//  WiFi.mode(WIFI_OFF);
  
  Serial.begin(115200);
  Serial.write("Program start\n");

  
  pinMode(18, OUTPUT);
  
  SetAllLinesToInput();

  Serial.write("Monitor started\n");

  // Make HALT and RESET output
  SetHALTPinDirection(true);
  SetRESETPinDirection(false);
  SetRWPinDirection(true);
  SetVMAPinDirection(true);

  SetClockPinDirection(false);
  SetAddressPinsDirection(true);
  SetDataPinsDirection(false);

  // Halting the 6800
  SetHALT(false);

  InitializeU11PIA();
  TestLightOn();
  unsigned long clockCountStart = millis();
  WaitOneClockCycle(500000);
  unsigned long timeDiff = millis()-clockCountStart;
  char buf[256];
  sprintf(buf, "Took %d ticks\n", timeDiff);
  Serial.write(buf);
  TestLightOff();

  unsigned short curAddress = 0;  

  SetAddressPinsDirection(true);
/*  
  for (curAddress=0x5800; curAddress<0x6000; curAddress++) {
    byte curData[40]; 
    BurstRead(curAddress, curData);
    
    char buf[256];
    sprintf(buf, "0x%04X", curAddress);
    Serial.write(buf);    
    for (int count=0; count<40; count++) {
      sprintf(buf, " 0x%02X", curData[count]);
      Serial.write(buf);
    }
    Serial.write("\n");    
  }
/*
/*
  for (curAddress=0x5800; curAddress<0x6000; curAddress++) {
    byte curData = BSOS_DataRead(curAddress);
      
    char buf[256];
    sprintf(buf, "0x%04X 0x%02X\n\r", curAddress, curData);
    Serial.write(buf);
  }
*/  

  for (curAddress=0x1000; curAddress<0x1900; curAddress++) {
    byte curData = BSOS_DataRead(curAddress);
      
    char buf[256];
    sprintf(buf, "0x%04X 0x%02X\n\r", curAddress, curData);
    Serial.write(buf);
  }
  
  SetAddressPinsDirection(true);

  DataCollected = 0;
  DataShown = 0;
  
}

boolean CRAMStarted = true, CRAMDone = false;
boolean StartCollecting = false;
boolean StartSending = false;
unsigned short LastAddressSeen = 0;
boolean MessageSent = false;
unsigned long LastMessageSent = 2000;

// only a few of the lines are set
//unsigned short LineMask = 0x009B;
unsigned short LineMask = 0xFFFF;


void loop() {


/*
  // Don't start collecting data until the program has started
  if (!StartCollecting && !StartSending) {
    if (GPIO.in & 0x8000) StartCollecting = true;    
  }

  if (StartCollecting && DataCollected<DATA_ARRAY_SIZE) {
    while (StartCollecting && (GPIO.in & 0x00080000)) {
      dataArray0[DataCollected] = GPIO.in;
      dataArray1[DataCollected] = GPIO.in1.val;
      DataCollected += 1;
      if (DataCollected == DATA_ARRAY_SIZE) {
        StartCollecting = false;
        StartSending = true;
      }
    }
  } else if (StartSending) {
    byte curData = 0;
    unsigned short curAddress = 0;
    byte rw = 0, vma = 0, clk = 0, irq = 0, res = 0;
    rw = (dataArray1[DataShown] & 0x00000080) ? 1 : 0;
    vma = (dataArray1[DataShown] & 0x00000040) ? 1 : 0;
    clk = (dataArray1[DataShown] & 0x00000020) ? 1 : 0;
    irq = (dataArray0[DataShown] & 0x04000000) ? 1 : 0;
    res = (dataArray0[DataShown] & 0x00008000) ? 1 : 0;
    curAddress = ((dataArray0[DataShown] & 0x00007F00) >> 8) | ((dataArray0[DataShown] >> 12) & 0x0380) | ((dataArray1[DataShown] & 0x1E) << 9);
//    curAddress &= LineMask;
    curAddress |= ((dataArray0[DataShown] & 0x00010000)>>2);
    curData = (dataArray0[DataShown] & 0x000000FF);

    if (DataShown == 0) Serial.write("ID     Addr   Data C R V I r\n");
    char buf[256];
    sprintf(buf, "%05d  0x%04X 0x%02X %d %d %d %d %d\n", DataShown, curAddress, curData, clk, rw, vma, irq, res);
    Serial.write(buf);
    DataShown += 1;
    if (DataShown == DATA_ARRAY_SIZE) {
      StartSending = false;
      DataCollected = 0;
      DataShown = 0;
    }
  }
*/

  if ((millis()-LastMessageSent)>1000 && !MessageSent) {
    Serial.print("Hello - I'm alive\n");
    LastMessageSent = millis();
    byte toggle = (millis()/1000)%2;
    SetAddressPinsDirection(true);
    SetAddressLines(toggle?0x5800:0x1000);
    if (toggle) TestLightOn();
    else TestLightOff();
/*    pinMode(8, OUTPUT); 
    pinMode(9, OUTPUT);
    pinMode(10, OUTPUT);
    pinMode(11, OUTPUT);
    pinMode(12, OUTPUT);
    pinMode(13, OUTPUT);
    pinMode(14, OUTPUT);
    digitalWrite(8, toggle);
    digitalWrite(9, toggle);
    digitalWrite(10, toggle);
    digitalWrite(11, toggle);
    digitalWrite(12, toggle);
    digitalWrite(13, toggle);
    digitalWrite(14, toggle);
    digitalWrite(40, toggle);
*/
        
  }

  
}
