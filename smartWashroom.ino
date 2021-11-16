// Libraries

#include <LiquidCrystal.h>


/*
 * Smart washroom
 * Author: Moise Sangwa Mukunzi
 * Andrew ID: msangwam
 * 
 * 
 */

// Initializing an LCD
const int rs = 30, en = 31, d4 = 32, d5 = 33, d6 = 34, d7 = 35;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//Analog inputs
int waterLevelSensor1 = A1;
int waterLevelSensor2 = A2;
int pumpSensor = A3;
int washRoomStatus = 0;

// flow sensor
volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 36; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;


// Output pins

int redPin = 2;
int yellowPin = 3;
int bluePin = 4;
int buzzerPin = 5;
int waterPumpPin = 6;
int greenPin = 7;
int heater = 8;
int tmpRead = 0;
int loopCount = 0;
int nodeMCU = A4;
int nodeMCUValue=0;

// values
int tmpWaterLevel1 = 0;
int tmpWaterLevel2 = 0;
int waterLevel1 = 0;
int waterLevel2 = 0;
int pumpSensorValue = 0;
int consumedLiters = 0;
int counter1 = 0;
int counter2 = 0;
int sumOfFlows = 0;
int averageFlow = 0;
int timeDifferanace = 0;
int sumOfPulses = 0;



void setup() {
  // Setting up LCD
  lcd.begin(16,2);
  lcd.setCursor(1, 0);
  lcd.print("Welcome to the ");
  lcd.setCursor(1, 1);
  lcd.print("Smart Washroom");

  // Setting up pins output
  pinMode(redPin, OUTPUT);
  pinMode(yellowPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(waterPumpPin, OUTPUT);
  pinMode(heater,OUTPUT);
  //washRoomStatus = 1;
  pinMode(nodeMCU, INPUT);

  // water flow sensor

  pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   //Serial.begin(9600);
   attachInterrupt(0, flow, RISING); // Setup Interrupt
   sei(); // Enable interrupts
   currentTime = millis();
   cloopTime = currentTime;

  // Serial monitor setting up

  Serial.begin(9600);  // baud rate of 9600
  Serial.println("");
  Serial.println("System is ON");
  digitalWrite(bluePin, HIGH);
  delay(3000);
  digitalWrite(bluePin, LOW);
  lcd.clear();

  // Node mcu
  Serial.print("Node MCU value: ");
  Serial.println(washRoomStatus);
  washRoomStatus = analogRead(nodeMCU);

  //Read initials
  washRoomStatus = analogRead(nodeMCU);
  tmpWaterLevel1 = analogRead(waterLevelSensor1);
  tmpWaterLevel2 = analogRead(waterLevelSensor2);
  waterLevel1 = tmpWaterLevel1/7.14;
  waterLevel2 = tmpWaterLevel2/7.14;
  //washRoomStatus = 1;
  

  

}

void flow () // Interrupt function
{
   flow_frequency++;
}


void dirtyWaterReservoirCheck()
{
  
  tmpWaterLevel2 = analogRead(waterLevelSensor2);
  waterLevel2 = tmpWaterLevel2/7.14;
   while (waterLevel2 > 90)
  {
    //Turning off heater and pump
    digitalWrite(waterPumpPin, LOW);
    digitalWrite(heater,LOW);
  // Turning ON Red Color
// Turning ON green Color
  digitalWrite(redPin, HIGH);  
  digitalWrite(greenPin, LOW); 
  Serial.println("---------------------"); 
  lcd.print(" Water level is HIGH");
  Serial.print("Dirty Water level is: ");
  Serial.print(waterLevel2);
  Serial.println("%");
  Serial.println("Dirty water reservot is full");

  lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("System not ready");
  lcd.setCursor(1, 1);
  lcd.print("Drain DW");
  
  delay(5000);
  tmpWaterLevel2 = analogRead(waterLevelSensor2);
  waterLevel2 = tmpWaterLevel2/7.14;
  lcd.clear(); 
  digitalWrite(redPin, LOW);  
  }
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH); 
  //void loop();
  
  }

void cleanWaterReservoirCheck()
{
  tmpWaterLevel1 = analogRead(waterLevelSensor1);
  waterLevel1 = tmpWaterLevel1/7.14;
  while (waterLevel1 <= 30)
  {
    //Turning off heater and pump
    digitalWrite(waterPumpPin, LOW);
    digitalWrite(heater,LOW);
    lcd.clear(); 
  digitalWrite(redPin, HIGH);  
  digitalWrite(greenPin, LOW); 

  Serial.println("---------------------"); 
  Serial.println("Clean w.: Refill");
  Serial.print("Clean Water level is: ");
  Serial.print(waterLevel1 );
  Serial.println("%");

  lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("System not ready");
  lcd.setCursor(1, 1);

   // print the number of seconds since reset:
  lcd.print("Refill");
  

  delay(5000); 
  tmpWaterLevel1 = analogRead(waterLevelSensor1);
  waterLevel1 = tmpWaterLevel1/7.14;
  lcd.clear();
  //digitalWrite(redPin, LOW);
  //lcd.clear();

  }
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH); 
  //void loop();
    
  
  }
  


void loop() {

 
  while(washRoomStatus>0)
  {
    
    dirtyWaterReservoirCheck();
  cleanWaterReservoirCheck();
  digitalWrite(redPin, LOW);
  digitalWrite(greenPin, HIGH); 

    
    
    lcd.clear();
    consumedLiters = 0;
  // put your main code here, to run repeatedly:
  tmpWaterLevel1 = analogRead(waterLevelSensor1);
  tmpWaterLevel2 = analogRead(waterLevelSensor2);
  pumpSensorValue = analogRead(pumpSensor);



  waterLevel1 = tmpWaterLevel1/7.14;
  waterLevel2 = tmpWaterLevel2/7.14;

    Serial.println("    Read values");
  Serial.println("--------------------");
  Serial.print("Pump senor: ");
  Serial.println(pumpSensorValue);
  Serial.print("Water level 1 : ");
  Serial.println(waterLevel1);
  Serial.print("Water level 2 : ");
  Serial.println(waterLevel2);
  Serial.println("--------------------");

 
    if (waterLevel2<=90 && waterLevel1 > 30)
  {
    if(pumpSensorValue<50)
    {
      
      digitalWrite(waterPumpPin, HIGH);
      digitalWrite(heater,HIGH);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Pump ON ");
      lcd.setCursor(0, 1);
      lcd.print("Heater ON");
      delay(2000);
      pumpSensorValue = analogRead(pumpSensor);
      while(pumpSensorValue>50)
      {
        dirtyWaterReservoirCheck();
        cleanWaterReservoirCheck();
        
        lcd.setCursor(0, 0);
      lcd.print(" Pump ON ");
      lcd.setCursor(0, 1);
      lcd.print("Heater ON");
        tmpRead = digitalRead(flowsensor);
        //Serial.println("loop1");
        //Serial.print("Digital read: ");
        //Serial.println(tmpRead);
        digitalWrite(waterPumpPin, HIGH);
        pumpSensorValue = analogRead(pumpSensor);
        loopCount++;

        //water flow sensor

        currentTime = millis();
        //Serial.print("Current time: ");
        //Serial.println(currentTime);
        //Serial.print("Cloop Time: ");
        //Serial.println(cloopTime);
        //Serial.print("Flow frequency: ");
        //Serial.println(flow_frequency);

        
        if(tmpRead==0)
        {
          flow_frequency++; 
          }
   // Every second, calculate and print litres/hour
   if((currentTime-cloopTime)>=400)
   {
    timeDifferanace = currentTime-cloopTime;
    //Serial.print("Flow frequency: ");
    //Serial.println(flow_frequency);
      cloopTime = currentTime; // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      sumOfFlows = sumOfFlows + l_hour;
      counter1++;
      sumOfPulses = sumOfPulses + flow_frequency;
      Serial.print(l_hour, DEC); // Print litres/hour
      Serial.println(" L/hour");
      loopCount=0;
      flow_frequency = 0;
      
      
   }
   //Serial.print("Node MCU value: ");
        //Serial.println(washRoomStatus);
      //washRoomStatus = digitalRead(nodeMCU);
    
       
        }
        digitalWrite(waterPumpPin, LOW);
        digitalWrite(heater,LOW);
        flow_frequency = 0;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print(" Pump OFF ");
        lcd.setCursor(0, 1);
        lcd.print("Heater OFF");

        
        
//en water flow sensor
        delay(500);
        
        pumpSensorValue = analogRead(pumpSensor);

      
      }

      if(pumpSensorValue>50)
    {
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(" Pump OFF ");
      lcd.setCursor(0, 1);
      lcd.print("Heater OFF");
      digitalWrite(waterPumpPin, LOW);
      digitalWrite(heater,LOW);
      delay(2000);
      lcd.clear();
      pumpSensorValue = analogRead(pumpSensor);
      while(washRoomStatus>0 && pumpSensorValue>50)
      {
        //Serial.println("loop2");
        Serial.print("Node MCU value: ");
  Serial.println(washRoomStatus);
  Serial.println("This loop***** ");
      washRoomStatus = analogRead(nodeMCU);
        digitalWrite(waterPumpPin, LOW);
        digitalWrite(heater,LOW);
        pumpSensorValue = analogRead(pumpSensor);
        dirtyWaterReservoirCheck();
        cleanWaterReservoirCheck();
        /*
        lcd.setCursor(0, 0);
      lcd.print(" Pump OFF ");
      lcd.setCursor(0, 1);
      lcd.print("Heater OFF"); */
      lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("System ready");
  lcd.setCursor(1, 1);
  lcd.print("CW:");
  lcd.print(waterLevel1);
  lcd.print("%");
  lcd.print(" ");
  lcd.print("DW:");
  lcd.print(waterLevel2);
  lcd.print("%");
       
        }
        lcd.clear();
        digitalWrite(waterPumpPin, HIGH);
        digitalWrite(heater,HIGH);

      
      }
      //Serial.print("Node MCU value: ");
      //Serial.println(washRoomStatus);
      washRoomStatus = digitalRead(nodeMCU);
      
    
  }

  
    consumedLiters = sumOfPulses/(7.5*60);
    Serial.println("");
    Serial.print("Liters: ");
    Serial.println(consumedLiters);
    
  }

  
  while(washRoomStatus==0)
  {
    digitalWrite(waterPumpPin, LOW);
    digitalWrite(heater,LOW);

        lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("     Start");
    lcd.setCursor(1, 1);
    lcd.print("  the shower");

    
    /*lcd.clear();
    lcd.setCursor(1, 0);
    lcd.print("System ready");
  lcd.setCursor(1, 1);
  lcd.print("CW:");
  lcd.print(waterLevel1);
  lcd.print("%");
  lcd.print(" ");
  lcd.print("DW:");
  lcd.print(waterLevel2);
  lcd.print("%");*/
  delay(2000);
  dirtyWaterReservoirCheck();
  cleanWaterReservoirCheck();
  Serial.print("Node MCU value: ");
    Serial.println(washRoomStatus);
    //Serial.println("Start the shower");

  
      washRoomStatus = analogRead(nodeMCU);
    
        
  
     }
    

      
}
