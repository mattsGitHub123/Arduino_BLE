#include <RBL_nRF8001.h>
#include <RBL_services.h>


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
float thermisterData = 0;      // Value to be read from the sensor
int serviceStarted = 0;

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
  Serial.print("In read from photo cell");
  photocellData = analogRead(photocellPin); //read the value of A0
  Serial.print("Photo Resister data: ");
  Serial.print(photocellData);
}

void readFromThermister()
{
  Serial.print("in read from thermister");
  long thermisterData = analogRead(analogPin);
  
  //the calculating formula of temperature
  float tempC = beta /(log((1025.0 * 10 / thermisterData - 10) / 10) + beta / 298.0) - 273.0;
  float tempF = 1.8 * tempC + 32.0;
  thermisterData = tempF;
  
  //Serial.print("TempC: ");//print" TempC: "
  //Serial.print(tempC);//print Celsius temperature
  //Serial.print(" C");//print the unit
  //Serial.println();
  Serial.print(" TempF: ");
  Serial.print(tempF);
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
        
        ble_write(photocellData);
        ble_write(thermisterData);
        ble_write(0);
        serviceStarted = 0;
     
      }
    }
  }
  ble_do_events();
  //delay(2000); //wait for 2000 milliseconds

}
