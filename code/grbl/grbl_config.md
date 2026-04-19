# 🏖️ Sand Plotter GRBL Configuration (CoreXY)

This repository contains the GRBL 1.1 configuration used for a **CoreXY Sand Plotter system** running on Arduino UNO.

It includes motor tuning, homing setup, motion limits, and essential GRBL parameters to get your machine running.

---

## ⚠️ Important Notes

- GRBL **does NOT natively support CoreXY kinematics**
- You must either:
  - Use a **custom CoreXY Arduino firmware**, OR
  - Use **Marlin firmware with `COREXY` enabled**
- This file is mainly for **GRBL parameter tuning + motion setup**

---

## 🔧 Requirements

- Arduino UNO
- GRBL 1.1 firmware  
  https://github.com/gnea/grbl/releases
- CNC controller software:
  - Universal Gcode Sender (UGS)
  - bCNC
  - CNCjs

---

## 🚀 Installation Steps

### 1. Flash GRBL
Upload GRBL to Arduino UNO using Arduino IDE.

---

### 2. Connect & Open Serial Monitor
- Baud rate: `115200`
- Send:

```gcode
$X      ; Unlock alarm
$$      ; View current settings
