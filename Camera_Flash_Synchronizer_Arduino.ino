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


//flash0-D54, flash1-D55, ..., flash8-D62, ..., flash13-D67
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
    if(pinIdx == -1) continue;
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


void uniform_configure()
{
  delay(1000);
  int group[32] = {0,1,-1,-1,   22,23,-1,-1,   2,3,4,-1,   19,20,21,-1,   5,6,8,-1,   15,17,18,-1,   7,10,12,14,   9,11,13,16};  //8*4
  int flashForSpecular[8]= {0,10,2,9,4,8,5,6};
  
 
  for(int i=0; i<24; i++)   { digitalWrite(cameraPin[i], LOW); }
  delay(400);
  for(int i=0; i<24; i++)   { digitalWrite(cameraPin[i], HIGH); } 
  delay(200);
  resetAllPins();
  return;
}

void uniform_fyffe_configure()
{
  delay(1000);
  int group[32] = {0,1,-1,-1,   22,23,-1,-1,   2,3,4,-1,   19,20,21,-1,   5,6,8,-1,   15,17,18,-1,   7,10,12,14,   9,11,13,16};  //8*4
  int flashForSpecular[8]= {0,10,2,9,4,8,5,6};
  
  if(false){
 
    for(int i=0; i<24; i++)   { digitalWrite(cameraPin[i], LOW); }
    unsigned long startTime = micros();
    while((micros()-startTime)/1000 < 300)
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
    }
    for(int i=0; i<24; i++)   { digitalWrite(cameraPin[i], HIGH); } 
 
  
  delay(5000);
  }
  
  if(true)
  {
    // for flash
    Serial.println(micros()/1000);
    int camera_skip_time = 20;
    for(int i=0; i<8; i++)
    {
      for(int j=0; j<4; j++)
      {
        int idx = group[4*i+j];
        if(idx == -1) continue;
        digitalWrite(cameraPin[idx], LOW);
      }
      if(i!=7) delay(camera_skip_time);
    }
    Serial.println(micros()/1000);
    
    
    boolean triggerFlag = true;
    unsigned long startTime = micros();
    while((micros()-startTime)/1000 < 400)
    {
      triggerFlag = true;
      for(int j=0; j<4; j++)
      {
        int idx = group[j];
        if(idx == -1) continue;
        if(digitalRead(hotshoePin[idx])) {triggerFlag = false;}
      }
      if(triggerFlag)
      {
        Serial.print("0: ");
        Serial.print(micros()/1000);
        
        int idx = flashForSpecular[0];
        
        Serial.print(" -> flash:  ");
        Serial.println(idx);
        
        digitalWrite(flashPin[idx], LOW);
        delay(1);
        digitalWrite(flashPin[idx], HIGH);
        
        for(int j=0; j<4; j++)
        {
          int idx = group[j];
          if(idx == -1) continue;
          digitalWrite(cameraPin[idx], HIGH);
        }
        break;
      }
    }
    
    //if(triggerFlag == false){
    //  Serial.println("Error");
    //  resetAllPins();
    //  return;
    //}
    
    
    for(int i=1; i<8; i++)
    {
      unsigned long startTime = micros();
      while((micros()-startTime)/1000 < 25)
      {
        boolean triggerFlag = true;
        for(int j=0; j<4; j++)
        {
          int idx = group[4*i+j];
          if(idx == -1) continue;
          if(digitalRead(hotshoePin[idx])) {triggerFlag = false;}
        }
        
        if(triggerFlag)
        {
          Serial.print(i);
          Serial.print(": ");
          Serial.print(micros()/1000);
          
          int idx = flashForSpecular[i];
          
          Serial.print(" -> flash:  ");
          Serial.println(idx);
          
          digitalWrite(flashPin[idx], LOW);
          delay(1);
          digitalWrite(flashPin[idx], HIGH);
          
          for(int j=0; j<4; j++)
          {
            int idx = group[4*i+j];
            if(idx == -1) continue;
            digitalWrite(cameraPin[idx], HIGH);
          }
          
          break;
        }
      }
    }
  }
  
  resetAllPins();
  return;
}

void all_fyffe_configure()
{
  delay(1000);
  int group_geo[8] = {1,5,6,10,13,17,18,23}; // 6 cameras under uniform lighting, used for geometry
  int flashForDiffuse[4] = {0,3,8,10};
  
  //later 18 cameras under fyffe lighting, used for material
  int group_mat[24] = {-1,3,4,  0,2,-1,  -1,8,11,  7,-1,14,  9,-1,16,  12,-1,15,  19,-1,22,  20,21,-1};  //8*3
  int flashForSpecular[8]= {1,2,4,5,6,7,9,11};
  
  
  if(true)
  {
    // for flash
    Serial.println(micros()/1000);
    int camera_skip_time = 20;
    for(int i=0; i<8; i++){
      int idx = group_geo[i];
      digitalWrite(cameraPin[idx], LOW);
    }
    delay(camera_skip_time);
    
    for(int i=0; i<8; i++)
    {
      for(int j=0; j<3; j++)
      {
        int idx = group_mat[3*i+j];
        if(idx == -1) continue;
        digitalWrite(cameraPin[idx], LOW);
      }
      if(i!=7) delay(camera_skip_time);
    }
    Serial.println(micros()/1000);
    
    //flash 0 wating for the signal of camera shutter
    boolean triggerFlag = true;
    unsigned long startTime = micros();
    while((micros()-startTime)/1000 < 400)
    {
      triggerFlag = true;
      for(int j=0; j<8; j++)
      {
        int idx = group_geo[j];
        if(idx == -1) continue;
        if(digitalRead(hotshoePin[idx])) {triggerFlag = false;}
      }
      if(triggerFlag)
      {
        Serial.print("0: ");
        Serial.print(micros()/1000);
        Serial.print(" -> flash:  ");
        for(int j=0; j<4; j++){
          int idx = flashForDiffuse[j];
          Serial.print(idx);
          Serial.print(" ");
          digitalWrite(flashPin[idx], LOW);
          delay(1);
          digitalWrite(flashPin[idx], HIGH);
        }
        Serial.println("");
        for(int j=0; j<8;j++){
          int idx = group_geo[j];
          digitalWrite(cameraPin[idx], HIGH);
        }
        
        break;
      }
    }
    
    if(triggerFlag == false){
      Serial.println("Error");
      resetAllPins();
      return;
    }
    
    //camera 0 shutter signal arriving, 
    for(int i=0; i<8; i++)
    {
      
      startTime = micros();
      
      while((micros()-startTime)/1000 < 25)
      {
        triggerFlag = true;
        
        for(int j=0; j<3; j++)
        {
          int idx = group_mat[3*i+j];
          if(idx == -1) continue;
          if(digitalRead(hotshoePin[idx])) {triggerFlag = false;}
        }
        delay(1);
        
        if(triggerFlag)
        {
          Serial.print(i+1);
          Serial.print(": ");
          Serial.print(micros()/1000);
          
          int idx = flashForSpecular[i];
          
          Serial.print(" -> flash:  ");
          Serial.println(idx);
          
          digitalWrite(flashPin[idx], LOW);
          delay(1);
          digitalWrite(flashPin[idx], HIGH);
          
          for(int j=0; j<3; j++)
          {
            int idx = group_mat[3*i+j];
            if(idx == -1) continue;
            digitalWrite(cameraPin[idx], HIGH);
          }
          
          break;
        }
      }
    }
  }
  
  resetAllPins();
  return;
}

void burst_mode()
{
  delay(1000);
  int flashForUniform = 4;
  
  for(int i=0; i<24; i++)   { digitalWrite(cameraPin[i], LOW); }
  delay(20);
  digitalWrite(flashPin[flashForUniform], LOW);
  delay(1);
  digitalWrite(flashPin[flashForUniform], HIGH);
  delay(280);
  for(int i=0; i<24; i++)   { digitalWrite(cameraPin[i], HIGH); }
  
  
  int flashCount = 0;
  
  unsigned long startTime = micros();
  //while((micros()-startTime)/1000 < 1000)
  //{
  //  boolean triggerFlag = true;
  //  for(int i=0; i<24; i++)
  //  {
  //     if(digitalRead(hotshoePin[i]))
  //      {
  //        triggerFlag = false;
  //        break;
  //      }
  //  }
  //  if(triggerFlag)
  //  {
      //if(flashCount > 6) break;
  //    for(int j=0; j<4; j++){
  //      int idx = flashForUniform[j];
  //      digitalWrite(flashPin[idx], LOW);
  //      delay(1);
  //      digitalWrite(flashPin[idx], HIGH);
  //    }
  //    Serial.println("okay");
  //    for(int i=0; i<24; i++)   { digitalWrite(cameraPin[i], HIGH); }

  //    break;
  //  }
      //int triggerId = flashForUniform[flashCount];
      
      //if (triggerId != 100)
      //{
      //  digitalWrite(flashPin[triggerId], LOW);
      //  delay(1);
      //  digitalWrite(flashPin[triggerId], HIGH);
      //}
      //else
      //{
      //  digitalWrite(flashPin[1],  LOW);
      //  digitalWrite(flashPin[3],  LOW);
      //  digitalWrite(flashPin[7],  LOW);
      //  digitalWrite(flashPin[11], LOW);
      //  delay(1);
      //  digitalWrite(flashPin[1],  HIGH);
      //  digitalWrite(flashPin[3],  HIGH);
      //  digitalWrite(flashPin[7],  HIGH);
      //  digitalWrite(flashPin[11], HIGH);
      //}
      
      
      //Serial.println((micros()-startTime)/1000.0);
        
        
      //flashCount = flashCount+1;
      //delay(100);
  //}
  //for(int i=0; i<24; i++)   { digitalWrite(cameraPin[i], HIGH); }
  
  Serial.println();
  Serial.println("----------------Done------------------");
  Serial.println((micros()-startTime)/1000.0);
  resetAllPins();
  return;
    
}



void debug_mode_test_flash()
{
  int id = Serial.parseInt();
  Serial.println(id);
  
  triggerFlash(id);
}


void debug_mode_test_flash_seq()
{
  delay(1000);
  
  for(int id=0; id<12; id++)
  {
    Serial.print("Function: debug_mode_test_flash -> ");
    Serial.println(id);
    triggerFlash(id);
    delay(2000);
  }
}


void debug_mode_test_camera()
{
  int id = Serial.parseInt();
  Serial.println(id);
  
  digitalWrite(cameraPin[id], LOW);
  delay(200);
  digitalWrite(cameraPin[id], HIGH);
   
}


void debug_mode_test_camera_seq()
{
  int id = Serial.parseInt();
  Serial.println(id);
  
  for(int i=id; i<id+1; i++)
  {
    unsigned long start_time, end_time;
    start_time = micros();
    end_time = start_time;
    
    digitalWrite(cameraPin[i], LOW);
    
    
    while(1)
    {
      if((micros()-start_time)/1000 > 300) { 
        Serial.println("Wait Too Long. Quit."); 
        break;
      }
      if(!digitalRead(hotshoePin[i]))
      {
        end_time = micros();
        break;
      }
    }
    Serial.print("Camera: ");
    Serial.print(i);
    Serial.print(" :");
    Serial.println((end_time-start_time)/1000.0);
    digitalWrite(cameraPin[i], HIGH);
    delay(1000);
  }
}

void video()
{
  for(int idx=0; idx<24; idx++) digitalWrite(cameraPin[idx], LOW);
  delay(200);
  for(int idx=0; idx<24; idx++) digitalWrite(cameraPin[idx], HIGH);
  
  delay(60000);
  
  for(int idx=0; idx<24; idx++) digitalWrite(cameraPin[idx], LOW);
  delay(200);
  for(int idx=0; idx<24; idx++) digitalWrite(cameraPin[idx], HIGH);
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
    case 0:
      Serial.println("uniform lighting");
      uniform_configure();
      break;
      
    case 1:    // not burst mode: 24 images
      Serial.println("fyffe configuration (not burst mode)");
      fyffe_configure();
      break;
      
    case 2:    // burst mode: 24 + 24 images
      Serial.println("uniform lighting with fyffe configuration (burst mode)");
      uniform_fyffe_configure();
      //all_fyffe_configure();
      break;
  
    case 3:    // burst mode: 144 + 24 images 
      Serial.println("burst mode all cameras");
      burst_mode();
      break;
      
    case 100:   
      Serial.print("Function: debug_mode_test_flash -> ");
      debug_mode_test_flash();
      break;
      
    case 101:
      Serial.print("Function: debug_mode_test_flash sequence");
      //debug_mode_test_flash_seq();
      break;
      
    case 102:
      Serial.print("Function: debug_mode_test_camera -> ");
      debug_mode_test_camera();
      break;
      
    case 103:
      Serial.println("Function: debug_mode_test_camera sequence");
      debug_mode_test_camera_seq();
      break;
      
    case 104:
      Serial.println("Video");
      video();
      break;
      
      
    default:
      break;

    }//end switch

  }//end while
  
  
}//end loop function





