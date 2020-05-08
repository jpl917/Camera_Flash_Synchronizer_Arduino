# Camera_Flash_Synchronizer_Arduino


# How to use
sudo apt-get install arduino

Tools -> Board -> Arduino Mega 2560

# Problems

if cannot open the serial port, try 'sudo arduino' 


# Pin setting

1. 26 cameraPin:  D2  - D27      (pin13 error, skip. change camera11 from pin13 to pin27)
2. 26 hotshoePin: D28 - D53
3. 16 flashPin:   D54 - D69

[Notice] Serial Communication: RX0(D0) TX0(D1)


# Pin Value Initialization
1. D2  - D27: pinMode(OUTPUT), High
2. D28 - D53: pinMode(INPUT_PULLUP)
3. D54 - D69: pinMode(OUTPUT), HIGH

# How to trigger flash
```
void triggerFlash(int idx)
{
  digitalWrite(flashPin[idx], LOW);
  delay(1);
  digitalWrite(flashPin[idx], HIGH);
}
```

# Environment Light
1-3-7-11
0-2-4-5-6-8-9-10

# How to judge the camera status
```
boolean hotshoeReady(int hotshoeIdx[], int count)
{
  for(int i=0; i<count; i++)
  {
    int pinIdx = hotshoeIdx[i];
    if(digitalRead(hotshoePin[pinIdx])) {return false;}
  }
  return true;
}
```

# Loop Mode
+ 1 -> fyffe configuration
+ 2 -> uniform illumination (24 cameras + 4 lights)
+ 3 -> burst mode all cameras
+ 4 -> burst mode camera 12
+ 5 -> all flashes
+ 100 -> debug_mode_test_flash             (100,0)
+ 101 -> debug_mode_test_hotshoe_time      (101)






