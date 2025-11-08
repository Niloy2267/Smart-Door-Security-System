#include <Keypad.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Servo.h>

// --- I. Pin Configuration ---
// Keypad: PINS - D2, D3, D4, D5 (Rows) | D6, D7, D8, A0 (Cols)
const byte ROWS = 4;
const byte COLS = 4;
char hexaKeys[ROWS][COLS] = {
    {'1','2','3','A'}, {'4','5','6','B'}, {'7','8','9','C'}, {'*','0','#','D'}
};
byte rowPins[ROWS] = {2, 3, 4, 5};   
byte colPins[COLS] = {6, 7, 8, A0};  
Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);

// RFID (SPI Communication): PINS - D10 (SS), A1 (RST)
#define SS_PIN 10  
#define RST_PIN A1 
MFRC522 mfrc522(SS_PIN, RST_PIN);

// Other Components
#define BUZZER_PIN A2     // Buzzer
#define IR_PIN A3         // IR Sensor 
#define SERVO_PIN 9       // Servo Motor 

// --- II. Global Variables & Settings ---
LiquidCrystal_I2C lcd(0x27, 16, 2); 
Servo doorServo;

// IR Sensor Logic: HIGH means 'No Obstacle'
#define NO_OBSTACLE HIGH

// Door State Management 
enum DoorState {LOCKED, UNLOCKED};
DoorState currentDoorState = LOCKED;

const int LOCK_ANGLE = 0;    
const int UNLOCK_ANGLE = 90; 
const long AUTOLOCK_DELAY = 5000; 
unsigned long unlockTime = 0;

// Authorized Access Data
const String MASTER_PASS = "1234";  
const String AUTHORIZED_UID_1 = "A3 22 FF A2"; 
const String AUTHORIZED_UID_2 = "52 08 1A 4D"; 
String enteredCode = "";


// --- III. Function Prototypes ---
void displayMessage(String line1, String line2, int duration);
void buzz(int freq, int duration);
void unlockDoor();
void lockDoor();
String getRfidUid();
bool isUidAuthorized(String uid);
void handleKeypadInput(char key);


void setup() {
  Serial.begin(9600); 
  
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(IR_PIN, INPUT);
  
  // LCD Setup
  lcd.init();
  lcd.backlight();
  displayMessage("SMART DOOR SYSTEM", "Starting...", 1500); 
  
  // RFID/SPI Setup
  SPI.begin();
  mfrc522.PCD_Init();
  
  // Servo Setup
  doorServo.attach(SERVO_PIN);
  lockDoor(); 
  
  // Final Ready Display
  displayMessage("SYSTEM READY", "Scan/Enter Code", 0);
}

void loop() {
  // --- A. Keypad Input Handling ---
  char key = customKeypad.getKey();
  if (key) {
    handleKeypadInput(key);
  }

  // --- B. RFID Input Handling ---
  if (mfrc522.PICC_IsNewCardPresent() && mfrc522.PICC_ReadCardSerial()) {
    String scannedUID = getRfidUid();
    
    if (isUidAuthorized(scannedUID)) {
      displayMessage("RFID ACCESS", "GRANTED! Door Open", 1000);
      unlockDoor();
    } else {
      displayMessage("RFID ACCESS", "DENIED! ALARM", 1000);
      buzz(500, 500);
    }
    mfrc522.PICC_HaltA();  
    mfrc522.PCD_StopCrypto1();  
  }
  
  // --- C. Door State and Auto-Lock ---
  if (currentDoorState == UNLOCKED) {
    // Check for automatic lock time
    if (millis() - unlockTime >= AUTOLOCK_DELAY) {
      if (digitalRead(IR_PIN) == NO_OBSTACLE) { 
        lockDoor();
        displayMessage("Door Locked", "System Ready", 1000);
      } else {
        // Alarm and display if obstacle is found
        buzz(1000, 100);  
        displayMessage("SAFETY ALERT!", "OBSTACLE FOUND!", 500);
      }
    }
  } else {
    // Display initial message if locked and no code is being entered
    if (enteredCode.length() == 0) {
      displayMessage("SYSTEM READY", "Scan/Enter Code", 0);
    }
  }
}

// --- IV. Function Library ---

// Function to handle Keypad input and logic
void handleKeypadInput(char key) {
    if (key >= '0' && key <= '9') {
        enteredCode += key;
        lcd.setCursor(0, 1);
        lcd.print("Code: ");
        for(int i=0; i<enteredCode.length(); i++) lcd.print("*");
    } else if (key == '#') {
        // '#' is the submit button
        if (enteredCode == MASTER_PASS) {
            displayMessage("Keypad ACCESS", "GRANTED! Door Open", 1000);
            unlockDoor();
        } else {
            displayMessage("Keypad ACCESS", "DENIED! ALARM", 1000);
            buzz(500, 500); 
        }
        enteredCode = ""; 
    } else if (key == '*') {
        // '*' is the reset/clear button
        enteredCode = "";
        displayMessage("Code Reset", "Enter Code:", 0);
    }
}

void displayMessage(String line1, String line2, int duration) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(line1);
  lcd.setCursor(0, 1);
  lcd.print(line2);
  if (duration > 0) delay(duration);
}

void buzz(int freq, int duration) {
  tone(BUZZER_PIN, freq, duration);
  delay(duration);
  noTone(BUZZER_PIN);
}

void unlockDoor() {
  doorServo.write(UNLOCK_ANGLE);
  buzz(800, 200); 
  currentDoorState = UNLOCKED;
  unlockTime = millis();
}

void lockDoor() {
  doorServo.write(LOCK_ANGLE);
  currentDoorState = LOCKED;
}

// Function to check UID authorization
bool isUidAuthorized(String uid) {
    return (uid == AUTHORIZED_UID_1 || uid == AUTHORIZED_UID_2);
}

String getRfidUid() {
  String uid = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    uid += (mfrc522.uid.uidByte[i] < 0x10 ? "0" : "");
    uid += String(mfrc522.uid.uidByte[i], HEX);
    if (i < mfrc522.uid.size - 1) uid += " ";
  }
  uid.toUpperCase();
  return uid;
}