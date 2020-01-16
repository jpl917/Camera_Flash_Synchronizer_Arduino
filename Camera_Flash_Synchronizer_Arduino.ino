/*
* Serial Communication: RX0(D0) TX0(D1)
* 27 Camera:  D2-D28      
* 27 Hotshoe: D29-D55 
* 14 Flash:   D56-D69  (=> A2-A15)
*/

int numCameraPin  = 27;
int numHotshoePin = 27;
int numFlashPin   = 14;

int cameraPin[27];  //2-28
int hotshoePin[27]; //29-55
int flashPin[14];   //56-69

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
  for(int i=0; i<numHotshoePin; i++) {hotshoePin[i] = i + 2 + numCameraPin;}
  for(int i=0; i<numFlashPin; i++)   {flashPin[i] = i + 2 + numCameraPin + numHotshoePin;}
    
  resetAllPins();
}


//flash0-D56, flash1-D57, ..., flash13-D69
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
    if(!digitalRead(hotshoePin[pinIdx])) {return false;}
  }
  return true;
}


void fyffe_configure()
{
  int group0[7] = {0,1,2,3,4,5,6};
  int group1[7] = {17,18,19,20,21,22,23};
  int group2[4] = {7,10,12,14};
  int group3[4] = {9,11,13,16};

  for(int i=0; i<7; i++) {digitalWrite(cameraPin[group0[i]], LOW);}
  delay(15);
  for(int i=0; i<7; i++) {digitalWrite(cameraPin[group1[i]], LOW);}
  delay(15);
  for(int i=0; i<4; i++) {digitalWrite(cameraPin[group2[i]], LOW);}
  delay(15);
  for(int i=0; i<4; i++) {digitalWrite(cameraPin[group3[i]], LOW);}
  delay(15);
  digitalWrite(cameraPin[8], LOW); 
  delay(15);
  digitalWrite(cameraPin[15], LOW);
  delay(15);

  //flash0
  boolean flag = true; 
  unsigned long startTime = micros(); 
  while(flag)
  {
    if((micros()-startTime)/1000 > 1000) { 
      Serial.println("Flash 0 Wait Too Long. Quit."); 
      resetAllPins(); 
      return;
    }
    
    if(hotshoeReady(group0, 7))
    {
      delay(2);
      if(hotshoeReady(group0, 7))
      {
        triggerFlash(0);        
        flag = false;
      }
    }
  }

  //flash 1
  flag = true;
  startTime = micros(); 
  while(flag)
  {
    if((micros()-startTime)/1000 > 1000) { 
      Serial.println("Flash 1 Wait Too Long. Quit."); 
      resetAllPins(); 
      return;
    }

    if(hotshoeReady(group1, 7))
    {
      delay(2);
      if(hotshoeReady(group1, 7))
      {
        triggerFlash(1);        
        flag = false;
      }
    }
  }

  //flash 2
  flag = true;
  startTime = micros(); 
  while(flag)
  {
    if((micros()-startTime)/1000 > 1000) { 
      Serial.println("Flash 2 Wait Too Long. Quit."); 
      resetAllPins(); 
      return;
    }

    if(hotshoeReady(group2, 4))
    {
      delay(2);
      if(hotshoeReady(group2, 4))
      {
        triggerFlash(2);        
        flag = false;
      }
    }
  }

  //flash 3
  flag = true;
  startTime = micros(); 
  while(flag)
  {
    if((micros()-startTime)/1000 > 1000) { 
      Serial.println("Flash 3 Wait Too Long. Quit."); 
      resetAllPins(); 
      return;
    }

    if(hotshoeReady(group3, 4))
    {
      delay(2);
      if(hotshoeReady(group3, 4))
      {
        triggerFlash(3);        
        flag = false;
      }
    }
  }

  //flash 4
  flag = true;
  startTime = micros(); 
  while(flag)
  {
    if((micros()-startTime)/1000 > 1000) { 
      Serial.println("Flash 4 Wait Too Long. Quit."); 
      resetAllPins(); 
      return;
    }

    if(!digitalRead(hotshoePin[8]))
    {
      delay(2);
      if(!digitalRead(hotshoePin[8]))
      {
        triggerFlash(4);        
        flag = false;
      }
    }
  }

  //flash 5
  flag = true;
  startTime = micros(); 
  while(flag)
  {
    if((micros()-startTime)/1000 > 1000) { 
      Serial.println("Flash 5 Wait Too Long. Quit."); 
      resetAllPins(); 
      return;
    }

    if(!digitalRead(hotshoePin[15]))
    {
      delay(2);
      if(!digitalRead(hotshoePin[15]))
      {
        triggerFlash(5);        
        flag = false;
      }
    }
  }

  //finish
  delay(200);
  resetAllPins();
}


void uniform_lighting()
{
  for(int i=0; i<=numCameraPin; i++)   { digitalWrite(cameraPin[i], LOW); }

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
      digitalWrite(flashPin[8], LOW); 
      digitalWrite(flashPin[9], LOW); 
      digitalWrite(flashPin[10], LOW); 
      digitalWrite(flashPin[11], LOW);
      delay(1);
      digitalWrite(flashPin[8], HIGH);
      digitalWrite(flashPin[9], HIGH);
      digitalWrite(flashPin[10], HIGH);
      digitalWrite(flashPin[11], HIGH);

      listenFlag = false;
    }
  }

  resetAllPins();


}

void burst_mode()
{
  
  for(int flashIdx = 0; flashIdx < 6; flashIdx++)
  {
    for(int i=0; i<=numCameraPin; i++)   { digitalWrite(cameraPin[i], LOW); }
    
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
      Serial.println("uniform lighting");
      uniform_lighting();
      break;

    case 3: 
      Serial.println("dense capture configuration");
      burst_mode();
      break;
      
    case 100:   //100,4,100
      idx_debug = Serial.parseInt();
      delaytime_debug = Serial.parseInt();
      digitalWrite(idx_debug, LOW);
      delay(delaytime_debug);
      digitalWrite(idx_debug, HIGH);
      
      Serial.println(idx_debug);
      Serial.println(delaytime_debug);
      
    default:
      break;

    }//end switch

  }//end while
}//end loop function





