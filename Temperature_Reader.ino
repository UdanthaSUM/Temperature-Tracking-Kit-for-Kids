/**

 * @author IDEA8
 * @brief Temperature IR Bulb panel
 * @date 2024-1-8
 * 
 * @copyright Copyright (c) 2024
 * 
 */

 // Include neccessary files
#include <Wire.h>
#include "Adafruit_TMP117.h"
#include <Adafruit_Sensor.h>

Adafruit_TMP117 tmp117;

// Define IR pins
const int level_1 = 0; //27-27.71
const int level_2 = 1; //27.71 - 28.42
const int level_3 = 2; //28.42 - 29.13
const int level_4 = 3; //29.13 - 29.84
const int level_5 = 4; //29.84 - 30.55
const int level_6 = 5; //30.55 - 31.26
//const int level_7 = 15; //31.66 - 32
//const int level_8 = 15;

// Level array for the loop 
const int level_pins[] = {level_1, level_2, level_3, level_4, level_5, level_6};
const int num_levels = sizeof(level_pins) / sizeof(level_pins[0]);

// Neccessary variables
float red;
float green;
float temperature;
int check_tmp_count = 0; // create a counter for checking whether TMP117 sensor sends data
bool success_tmp_state = false;
int dotCount = 0;


// Function to initialize TMP117 sensor
void initializeTMP117() {
  bool firstAttempt = true;

  while (!tmp117.begin()) {

    if (firstAttempt) {
      Serial.println("Failed to find TMP117 chip");
      firstAttempt = false;
      success_tmp_state = false;  
      Serial.print("Trying to connect");
    } else {
      
      Serial.print(".");
      dotCount++;
      if (dotCount > 3) {
        dotCount = 1;
        Serial.print("\b\b\b"); // Clear the dots
      }
    }
    delay(1000); // Wait 1 second before retrying
    
  }
  
  if (success_tmp_state == false) {
    Serial.println();
    Serial.println("TMP117 Found!");
  }
  success_tmp_state = true;
}

// Function to Turn one IR Bulb
void lightingIRBulb() {
  int i = 0;

    // Turn on levels based on temperature condition
    while (temperature > 28 + i * 1 && i < num_levels) {
          digitalWrite(level_pins[i], HIGH);
          Serial.println(level_pins[i]);
          
        i++;
    }

    // Reset i for the next loop
    
    for (; i < num_levels; i++) {
        digitalWrite(level_pins[i], LOW);
        Serial.println(level_pins[i]);
        
    }
}


// Setup -------------------------------------------------------------
void setup(void) {
  Serial.begin(115200);
  // while (!Serial) delay(10);     // will pause Zero, Leonardo until serial console opens
  Serial.println("Adafruit TMP117 test!");

  // Initialize LED pins as outputs
  pinMode(level_1, OUTPUT);
  pinMode(level_2, OUTPUT);
  pinMode(level_3, OUTPUT);
  pinMode(level_4, OUTPUT);
  pinMode(level_5, OUTPUT);
  pinMode(level_6, OUTPUT);
  //pinMode(level_7, OUTPUT);
  


  // Try to initialize TMP117
  initializeTMP117();
}


// Loop ---------------------------------------------------------------
void loop() {

  sensors_event_t temp; // create an empty event to be filled
  tmp117.getEvent(&temp); //fill the empty event object with the current measurements
  temperature = temp.temperature;
  
  Serial.print("Temperature  "); Serial.print(temperature); Serial.println(" degrees C");
  Serial.println("");

  lightingIRBulb();
  
  // Checking whether data is received from the sensor it checks every 30 seconds continuously
  check_tmp_count++;
  if (check_tmp_count == 30) {
    initializeTMP117();
    check_tmp_count = 0;
  }


  delay(1000);
}


