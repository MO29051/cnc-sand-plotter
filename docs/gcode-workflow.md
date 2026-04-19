# G-code Implementation

## 📌 Overview

G-code is the core control language used to operate the CNC sand plotter.

---

## 🧾 Types of G-code Used

### Manual G-code

Used for simple shapes such as:

* Squares
* Straight lines

This helped in understanding:

* Coordinate movement
* Axis control

---

### Generated G-code

Complex patterns were created using an external tool that converts shapes or images into G-code.

This allows:

* Continuous paths
* Complex designs
* Efficient workflow

---

## 🔄 Execution Process

1. Generate G-code
2. Load into Universal G-code Sender (UGS)
3. Send commands to Arduino
4. Machine executes motion

---

## 🧠 Key Learning

G-code is essentially a sequence of position commands that define the path followed by the system.

---

## 📊 Result

The system successfully executed:

* Basic geometric shapes
* Complex patterns with smooth motion

