#include <RBL_nRF8001.h>
#include <RBL_services.h>



// Define output pin
#define LED_PIN 13

void setup()
{
	// Enable serial debug.
	Serial.begin(9600);

	// Enable output.
	pinMode(LED_PIN, OUTPUT);

	// Turn off LED.
	digitalWrite(LED_PIN, LOW);

  // Set a custom BLE name.
  ble_set_name("ble_led");
	
	// Initialize BLE library.
	ble_begin();

	Serial.println("ble_begin done!");
}

// This function is called continuously, after setup() completes.
void loop()
{
	
	ble_available();

  char conn = ble_connected();
  Serial.println(conn);
	// Process BLE events.
	//ble_do_events();
}
