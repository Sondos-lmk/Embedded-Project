# Wiring Guide - Test Setup (Without Rail)

## Quick Start Wiring

This guide shows the minimum wiring needed to test the system **without the linear rail**.

---

## Power Supply Setup

### ⚠️ IMPORTANT - Power Requirements

```
┌─────────────────────────────────────────────────┐
│  5V Power Supply (2A minimum)                   │
│  ├── (+) → Breadboard Power Rail (RED)         │
│  └── (-) → Breadboard Ground Rail (BLACK)      │
└─────────────────────────────────────────────────┘

┌─────────────────────────────────────────────────┐
│  Raspberry Pi Pico                              │
│  ├── VBUS (Pin 40) → 5V Rail (when USB powered)│
│  └── GND (Pin 3, 8, 13, 18, 23, 28, 33, 38)    │
│      → Ground Rail                              │
└─────────────────────────────────────────────────┘
```

**CRITICAL**: The MG996 servo draws significant current. Use an **external 5V 2A power supply**, not just USB power!

---

## Component Connections

### 1. MG996 Servo (Gripper) - GPIO 15

```
MG996 Servo
├── Brown/Black Wire  → GND (Ground Rail)
├── Red Wire          → 5V (External Power Rail)
└── Orange/Yellow Wire → GPIO 15 (Pico Pin 20)
```

**Note**: Connect servo power to the **external 5V supply**, share ground with Pico!

---

### 2. HC-SR04 Ultrasonic Sensor - GPIO 10, 11

```
HC-SR04
├── VCC   → 5V (Power Rail)
├── TRIG  → GPIO 10 (Pico Pin 14)
├── ECHO  → GPIO 11 (Pico Pin 15)
└── GND   → GND (Ground Rail)
```

---

### 3. 4×4 Keypad - GPIO 2-9

```
Keypad (4 Rows × 4 Columns)

Rows (Output Pins):
├── Row 1 → GPIO 2 (Pico Pin 4)
├── Row 2 → GPIO 3 (Pico Pin 5)
├── Row 3 → GPIO 4 (Pico Pin 6)
└── Row 4 → GPIO 5 (Pico Pin 7)

Columns (Input Pins with Pull-up):
├── Col 1 → GPIO 6 (Pico Pin 9)
├── Col 2 → GPIO 7 (Pico Pin 10)
├── Col 3 → GPIO 8 (Pico Pin 11)
└── Col 4 → GPIO 9 (Pico Pin 12)
```

**Keypad Layout:**
```
┌───┬───┬───┬───┐
│ 1 │ 2 │ 3 │ A │
├───┼───┼───┼───┤
│ 4 │ 5 │ 6 │ B │
├───┼───┼───┼───┤
│ 7 │ 8 │ 9 │ C │
├───┼───┼───┼───┤
│ * │ 0 │ # │ D │
└───┴───┴───┴───┘
```

---

### 4. Active Buzzer - GPIO 26

```
Active Buzzer (5V)
├── (+) → GPIO 26 (Pico Pin 31)
└── (-) → GND (Ground Rail)

Note: Some buzzers may need a transistor if they
draw more than 12mA. Test first!
```

---

### 5. Pushbuttons (Minimum 2 Required)

#### Grip Button - GPIO 21
```
Pushbutton (Normally Open)
├── One side → GPIO 21 (Pico Pin 27)
└── Other side → GND (Ground Rail)

Pico internal pull-up enabled in code ✓
```

#### Stop Button - GPIO 17
```
Pushbutton (Normally Open)
├── One side → GPIO 17 (Pico Pin 22)
└── Other side → GND (Ground Rail)

Pico internal pull-up enabled in code ✓
```

---

## Complete Pin Reference Table

| Component        | Pico GPIO | Pico Pin | Notes                    |
|------------------|-----------|----------|--------------------------|
| **Keypad**       |           |          |                          |
| Keypad Row 1     | GPIO 2    | Pin 4    | Output                   |
| Keypad Row 2     | GPIO 3    | Pin 5    | Output                   |
| Keypad Row 3     | GPIO 4    | Pin 6    | Output                   |
| Keypad Row 4     | GPIO 5    | Pin 7    | Output                   |
| Keypad Col 1     | GPIO 6    | Pin 9    | Input (pull-up)          |
| Keypad Col 2     | GPIO 7    | Pin 10   | Input (pull-up)          |
| Keypad Col 3     | GPIO 8    | Pin 11   | Input (pull-up)          |
| Keypad Col 4     | GPIO 9    | Pin 12   | Input (pull-up)          |
| **Ultrasonic**   |           |          |                          |
| HC-SR04 Trigger  | GPIO 10   | Pin 14   | Output                   |
| HC-SR04 Echo     | GPIO 11   | Pin 15   | Input                    |
| **Servo**        |           |          |                          |
| MG996 Gripper    | GPIO 15   | Pin 20   | PWM (ext. 5V power!)     |
| **Buttons**      |           |          |                          |
| Stop Button      | GPIO 17   | Pin 22   | Input (pull-up)          |
| Grip Button      | GPIO 21   | Pin 27   | Input (pull-up)          |
| **Buzzer**       |           |          |                          |
| Active Buzzer    | GPIO 26   | Pin 31   | Output                   |
| **Power**        |           |          |                          |
| 5V Input (USB)   | VBUS      | Pin 40   | USB power in             |
| 3.3V Output      | 3V3(OUT)  | Pin 36   | Don't use for servo!     |
| Ground (any)     | GND       | Pin 3,8,13,18,23,28,33,38 | |

---

## Not Used in Test Mode (For Future)

These pins are defined but **not connected** until you have the rail:

| Component        | Pico GPIO | Pico Pin | Status          |
|------------------|-----------|----------|-----------------|
| Motor IN1        | GPIO 12   | Pin 16   | ❌ Not connected|
| Motor IN2        | GPIO 13   | Pin 17   | ❌ Not connected|
| Motor ENA (PWM)  | GPIO 14   | Pin 19   | ❌ Not connected|
| Home Button      | GPIO 18   | Pin 24   | ❌ Not connected|
| Manual Fwd       | GPIO 19   | Pin 25   | ❌ Not connected|
| Manual Rev       | GPIO 20   | Pin 26   | ❌ Not connected|
| Limit Switch     | GPIO 22   | Pin 29   | ❌ Not connected|

---

## Testing Checklist

After wiring, verify each component:

### 🔌 Power
- [ ] 5V power supply connected to breadboard rails
- [ ] Pico GND connected to ground rail
- [ ] Pico USB connected (for programming and serial output)

### 🎮 Keypad
- [ ] All 8 wires connected (4 rows, 4 columns)
- [ ] Rows to GPIO 2-5
- [ ] Columns to GPIO 6-9

### 📏 Ultrasonic Sensor
- [ ] VCC to 5V
- [ ] GND to ground
- [ ] TRIG to GPIO 10
- [ ] ECHO to GPIO 11

### 🤖 Servo
- [ ] Brown/Black to ground
- [ ] Red to **external** 5V (NOT Pico 3.3V!)
- [ ] Orange/Yellow to GPIO 15
- [ ] Common ground between Pico and power supply

### 🔊 Buzzer
- [ ] (+) to GPIO 26
- [ ] (-) to ground

### 🔘 Buttons
- [ ] Grip button: GPIO 21 to GND
- [ ] Stop button: GPIO 17 to GND

---

## Quick Visual Check

```
                    ┌─────────────┐
   External 5V ────►│ Power Rails │◄──── Pico GND
                    └─────────────┘
                           │
        ┌──────────────────┼──────────────────┐
        ▼                  ▼                  ▼
    ┌───────┐         ┌────────┐        ┌────────┐
    │MG996  │         │HC-SR04 │        │ Buzzer │
    │Servo  │         │Sensor  │        └────────┘
    └───┬───┘         └────┬───┘
        │                  │
        └─────────┬────────┘
                  │
            ┌─────▼─────┐
            │    Pico   │
            │  RP2040   │
            └─────┬─────┘
                  │
        ┌─────────┴─────────┐
        ▼                   ▼
    ┌───────┐          ┌────────┐
    │Keypad │          │Buttons │
    │ 4×4   │          └────────┘
    └───────┘
```

---

## Safety Notes

⚠️ **DO NOT** connect the servo directly to Pico's 3.3V pin - it will damage the Pico!  
⚠️ **ALWAYS** use an external 5V supply for the servo  
⚠️ **ENSURE** common ground between all components  
✅ **TEST** with a multimeter before powering on  

---

## Next Steps

1. Wire everything according to this guide
2. Flash the test program (`main_test_no_rail.cpp`)
3. Open serial monitor to see output
4. Press keys on keypad to test each function
5. Verify servo moves smoothly
6. Check ultrasonic readings are accurate

When you get the rail:
- Add motor driver and DC motor connections
- Add limit switch for homing
- Switch back to `main_full.cpp`

---

**Happy Building! 🔧⚡**
