#include <math.h>
#include <ros.h>
#include <radiation_msgs/DoseRate.h>

// Pin assignment
const int buzzer = 11;  // Audio chirp
const int samplePin = A1;  // Used to seed random number for RNG with analog noise

const int toneSwitch = A0;
int toneFlag = 0;

const int redPin = 3;
const int greenPin = 5;
const int bluePin = 6;

// Use for conversion of value to count rate
float scaling = 1.0;

// Used for timings
int periodDuration = 0;  // Time between interval - randomly generated
long lastTimeStart = 0;  // Time of last event
const int onDuration = 4;  // How long the buzzer should chirp for in ms

// ROS //
ros::NodeHandle nh;

float radRate = 0.0;  // Set to known rate to begin with

bool msgReceived = false;

void messageCb( const radiation_msgs::DoseRate& msg)
{
  msgReceived = true;
  radRate = msg.rate / scaling;  // scaling factor is for convenience, giving chirps at a rate that seem sensible to listen to
  periodDuration = random_gen(radRate);  // Generate new interval time

}

ros::Subscriber<radiation_msgs::DoseRate> sub("geiger_emulator", &messageCb );


// Random interval time generation
int random_gen(float rate) {

  // If count rate is zero (i.e. no detections that second), set rate to a very small number - avoid divide zero error
  if (rate == 0.0) {
    rate = 0.0000001;
    msgReceived = false;  // Stop all output
  }

  long randNumber = random(0, 10000);  // Generate a random number between 0 and 10000-1

  double fraction = randNumber / 10000.0;  // Normalise to (0,1]

  if (fraction == 1.00)  // Double check there is no 1.00 rounding
  {
    fraction = 0.99;
  }

  double logOfNumber = log(fraction);

  unsigned exp_random = 1000.0 * (logOfNumber * (-1.0 / rate)); //1000 converts seconds to milliseconds


  return exp_random;

}


void setup() {

  //  Serial.begin(9600);

  // Initialise pins
  pinMode(toneSwitch, INPUT_PULLUP);
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  pinMode(buzzer, OUTPUT);
  randomSeed(analogRead(samplePin));


  toneFlag = digitalRead(toneSwitch);
  chirpCheck();


  nh.initNode();
  nh.subscribe(sub);
  nh.spinOnce();
  delay(1);




}

void loop() {
  if (msgReceived) {  // Don't chirp/flash until a message has been received

    toneFlag = digitalRead(toneSwitch);

    // If the random time interval has passed
    if (millis() - lastTimeStart >= periodDuration) {

      chirp(toneFlag);

      // Generate and reset new time interval
      lastTimeStart = millis();
      periodDuration = random_gen(radRate);

    }

  }

  nh.spinOnce();
  delay(1);

}

// Chirp Function to handle buzzer and LEDs
void chirp(bool chirpFlag)
{
  // Play tone and flash LED
  if (chirpFlag) {
    RGB_colour(0, 175, 0);
    tone(buzzer, 400, onDuration);
    delay(2);
    RGB_colour(0, 0, 0);
  }

  // only flash LED
  else {
    RGB_colour(0, 0, 175);
    delay(onDuration);
    RGB_colour(0, 0, 0);

  }
}


// Helper function for converting RGB values to PWM values on tri-colour LEDs
void RGB_colour(int redValue, int greenValue, int blueValue)
{
  analogWrite(redPin, redValue);
  analogWrite(greenPin, greenValue);
  analogWrite(bluePin, blueValue);
}

void chirpCheck()
{
  // Check buzzer works
  chirp(true);
  delay(250);
  chirp(true);
  delay(250);
  // Turn off buzzer check
  chirp(false);
  delay(250);
  chirp(false);
  delay(250);
  // Check the current state of the switch as a check
  chirp(toneFlag);
  delay(250);
  chirp(toneFlag);
  delay(250);
  chirp(toneFlag);
  delay(250);
  chirp(toneFlag);
}
