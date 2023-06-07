/*
   Arduino to Monster Motor Driver connections
   Arduino pin     Monster Motor Driver Pin
   4               4
   5               5
   6               6
   7               7   
   8               8
   9               9

   Arduino to IR sensor array connections
   Arduino pin    Sensor Pin
   2              S1 (Orange)
   3              S2 (Yellow)
   10             S3 (Blue)
   11             S4 (Grey)
   12             S5 (Dark purple)
   Vcc            Vcc
   Gnd            Gnd
*/

// For Motor 1  (Right Side Motor) - Clockwise Straight
#define A1_PIN 7 // Clockwise for motor 1
#define B1_PIN 8 // Counterclockwise for Motor 1
#define PWM_1 5 // PWM for Motor 1
#define EN1 A0 // Enable Pin for Motor 1

// For Motor 2   (Left Side Motor) - Anticlockwise Straight
#define A2_PIN 4 // Clockwise for motor 2
#define B2_PIN 9 // Counterclockwise for Motor 2
#define PWM_2 6 // PWM for Motor 2
#define EN2 A1 // Enable Pin for Motor 2

// IR Sensor Array
#define S1 2    //Sensor1
#define S2 3    //Sensor2
#define S3 10   //Sensor3
#define S4 11   //Sensor4
#define S5 12   //Sensor5

/*When it detects a black surface, the line sensor will have '0' logic output*/
/*This means that black line detection will give 'Low' reading*/

//Setting a finite value for these motor speeds
#define Motor1_set_speed 34   //Right 
#define Motor2_set_speed 30   //Left

#define ACK_PIN 0
int T_count = 0;

void setup() 
{ 
  Serial.begin(9600);
  
  // Set Pins as Input and Output
  pinMode(S1,INPUT);
  pinMode(S2,INPUT);
  pinMode(S3,INPUT);
  pinMode(S4,INPUT);
  pinMode(S5,INPUT);

  pinMode(ACK_PIN,INPUT);

  pinMode(A1_PIN,OUTPUT);
  pinMode(B1_PIN,OUTPUT);

  pinMode(A2_PIN,OUTPUT); 
  pinMode(B2_PIN,OUTPUT); 

  pinMode(PWM_1,OUTPUT); 
  pinMode(PWM_2,OUTPUT); 

  pinMode(EN1,OUTPUT); 
  pinMode(EN2,OUTPUT);

  // Enable Both Motors 
  digitalWrite(EN1, HIGH);
  digitalWrite(EN2, HIGH);
  
  // Motor 1 Clockwise
  digitalWrite(A1_PIN, HIGH);
  digitalWrite(B1_PIN, LOW);

  // Motor 2 Anticlockwise
  digitalWrite(B2_PIN, HIGH); 
  digitalWrite(A2_PIN, LOW);

  // Initially both motors off
  analogWrite(PWM_1, 0);
  analogWrite(PWM_2, 0); 


}

void Straight(void)
{
  // Both motors ON
  digitalWrite(EN1,HIGH);
  digitalWrite(EN2,HIGH);
  analogWrite(PWM_2, Motor2_set_speed);
  delay(5);
  analogWrite(PWM_1, Motor1_set_speed);
}

void Stop(void)
{
  // Both motors OFF
  analogWrite(PWM_1, 0);
  analogWrite(PWM_2, 0);
  digitalWrite(EN1,LOW);
  digitalWrite(EN2,LOW);
}

void Right(void)
{
  //Motor 1 OFF - Motor 2 ON
  analogWrite(PWM_1,0);
  analogWrite(PWM_2,Motor2_set_speed);
}

void Left(void)
{
  //Motor 1 ON - Motor 2 OFF
  analogWrite(PWM_1, Motor1_set_speed);
  analogWrite(PWM_2,0);
}

void Softright(void)
{
  analogWrite(PWM_1, Motor1_set_speed - 6);
  analogWrite(PWM_2, Motor2_set_speed);
}

void Softleft(void)
{
  analogWrite(PWM_1, Motor1_set_speed);
  analogWrite(PWM_2, Motor2_set_speed - 6);
}

void Slightright(void)
{
  analogWrite(PWM_1, Motor1_set_speed - 4);
  analogWrite(PWM_2, Motor2_set_speed);  
}

void Slightleft(void)
{
  analogWrite(PWM_1, Motor1_set_speed);
  analogWrite(PWM_2, Motor2_set_speed - 4);
}

void Tcount(void)
{
  if (T_count == 1)
  {
    Right();
    delay(1500);
  }
  else if (T_count == 2)
  {
    while(digitalRead(ACK_PIN)==LOW)
    {
      Stop();     
    }
    Straight();
  }
  else if (T_count == 3)
  {
    while(digitalRead(ACK_PIN)==LOW)
    {
      Stop();     
    }
    Straight();
  }
  else if (T_count == 4)
  {
    while(digitalRead(ACK_PIN)==LOW)
    {
      Stop();     
    }
    Straight();
  }
  else if (T_count == 5)
  {
    while(digitalRead(ACK_PIN)==LOW)
    {
      Stop();     
    }
    Straight();
    T_count = 0;
  }
}
void loop() 
{ 
  
  // Move Vehicle Forward
  if((digitalRead(S1)==LOW)&&(digitalRead(S2)==LOW)&&(digitalRead(S3)==HIGH)&&(digitalRead(S4)==LOW)&&(digitalRead(S5)==LOW))
  {
    Straight();
  }
  else if((digitalRead(S1)==LOW)&&(digitalRead(S2)==HIGH)&&(digitalRead(S3)==HIGH)&&(digitalRead(S4)==HIGH)&&(digitalRead(S5)==LOW))
  {
    //continue;
  }
  else if((digitalRead(S1)==LOW)&&(digitalRead(S2)==HIGH)&&(digitalRead(S3)==HIGH)&&(digitalRead(S4)==LOW)&&(digitalRead(S5)==LOW))
  {
    Slightright();
  }
  else if((digitalRead(S1)==LOW)&&(digitalRead(S2)==HIGH)&&(digitalRead(S3)==LOW)&&(digitalRead(S4)==HIGH)&&(digitalRead(S5)==LOW))
  {
    //continue;
  }
  else if((digitalRead(S1)==LOW)&&(digitalRead(S2)==HIGH)&&(digitalRead(S3)==LOW)&&(digitalRead(S4)==LOW)&&(digitalRead(S5)==LOW))
  {
    Softleft();
  }
  else if((digitalRead(S1)==LOW)&&(digitalRead(S2)==LOW)&&(digitalRead(S3)==HIGH)&&(digitalRead(S4)==HIGH)&&(digitalRead(S5)==LOW))
  {
    Slightleft();
  }
  else if((digitalRead(S1)==LOW)&&(digitalRead(S2)==LOW)&&(digitalRead(S3)==HIGH)&&(digitalRead(S4)==LOW)&&(digitalRead(S5)==LOW))
  {
    Straight();
  }
  else if((digitalRead(S1)==LOW)&&(digitalRead(S2)==LOW)&&(digitalRead(S3)==LOW)&&(digitalRead(S4)==HIGH)&&(digitalRead(S5)==LOW))
  {
    Softright();
  }
  else if((digitalRead(S1)==LOW)&&(digitalRead(S2)==LOW)&&(digitalRead(S3)==LOW)&&(digitalRead(S4)==LOW)&&(digitalRead(S5)==LOW)) 
  { 
    //continue;
  }
  else if((digitalRead(S1)==HIGH)&&(digitalRead(S2)==HIGH)&&(digitalRead(S3)==HIGH)&&(digitalRead(S4)==HIGH)&&(digitalRead(S5)==HIGH))  //Main Case
  { 
    Stop();
    delay(500);
    T_count++;
    Tcount();
  }
  else if((digitalRead(S1)==HIGH)&&(digitalRead(S2)==HIGH)&&(digitalRead(S3)==HIGH)&&(digitalRead(S4)==LOW)&&(digitalRead(S5)==LOW))    //Left Turn at Corners 
  {
    Left();
  }
  else if((digitalRead(S1)==HIGH)&&(digitalRead(S2)==HIGH)&&(digitalRead(S3)==HIGH)&&(digitalRead(S4)==HIGH)&&(digitalRead(S5)==LOW))    //Left Turn at Corners 
  {
    Left();
  }
  else if((digitalRead(S1)==LOW)&&(digitalRead(S2)==HIGH)&&(digitalRead(S3)==HIGH)&&(digitalRead(S4)==HIGH)&&(digitalRead(S5)==HIGH))    //Right Turn at first 'T' 
  {
    Right();
  }
  else if((digitalRead(S1)==LOW)&&(digitalRead(S2)==LOW)&&(digitalRead(S3)==HIGH)&&(digitalRead(S4)==HIGH)&&(digitalRead(S5)==HIGH))    //Right Turn at first 'T' 
  {
    Right();
  }
}
