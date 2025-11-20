# Symbion Station 8 - Puzzle Sorting Robot

A complete C++ project for the Raspberry Pi Pico (RP2040) that controls a puzzle-sorting robot using the Pico SDK.

> **NOTE**: This version is configured for **stationary testing** without a linear rail. The robot can test the gripper, sensors, and keypad input while remaining in a fixed position. When you get the rail and slider, simply uncomment the motor movement code.

> **🚀 NEW TO THIS PROJECT?** Start here: **[QUICK_START.md](QUICK_START.md)** - Complete guide for testing without the rail!

## Project Overview

This system is a puzzle-sorting robot that:
- Uses an ultrasonic sensor to detect puzzle pieces
- Accepts user input via a 4×4 keypad to select puzzle pieces (1-9)
- Controls a linear rail with a DC motor and L298N driver
- Uses two servo motors to pick up and release puzzle pieces
- Provides audio feedback through an active buzzer
- Supports manual control via 6 pushbuttons

## Hardware Components

### Currently Required (Testable Now):
- **Raspberry Pi Pico** (RP2040)
- **4×4 Digital Keypad**
- **HC-SR04 Ultrasonic Sensor**
- **6 Pushbuttons** (stop, home, manual fwd/rev, grip, limit switch)
- **1× Servo Motor** (MG996 for gripper)
- **Active Buzzer** (5V)
- **5V 2A Power Supply**
- **Breadboard and jumper wires**

### For Later (When Available):
- **L298N Motor Driver**
- **DC Motor** (3–6V)
- **Linear Rail + Slider**
- **9 Puzzle Blocks**

---

## Project Structure

```
symbion_station8/
├── CMakeLists.txt              # Main build configuration
├── pico_sdk_import.cmake       # Pico SDK import script
├── main.cpp                    # Main program (FULL VERSION with rail)
├── main_test_no_rail.cpp       # TEST VERSION (without rail) ⭐ USE THIS NOW
├── README.md                   # This file
├── WIRING_NO_RAIL.md          # Wiring guide for testing without rail ⭐
│
├── include/                    # Configuration headers
│   └── config.h               # Pin definitions and constants
│
└── lib/                        # Modular hardware libraries
    ├── keypad/
    │   ├── CMakeLists.txt
    │   ├── Keypad4x4.h
    │   └── Keypad4x4.cpp
    │
    ├── ultrasonic/
    │   ├── CMakeLists.txt
    │   ├── Ultrasonic.h
    │   └── Ultrasonic.cpp
    │
    ├── motor/
    │   ├── CMakeLists.txt
    │   ├── MotorDriver.h
    │   └── MotorDriver.cpp
    │
    ├── servo/
    │   ├── CMakeLists.txt
    │   ├── ServoController.h
    │   └── ServoController.cpp
    │
    └── buttons/
        ├── CMakeLists.txt
        ├── PushButton.h
        ├── PushButton.cpp
        ├── Buzzer.h
        └── Buzzer.cpp
```

---

## Pin Configuration

| Component        | Pin(s)              | Description                    |
|------------------|---------------------|--------------------------------|
| Keypad Rows      | GP2, GP3, GP4, GP5  | Matrix rows (output)           |
| Keypad Columns   | GP6, GP7, GP8, GP9  | Matrix columns (input, pull-up)|
| Ultrasonic Trig  | GP10                | Trigger pulse output           |
| Ultrasonic Echo  | GP11                | Echo pulse input               |
| Motor IN1        | GP12                | Direction control 1            |
| Motor IN2        | GP13                | Direction control 2            |
| Motor ENA        | GP14                | PWM speed control              |
| Gripper Servo    | GP15                | PWM signal (MG996)             |
| ~~Arm Servo~~    | ~~GP16~~            | ~~Not used - only 1 servo~~    |
| Stop Button      | GP17                | Emergency stop                 |
| Home Button      | GP18                | Return to home position        |
| Manual Fwd       | GP19                | Manual forward movement        |
| Manual Rev       | GP20                | Manual reverse movement        |
| Grip Button      | GP21                | Manual gripper control         |
| Limit Switch     | GP22                | Home position sensor           |
| Buzzer           | GP26                | Audio feedback                 |

---

## Getting Started Without the Rail

### What You Can Test Now:

Even without the linear rail and motor, you can build and test most of the system:

#### ✅ **Components You Can Test:**
1. **Keypad** - Test all 16 keys (1-9, *, 0, #, A-D)
2. **Ultrasonic Sensor** - Measure distances in real-time
3. **Servo Gripper** - Test open/close movements
4. **Buzzer** - All beep patterns and sequences
5. **Pushbuttons** - Verify all button inputs work

#### 📦 **What to Build First:**
1. Connect the Pico to your breadboard
2. Wire up the 4×4 keypad (GP2-GP9)
3. Connect the HC-SR04 ultrasonic sensor (GP10-GP11)
4. Connect the MG996 servo (GP15, external 5V supply!)
5. Connect the buzzer (GP26)
6. Connect 2 pushbuttons minimum: Grip (GP21) and Stop (GP17)

#### 🔧 **Using the Test Version:**

I've created `main_test_no_rail.cpp` - a simplified version that works without the rail:

**To use it:**
```powershell
# Rename the files
mv main.cpp main_full.cpp
mv main_test_no_rail.cpp main.cpp

# Then rebuild normally
cd build
nmake
```

**Test mode features:**
- Press **[1]** on keypad: Test all keypad buttons
- Press **[2]** on keypad: Continuous distance measurement
- Press **[3]** on keypad: Automatic gripper open/close test
- Press **[4]** on keypad: Test all buzzer sounds
- Press **Grip Button**: Manually toggle gripper
- Press **Stop Button**: Show menu

#### ⏭️ **When You Get the Rail:**

Simply rename the files back:
```powershell
mv main.cpp main_test_no_rail.cpp
mv main_full.cpp main.cpp
```

And rebuild - the full version will work with motor control!

---

## Building the Project

### Prerequisites

1. **Install the Pico SDK**
   - Follow: https://github.com/raspberrypi/pico-sdk
   - Set the `PICO_SDK_PATH` environment variable

2. **Install Build Tools**
   - **Windows**: CMake, ARM GCC toolchain
   - **Linux/macOS**: cmake, gcc-arm-none-eabi

### Build Steps

#### Windows (PowerShell)

```powershell
# Navigate to project
cd symbion_station8

# Set SDK path
$env:PICO_SDK_PATH = "C:\path\to\pico-sdk"

# Build
mkdir build
cd build
cmake -G "NMake Makefiles" ..
nmake

# Output: symbion_station8.uf2
```

#### Linux/macOS

```bash
# Navigate to project
cd symbion_station8

# Set SDK path
export PICO_SDK_PATH=/path/to/pico-sdk

# Build
mkdir build
cd build
cmake ..
make -j4

# Output: symbion_station8.uf2
```

#### VS Code

1. Install **CMake Tools** extension
2. Set `PICO_SDK_PATH` in settings
3. Run **CMake: Configure**
4. Run **CMake: Build**

---

## Flashing

1. Hold BOOTSEL button on Pico
2. Connect USB while holding BOOTSEL
3. Release BOOTSEL (Pico appears as USB drive)
4. Copy `symbion_station8.uf2` to the drive
5. Pico reboots and runs the program

---

## Operation

### Startup
1. Power on → Startup beeps
2. Auto-home to limit switch
3. Ready for input

### Normal Operation
1. Press 1-9 on keypad
2. Robot moves to position
3. Verifies object with ultrasonic
4. Picks up puzzle piece
5. Moves to drop-off
6. Releases piece
7. Returns home
8. Success beeps!

### Manual Control
- **Stop (GP17)**: Emergency stop
- **Home (GP18)**: Return home
- **Fwd (GP19)**: Manual forward (hold)
- **Rev (GP20)**: Manual reverse (hold)
- **Grip (GP21)**: Toggle gripper

---

## Calibration

Edit values in `include/config.h`:

```cpp
// Motor speed calibration
const float MM_PER_SECOND = 50.0f;  // Adjust based on actual speed
const uint8_t MOTOR_SPEED = 70;     // 0-100%

// Servo positions (MG996)
const float GRIPPER_OPEN_ANGLE = 90.0f;      // Adjust for your gripper
const float GRIPPER_CLOSED_ANGLE = 30.0f;    // Adjust for your gripper
// ARM positions not used - only 1 servo available

// Puzzle positions (mm from home)
const float PUZZLE_POSITIONS[9] = {
    50.0f, 100.0f, 150.0f, 200.0f, 250.0f,
    300.0f, 350.0f, 400.0f, 450.0f
};
```

---

## Features

✅ **Modular library structure** - Each component is independent  
✅ **Non-blocking operations** - Smooth servo motion with update()  
✅ **Debounced inputs** - 50ms debounce for keypad and buttons  
✅ **State machine** - Clear workflow control  
✅ **Error handling** - Automatic retry and recovery  
✅ **Manual override** - Full manual control mode  
✅ **Audio feedback** - Beep patterns for all operations  

---

## Troubleshooting

**Build fails**: Verify `PICO_SDK_PATH` is set correctly  
**Keypad not working**: Check row/column wiring and pull-ups  
**Motor doesn't move**: Verify L298N connections and power supply (when rail available)  
**Servos jittering**: Use external 5V supply, not Pico 3.3V  
**Ultrasonic timeout**: Check wiring, ensure 5V power and common ground  
**No serial output**: Wait 2-3 seconds after connecting USB  

---

## Current Status

✅ **Working Now (No Rail Required):**
- Keypad input and debouncing
- Ultrasonic distance measurement
- Servo gripper control
- Buzzer audio feedback
- Button inputs
- Interactive test program

⏳ **Waiting for Hardware:**
- Linear rail + slider
- Motor movement and position tracking
- Full automated pickup/dropoff sequence
- L298N motor driver integration

💡 **Next Steps:**
1. Build and test the basic components (keypad, sensor, servo, buzzer)
2. Verify all connections work with the test program
3. Source the linear rail and motor
4. Switch to the full version with motor control

---

## License

Educational and hobbyist use. Provided as-is.

---

**Symbion Station 8** - Built with Pico SDK ❤️
