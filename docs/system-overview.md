# System Understanding

## 📌 Overview

The CNC sand plotter operates as a 2-axis motion control system that translates G-code commands into mechanical movement.

---

## ⚙️ System Components

* Arduino Uno (running GRBL firmware)
* Stepper motors (X and Y axes)
* Motor drivers
* Mechanical frame and motion system
* Sand surface with drawing mechanism

---

## 🔄 Working Principle

1. G-code is generated externally
2. Commands are sent to Arduino via serial communication
3. GRBL interprets commands
4. Stepper motors execute movement
5. Motion creates patterns in sand

---

## 🧠 Key Insight

The system acts as a physical representation of coordinate-based motion, where each movement corresponds to a precise position in space.

---

## 📊 Observation

Smoothness of motion depends on:

* Feed rate
* Mechanical stability
* Motor synchronization
