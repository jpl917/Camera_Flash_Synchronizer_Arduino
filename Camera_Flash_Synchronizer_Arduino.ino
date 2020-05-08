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
//flash8-D62
void triggerFlash(int idx)
{
  digitalWrite(flashPin[idx], LOW);
  delay(5);
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


void burst_mode_backup()
{
  delay(1000);
  int flashForSpecular[8]={0,2,4,5,6,8,9,10};
  for(int flashIdx = 0; flashIdx < 2; flashIdx++)
  {
    for(int i=0; i<24; i++)   { digitalWrite(cameraPin[i], LOW); }
    
    boolean triggerFlag = true;
    boolean listenFlag  = true;
    unsigned long startTime = micros();
    while(listenFlag)
    {
      if((micros()-startTime)/1000 > 300) { 
        Serial.println("Wait Too Long. Quit."); 
        resetAllPins(); 
        return;
      }
      triggerFlag = true;
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
        Serial.println((micros()-startTime)/1000.0);
        triggerFlash(flashForSpecular[flashIdx]);
        listenFlag = false;
      }
    }
    
    resetAllPins();
    delay(10000);
  }
}



void burst_mode()
{
  delay(1000);
  int flashForUniform[4] = {1,3,7,11};
  //int flashForSpecular[9]= {0,10,2,9,-1,4,8,5,6};   //{0,2,4,5,-1,6,8,9,10};//{0,10,2,9,-1,4,8,5,6};
  //int flashForSpecular[13]= {0,10,2,9,4,8,5,6,100,12,13,14,15};
  int flashForSpecular[13]= {2,9,4,8,5,6,100,0,10,12,13,14,15};
  //int flashForSpecular[16]= {1,3,7,11,2,9,4,8,5,6,0,10,12,13,14,15};
  for(int i=0; i<24; i++)   { digitalWrite(cameraPin[i], LOW); }
  
  int flashCount = 0;
  
  unsigned long startTime = micros();
  while((micros()-startTime)/1000 < 1500)
  {
    boolean triggerFlag = true;
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
      if(flashCount > 6) break;
      
      int triggerId = flashForSpecular[flashCount];
      
      //Serial.println("--------------------------------------");
      //Serial.println(triggerId);
      //Serial.println((micros()-startTime)/1000.0);
      
      if (triggerId != 100)
      {
        digitalWrite(flashPin[triggerId], LOW);
        delay(1);
        digitalWrite(flashPin[triggerId], HIGH);
      }
      else
      {
        digitalWrite(flashPin[1],  LOW);
        digitalWrite(flashPin[3],  LOW);
        digitalWrite(flashPin[7],  LOW);
        digitalWrite(flashPin[11], LOW);
        delay(1);
        digitalWrite(flashPin[1],  HIGH);
        digitalWrite(flashPin[3],  HIGH);
        digitalWrite(flashPin[7],  HIGH);
        digitalWrite(flashPin[11], HIGH);
      }
      
      
      //Serial.println((micros()-startTime)/1000.0);
        
        
      flashCount = flashCount+1;
      delay(100);
    }
   
  }
  
  Serial.println();
  Serial.println("----------------Done------------------");
  Serial.println((micros()-startTime)/1000.0);
  for(int i=0; i<24; i++)   { digitalWrite(cameraPin[i], HIGH); }
    
}



void burst_mode_camera_12()
{
  delay(1000);
  int flashForUniform[4] = {1,3,7,11};
  int flashForSpecular[13]= {2,9,4,8,5,6,100,0,10,12,13,14,15};   //{0,2,4,5,-1,6,8,9,10};//{0,10,2,9,-1,4,8,5,6};
  for(int i=12; i<13; i++)   { digitalWrite(cameraPin[i], LOW); }
  
  int flashCount = 0;
  
  unsigned long startTime = micros();
  while((micros()-startTime)/1000 < 1500)
  {
    boolean triggerFlag = true;
    for(int i=12; i<13; i++)
    {
       if(digitalRead(hotshoePin[i]))
        {
          triggerFlag = false;
          break;
        }
    }
    if(triggerFlag)
    {
      if(flashCount > 6) break;
      
      int triggerId = flashForSpecular[flashCount];
      
      Serial.println();
      Serial.println("--------------------------------------");
      Serial.println(triggerId);
      Serial.println((micros()-startTime)/1000.0);
      
      if (triggerId != 100)
      {
        //triggerFlash(triggerId);
        digitalWrite(flashPin[triggerId], LOW);
        delay(5);
        digitalWrite(flashPin[triggerId], HIGH);
      }
      else
      {
        digitalWrite(flashPin[1], LOW);
        digitalWrite(flashPin[3], LOW);
        digitalWrite(flashPin[7], LOW);
        digitalWrite(flashPin[11], LOW);
        delay(5);
        digitalWrite(flashPin[1], HIGH);
        digitalWrite(flashPin[3], HIGH);
        digitalWrite(flashPin[7], HIGH);
        digitalWrite(flashPin[11], HIGH);
      }
      
      
      Serial.println((micros()-startTime)/1000.0);
        
        
      flashCount = flashCount+1;
      delay(100);
    }
   
  }
  
  Serial.println();
  Serial.println("----------------Done------------------");
  Serial.println((micros()-startTime)/1000.0);
  for(int i=12; i<13; i++)   { digitalWrite(cameraPin[i], HIGH); }
    
}



void uniform_illumination()
{
  for(int i=0; i<24; i++)   { digitalWrite(cameraPin[i], LOW); }

  boolean triggerFlag = true;
  boolean listenFlag  = true;
  unsigned long startTime = micros();
  
  while((micros()-startTime)/1000 < 300)
  {
    triggerFlag = true;
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
      for(int i=0; i<12; i++)
        triggerFlash(i);
      
    }
  }
  
  for(int i=0; i<24; i++)   { digitalWrite(cameraPin[i], HIGH); }
  
}


void trigger_all_flashes()
{
   for(int i=0; i<12; i++) triggerFlash(i);
}



void debug_mode_test_flash()
{
  int id = Serial.parseInt();
  
  Serial.print("Function: debug_mode_test_flash -> ");
  Serial.println(id);
  
  triggerFlash(id);
}


void debug_mode_test_flash1()
{
  delay(1000);
  
  for(int id=0; id<12; id++)
  {
    triggerFlash(id);
    delay(2000);
  }
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
      Serial.println("burst mode all cameras");
      burst_mode();
      break;
      
    case 4:
      Serial.println("burst mode camera12");
      burst_mode_camera_12();
      break;
      
    case 5:
      Serial.println("trigger all flashes");
      trigger_all_flashes();
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





