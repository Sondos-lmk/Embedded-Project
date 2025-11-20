# Complete Wiring Guide - Full System (With Linear Rail)

## Overview

This guide shows the **complete wiring** for the full puzzle-sorting robot system including the linear rail, motor driver, and all components.

---

## Power Supply Requirements

### ⚠️ CRITICAL - Multiple Power Supplies Needed

```
┌─────────────────────────────────────────────────────────────┐
│  POWER DISTRIBUTION                                         │
├─────────────────────────────────────────────────────────────┤
│                                                             │
│  [5V 3A Power Supply] ──┬── Servo Motor (MG996)           │
│                         └── HC-SR04 Ultrasonic Sensor      │
│                                                             │
│  [12V 2A Power Supply] ──── L298N Motor Driver             │
│                             (for DC motor)                  │
│                                                             │
│  [USB 5V] ──────────────── Raspberry Pi Pico               │
│                             (programming + serial output)   │
│                                                             │
│  ⚠️ ALL GROUNDS MUST BE CONNECTED TOGETHER ⚠️             │
└─────────────────────────────────────────────────────────────┘
```

**Power Budget:**
- **Pico**: ~100mA (from USB)
- **MG996 Servo**: 500-2500mA (peaks at 2.5A!)
- **DC Motor**: 500-1000mA (via L298N)
- **HC-SR04**: ~15mA
- **Buzzer**: ~30mA
- **Keypad/Buttons**: <1mA each

**Total**: ~4A peak (use adequate supplies!)

---

## Complete Component Connections

### 1. Raspberry Pi Pico - Main Controller

```
Raspberry Pi Pico RP2040
├── Pin 40 (VBUS)     → USB 5V IN (when connected)
├── Pin 36 (3V3 OUT)  → Available (do NOT use for servo/motor!)
├── Pin 3,8,13,18,23,28,33,38 (GND) → Common Ground Rail
└── USB Port → PC (for programming and serial monitor)
```

---

### 2. 4×4 Matrix Keypad - GPIO 2-9

```
4×4 Keypad Connections
┌─────────────────────────────────────────────┐
│ Pin Layout:  [1] [2] [3] [A]                │
│              [4] [5] [6] [B]                │
│              [7] [8] [9] [C]                │
│              [*] [0] [#] [D]                │
└─────────────────────────────────────────────┘

Row Pins (Output):
├── Row 1 → GPIO 2 (Pico Pin 4)
├── Row 2 → GPIO 3 (Pico Pin 5)
├── Row 3 → GPIO 4 (Pico Pin 6)
└── Row 4 → GPIO 5 (Pico Pin 7)

Column Pins (Input with Pull-up):
├── Col 1 → GPIO 6 (Pico Pin 9)
├── Col 2 → GPIO 7 (Pico Pin 10)
├── Col 3 → GPIO 8 (Pico Pin 11)
└── Col 4 → GPIO 9 (Pico Pin 12)

Note: Internal pull-ups enabled in software
```

---

### 3. HC-SR04 Ultrasonic Sensor - GPIO 10, 11

```
HC-SR04 Distance Sensor
├── VCC   → 5V Power Rail (external supply)
├── TRIG  → GPIO 10 (Pico Pin 14)
├── ECHO  → GPIO 11 (Pico Pin 15)
└── GND   → Common Ground Rail

Mounting: Position sensor on slider/gripper assembly
          to detect puzzle pieces
```

---

### 4. L298N Motor Driver + DC Motor - GPIO 12, 13, 14

```+
L298N Motor Driver Module
┌─────────────────────────────────────────────┐
│ Power Inputs:                               │
│  ├── +12V    → 12V Power Supply (+)         │
│  ├── GND     → Common Ground Rail           │
│  └── +5V     → NOT CONNECTED (use jumper)   │
│                                             │
│ Control Inputs (from Pico):                 │
│  ├── IN1     → GPIO 12 (Pico Pin 16)        │
│  ├── IN2     → GPIO 13 (Pico Pin 17)        │
│  ├── ENA     → GPIO 14 (Pico Pin 19) [PWM]  │
│  └── GND     → Common Ground Rail           │
│                                             │
│ Motor Outputs:                              │
│  ├── OUT1    → DC Motor Wire 1              │
│  └── OUT2    → DC Motor Wire 2              │
└─────────────────────────────────────────────┘

DC Motor (3-6V):
├── Wire 1 → L298N OUT1
└── Wire 2 → L298N OUT2

Notes:
- Enable 5V regulator jumper on L298N
- Motor direction: IN1=HIGH,IN2=LOW (forward)
                  IN1=LOW,IN2=HIGH (reverse)
- Speed control: PWM on ENA (0-100%)
```

---

### 5. MG996 Servo Motor - GPIO 15

```
MG996 Servo (Gripper)
├── Brown/Black Wire  → Common Ground Rail
├── Red Wire          → 5V Power Rail (external 3A supply!)
└── Orange Wire       → GPIO 15 (Pico Pin 20) [PWM]

⚠️ CRITICAL WARNINGS:
- DO NOT power from Pico 3.3V or VBUS
- Use EXTERNAL 5V 3A power supply
- Common ground with Pico is REQUIRED
- Servo can draw 2.5A under load!

Mounting: Attach to slider assembly for gripper control
```

---

### 6. Active Buzzer - GPIO 26

```
Active Buzzer (5V)
├── Positive (+) → GPIO 26 (Pico Pin 31)
└── Negative (-) → Common Ground Rail

Alternative (if buzzer draws >12mA):
Use NPN transistor (2N2222):
├── Buzzer (+)        → 5V
├── Buzzer (-)        → Transistor Collector
├── Transistor Base   → GPIO 26 via 1kΩ resistor
└── Transistor Emitter → GND
```

---

### 7. Pushbuttons - GPIO 17-22

All buttons wired identically (Normally Open, Active Low):

```
┌────────────────────────────────────────────┐
│  Each Button:                              │
│  ├── One side  → GPIO Pin                  │
│  └── Other side → GND                      │
│                                            │
│  Internal pull-up resistors enabled        │
│  Pressed = LOW (0), Released = HIGH (1)    │
└────────────────────────────────────────────┘

Button Assignments:
├── Stop Button      → GPIO 17 (Pico Pin 22)
├── Home Button      → GPIO 18 (Pico Pin 24)
├── Manual Fwd       → GPIO 19 (Pico Pin 25)
├── Manual Rev       → GPIO 20 (Pico Pin 26)
├── Grip Button      → GPIO 21 (Pico Pin 27)
└── Limit Switch     → GPIO 22 (Pico Pin 29) [Home position sensor]

Limit Switch:
- Normally Open switch at home position
- Closes when slider reaches home
- Used for homing routine
```

---

## Complete Pin Reference Table

| Component          | Function        | Pico GPIO | Pico Pin | Type          | Notes                           |
|--------------------|-----------------|-----------|----------|---------------|---------------------------------|
| **POWER**          |                 |           |          |               |                                 |
| USB Power          | 5V Input        | VBUS      | Pin 40   | Power In      | For Pico only                   |
| 3.3V Output        | Regulated 3.3V  | 3V3(OUT)  | Pin 36   | Power Out     | Max 300mA - not for motors!     |
| Ground             | Common GND      | GND       | 3,8,13,18,23,28,33,38 | Ground | Connect to all GND rails |
| **KEYPAD**         |                 |           |          |               |                                 |
| Keypad Row 1       | Matrix Row      | GPIO 2    | Pin 4    | Output        | Set LOW to scan                 |
| Keypad Row 2       | Matrix Row      | GPIO 3    | Pin 5    | Output        | Set LOW to scan                 |
| Keypad Row 3       | Matrix Row      | GPIO 4    | Pin 6    | Output        | Set LOW to scan                 |
| Keypad Row 4       | Matrix Row      | GPIO 5    | Pin 7    | Output        | Set LOW to scan                 |
| Keypad Col 1       | Matrix Column   | GPIO 6    | Pin 9    | Input (PU)    | Read LOW when key pressed       |
| Keypad Col 2       | Matrix Column   | GPIO 7    | Pin 10   | Input (PU)    | Read LOW when key pressed       |
| Keypad Col 3       | Matrix Column   | GPIO 8    | Pin 11   | Input (PU)    | Read LOW when key pressed       |
| Keypad Col 4       | Matrix Column   | GPIO 9    | Pin 12   | Input (PU)    | Read LOW when key pressed       |
| **ULTRASONIC**     |                 |           |          |               |                                 |
| HC-SR04 Trigger    | Trigger Pulse   | GPIO 10   | Pin 14   | Output        | 10µs pulse to trigger           |
| HC-SR04 Echo       | Echo Response   | GPIO 11   | Pin 15   | Input         | Read pulse duration             |
| **MOTOR DRIVER**   |                 |           |          |               |                                 |
| L298N IN1          | Direction 1     | GPIO 12   | Pin 16   | Output        | Motor direction control         |
| L298N IN2          | Direction 2     | GPIO 13   | Pin 17   | Output        | Motor direction control         |
| L298N ENA          | Speed (PWM)     | GPIO 14   | Pin 19   | PWM Output    | 0-100% speed control            |
| **SERVO**          |                 |           |          |               |                                 |
| MG996 Signal       | PWM Control     | GPIO 15   | Pin 20   | PWM Output    | 50Hz servo signal               |
| **BUTTONS**        |                 |           |          |               |                                 |
| Stop Button        | Emergency Stop  | GPIO 17   | Pin 22   | Input (PU)    | Halt all operations             |
| Home Button        | Manual Home     | GPIO 18   | Pin 24   | Input (PU)    | Return to home position         |
| Manual Fwd Button  | Manual Forward  | GPIO 19   | Pin 25   | Input (PU)    | Manual rail forward             |
| Manual Rev Button  | Manual Reverse  | GPIO 20   | Pin 26   | Input (PU)    | Manual rail reverse             |
| Grip Button        | Manual Grip     | GPIO 21   | Pin 27   | Input (PU)    | Toggle gripper open/close       |
| Limit Switch       | Home Sensor     | GPIO 22   | Pin 29   | Input (PU)    | Detects home position           |
| **BUZZER**         |                 |           |          |               |                                 |
| Active Buzzer      | Audio Feedback  | GPIO 26   | Pin 31   | Output        | Beep patterns                   |

**Legend:**
- PU = Pull-Up resistor enabled (internal)
- PWM = Pulse Width Modulation capable

---

## Power Distribution Diagram

```
                    POWER DISTRIBUTION
                    
    ┌──────────────────────────────────────────────────┐
    │         12V 2A Power Supply                      │
    │              (for Motor)                         │
    └───────────┬──────────────────────────────────────┘
                │
                ├─(+12V)──→ L298N VCC
                └─(GND)───→ Common Ground ─┐
                                           │
    ┌──────────────────────────────────────┼───────────┐
    │         5V 3A Power Supply           │           │
    │         (for Servo & Sensor)         │           │
    └───────────┬──────────────────────────┼───────────┘
                │                          │
                ├─(+5V)──→ MG996 Servo     │
                ├─(+5V)──→ HC-SR04 Sensor  │
                ├─(+5V)──→ Buzzer (opt.)   │
                └─(GND)──→ Common Ground ──┤
                                           │
    ┌──────────────────────────────────────┼───────────┐
    │         USB 5V                       │           │
    │         (for Pico)                   │           │
    └───────────┬──────────────────────────┼───────────┘
                │                          │
                ├─(+5V)──→ Pico VBUS       │
                └─(GND)──→ Common Ground ──┘
                
    ═══════════════════════════════════════════════════
                    COMMON GROUND RAIL
    ═══════════════════════════════════════════════════
    All GND connections must be joined together!
```

---

## Physical Mounting Layout

```
                    TOP VIEW - LINEAR RAIL SYSTEM
                    
    [Home]                                        [Drop-off]
    Limit                                         Position
    Switch                                        
      │                                              
      ▼                                              
    ┌─┴────────────────────────────────────────────────┐
    │░░░░░░░░░░░░░░  Linear Rail  ░░░░░░░░░░░░░░░░░░░░│
    └──────────────────────────────────────────────────┘
           ▲                                ▲
           │                                │
       [Slider Assembly]              [DC Motor]
           │                           (moves slider)
           │
      ┌────┴─────┐
      │  Gripper │ ← MG996 Servo
      │   (open) │
      └──────────┘
           │
      ┌────┴─────┐
      │ HC-SR04  │ ← Ultrasonic Sensor
      │  Sensor  │   (detects pieces)
      └──────────┘
           
           ▼
    [  Puzzle Piece  ]
    [   Position 1   ]
```

---

## Wiring Checklist

### ✅ Before Powering On:

#### Power Connections:
- [ ] 12V supply connected to L298N only
- [ ] 5V supply connected to servo and sensor
- [ ] USB connected to Pico
- [ ] **ALL GROUNDS CONNECTED TOGETHER**
- [ ] Verify polarity with multimeter

#### Keypad:
- [ ] 4 row wires to GPIO 2-5
- [ ] 4 column wires to GPIO 6-9
- [ ] Keypad oriented correctly

#### Ultrasonic Sensor:
- [ ] VCC to 5V
- [ ] TRIG to GPIO 10
- [ ] ECHO to GPIO 11
- [ ] GND to common ground

#### Motor Driver (L298N):
- [ ] +12V to VCC
- [ ] GND to common ground
- [ ] IN1 to GPIO 12
- [ ] IN2 to GPIO 13
- [ ] ENA to GPIO 14
- [ ] Motor wires to OUT1, OUT2
- [ ] 5V regulator jumper installed

#### Servo (MG996):
- [ ] Brown/Black to common ground
- [ ] Red to **external** 5V (NOT Pico!)
- [ ] Orange to GPIO 15
- [ ] Secure mechanical mounting

#### Buttons:
- [ ] Stop → GPIO 17 + GND
- [ ] Home → GPIO 18 + GND
- [ ] Manual Fwd → GPIO 19 + GND
- [ ] Manual Rev → GPIO 20 + GND
- [ ] Grip → GPIO 21 + GND
- [ ] Limit Switch → GPIO 22 + GND

#### Buzzer:
- [ ] (+) to GPIO 26
- [ ] (-) to common ground

---

## Testing Sequence (Power-On)

### Step 1: Power Supply Test (NO PICO YET)
```
1. Connect ONLY the 12V supply to L298N
2. Measure voltage at L298N VCC (should be 12V)
3. Connect ONLY the 5V supply to servo power
4. Measure voltage at servo red wire (should be 5V)
5. Verify all grounds are at 0V
6. DISCONNECT power before proceeding
```

### Step 2: Pico Test (MOTORS DISCONNECTED)
```
1. Disconnect motor and servo signal wires
2. Connect USB to Pico
3. Flash test program
4. Verify serial output
5. Test keypad only
6. DISCONNECT USB before proceeding
```

### Step 3: Sensor Test
```
1. Connect all power supplies
2. Connect USB to Pico
3. Test ultrasonic sensor (should read distance)
4. Test buzzer (should beep)
5. Test all buttons
```

### Step 4: Servo Test
```
1. Ensure 5V 3A supply connected to servo
2. Connect servo signal wire to GPIO 15
3. Test gripper movement (watch for jitter)
4. If jittering, check power supply amperage
```

### Step 5: Motor Test
```
1. Ensure 12V supply connected to L298N
2. Ensure slider can move freely on rail
3. Test manual forward (button on GPIO 19)
4. Test manual reverse (button on GPIO 20)
5. Verify smooth movement
6. Test emergency stop (button on GPIO 17)
```

### Step 6: Limit Switch Test
```
1. Move slider to home position manually
2. Verify limit switch closes (reads LOW on GPIO 22)
3. Move slider away from home
4. Verify limit switch opens (reads HIGH on GPIO 22)
```

### Step 7: Homing Test
```
1. Move slider away from home position
2. Press Home button (GPIO 18)
3. Verify slider moves toward home
4. Verify slider stops when limit switch triggered
5. Verify buzzer confirms completion
```

### Step 8: Full System Test
```
1. Press '1' on keypad
2. Verify slider moves to position 1
3. Place puzzle piece at position 1
4. Verify ultrasonic detects piece
5. Verify gripper closes
6. Verify slider moves to drop-off
7. Verify gripper opens
8. Verify slider returns home
9. Verify success beeps
```

---

## Troubleshooting

### Motor Won't Move
- Check 12V power supply is connected
- Verify L298N 5V regulator jumper installed
- Test IN1/IN2 toggle with multimeter
- Check motor wires are connected to OUT1/OUT2
- Verify ENA receives PWM signal

### Motor Moves Wrong Direction
- Swap motor wires on L298N outputs (OUT1 ↔ OUT2)
- OR swap IN1/IN2 in code

### Servo Jitters
- **Most common**: Power supply insufficient (<2A)
- Move servo power wire away from motor wires
- Add capacitor (100µF-1000µF) across servo power
- Check all ground connections

### Ultrasonic Always Returns Error
- Check 5V power to VCC
- Verify TRIG and ECHO not swapped
- Ensure common ground
- Test with different object distances (5-50cm)

### Homing Never Stops
- Check limit switch wiring (GPIO 22 + GND)
- Test switch with multimeter (should close at home)
- Verify internal pull-up enabled in code
- Check switch orientation (Normally Open required)

### Keypad Doesn't Respond
- Verify all 8 wires connected
- Check rows are outputs (GPIO 2-5)
- Check columns are inputs (GPIO 6-9)
- Test each key individually with serial output

### Random Resets/Brownouts
- **Most common**: Power supply overload
- Use separate supplies for motor and servo
- Ensure all grounds connected
- Add bulk capacitors (1000µF) at power rails

---

## Advanced: Noise Reduction

### For Cleaner Operation:

#### Power Line Filtering:
```
5V Supply ──[100µF]──┬── Servo
                     └── GND

12V Supply ──[1000µF]──┬── L298N
                       └── GND
```

#### Signal Line Protection:
```
GPIO 14 (ENA) ──[220Ω]── L298N ENA
GPIO 15       ──[220Ω]── Servo Signal
```

#### Motor Noise Suppression:
```
Across DC Motor Terminals:
├── 0.1µF capacitor (ceramic)
└── 0.01µF capacitor (ceramic)

From each terminal to motor case:
├── 0.1µF capacitor
```

---

## Safety Guidelines

### ⚠️ IMPORTANT SAFETY RULES:

1. **Never connect servo to Pico 3.3V** - Will damage Pico!
2. **Always connect all grounds together** - Prevents voltage spikes
3. **Use correct voltage for each component** - Check datasheets
4. **Fuse all power supplies** - 2A for 5V, 2A for 12V
5. **Emergency stop must be accessible** - GPIO 17 button
6. **Secure all moving parts** - Rail can pinch fingers
7. **Power off before wiring changes** - Prevent shorts
8. **Double-check polarity** - Use multimeter

### Recommended Safety Additions:
- [ ] Emergency stop button in easy reach
- [ ] Fuses on all power supplies
- [ ] LED power indicators
- [ ] Limit switches at both ends of rail
- [ ] Protective enclosure for electronics

---

## Bill of Materials (BOM)

| Component              | Quantity | Notes                              |
|------------------------|----------|------------------------------------|
| Raspberry Pi Pico      | 1        | RP2040 microcontroller            |
| 4×4 Matrix Keypad      | 1        | 16-key membrane keypad            |
| HC-SR04 Sensor         | 1        | Ultrasonic distance sensor        |
| MG996 Servo            | 1        | High-torque metal gear servo      |
| L298N Motor Driver     | 1        | Dual H-bridge module              |
| DC Motor (3-6V)        | 1        | Geared motor recommended          |
| Active Buzzer (5V)     | 1        | Piezo buzzer with oscillator      |
| Pushbuttons (NO)       | 6        | Normally Open tactile switches    |
| Linear Rail            | 1        | 400-600mm recommended             |
| Slider/Carriage        | 1        | Compatible with rail              |
| 5V 3A Power Supply     | 1        | For servo and sensor              |
| 12V 2A Power Supply    | 1        | For DC motor via L298N            |
| Breadboard (large)     | 1        | For prototyping                   |
| Jumper Wires (M-M)     | 40+      | Various lengths                   |
| Jumper Wires (M-F)     | 20+      | For sensors                       |
| Capacitors (100µF)     | 2-3      | Power filtering                   |
| Capacitors (1000µF)    | 1-2      | Motor noise filtering             |
| Resistors (220Ω)       | 2-3      | Signal protection (optional)      |
| USB Cable (Micro-B)    | 1        | For Pico programming              |

---

## Calibration Values

After assembly, adjust these values in `include/config.h`:

```cpp
// Motor speed calibration (measure actual speed)
const float MM_PER_SECOND = 50.0f;  // Adjust based on testing
const uint8_t MOTOR_SPEED = 70;      // 0-100%, tune for smoothness

// Servo angles (adjust for your gripper design)
const float GRIPPER_OPEN_ANGLE = 90.0f;
const float GRIPPER_CLOSED_ANGLE = 30.0f;

// Puzzle piece positions (measure from home position)
const float PUZZLE_POSITIONS[9] = {
    50.0f,   // Position 1 - MEASURE THIS
    100.0f,  // Position 2 - MEASURE THIS
    150.0f,  // Position 3 - MEASURE THIS
    200.0f,  // Position 4 - MEASURE THIS
    250.0f,  // Position 5 - MEASURE THIS
    300.0f,  // Position 6 - MEASURE THIS
    350.0f,  // Position 7 - MEASURE THIS
    400.0f,  // Position 8 - MEASURE THIS
    450.0f   // Position 9 - MEASURE THIS
};

const float DROP_OFF_POSITION = 500.0f;  // MEASURE THIS

// Ultrasonic threshold (adjust for your setup)
const float OBJECT_DETECTION_THRESHOLD_CM = 8.0f;
```

---

## Final Verification

Before running the full automation:

- [ ] All components individually tested
- [ ] All power supplies provide correct voltage
- [ ] All grounds connected together
- [ ] Emergency stop button functional
- [ ] Homing routine works reliably
- [ ] Manual controls all work
- [ ] Servo moves smoothly without jitter
- [ ] Motor moves slider smoothly
- [ ] Ultrasonic reads accurately
- [ ] All buttons respond correctly
- [ ] Keypad all keys work
- [ ] Buzzer plays all patterns
- [ ] Limit switch triggers at home
- [ ] No loose wires or connections
- [ ] No shorts detected with multimeter
- [ ] Serial monitor shows correct output

---

**Ready to Build!** 🚀

Follow this guide step-by-step, test incrementally, and you'll have a fully functional puzzle-sorting robot!

For the test version (without rail), see `WIRING_NO_RAIL.md`.

---

*Complete Wiring Guide - Symbion Station 8*  
*Last Updated: November 17, 2025*
