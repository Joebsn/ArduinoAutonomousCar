#include<Servo.h>

const int en1 = 5, en2 = 6, in1 = 4, in2 = 7, in3 = 8, in4 = 12, servopin = 9, echopin = 11, triggerpin = 13, irsensor=A0, IRpin = 3;
const int incrementAngle = 20, deceleration = 30, acceleration = 5, minangle = 30, maxangle = 150, averageangle=90, ThresholdDistance = 30;
const float speedofsound = 343;

int counter=0, vitesse=0, angle = minangle, DirectionToMove, CanWalk=0;
float  maxdistance=0, timeneeded, distance, table[5];
bool b=false;

Servo servo;

//functions

void mindelay(void){
  delay(10);
}

void delayservodistance(void){
  delay(50);
}

float calculatedistance(void){
delay(20);
digitalWrite(triggerpin,LOW);
delayMicroseconds(100);
digitalWrite(triggerpin,HIGH);
delayMicroseconds(50);
digitalWrite(triggerpin,LOW);
timeneeded = pulseIn(echopin,HIGH);
distance= speedofsound*timeneeded/20000;
mindelay();
return distance;
}

float calculateheight(void)
{
mindelay();
servo.write(50);
delayservodistance();
float l=calculatedistance();
delay(100);
servo.write(110);
delayservodistance();
float p= calculatedistance();
delay(100);
float height=(l*l)+(p*p)-(2*(l*p*0.5));
height=sqrt(height);
mindelay();
return height;
}

void modearriere(void){
digitalWrite(in1,HIGH);
digitalWrite(in3,HIGH);
digitalWrite(in2,LOW);
digitalWrite(in4,LOW);
mindelay();
}

void modeavant(void){
digitalWrite(in1,LOW);
digitalWrite(in3,LOW);
digitalWrite(in2,HIGH);
digitalWrite(in4,HIGH); 
mindelay();
}

void AccelerateOrDecelerate(void) //it'll calculate the distance and choose wheter it'll accelerate or decelerate
{
  mindelay();
  distance=calculatedistance();
  
  if((distance>ThresholdDistance) && (vitesse<=120))
  {
    vitesse = vitesse + acceleration;
    SpeedValue();
  }
  
  while(distance<=ThresholdDistance) //If bayyan obstacle eddem el car w reji3 zet hek ma bet wa22if el car
  {
    vitesse = vitesse-deceleration;
    SpeedValue();
    if(vitesse == 0) b=true;
    distance = calculatedistance();
  }              
  mindelay();
}

void SpeedValue(void)
{
analogWrite(en1,vitesse);
analogWrite(en2,vitesse);
mindelay();
}

void stopcar(void){
  analogWrite(en1,0);
  analogWrite(en2,0);
  mindelay();
}

void turnright(void)
{
  counter=0;
  while(true)
  {
    distance=calculatedistance();
    if((counter==5) || (distance<ThresholdDistance))
    {
      break;
    }
    analogWrite(en1,180);
    analogWrite(en2,70);
    delay(100);
    counter++;
  }
  stopcar();
}


void turnleft(void){
  counter = 0;
  while(true){
    distance=calculatedistance();
    if((counter==5) || (distance < ThresholdDistance)){
      break;
    }
    analogWrite(en1,60);
    analogWrite(en2,200);
    delay(100);
    counter++;
  }
  stopcar();
}

void arrieredirect(void)
{
  int SensorValue=analogRead(irsensor);
  counter=0;
  while((SensorValue>500) || (counter==5)){
    analogWrite(en1,100);
    analogWrite(en2,100);
    delay(100);
    counter++;
    SensorValue=analogRead(irsensor);
  }
  stopcar();
}

void CanTheCarWalkAvant(void)
{
    for(angle = minangle; angle <= maxangle; angle = angle + incrementAngle)
    {
      servo.write(angle);
      table[counter] = calculatedistance();
      if(table[counter] > ThresholdDistance){ CanWalk++ ; }
      counter++;
    }
  
    if(CanWalk==6) b = false; //The car can Walk easily in all the directions
    
    else
    {
       for(counter=0; counter<6; counter++)
       {
          if(table[counter] > maxdistance)
          {
            maxdistance = table[counter];
            DirectionToMove = counter;
          }
        }
        
       if(DirectionToMove<2) 
       {
        turnright();
        servo.write(averageangle);
        mindelay(); 
       }
       if(DirectionToMove>3)
       {
        turnleft();
        servo.write(averageangle);
        mindelay(); 
       }
       if((DirectionToMove==2)|| (DirectionToMove == 3)) b=false;
    }
}

void hi(void)
{
  digitalWrite(in1,LOW);
  digitalWrite(in3,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in4,LOW);
  analogWrite(en1,170);
  analogWrite(en2,170);  
}

void setup() {
Serial.begin(9600);
pinMode(in1,OUTPUT);
pinMode(in2,OUTPUT);
pinMode(in3,OUTPUT);
pinMode(in4,OUTPUT);
pinMode(en1,OUTPUT);
pinMode(en2,OUTPUT);
pinMode(triggerpin,OUTPUT);
pinMode(echopin,INPUT);
pinMode(irsensor, INPUT);
servo.attach(servopin);
servo.write(angle);
modeavant();
}

void loop(){
hi();
/*CanTheCarWalkAvant();

while (b==false) //if b == true yaane el obstacle sar a2rab mnel threshold distance
{
  for(angle = maxangle; angle >= minangle; angle =angle - incrementAngle)
  {
    servo.write(angle);
    delayservodistance();
    AccelerateOrDecelerate();
  }
  
  for(angle = minangle; angle <= maxangle; angle = angle + incrementAngle)
  {
    servo.write(angle);
    delayservodistance();
    AccelerateOrDecelerate();
  }
  
 }
if(b==true)
{
   modearriere();
   for(angle = minangle; angle <=maxangle ; angle = angle + incrementAngle)
   {
     servo.write(angle);
     delayservodistance();
     counter = 0;
     table[counter]=calculatedistance();
     if(table[counter]>maxdistance)
     {
        maxdistance=table[counter];
        DirectionToMove = counter;
     }
     counter++;
   }
   if(DirectionToMove<2) turnleft(); 
   if(DirectionToMove>3) turnright();
   if((DirectionToMove==2) || (DirectionToMove == 3)) arrieredirect();
}
modeavant();
b=false;
counter=0;
vitesse=0;
maxdistance=0;
DirectionToMove=2;
mindelay();*/
}










//remotefunctions

/*

#include <IRremote.h>

Servo servo;
//IRrecv irrecv(IRpin);
//decode_results results; 


pinMode(echopin,INPUT);
pinMode(irsensor, INPUT);
//irrecv.enableIRIn();
servo.attach(servopin);
servo.write(angle);


int u=0, c=0, j=0,counter=0,x;
 
void remoteleft(void){
  analogWrite(en1,70);
  analogWrite(en2,160); 
}

void remoteright(void){
  analogWrite(en1,160);
  analogWrite(en2,70); 
}

void remotestop(void){
  analogWrite(en1,0);
  analogWrite(en2,0);
}

void remoteavant(void){
  modeavant();
}

void remotearriere(void){
  modearriere();
}

void remotelawara(void){
  modearriere();
  analogWrite(en1,100);
  analogWrite(en2,100);
}

void remotelaeddem(void){
  modeavant();
  analogWrite(en1,100);
  analogWrite(en2,100);
}

void remoteser3a(void){
 analogWrite(en1,160);
 analogWrite(en2,160); 
}

void remotetopspeed(void){
 analogWrite(en1,220);
 analogWrite(en2,220); 
}

void modeout(void)
{
stopcar();
u=0;
c=0;
j=0;
}

*/

/*
void loop() {
  if (irrecv.decode(&results)){
    if(results.value == 0x1FE48B7) { //manual
        u=1;
        while(u==1){
        if (irrecv.decode(&results)){
        if(results.value == 0x1FE807F) remoteleft();
        if(results.value == 0x1FE40BF) remoteright();  
        if(results.value == 0x1FEC03F) remoteavant();  
        if(results.value == 0x1FE20DF) remotelawara();
        if(results.value == 0x1FEA05F) remotelaeddem();
        if(results.value == 0x1FE609F) remotearriere();
        if(results.value == 0x1FEE01F) remotestop();
        if(results.value == 0x1FE10EF) remoteser3a();
        if(results.value == 0x1FE906F) remotetopspeed(); 
        if(results.value == 0x1FE50AF) modeout();
        mindelay();
        irrecv.resume();
        }
        }
    }
  
    if(results.value == 0x1FE58A7){ //manual+automatic
        j=1;
        while(j==1){
        if (irrecv.decode(&results)){
        if(results.value == 0x1FE807F) remoteleft();
        if(results.value == 0x1FE40BF) remoteright();  
        if(results.value == 0x1FEC03F) remoteavant();  
        if(results.value == 0x1FE20DF) remotelawara();
        if(results.value == 0x1FEA05F) remotelaeddem();
        if(results.value == 0x1FE609F) remotearriere();
        if(results.value == 0x1FEE01F) remotestop();
        if(results.value == 0x1FE10EF) remoteser3a();
        if(results.value == 0x1FE906F) remotetopspeed(); 
        if(results.value == 0x1FE50AF) modeout();
        mindelay();
        irrecv.resume();
        }
        }      
    }
    
    if(results.value == 0x1FE7887){ //automatic
      
        while(true){
          while (b==false){
            for(s=minangle;s<=maxangle;s=s+se){
                if (irrecv.decode(&results)){
                  if(results.value == 0x1FE50AF){
                      modeout();
                  }                  
              }
              servo.write(s);
              delayservodistance();
              d=calculatedistance();
              mindelay();
              if(d>a){
              icondition();
              ivalue();
              }
              if(d<=a){
                for(i;i>0;i=i-deceleration){
                    ivalue();
                     }
                b=true;
                    }
              mindelay();
     }
     for(s=maxangle;s>=minangle;s=s-se){
            if (irrecv.decode(&results)){
                if(results.value == 0x1FE50AF){
                    modeout();
                }                      
            }
            servo.write(s);
            delayservodistance();
            d=calculatedistance();
            mindelay();
            if(d>a){
            icondition();
            ivalue();
            }
            if(d<=a){
                for(i;i>0;i=i-deceleration){
                ivalue();
                }
            b=true;
            }
        mindelay();
        }
   }
  if(b==true){
      modearriere();
      for(s=minangle;s<=maxangle;s=s+se){
         if (irrecv.decode(&results)){
             if(results.value == 0x1FE50AF){
                 modeout();
             }                      
         }
         servo.write(s);
         delayservodistance();
         table[y]=calculatedistance();
         mindelay();
         if(table[y]>maxdistance){
              maxdistance=table[y];
              z=y;
              }
         y++;
         }
      if(z<2) turnleft(); 
      if(z>2) turnright();
      if(z==2) arrieredirect();
  }
  modeavant();
  b=false;
  y=0;
  i=0;
  maxdistance=0;
  z=2;
  delay(50); 
  mindelay();  
}
}

//  irrecv.resume();
 
  /*if(results.value == 0x1FED827)

  if(results.value == 0x1FEF807)

  if(results.value == 0x1FE30CF)

  if(results.value == 0x1FEB04F)

  if(results.value == 0x1FE708F)
    
  if(results.value == 0x1FE00FF)

  if(results.value == 0x1FEF00F)
    
  if(results.value == 0x1FE9867)*/
