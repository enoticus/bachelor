#include <LiquidCrystal.h>
 
LiquidCrystal lcd(4, 5, 10, 11, 12, 13);
double er0,er1,er2,er3,er4,ten,ser;
int val;
float I=0,D=0,P=0, d0=0;
void setup() {
  // put your setup code here, to run once:
Serial.begin(9600); 
double sr; 
//sr=(Getterm(analogRead(A0))+Getterm(analogRead(A1))+Getterm(analogRead(A2))+Getterm(analogRead(A3))+Getterm(analogRead(A4)))/1;
er0=Getterm(analogRead(A0))-sr;
er1=Getterm(analogRead(A1))-sr;
er2=Getterm(analogRead(A2))-sr;
er3=Getterm(analogRead(A3))-sr;
er4=Getterm(analogRead(A4))-sr;
pinMode(3,OUTPUT);
pinMode(7,INPUT);
pinMode(6,INPUT);
   lcd.begin(16, 2);
     ten=33.03;
}
#define PID_P 0.5
#define PID_I 0.017
#define PID_D 0.16
float calc_pid(float P, float I, float D){
  return (PID_P*P+PID_I*I+PID_D*D);
}
#define maxI 10

float calc_heatter_PWM(float current_temp, float target_temp){
 
  P =target_temp-current_temp;
I=I+P;
  if(I>maxI){
    I=maxI;
  }
  if(I<-maxI){
    I=-maxI;
  }
 D=current_temp-d0;
float pid =calc_pid(P,I,D); 
 d0=current_temp; 
return (pid);
}
  #define PWM_period 10000 // ml seconds
//this functions generates PWM signal which controls the main heater, recieves PWM duty cycle from 0 to 1
void PWM_gen(float pidcoeff){

  if(pidcoeff>1){
    pidcoeff=1;
  }
  if(pidcoeff<0){
    pidcoeff=0;
  }
  int timeOn=PWM_period*pidcoeff;
 int timeOff=PWM_period-timeOn;
 if(timeOn!=0){  
  digitalWrite(3,HIGH);
  delay(timeOn);}
  if(timeOff!=0){
  digitalWrite(3,LOW);
  delay(timeOff);
  }  
}

void loop()
{
 /* if (Serial.available()>-1) // проверяем, поступают ли какие-то команды
  {

    val = Serial.read(); // переменная val равна полученной команде

    if (val == "1")
    {
      ten++;
    }
    if (val == "2")
    {
      ten--;
    }
  }*/

/*if(ten>ser)
{
  digitalWrite(3,HIGH);
}
if(ten<ser)  
{
  digitalWrite(3,LOW);
} */



/*double temp0=Getterm(analogRead(A0));
Serial.println(temp0);
double temp1=Getterm(analogRead(A1));
Serial.println(temp1);
 double temp2=Getterm(analogRead(A2));
Serial.println(temp2);
double temp3=Getterm(analogRead(A3));
Serial.println(temp3);*/
/*double temp0=Getterm(analogRead(A0));
Serial.println(temp0);*/
double temp1=Getterm(analogRead(A1));
Serial.print(temp1);
Serial.print(",");
//ser=(temp0+temp1+temp2+temp3+temp4)/5;
/*Serial.println(ser);
Serial.println(" ");*/

ser=temp1;
float pid =calc_heatter_PWM(ser,ten);
PWM_gen(pid);

lcd.clear();
 lcd.setCursor(0,0);
 lcd.print("now ");
 lcd.print(ser);
 lcd.print(" C");
 lcd.setCursor(0,1);
 lcd.print("need ");
 lcd.print(ten);
 lcd.print(" C");
// Serial.println( ten);
Serial.print( P);
Serial.print(",");
Serial.print( I);
Serial.print(",");
Serial.print( D);
Serial.print(",");
Serial.print(pid);
Serial.println(" ");
delay(5000); //5сек
 
}
double Getterm(int RawADC) {
  double temp;
  temp = log(((10240000/RawADC) - 10000));
  temp = 1 / (0.001129148 + (0.000234125 * temp) + (0.0000000876741 * temp * temp * temp));
  temp = temp - 273.15;
  return temp;
}
