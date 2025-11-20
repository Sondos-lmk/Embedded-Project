# 🚀 QUICK START GUIDE - Testing Without Rail

## What You Need Right Now

### Hardware (Minimum for Testing):
- ✅ Raspberry Pi Pico (RP2040)
- ✅ 4×4 Matrix Keypad
- ✅ HC-SR04 Ultrasonic Sensor
- ✅ MG996 Servo Motor
- ✅ Active Buzzer (5V)
- ✅ 2× Pushbuttons (or more)
- ✅ Breadboard
- ✅ Jumper wires
- ✅ 5V 2A Power Supply (for servo)
- ✅ USB cable (for Pico programming)

### What You DON'T Need Yet:
- ❌ Linear rail + slider
- ❌ L298N Motor Driver
- ❌ DC Motor
- ❌ Puzzle blocks

---

## Step-by-Step Setup

### 1️⃣ Wire Everything

Follow the wiring guide in `WIRING_NO_RAIL.md`. Key points:

**Essential connections:**
```
Keypad:   GP2-GP5 (rows), GP6-GP9 (cols)
Sensor:   GP10 (trig), GP11 (echo)
Servo:    GP15 (signal) + EXTERNAL 5V power!
Buzzer:   GP26
Buttons:  GP17 (stop), GP21 (grip)
```

⚠️ **IMPORTANT**: Servo needs **external 5V 2A** power, NOT from Pico!

---

### 2️⃣ Prepare the Test Program

The test version is already created: `main_test_no_rail.cpp`

**To use it:**

#### Option A: Rename Files (Recommended)
```powershell
# In project root directory
mv main.cpp main_full.cpp
mv main_test_no_rail.cpp main.cpp
```

#### Option B: Modify CMakeLists.txt
Change line 32 in `CMakeLists.txt` from:
```cmake
add_executable(${PROJECT_NAME}
    main.cpp
)
```
to:
```cmake
add_executable(${PROJECT_NAME}
    main_test_no_rail.cpp
)
```

---

### 3️⃣ Build the Project

#### Windows (PowerShell):
```powershell
# Set Pico SDK path (adjust to your installation)
$env:PICO_SDK_PATH = "C:\path\to\pico-sdk"

# Create build directory
mkdir build
cd build

# Configure and build
cmake -G "NMake Makefiles" ..
nmake
```

#### Linux/macOS:
```bash
# Set Pico SDK path
export PICO_SDK_PATH=/path/to/pico-sdk

# Create build directory
mkdir build
cd build

# Configure and build
cmake ..
make -j4
```

**Output**: `symbion_station8.uf2` in the `build` directory

---

### 4️⃣ Flash to Pico

1. **Hold BOOTSEL button** on Pico
2. **Connect USB** while holding BOOTSEL
3. **Release BOOTSEL** - Pico appears as USB drive
4. **Copy** `symbion_station8.uf2` to the Pico drive
5. Pico will **reboot** and start running!

---

### 5️⃣ Open Serial Monitor

The program outputs debug info over USB serial.

#### Windows - PowerShell:
```powershell
# Find COM port
[System.IO.Ports.SerialPort]::getportnames()

# Open with PuTTY or screen
# Baud rate: 115200
```

#### Linux/macOS:
```bash
# Find device
ls /dev/tty*

# Open with screen
screen /dev/ttyACM0 115200
```

#### VS Code Extension:
Install "Serial Monitor" extension and connect to Pico's COM port at 115200 baud.

---

## 🎮 How to Use the Test Program

When powered on, you'll see a menu:

```
╔════════════════════════════════════════════════════╗
║              TEST MENU - NO RAIL MODE             ║
╠════════════════════════════════════════════════════╣
║  Press on Keypad:                                 ║
║  [1] - Test Keypad (shows all keys)              ║
║  [2] - Test Ultrasonic Sensor (continuous)       ║
║  [3] - Test Servo Gripper (open/close)           ║
║  [4] - Test Buzzer (all patterns)                ║
║  [0] - Show this menu                            ║
╚════════════════════════════════════════════════════╝
```

### Test Each Component:

#### Test 1: Keypad
- Press **[1]** on keypad
- Press any key - it should display in serial monitor
- Press **[#]** to exit

#### Test 2: Ultrasonic Sensor
- Press **[2]** on keypad
- Wave your hand in front of sensor
- Distance updates every 500ms
- Shows visual indicators: VERY CLOSE, CLOSE, NEAR, FAR
- Press any key to exit

#### Test 3: Servo Gripper
- Press **[3]** on keypad
- Servo automatically opens → closes → opens
- Watch for smooth movement
- Should hear beeps at each step

#### Test 4: Buzzer
- Press **[4]** on keypad
- Plays all beep patterns:
  - Startup sequence
  - Confirm beep
  - Success beep
  - Error beep
  - Custom pattern

### Manual Controls:

**Grip Button (GP21)**:
- Press to toggle gripper open/close
- Works anytime during operation
- Confirms with beep

**Stop Button (GP17)**:
- Press to show menu
- Useful if you get lost

---

## 🔍 Troubleshooting

### No Serial Output
- Wait 2-3 seconds after connecting USB
- Check baud rate is 115200
- Try different serial terminal program

### Keypad Not Working
- Verify all 8 wires connected
- Check rows: GP2-GP5, columns: GP6-GP9
- Make sure keypad is oriented correctly

### Ultrasonic Shows "ERROR"
- Check 5V power to VCC
- Verify TRIG on GP10, ECHO on GP11
- Ensure common ground
- Object might be too close (<2cm) or too far (>400cm)

### Servo Doesn't Move
- **Check external 5V power supply!**
- Verify signal wire on GP15
- Ensure common ground between Pico and servo power
- Try adjusting angles in `config.h`

### Servo Jitters
- Power supply too weak - use 2A minimum
- Check all ground connections
- Move servo away from power wires

### Buzzer Silent
- Check polarity (+/-)
- Verify GP26 connection
- Some buzzers need transistor for higher current

---

## ⚙️ Adjusting Settings

Edit `include/config.h`:

### Servo Angles (MG996):
```cpp
const float GRIPPER_OPEN_ANGLE = 90.0f;      // Adjust this
const float GRIPPER_CLOSED_ANGLE = 30.0f;    // And this
```

Test different angles to find what works for your gripper mechanism.

### Ultrasonic Threshold:
```cpp
const float OBJECT_DETECTION_THRESHOLD_CM = 8.0f;  // Adjust sensitivity
```

---

## 🎯 What You're Testing

This test program validates:

✅ **Keypad**: All keys work, debouncing is correct  
✅ **Ultrasonic**: Distance measurement is accurate  
✅ **Servo**: Smooth movement, correct angles  
✅ **Buzzer**: All sound patterns play  
✅ **Buttons**: Input detection and debouncing  
✅ **GPIO**: All pins configured correctly  
✅ **PWM**: Servo control signals working  
✅ **Power**: All components getting proper voltage  

When these all work, you're **95% ready** for the full system!

---

## 📦 When You Get the Rail

### Hardware to Add:
1. Linear rail + slider mechanism
2. L298N motor driver module
3. DC motor (3-6V)
4. Limit switch (for homing)

### Software Change:
```powershell
# Restore full version
mv main.cpp main_test_no_rail.cpp
mv main_full.cpp main.cpp

# Rebuild
cd build
nmake
```

### Additional Wiring:
```
Motor Driver L298N:
├── IN1  → GPIO 12
├── IN2  → GPIO 13
├── ENA  → GPIO 14
└── Motor outputs to DC motor

Limit Switch:
└── GPIO 22 (normally open, to GND)

Additional Buttons:
├── Home Button    → GPIO 18
├── Manual Fwd     → GPIO 19
└── Manual Rev     → GPIO 20
```

---

## 📚 Additional Resources

- `README.md` - Full project documentation
- `WIRING_NO_RAIL.md` - Detailed wiring guide
- `include/config.h` - All pin definitions and constants
- `lib/*/` - Individual component libraries with examples

---

## 🎓 Learning Resources

Each library (`lib/keypad/`, `lib/ultrasonic/`, etc.) is **standalone** and **documented**. You can use them in other projects!

**Explore the code:**
- `lib/keypad/Keypad4x4.h` - Matrix keypad scanning
- `lib/ultrasonic/Ultrasonic.h` - Distance measurement
- `lib/servo/ServoController.h` - Smooth servo control
- `lib/buttons/Buzzer.h` - Beep patterns

---

## ✅ Success Checklist

Before moving to the full system, verify:

- [ ] All keypad keys work (test 1-9, *, 0, #, A-D)
- [ ] Ultrasonic sensor reads 2-400cm accurately
- [ ] Servo moves smoothly without jittering
- [ ] Gripper can open and close reliably
- [ ] All buzzer patterns play correctly
- [ ] Buttons respond without bouncing
- [ ] Serial monitor shows clear output
- [ ] No unexpected resets or errors

---

## 🎉 You're Ready!

Once all tests pass, you have a working foundation. The remaining work (when you get the rail) is just:
- Physical mounting
- Motor calibration
- Position tuning

**Good luck building! 🚀**

---

**Questions or Issues?**
- Check wiring against `WIRING_NO_RAIL.md`
- Verify power supply is adequate (2A for servo)
- Ensure Pico SDK is properly installed
- Double-check all ground connections
