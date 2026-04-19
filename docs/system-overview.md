# CNC Sand Plotter – System Overview

## 📌 Overview

This project is a 2-axis CNC sand plotter designed to draw patterns using G-code instructions executed by an Arduino running GRBL firmware.

The system converts digital designs into precise mechanical motion to create drawings on sand using a controlled pen mechanism.

---

## ⚙️ System Architecture

PC (G-code Sender)
→ GRBL (Arduino Uno)
→ Stepper Drivers
→ X & Y Axis Motors
→ Mechanical Plotting System
→ Servo (Pen Up/Down)

---

## 🧠 Key Idea

The system works by converting digital toolpaths into step-by-step motor movements, allowing precise control over X and Y coordinates.

---

## 🎯 Objective

* Understand CNC motion control principles
* Implement G-code based automation
* Build a low-cost drawing system

