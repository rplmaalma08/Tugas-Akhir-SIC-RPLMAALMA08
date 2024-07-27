#include <arduino.h>
#include <Adafruit_Fingerprint.h>
#include <HardwareSerial.h>
#include <ESP32Servo.h> 

HardwareSerial mySerial(2); // RX, TX
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);
Servo myservo;  // create servo object to control a servo


int servoPin = 18;    //pin servo

char mode = 'd'; // Default mode is detection

void setup() {


  Serial.begin(9600);

  while (!Serial);
  delay(100);
  Serial.println("Adafruit Fingerprint sensor test");
  mySerial.begin(57600,SERIAL_8N1,16,17);
  finger.begin(57600);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1);
  }

  // Allow allocation of all timers
	ESP32PWM::allocateTimer(0);
	ESP32PWM::allocateTimer(1);
	ESP32PWM::allocateTimer(2);
	ESP32PWM::allocateTimer(3);
  myservo.setPeriodHertz(50);// Standard 50hz servo
  myservo.attach(servoPin, 500, 2400);   // attaches the servo on pin 18 to the servo object
                                         // using SG90 servo min/max of 500us and 2400us
                                         // for MG995 large servo, use 1000us and 2000us,
                                         // which are the defaults, so this line could be
                                         // "myservo.attach(servoPin);"

}

void loop() {

  if (Serial.available()) {
    char key = Serial.read();
    if (key == 'r') {
      mode = 'r';
      Serial.println("Mode changed to registration");
    } else if (key == 'd') {
      mode = 'd';
      Serial.println("Mode changed to detection");
    }
  }

  if (mode == 'r') {
    registerFingerprint();
  } else if (mode == 'd') {
    detectFingerprint();
  }
  delay(100);
}

void registerFingerprint() {
  Serial.println("Ready to enroll a fingerprint! Please type in the ID # you want to save this finger as...");
  int id = readNumber();
  if (id == -1) return;
  
  Serial.print("Enrolling ID #");
  Serial.println(id);

  while (!getFingerprintEnroll(id));
}


void detectFingerprint() { 
  Serial.println("Waiting for valid finger..."); 
  uint8_t p = finger.getImage(); 
  switch (p) { 
    case FINGERPRINT_OK: 
      Serial.println("Image taken"); 
      break; 
    case FINGERPRINT_NOFINGER: 
      Serial.println(". "); 
      break; 
    case FINGERPRINT_PACKETRECIEVEERR: 
      Serial.println("Communication error"); 
      break; 
    case FINGERPRINT_IMAGEFAIL: 
      Serial.println("Imaging error"); 
      break; 
    default: 
      Serial.println("Unknown error"); 
      break; 
  } 
 
  p = finger.image2Tz(); 
  switch (p) { 
    case FINGERPRINT_OK: 
      Serial.println("Image converted"); 
      break; 
    case FINGERPRINT_IMAGEMESS: 
      Serial.println("Image too messy"); 
      return; 
    case FINGERPRINT_PACKETRECIEVEERR: 
      Serial.println("Communication error"); 
      return; 
    case FINGERPRINT_FEATUREFAIL: 
      Serial.println("Could not find fingerprint features"); 
      return; 
    case FINGERPRINT_INVALIDIMAGE: 
      Serial.println("Could not find fingerprint features"); 
      return; 
    default: 
      Serial.println("Unknown error"); 
      return; 
  } 
 
  p = finger.fingerFastSearch(); 
  if (p == FINGERPRINT_OK) { 
    Serial.print("Found a print match! ID: "); 
    Serial.println(finger.fingerID); 
    setServo(90); // Set servo angle to 90 degrees 
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) { 
    Serial.println("Communication error"); 
  } else if (p == FINGERPRINT_NOTFOUND) { 
    Serial.println("Did not find a match"); 
  } else { 
    Serial.println("Unknown error"); 
  } 
}

void setServo(int angle) {
  myservo.write(angle); 
  delay(200); 
}

int readNumber() {
  while (!Serial.available());
  int number = Serial.parseInt();
  if (number == 0) {
    Serial.println("Invalid input");
    return -1;
  }
  return number;
}

uint8_t getFingerprintEnroll(uint8_t id) {
  int p = -1;
  Serial.println("Waiting for valid finger to enroll as ID #" + String(id));
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  p = finger.image2Tz(1);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.println("Remove finger");
  delay(2000);
  p = 0;
  while (p != FINGERPRINT_NOFINGER) {
    p = finger.getImage();
  }
  Serial.println("Place same finger again");

  p = -1;
  while (p != FINGERPRINT_OK) {
    p = finger.getImage();
    switch (p) {
      case FINGERPRINT_OK:
        Serial.println("Image taken");
        break;
      case FINGERPRINT_NOFINGER:
        Serial.println(".");
        break;
      case FINGERPRINT_PACKETRECIEVEERR:
        Serial.println("Communication error");
        break;
      case FINGERPRINT_IMAGEFAIL:
        Serial.println("Imaging error");
        break;
      default:
        Serial.println("Unknown error");
        break;
    }
  }

  p = finger.image2Tz(2);
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  Serial.print("Creating model for #");
  Serial.println(id);

  p = finger.createModel();
  if (p == FINGERPRINT_OK) {
    Serial.println("Prints matched!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_ENROLLMISMATCH) {
    Serial.println("Fingerprints did not match");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  Serial.print("ID ");
  Serial.println(id);
  p = finger.storeModel(id);
  if (p == FINGERPRINT_OK) {
    Serial.println("Stored!");
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_BADLOCATION) {
    Serial.println("Could not store in that location");
    return p;
  } else if (p == FINGERPRINT_FLASHERR) {
    Serial.println("Error writing to flash");
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }


  return true;
}