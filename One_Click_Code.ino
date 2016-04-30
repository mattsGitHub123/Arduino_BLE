#include <RBL_nRF8001.h>
#include <RBL_services.h>
#include <math.h>


/**
 * One_Click Arudino Code
 * @data 4/14/16
 * @author Matthew Joyce, Ben Forman 
 */
#include <SPI.h>

#define analogPin    A0    //the thermistor attach to 
#define photocellPin A1    //photoresistor attach to A0 
#define beta         4090  //the beta of the thermistor
#define resistance   10    //the value of the pull-down resistor
#define keyPin       12    //Button Pin
#define ledPin       13    //LED PIN
#define BLE_NAME     "BLE SHEILD"

int photocellData = 0;         // value read from the sensor
int serviceStarted = 0;
char thermisterArray[6];

void setup()
{
  Serial.begin(57600);
  Serial.print("In setup");
  pinMode(keyPin,INPUT);      //initialize the key pin as input 
  pinMode(ledPin,OUTPUT);     //initialize the led pin as output
}

void startBLEService()
{
  ble_set_name("BLE Shield");
  ble_begin();
  serviceStarted = 1;
}

void readFromPhotoCell()
{
  Serial.println("In read from photo cell");
  photocellData = analogRead(photocellPin); //read the value of A0
  Serial.print("Photo Resister data: ");
  Serial.println(photocellData);
 
}

void readFromThermister()
{
  Serial.println("in read from thermister");
  long thermisterData = analogRead(analogPin);
  
  //the calculating formula of temperature
  float tempC = beta /(log((1025.0 * 10 / thermisterData - 10) / 10) + beta / 298.0) - 273.0;
  float tempF = 1.8 * tempC + 32.0;

  dtostrf(tempF, 4, 2, thermisterArray);
  
  //Serial.print("TempC: ");//print" TempC: "
  //Serial.print(tempC);//print Celsius temperature
  //Serial.print(" C");//print the unit
  //Serial.println();
  Serial.print(" TempF: ");
  Serial.print(tempF);
  Serial.print(" ");
  Serial.print(thermisterArray);
  //Serial.print(" F");
  Serial.println();
}

void loop()
{
  if (digitalRead(keyPin) == HIGH)
  {
      Serial.println("Button Pressed, Starting BLE Service");
      digitalWrite(ledPin,HIGH);      //turn on the led
      delay(1000);
      startBLEService();
      
  }
 
  if (ble_connected() && serviceStarted)
  {
    if (ble_available() > 0)
    {
      int value = ble_read();
      Serial.println("Read value: ");
      Serial.println(value);
      
      if (value == 49)
      {
        readFromPhotoCell();
        readFromThermister();

        Serial.println("Connected to Iphone and received 1");
        Serial.println("Dumping Data....");

        Serial.print("thermisterArray ");
        Serial.print(thermisterArray);
        
        ble_write(thermisterArray[0]);
        ble_write(thermisterArray[1]);
        ble_write(thermisterArray[2]);
        ble_write(thermisterArray[3]);
        ble_write(thermisterArray[4]);

        ble_do_events();
        
        int temp = (photocellData >> 8 ) & 0xff;
        Serial.print("Low bit: ");
        Serial.print(temp);
        ble_write(temp);
        temp = (photocellData) & 0xff;
        Serial.print(" High byte: ");
        Serial.print(temp);
        ble_write(temp);
       
        
      
        
        
       // serviceStarted = 0;
     
      }
    }
  }
  ble_do_events();
  

}
