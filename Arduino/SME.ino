#include <SoftwareSerial.h>
#define debug false

SoftwareSerial ESP(10,11); //RX:TX of arduino

int value = 0;
long int milliz;
double real_power=0;
double prev = 0;
String response,message,temp;
double millis1=0;
double sum_inst_power=0;
double inst_power=0;
double inst_voltage=0;
double inst_current=0; 
double squared_voltage=0;
double sum_squared_voltage=0;
double squared_current=0;
double sum_squared_current=0;
double power_factor=0;
double apparent_power=0;
double root_mean_square_voltage=0;
double root_mean_square_current=0;
double prevapp=0;
double prevfac=0;
long int count=0;
char TempString[10];
//int reconnect = 0;

String sendData(String command, const int timeout, boolean bebug)
{
    String reply = "";
    //Serial.println(command);
    ESP.println(command);
    long int time = millis();
    while((time+timeout) > millis())
    {
        while(ESP.available())
        {
            reply += char(ESP.read());
        }
    }
    if(debug)
      Serial.println(reply);
    return reply;
}
void setup() {
  Serial.begin(9600);
  ESP.begin(9600);
  while(ESP.available())
  {
    ESP.read();
  }
  while(Serial.available())
  {
      Serial.read();
  }
  pinMode(12,OUTPUT);
  digitalWrite(12,value);
  sendData("AT",2000,debug);
  sendData("AT+CWMODE=1",2000,debug);
  sendData("AT+CIPMUX=1",2000,debug);
  //sendData("AT+CWJAP=\"suyash\",\"behera123\"",8000,debug);
  sendData("AT+CIPSERVER=1,80",2000,debug);
  sendData("AT+CIPSTART=4,\"TCP\",\"192.168.0.100\",8000",1000,debug);
  //String message = "GET /ESP?power=4";
  // char temp[6];
  //  dtostrf(5,6,2,temp);
  //  //message +=temp;
  //  message += " HTTP/1.1";
  //  sendData("AT+CIPSTART=4,\"TCP\",\"192.168.43.58\",5000",3000,debug);
  //  sendData("AT+CIPSEND=4,27",2000,debug);
  //  sendData(message,2000,debug);
  //  sendData("AT+CIPCLOSE=4",2000,debug);
  milliz=millis();

}
 
void loop() 
{

  millis1=millis();
  while(millis()-millis1<20)
  {
  // inst_voltage and inst_current calculation from raw ADC input goes here

    count++;
  //  inst_current=(analogRead(A0)*(5000/1024) - 2500)/66;//calc
    inst_current = ((analogRead(A0)*.07398)-37.8787);
    inst_voltage=((analogRead(A1)-330)*1.469726);//calc
  
 
  
//Serial.println("\t");
  
//RMS VOLTAGE
   squared_voltage = inst_voltage * inst_voltage;

   sum_squared_voltage += squared_voltage;
 
//RMS CURRENT

   squared_current = inst_current * inst_current;

   sum_squared_current += squared_current;
  
//INSTANTANEOUS POWER 
    inst_power = inst_voltage * inst_current;


    sum_inst_power += inst_power;

  }

  root_mean_square_voltage = sqrt(sum_squared_voltage / count);
  root_mean_square_current = sqrt(sum_squared_current / count); 
  real_power = abs(sum_inst_power/count);
  real_power = abs(0.7 * prev + 0.3*real_power);
  prev = real_power;
  apparent_power = abs(root_mean_square_voltage * root_mean_square_current);
  apparent_power = 0.7 * prevapp + 0.3*apparent_power;
  prevapp=apparent_power;
  power_factor = abs(real_power / apparent_power);
  power_factor=0.7*prevfac+0.3*power_factor;
  prevfac=power_factor;
  /*Serial.print(" R.P         ");
  Serial.println(real_power);
  Serial.print(" A.P          ");
  Serial.println(apparent_power);*/
  if(real_power > 2)
  {
    Serial.print(" R.p         ");
    Serial.println(real_power);
    Serial.print(" p.f          ");
    Serial.println(power_factor);
     if(millis()-milliz > 20000)
     {
  
      if((power_factor>=0.75&& real_power>40) && real_power<60)
        Serial.println("Appliance Detected... Class III... Might be a Table Fan");

     else if((power_factor>=0.58 && real_power>330) && real_power<800)
        Serial.println("Appliance Detected... Class I... Might be a Electric Iron");
        
     else if((power_factor>=0.3 && power_factor<=0.5) && (real_power>20&& real_power<63))
        Serial.println("Appliance Detected... Class II... Might be a Laptop");
    
      
  }

  }
  if(real_power <= 2)
    message = "2";
  else
  {
   //dtostrf(power_factor,1,3,TempString);
   message = String(int(real_power)) + "\r\n";
  }
  temp = "AT+CIPSEND=4," + String(message.length()+2);
  response =  sendData(temp,800,debug);
  response += sendData(message,800,debug);
  //reconnect += 1;
  int index1 = response.indexOf("pin");
  if(index1 >= 0)
  { 
    String pin = response.substring(index1+4,index1+6);
    if(pin == "13")
    {
      value ^= 1;
      digitalWrite(12,value);
    }
  }
  response = "";
  millis1=0;
  sum_inst_power=0;
  inst_power=0;
  inst_voltage=0;
  inst_current=0; 
  squared_voltage=0;
  sum_squared_voltage=0;
  squared_current=0;
  sum_squared_current=0;
  power_factor=0;
  apparent_power=0;
  root_mean_square_voltage=0;
  root_mean_square_current=0;
  count = 0;
  real_power = 0;
 /* if(reconnect == 10)
  {
    sendData("AT+CIPCLOSE=4",1000,debug);
    sendData("AT+CIPMUX=1",2000,debug);
    sendData("AT+CIPSERVER=1,80",2000,debug);
    sendData("AT+CIPSTART=4,\"TCP\",\"192.168.0.100\",8000",1000,debug);
    reconnect = 0;
  }*/
    
//average out code here


//APPARENT POWER


//POWER FACTOR

 /* if(millis()-milliz > 10000)
  {
  
    if((power_factor>=0.3 && power_factor<=0.7) && (real_power>25 && real_power<63))
        Serial.println("Appliance Detected... Class I... Might be a Laptop");
    
    if((power_factor>=0.9 && power_factor<=0.99) && real_power>1000)
        Serial.println("Appliance Detected... Class II... Might be a Kettle which is ON and filled with water");
    
    if((power_factor>=0.4 && power_factor<=0.6) && real_power>100&& real_power<200)
        Serial.println("Appliance Detected... Class II... Might be a Kettle which is Standby state");
    
    if((power_factor>=0.2 && power_factor<=0.4) && (real_power>200 && real_power<700))
        Serial.println("Appliance Detected... Class II... Might be a Kettle without Water");
    
     if((power_factor>=0.75&& real_power>40) && real_power<60)
        Serial.println("Appliance Detected... Class III... Might be a Table Fan");
    
     if((power_factor>=0.96 && real_power>250) && real_power<800)
        Serial.println("Appliance Detected... Class IV... Might be a Electric Iron");
  }*/

}
