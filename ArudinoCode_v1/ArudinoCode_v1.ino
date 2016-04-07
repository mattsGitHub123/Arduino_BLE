#include <RBL_nRF8001.h>
#include <RBL_services.h>
#include <SPI.h>


// Define output pin
#define LED_PIN 13


void setup()
{
  // Enable serial debug.
  Serial.begin(57600);

  // Enable output.
  pinMode(LED_PIN, OUTPUT);
  pinMode(2, INPUT);
  digitalWrite(2, HIGH);

  // Turn off LED.
  digitalWrite(LED_PIN, LOW);

  // Set a custom BLE name.
  ble_set_name("BLE Shield");
  
  // Initialize BLE library.
  ble_begin();

  Serial.println("ble_begin done!");
}

// This function is called continuously, after setup() completes.
void loop()
{
  
  //ble_available();

  if (ble_connected())
  {
    digitalWrite(LED_PIN, HIGH);
  }
  else
  {
    digitalWrite(LED_PIN, LOW);
  }
  
  if (ble_available())
  {
    Serial.write("BLE available");
    while (ble_available())
    {
       int value = ble_read();
       if (value)
       {
           Serial.write("one found");
           //unsigned char bytes[] = "73 ";
           unsigned char byte1 = 73;
           //ble_write_bytes(bytes, );
           ble_write(byte1);
       }
            
    }
  }
     
  //ble_write('X');
  // Process BLE events.
  ble_do_events();
}
