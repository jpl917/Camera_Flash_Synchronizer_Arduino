/*
* Serial Communication: RX0(D0) TX0(D1)
* 26 Camera:  D2-D27      
* 26 Hotshoe: D28-D53 
* 16 Flash:   D54-D69  (=> A0-A15)
*/

int numCameraPin  = 26;
int numHotshoePin = 26;
int numFlashPin   = 16;

int cameraPin[26];  //2-27   //NOTICE: pin13 error, skip. change camera11 from pin13 to pin 27
int hotshoePin[26]; //28-53
int flashPin[16];   //54-69

/*note
delayMicroseconds(us) ;
*/

/**
void resetAllPins();

*/

// reset the 
void resetAllPins(){
  //camera 
  for(int i=0; i<numCameraPin; i++)
  {
    int pinIdx = cameraPin[i];
    pinMode(pinIdx, OUTPUT);  
    digitalWrite(pinIdx, HIGH);
  }
  //hotshoe
  for(int i=0; i<numHotshoePin; i++)
  {
    int pinIdx = hotshoePin[i];
    pinMode(pinIdx, INPUT_PULLUP);  
  }
  //flash
  for(int i=0; i<numFlashPin; i++)
  {
    int pinIdx = flashPin[i];
    pinMode(pinIdx, OUTPUT); 
    digitalWrite(pinIdx, HIGH);
  }
}


void setup() {
  Serial.begin(115200);
  for(int i=0; i<numCameraPin; i++)  {cameraPin[i] = i + 2;}
  cameraPin[11] = 27; //NOTICE
  for(int i=0; i<numHotshoePin; i++) {hotshoePin[i] = i + 2 + numCameraPin;}
  for(int i=0; i<numFlashPin; i++)   {flashPin[i] = i + 2 + numCameraPin + numHotshoePin;}
    
    
  resetAllPins();
}


//flash0-D54, flash1-D55, ..., flash13-D67
void triggerFlash(int idx)
{
  digitalWrite(flashPin[idx], LOW);
  delay(1);
  digitalWrite(flashPin[idx], HIGH);
}


boolean hotshoeReady(int hotshoeIdx[], int count)
{
  for(int i=0; i<count; i++)
  {
    int pinIdx = hotshoeIdx[i];
    if(digitalRead(hotshoePin[pinIdx])) {return false;}
  }
  return true;
}


void fyffe_configure()
{
  
  int group0[7] = {0,1,2,3,4,5,6};
  int group1[7] = {17,18,19,20,21,22,23};
  int group2[4] = {7,10,12,14};
  int group3[4] = {9,11,13,16};
  int group4[1] = {8};
  int group5[1] = {15};
  
  int flashIdxs[6] = {2,9,5,6,4,8};
  
  int camera_skip_time = 20;


  for(int i=0; i<7; i++) {digitalWrite(cameraPin[group0[i]], LOW);}
  delay(camera_skip_time);
  for(int i=0; i<7; i++) {digitalWrite(cameraPin[group1[i]], LOW);}
  delay(camera_skip_time);
  for(int i=0; i<4; i++) {digitalWrite(cameraPin[group2[i]], LOW);}
  delay(camera_skip_time);
  for(int i=0; i<4; i++) {digitalWrite(cameraPin[group3[i]], LOW);}
  delay(camera_skip_time);
  for(int i=0; i<1; i++) {digitalWrite(cameraPin[group4[i]], LOW);} 
  delay(camera_skip_time);
  for(int i=0; i<1; i++) {digitalWrite(cameraPin[group5[i]], LOW);}

  //flash 0
  boolean listenFlag = true; 
  int flashIdx = flashIdxs[0];
  unsigned long startTime = micros(); 
  while(listenFlag)
  {
    if((micros()-startTime)/1000 > 1000) { 
      Serial.println("Flash 0 Wait Too Long. Quit."); 
      resetAllPins(); 
      return;
    }    
    if(hotshoeReady(group0, 7))
    {
        triggerFlash(flashIdx);        
        listenFlag = false;
    }
  }
  

  //flash 1
  listenFlag = true;
  flashIdx = flashIdxs[1];
  startTime = micros(); 
  while(listenFlag)
  {
    if((micros()-startTime)/1000 > 1000) { 
      Serial.println("Flash 1 Wait Too Long. Quit."); 
      resetAllPins(); 
      return;
    }

    if(hotshoeReady(group1, 7))
    {
       triggerFlash(flashIdx);        
       listenFlag = false;
    }
  }
  
  
  //flash 2
  listenFlag = true;
  flashIdx = flashIdxs[2];
  startTime = micros(); 
  while(listenFlag)
  {
    if((micros()-startTime)/1000 > 1000) { 
      Serial.println("Flash 2 Wait Too Long. Quit."); 
      resetAllPins(); 
      return;
    }

    if(hotshoeReady(group2, 4))
    {
       triggerFlash(flashIdx);        
       listenFlag = false;
    }
  }
  
  //flash 3
  listenFlag = true;
  flashIdx = flashIdxs[3];
  startTime = micros(); 
  while(listenFlag)
  {
    if((micros()-startTime)/1000 > 1000) { 
      Serial.println("Flash 3 Wait Too Long. Quit."); 
      resetAllPins(); 
      return;
    }

    if(hotshoeReady(group3, 4))
    {
       triggerFlash(flashIdx);        
       listenFlag = false;
    }
  }
  
  //flash 4
  listenFlag = true;
  flashIdx = flashIdxs[4];
  startTime = micros(); 
  while(listenFlag)
  {
    if((micros()-startTime)/1000 > 1000) { 
      Serial.println("Flash 4 Wait Too Long. Quit."); 
      resetAllPins(); 
      return;
    }

    if(hotshoeReady(group4, 1))
    {
       triggerFlash(flashIdx);        
       listenFlag = false;
    }
  }
  
  
  //flash 5
  listenFlag = true;
  flashIdx = flashIdxs[5];
  startTime = micros(); 
  while(listenFlag)
  {
    if((micros()-startTime)/1000 > 1000) { 
      Serial.println("Flash 5 Wait Too Long. Quit."); 
      resetAllPins(); 
      return;
    }

    if(hotshoeReady(group5, 1))
    {
       triggerFlash(flashIdx);        
       listenFlag = false;
    }
  }
  
  

  //finish
  delay(200);
  resetAllPins();
}


void burst_mode()
{
  for(int flashIdx = 0; flashIdx < 6; flashIdx++)
  {
    for(int i=0; i<24; i++)   { digitalWrite(cameraPin[i], LOW); }
    
    boolean triggerFlag = false;
    boolean listenFlag  = true;
    unsigned long startTime = micros();
    while(listenFlag)
    {
      if((micros()-startTime)/1000 > 1000) { 
        Serial.println("Wait Too Long. Quit."); 
        resetAllPins(); 
        return;
      }
      for(int i=0; i<numHotshoePin; i++) 
      {
        if(digitalRead(hotshoePin[i]))
        {
          triggerFlag = false;
          break;
        }
      }
      if(triggerFlag)
      {
        delay(2);
        triggerFlash(flashIdx);
        listenFlag = false;
      }
    }
    
    resetAllPins();
    delay(200);
  }
}



void uniform_illumination()
{
  for(int i=0; i<24; i++)   { digitalWrite(cameraPin[i], LOW); }

  boolean triggerFlag = true;
  boolean listenFlag  = true;
  unsigned long startTime = micros();
  
  while(listenFlag)
  {
    triggerFlag = true;
    if((micros()-startTime)/1000 > 1000) { 
      Serial.println("Wait Too Long. Quit."); 
      resetAllPins(); 
      return;
    }
    for(int i=0; i<24; i++) 
    {
      if(digitalRead(hotshoePin[i]))
      {
        triggerFlag = false;
        break;
      }
    }
    if(triggerFlag)
    {
      triggerFlash(1);
      triggerFlash(3);
      triggerFlash(7);
      triggerFlash(11);
      listenFlag = false;
    }
  }
  delay(200);
  resetAllPins();
}



void debug_mode_test_flash()
{
  int id = Serial.parseInt();
  
  Serial.print("Function: debug_mode_test_flash -> ");
  Serial.println(id);
  
  triggerFlash(id);
}


void debug_mode_test_hotshoe_time()
{
  Serial.println("Function: debug_mode_print_hotshoe_time()");
  for(int i=0; i<24; i++)
  {
    unsigned long start_time, end_time;
    start_time = micros();
    
    digitalWrite(cameraPin[i], LOW);
    
    while(1)
    {
      if((micros()-start_time)/1000 > 1000) { 
        Serial.println("Wait Too Long. Quit."); 
        break;
      }
      if(!digitalRead(hotshoePin[i]))
      {
        end_time = micros();
        break;
      }
    }
    Serial.print(i);
    Serial.print(" :");
    Serial.println((end_time-start_time)/1000.0);

    delay(200);
    digitalWrite(cameraPin[i], HIGH);
  }
}



void loop() 
{
  unsigned long time = micros();  
  
  int idx_debug;
  int delaytime_debug;
  

  while (Serial.available() > 0)
  {
    int choice = Serial.parseInt();
    switch(choice)
    {
    case 1:
      Serial.println("fyffe configuration");
      fyffe_configure();
      break;

    case 2:
      Serial.println("uniform illumination");
      uniform_illumination();
      break;

    case 3: 
      Serial.println("dense capture configuration 7x24");
      burst_mode();
      break;
      
    case 100:   
      debug_mode_test_flash();
      break;
      
    case 101:
      debug_mode_test_hotshoe_time();
      break;
      
      
    default:
      break;

    }//end switch

  }//end while
  
  
}//end loop function





