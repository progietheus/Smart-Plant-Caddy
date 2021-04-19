


// Variables will change:
int ledState = LOW;             // ledState used to set the LED
int pumpState = LOW;            // ledState used to set the LED
int moistureSensorValue = 0;    //Value of the analog moisture sensor
int brightness = 0;             // how bright the LED is
int fadeAmount = 5;             // how many points to fade the LED by

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long millisLed = 0;            // will store last time LED was updated
unsigned long millisSpeaker = 0;        // will store last time SPEAKER was updated
unsigned long millisPump = 0;           // will store last time PUMP was updated
unsigned long millisFade = 0;           // will store the last time the LED was faded in and out

// constants won't change:
const int moistureSensorPin = A2;             //Moisture Sensor Pin
const int ledPin = 3;                         //LED Pin
const int speakerPin = 11;                    //Speaker Pin
const int pumpPin = 12;                       //Water Pump Pin
const int moistureSensorFloor = 228;          //Wet Moisture Sensor value
const int moistureSensorCeiling = 503;        //Dry Moisture Sensor value
const int moistureInterval = (moistureSensorCeiling - moistureSensorFloor) / 3;

void setup() {
  Serial.begin(9600);
  pinMode(moistureSensorPin, INPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(speakerPin, OUTPUT);

}

void loop() {

  unsigned long currentMillis = millis();

  moistureSensorValue = analogRead(moistureSensorPin);

  if (moistureSensorValue >= moistureSensorCeiling) {
    Serial.println("Too Dry");
    Serial.print("Current Moisture: ");
    Serial.println(moistureSensorValue);
    
    //BLINK THE LED QUICKLY
    if (currentMillis - millisLed >= 500) {
      // save the last time you blinked the LED
      millisLed = currentMillis;

      // if the LED is off turn it on and vice-versa:
      if (ledState == LOW) {
        ledState = HIGH;
      } else {
        ledState = LOW;
      }

      // set the LED with the ledState of the variable:
      digitalWrite(ledPin, ledState);
    }

    //PLAY A TONE EVERY 5 SECONDS
    if (currentMillis - millisSpeaker >= 5000) {
      millisSpeaker = currentMillis;

      tone(speakerPin, 444, 500);

    }

    //CYCLE THE WATER PUMP ON FOR 10 SECONDS EVERY 10 SECONDS
    if (currentMillis - millisPump >= 10000) {
      millisPump = currentMillis;

      if (pumpState == "LOW") {
        pumpState = "HIGH";
      } else {
        pumpState = "LOW";
      }

      digitalWrite(pumpPin, pumpState);
    }

  }
  else if (moistureSensorValue < moistureSensorCeiling && moistureSensorValue > (moistureSensorCeiling - moistureInterval)) {
    Serial.println("Getting Dry");
    Serial.print("Current Moisture: ");
    Serial.println(moistureSensorValue);

    //Turn off LED
    digitalWrite(ledPin, LOW);

    digitalWrite(pumpPin, HIGH);
    delay(3000);
    digitalWrite(pumpPin, LOW);

  }
  else if (moistureSensorValue <= (moistureSensorCeiling - moistureInterval) && moistureSensorValue >= (moistureSensorFloor + moistureInterval)) {
    Serial.println("Perfect");
    Serial.print("Current Moisture: ");
    Serial.println(moistureSensorValue);

    analogWrite(ledPin, brightness);

    // change the brightness for next time through the loop:
    brightness = brightness + fadeAmount;

    // reverse the direction of the fading at the ends of the fade:
    if (brightness <= 0 || brightness >= 255) {
      fadeAmount = -fadeAmount;
    }
    // wait for 30 milliseconds to see the dimming effect
    delay(30);
  }
  else if (moistureSensorValue > moistureSensorFloor && moistureSensorValue < (moistureSensorFloor + moistureInterval)) {
    Serial.println("Get Too Wet");
    Serial.print("Current Moisture: ");
    Serial.println(moistureSensorValue);

    //Turn off LED
    digitalWrite(ledPin, LOW);


    if (currentMillis - millisSpeaker >= 1000) {
      millisSpeaker = currentMillis;

      tone(speakerPin, 444, 500);

    }
  }
  else if (moistureSensorValue <= moistureSensorFloor) {
    Serial.println("Too Wet");
    Serial.print("Current Moisture: ");
    Serial.println(moistureSensorValue);

    //Turn off LED
    digitalWrite(ledPin, LOW);
    
    if (currentMillis - millisSpeaker >= 3000) {
      millisSpeaker = currentMillis;

      tone(speakerPin, 444, 1500);

    }

  }


}
