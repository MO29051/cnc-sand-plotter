/*
 * ============================================================
 *  SAND PLOTTER — CoreXY Arduino Firmware
 *  Compatible with: Arduino UNO / Nano / Mega
 *  Motors: 2x NEMA 17 steppers via A4988 / DRV8825 drivers
 *
 *  WIRING:
 *    Motor A (Left)  → STEP=2, DIR=3
 *    Motor B (Right) → STEP=4, DIR=5
 *    Enable (both)   → PIN 8  (LOW = enabled)
 *    End-stop X      → PIN 9  (INPUT_PULLUP)
 *    End-stop Y      → PIN 10 (INPUT_PULLUP)
 *
 *  HOW IT WORKS (CoreXY kinematics):
 *    X move  → Motor A and B turn SAME direction
 *    Y move  → Motor A and B turn OPPOSITE directions
 *
 *  Upload this sketch, then send GCode over Serial at 115200 baud.
 *  Supported commands: G0, G1, G28, M0, M2, F (feed rate)
 * ============================================================
 */

#include <AccelStepper.h>

// ── Pin Definitions ──────────────────────────────────────────
#define STEP_A    2
#define DIR_A     3
#define STEP_B    4
#define DIR_B     5
#define ENABLE    8
#define ENDSTOP_X 9
#define ENDSTOP_Y 10

// ── Machine Settings ─────────────────────────────────────────
#define STEPS_PER_MM      80.0f   // Adjust: steps/rev ÷ (pulley_teeth × belt_pitch)
                                   // e.g. 200steps × 16microstep ÷ (20teeth × 2mm) = 80
#define MAX_SPEED         3000.0f  // steps/sec
#define ACCELERATION      1500.0f  // steps/sec²
#define HOMING_SPEED      500.0f   // steps/sec (slow for accuracy)
#define BED_WIDTH_MM      280.0f   // X travel in mm
#define BED_HEIGHT_MM     380.0f   // Y travel in mm

// ── Stepper Objects (DRIVER mode = just STEP + DIR) ──────────
AccelStepper motorA(AccelStepper::DRIVER, STEP_A, DIR_A);
AccelStepper motorB(AccelStepper::DRIVER, STEP_B, DIR_B);

// ── State ────────────────────────────────────────────────────
float currentX = 0, currentY = 0;    // mm, logical position
float feedRate = 1000.0f;             // mm/min
bool  absoluteMode = true;

// ── GCode Buffer ─────────────────────────────────────────────
String gcodeBuffer = "";

// ─────────────────────────────────────────────────────────────
void setup() {
  Serial.begin(115200);

  pinMode(ENABLE,    OUTPUT);
  pinMode(ENDSTOP_X, INPUT_PULLUP);
  pinMode(ENDSTOP_Y, INPUT_PULLUP);

  digitalWrite(ENABLE, LOW);  // Enable motors

  motorA.setMaxSpeed(MAX_SPEED);
  motorA.setAcceleration(ACCELERATION);
  motorB.setMaxSpeed(MAX_SPEED);
  motorB.setAcceleration(ACCELERATION);

  Serial.println(F("Sand Plotter Ready. Send GCode at 115200 baud."));
  Serial.println(F("Type G28 to home, then send your .gcode file."));

  homeAxes();
}

// ─────────────────────────────────────────────────────────────
void loop() {
  // Read incoming GCode line by line
  while (Serial.available()) {
    char c = Serial.read();
    if (c == '\n' || c == '\r') {
      gcodeBuffer.trim();
      if (gcodeBuffer.length() > 0) {
        processGCode(gcodeBuffer);
        gcodeBuffer = "";
        Serial.println(F("ok"));
      }
    } else {
      gcodeBuffer += c;
    }
  }

  // Keep steppers running
  runMotors();
}

// ─────────────────────────────────────────────────────────────
//  CoreXY MOVE — converts logical X/Y → motor A/B steps
// ─────────────────────────────────────────────────────────────
void moveTo(float targetX, float targetY) {
  // Clamp to bed limits
  targetX = constrain(targetX, 0, BED_WIDTH_MM);
  targetY = constrain(targetY, 0, BED_HEIGHT_MM);

  // CoreXY: A = X + Y,  B = X - Y
  long stepsA = (long)((targetX + targetY) * STEPS_PER_MM);
  long stepsB = (long)((targetX - targetY) * STEPS_PER_MM);

  // Set speed from feed rate (mm/min → steps/sec)
  float speedStepsPerSec = (feedRate / 60.0f) * STEPS_PER_MM;
  speedStepsPerSec = constrain(speedStepsPerSec, 1, MAX_SPEED);

  motorA.setMaxSpeed(speedStepsPerSec);
  motorB.setMaxSpeed(speedStepsPerSec);

  motorA.moveTo(stepsA);
  motorB.moveTo(stepsB);

  // Block until move is complete
  while (motorA.distanceToGo() != 0 || motorB.distanceToGo() != 0) {
    motorA.run();
    motorB.run();
  }

  currentX = targetX;
  currentY = targetY;
}

// ─────────────────────────────────────────────────────────────
//  HOMING — moves to endstops (0,0 = home corner)
// ─────────────────────────────────────────────────────────────
void homeAxes() {
  Serial.println(F("Homing..."));

  motorA.setMaxSpeed(HOMING_SPEED);
  motorB.setMaxSpeed(HOMING_SPEED);

  // Move toward X endstop
  while (digitalRead(ENDSTOP_X) == HIGH) {
    motorA.move(-1); motorB.move(-1);
    motorA.run();    motorB.run();
  }
  // Move toward Y endstop
  while (digitalRead(ENDSTOP_Y) == HIGH) {
    motorA.move(-1); motorB.move(1);
    motorA.run();    motorB.run();
  }

  // Zero position
  motorA.setCurrentPosition(0);
  motorB.setCurrentPosition(0);
  currentX = 0; currentY = 0;

  // Back off endstops slightly
  motorA.setMaxSpeed(MAX_SPEED);
  motorB.setMaxSpeed(MAX_SPEED);
  moveTo(5, 5);

  Serial.println(F("Homing complete."));
}

// ─────────────────────────────────────────────────────────────
//  KEEP MOTORS STEPPING (call in loop)
// ─────────────────────────────────────────────────────────────
void runMotors() {
  motorA.run();
  motorB.run();
}

// ─────────────────────────────────────────────────────────────
//  PARSE & EXECUTE GCODE
// ─────────────────────────────────────────────────────────────
void processGCode(String line) {
  // Strip comments (everything after ';' or in parentheses)
  int commentIdx = line.indexOf(';');
  if (commentIdx >= 0) line = line.substring(0, commentIdx);
  line.toUpperCase();
  line.trim();
  if (line.length() == 0) return;

  // Extract command letter + number
  char cmd = ' ';
  int  cmdNum = -1;
  if (line.length() >= 2 && isAlpha(line[0])) {
    cmd    = line[0];
    cmdNum = line.substring(1).toInt();
  }

  // Parse X Y F parameters
  float paramX = currentX, paramY = currentY;
  bool  hasX = false, hasY = false;

  int xi = line.indexOf('X');
  if (xi >= 0) { paramX = extractFloat(line, xi + 1); hasX = true; }

  int yi = line.indexOf('Y');
  if (yi >= 0) { paramY = extractFloat(line, yi + 1); hasY = true; }

  int fi = line.indexOf('F');
  if (fi >= 0) { feedRate = extractFloat(line, fi + 1); }

  // ── Execute ──
  if (cmd == 'G') {
    switch (cmdNum) {
      case 0:  // G0 — Rapid move (use high speed)
        if (hasX || hasY) {
          float savedFeed = feedRate;
          feedRate = (BED_WIDTH_MM * STEPS_PER_MM * 60.0f) / STEPS_PER_MM; // max
          feedRate = 6000;
          if (!absoluteMode) { paramX += currentX; paramY += currentY; }
          moveTo(paramX, paramY);
          feedRate = savedFeed;
        }
        break;

      case 1:  // G1 — Linear move
        if (hasX || hasY) {
          if (!absoluteMode) { paramX += currentX; paramY += currentY; }
          moveTo(paramX, paramY);
        }
        break;

      case 21: break;  // G21 mm mode (assumed)
      case 90: absoluteMode = true;  break;  // G90 absolute
      case 91: absoluteMode = false; break;  // G91 relative
      case 28: homeAxes(); break;             // G28 home
    }
  } else if (cmd == 'M') {
    switch (cmdNum) {
      case 0:   // M0 — pause (wait for serial)
      case 1:
        Serial.println(F("Paused. Send any character to continue."));
        while (!Serial.available()) {}
        Serial.read();
        break;
      case 2:   // M2 — end of program
      case 30:
        Serial.println(F("Program end."));
        digitalWrite(ENABLE, HIGH); // disable motors
        break;
      case 17:  digitalWrite(ENABLE, LOW);  break;  // M17 enable
      case 18:  digitalWrite(ENABLE, HIGH); break;  // M18 disable
    }
  }
}

// ─────────────────────────────────────────────────────────────
//  HELPER — extract float after a letter in the line
// ─────────────────────────────────────────────────────────────
float extractFloat(String &s, int startIdx) {
  int i = startIdx;
  // Allow leading minus
  while (i < (int)s.length() && (isDigit(s[i]) || s[i] == '-' || s[i] == '.')) i++;
  return s.substring(startIdx, i).toFloat();
}
