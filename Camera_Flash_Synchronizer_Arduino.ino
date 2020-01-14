
int numCameraPin  = 28;
int numHotshoePin = 28;
int numFlashPin   = 14;

int cameraPin[28];  //0-27
int hotshoePin[28]; //28-55
int flashPin[14];   //56-69


// reset the 
void resetAllPins(){
  /*
  for(int i=0; i<70; i++)
  {  pinMode(i, OUTPUT); 
    digitalWrite(i, HIGH);
    
  }*/
  pinMode(A8, INPUT_PULLUP); 
  //digitalWrite(A8, HIGH);
  pinMode(A9, INPUT_PULLUP); 
  //digitalWrite(A9, HIGH);
  /*
  //camera 
  for(int i=0; i<numCameraPin; i++)
  {
    cameraPin[i]  = i;
    int pinIdx = cameraPin[i];
    pinMode(pinIdx, OUTPUT);  
    digitalWrite(pinIdx, HIGH);
  }
  //hotshoe
  for(int i=0; i<numHotshoePin; i++)
  {
    hotshoePin[i] = i + numCameraPin;
    int pinIdx = hotshoePin[i];
    pinMode(pinIdx, INPUT);  
    digitalWrite(pinIdx, HIGH);
  }
  //flash
  for(int i=0; i<numFlashPin; i++)
  {
    flashPin[i] = i + numCameraPin + numHotshoePin;
    int pinIdx = flashPin[i];
    pinMode(pinIdx, OUTPUT); 
    digitalWrite(pinIdx, HIGH);
  }*/
}


void setup() {
  Serial.begin(115200);
  
  pinMode(A7, OUTPUT); 
  digitalWrite(A7, LOW);
  pinMode(A8, OUTPUT); 
  digitalWrite(A8, LOW);
  pinMode(A9, OUTPUT); 
  digitalWrite(A9, LOW);
  
  //resetAllPins();
}


//0-56, 1-57, ..., 13-69
void triggerFlash(int idx)
{

  digitalWrite(flashPin[idx], LOW);
  delay(1);
  digitalWrite(flashPin[idx], HIGH);
}


void fyffe_configure()
{
  unsigned long startTime = micros();

  //group 0 cameras
  for(int i=0; i<=6; i++)   { 
    digitalWrite(cameraPin[i], LOW);
  }
  delay(15);
  //group 1 cameras
  for(int i=17; i<=23; i++) { 
    digitalWrite(cameraPin[i], LOW);
  }
  delay(15);

  digitalWrite(cameraPin[7],  LOW); 
  digitalWrite(cameraPin[10], LOW); 
  digitalWrite(cameraPin[12], LOW); 
  digitalWrite(cameraPin[14], LOW);
  delay(15);

  digitalWrite(cameraPin[9],  LOW); 
  digitalWrite(cameraPin[11], LOW); 
  digitalWrite(cameraPin[13], LOW); 
  digitalWrite(cameraPin[16], LOW);
  delay(15);

  digitalWrite(cameraPin[8], LOW); 
  delay(15);

  digitalWrite(cameraPin[15], LOW);
  delay(15);

  //wait for hotshoe
  ////digitalWrite(13, HIGH); delay(100); digitalWrite(13, LOW); delay(100);
  //flash 0
  boolean flag = true;
  while(flag)
  {
    if((micros()-startTime)/1000 > 1000) { 
      Serial.println("Flash 0 Wait Too Long. Quit."); 
      resetAllPins(); 
      return;
    }

    if(!digitalRead(hotshoePin[0]) && !digitalRead(hotshoePin[1]) && !digitalRead(hotshoePin[2]) && !digitalRead(hotshoePin[3]) &&  
      !digitalRead(hotshoePin[4]) && !digitalRead(hotshoePin[5]) && !digitalRead(hotshoePin[6]))
    {
      delay(2);
      Serial.println((micros()-startTime)/1000.0);
      if(!digitalRead(hotshoePin[0]) && !digitalRead(hotshoePin[1]) && !digitalRead(hotshoePin[2]) && !digitalRead(hotshoePin[3]) &&  
        !digitalRead(hotshoePin[4]) && !digitalRead(hotshoePin[5]) && !digitalRead(hotshoePin[6]))
      {
        Serial.println((micros()-startTime)/1000.0);
        triggerFlash(0);        
        flag = false;
      }
    }
  }

  //flash 1
  flag = true;
  while(flag)
  {
    if((micros()-startTime)/1000 > 1000) { 
      Serial.println("Flash 1 Wait Too Long. Quit."); 
      resetAllPins(); 
      return;
    }

    if(!digitalRead(hotshoePin[17]) && !digitalRead(hotshoePin[18]) && !digitalRead(hotshoePin[19]) && !digitalRead(hotshoePin[20]) &&  
      !digitalRead(hotshoePin[21]) && !digitalRead(hotshoePin[22]) && !digitalRead(hotshoePin[23]))
    {
      delay(2);
      if(!digitalRead(hotshoePin[17]) && !digitalRead(hotshoePin[18]) && !digitalRead(hotshoePin[19]) && !digitalRead(hotshoePin[20]) &&  
        !digitalRead(hotshoePin[21]) && !digitalRead(hotshoePin[22]) && !digitalRead(hotshoePin[23]))
      {
        triggerFlash(1);        
        flag = false;
      }
    }
  }

  //flash 2
  flag = true;
  while(flag)
  {
    if((micros()-startTime)/1000 > 1000) { 
      Serial.println("Flash 2 Wait Too Long. Quit."); 
      resetAllPins(); 
      return;
    }

    if(!digitalRead(hotshoePin[7]) && !digitalRead(hotshoePin[10]) && !digitalRead(hotshoePin[12]) && !digitalRead(hotshoePin[14]))
    {
      delay(2);
      if(!digitalRead(hotshoePin[7]) && !digitalRead(hotshoePin[10]) && !digitalRead(hotshoePin[12]) && !digitalRead(hotshoePin[14]))
      {
        triggerFlash(2);        
        flag = false;
      }
    }
  }

  //flash 3
  flag = true;
  while(flag)
  {
    if((micros()-startTime)/1000 > 1000) { 
      Serial.println("Flash 3 Wait Too Long. Quit."); 
      resetAllPins(); 
      return;
    }

    if(!digitalRead(hotshoePin[9]) && !digitalRead(hotshoePin[11]) && !digitalRead(hotshoePin[13]) && !digitalRead(hotshoePin[16]))
    {
      delay(2);
      if(!digitalRead(hotshoePin[9]) && !digitalRead(hotshoePin[11]) && !digitalRead(hotshoePin[13]) && !digitalRead(hotshoePin[16]))
      {
        triggerFlash(3);        
        flag = false;
      }
    }
  }

  //flash 4
  flag = true;
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

    default:
      break;

    }//end switch

  }//end while
}//end loop function

