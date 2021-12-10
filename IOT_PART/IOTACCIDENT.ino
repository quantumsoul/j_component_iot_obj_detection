#include "TinyGPS++.h"
#include "SoftwareSerial.h"
SoftwareSerial GPRS(2,3);
SoftwareSerial serial_connection(10,11); 
TinyGPSPlus gps;

float latitude=0;
float longitude=0;
String Speed="";
int Contrast=20;

#define x A1
#define y A2
#define z A3

int xsample=0;
int ysample=0;
int zsample=0;
#define samples 10

#define minVal -50
#define MaxVal 50

void setup()
{
  Serial.begin(9600);
  serial_connection.begin(9600);
  Serial.println("Successfully Initialized...");
  Serial.println("GPS Start");

  for(int i=0;i<samples;i++) 
  {
    xsample+=analogRead(x);
    ysample+=analogRead(y);
    zsample+=analogRead(z);
  }

  xsample/=samples;
  ysample/=samples;
  zsample/=samples;
 
  Serial.println(xsample);
  Serial.println(ysample);
  Serial.println(zsample);
  delay(1000);
}

void loop()
{
  while(serial_connection.available())
  {gps.encode(serial_connection.read());
  }  
  if(gps.location.isUpdated())
  {Serial.println("Satellite Count:");
  Serial.println(gps.satellites.value());
  Serial.println("Latitude:");
  Serial.println(gps.location.lat(), 6);
  Serial.println("Longitude:");
  Serial.println(gps.location.lng(), 6);
  Serial.println("Speed MPH:");
  Serial.println(gps.speed.mph());
  Serial.println("Altitude Feet:");
  Serial.println(gps.altitude.feet());
  Serial.println("");
  }
  
  int value1=analogRead(x);
  int value2=analogRead(y);
  int value3=analogRead(z);

  int xValue=xsample-value1;
  int yValue=ysample-value2;
  int zValue=zsample-value3;

  Serial.print("x=");
  Serial.println(xValue);
  Serial.print("y=");
  Serial.println(yValue);
  Serial.print("z=");
  Serial.println(zValue);

  if(xValue<minVal||xValue>MaxVal||yValue<minVal||yValue>MaxVal||zValue<minVal||zValue>MaxVal)
  for(int i=0;i<5;i++)
  {Serial.println("Calling");
  GPRS.begin(9600);
  Serial.println("Connecting to network");
  delay(2000);
  Serial.println("Should be connected to network by now");
  GPRS.print("ATD+xxxxxxxxxx;\r");
  Serial.println("Dialling");
  delay(1200);
  GPRS.print("ATH\r");
  Serial.println("Disconnect");

  GPRS.print("Accident Detected:");
  GPRS.print("Location:");
  GPRS.print("Latitude - ");
  GPRS.print(latitude);
  GPRS.print("Longitude - ");
  GPRS.print(longitude);
  GPRS.print("Accident Detected:"); 
  GPRS.print(latitude);
  GPRS.print("+");
  GPRS.print(longitude);
  GPRS.write(26);
  GPRS.print("ATD+xxxxxxxxxx;\r");
  }
  delay(1000);}
  
