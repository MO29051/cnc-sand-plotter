# CNC Sand Plotter

## 📌 Overview

This project presents the design and implementation of a 2-axis CNC sand plotter capable of drawing precise patterns using G-code. The system is based on an Arduino Uno running GRBL firmware, enabling accurate motion control of stepper motors for smooth and repeatable plotting.

The plotter demonstrates core CNC principles including motion control, coordinate systems, and digital fabrication workflows.

---

## 🎯 Objectives

* Build a low-cost CNC system
* Execute G-code for precise motion control
* Implement a complete CAD-to-machine workflow
* Achieve smooth and accurate plotting

---

## 🚀 Key Features

* 2-axis CNC motion system (X-Y)
* Arduino Uno with GRBL firmware
* G-code based motion execution
* Stepper motor control with calibrated steps/mm
* Servo-based pen up/down mechanism
* PC-to-machine communication via G-code sender

---

## ⚙️ System Architecture

```text id="0y6y9r"
PC (G-code Sender) → Arduino (GRBL) → Stepper Drivers → Motors
                                      ↓
                                   Servo (Pen)
```

---

## 🔩 Hardware Components

* Arduino Uno
* Stepper motors (e.g., 28BYJ-48)
* Stepper drivers (ULN2003 or similar)
* Servo motor (pen control)
* Power supply
* Custom mechanical frame (3D printed)

---

## 💻 Software & Tools

* GRBL firmware
* Arduino IDE
* Universal G-code Sender (UGS)
* CAD/CAM tools for generating G-code

---

## 🧠 Working Principle

### G-code Execution

The system reads G-code commands and translates them into stepper motor movements along X and Y axes.

### Motion Control

* Each axis is calibrated in **steps/mm**
* Ensures accurate translation from digital commands to physical motion

### Pen Control

* A servo motor is used to lift/lower the pen
* Controlled via modified GRBL (servo-enabled version)

---

## 📊 Results

* Smooth and continuous drawing paths
* High repeatability
* Accurate geometric pattern plotting
* Stable operation with minimal vibration

---

## 📷 Project Preview

(Add images here)

---

## 🎥 Demo

(Add video link here)

---

## 📁 Repository Structure

```plaintext id="y3m3wd"
cnc-sand-plotter/
│── images/
│── videos/
│── code/
│── hardware/
│── docs/
```

---

## ▶️ How to Run

1. Upload GRBL firmware to Arduino
2. Connect motors and drivers
3. Open Universal G-code Sender
4. Send G-code file
5. Start plotting

---

## 🧠 Engineering Highlights

* Calibrated steps/mm for precise motion
* Modified GRBL to support servo control
* Implemented full digital fabrication workflow

---

## 🔮 Future Improvements

* Add Z-axis for full CNC capability
* Improve frame rigidity
* Add limit switches for homing
* Integrate wireless control

---

## 📬 Contact

Feel free to reach out for collaboration or questions.
