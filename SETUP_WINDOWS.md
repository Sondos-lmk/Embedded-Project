# Development Environment Setup - Windows

## Complete Guide to Setup Pico Development on Windows PC

This guide will help you set up everything needed to build and flash the Symbion Station 8 project on your Windows computer.

---

## 📋 Overview

You'll need to install:
1. **Pico SDK** - Raspberry Pi Pico development kit
2. **ARM GCC Toolchain** - Compiler for ARM processors
3. **CMake** - Build system
4. **Build Tools** - For compiling (Visual Studio or Ninja)
5. **Git** - Version control (for SDK)
6. **Python** - For SDK scripts
7. **VS Code** (Optional) - Code editor with extensions

**Estimated Time**: 30-60 minutes

---

## 🚀 Quick Setup (Recommended Method)

### Option 1: Automated Installer (Easiest)

Raspberry Pi provides an automated installer that sets up everything:

1. **Download the Pico Setup Installer**
   - Go to: https://github.com/raspberrypi/pico-setup-windows/releases/latest
   - Download: `pico-setup-windows-x64-standalone.exe`

2. **Run the Installer**
   ```
   - Double-click the downloaded .exe file
   - Click "Next" through the installation wizard
   - Choose installation directory (default: C:\Program Files\Raspberry Pi\Pico)
   - Select all components (ARM toolchain, CMake, Build Tools, VS Code)
   - Wait for installation (10-20 minutes)
   ```

3. **Verify Installation**
   - Open a new PowerShell window
   - Run: `arm-none-eabi-gcc --version`
   - Should see: `arm-none-eabi-gcc (GNU Arm Embedded Toolchain ...) ...`

4. **Set Environment Variable**
   ```powershell
   # The installer should set this, but verify:
   $env:PICO_SDK_PATH
   
   # Should show something like:
   # C:\Program Files\Raspberry Pi\Pico\pico-sdk
   
   # If empty, set it permanently:
   [System.Environment]::SetEnvironmentVariable('PICO_SDK_PATH', 'C:\Program Files\Raspberry Pi\Pico\pico-sdk', 'User')
   ```

5. **Skip to "Building the Project" section below!**

---

## 🔧 Manual Setup (Advanced Users)

If you prefer manual installation or the automated installer doesn't work:

### Step 1: Install Git

1. **Download Git for Windows**
   - Visit: https://git-scm.com/download/win
   - Download and run the installer

2. **Install with default settings**
   ```
   - Click "Next" through all options
   - Use default editor (Vim or choose VS Code if you prefer)
   - Let Git decide the default branch name
   - Use recommended PATH option
   ```

3. **Verify Installation**
   ```powershell
   git --version
   # Should output: git version 2.x.x
   ```

---

### Step 2: Install Python

1. **Download Python 3.9 or later**
   - Visit: https://www.python.org/downloads/
   - Download Python 3.11+ for Windows

2. **Install Python**
   ```
   - Run the installer
   - ✅ CHECK "Add Python to PATH" (IMPORTANT!)
   - Click "Install Now"
   ```

3. **Verify Installation**
   ```powershell
   python --version
   # Should output: Python 3.x.x
   ```

---

### Step 3: Install ARM GCC Toolchain

1. **Download ARM GNU Toolchain**
   - Visit: https://developer.arm.com/downloads/-/arm-gnu-toolchain-downloads
   - Download: `arm-gnu-toolchain-<version>-mingw-w64-i686-arm-none-eabi.exe`
   - Choose latest version (e.g., 12.3 or 13.2)

2. **Install Toolchain**
   ```
   - Run the installer
   - Install to: C:\Program Files (x86)\Arm GNU Toolchain arm-none-eabi\<version>
   - ✅ CHECK "Add path to environment variable" (IMPORTANT!)
   - Click "Finish"
   ```

3. **Verify Installation**
   ```powershell
   # Close and reopen PowerShell, then:
   arm-none-eabi-gcc --version
   # Should output: arm-none-eabi-gcc (Arm GNU Toolchain ...) ...
   ```

---

### Step 4: Install CMake

1. **Download CMake**
   - Visit: https://cmake.org/download/
   - Download: `cmake-<version>-windows-x86_64.msi`

2. **Install CMake**
   ```
   - Run the installer
   - Choose: "Add CMake to system PATH for all users"
   - Click "Next" and "Install"
   ```

3. **Verify Installation**
   ```powershell
   # Close and reopen PowerShell, then:
   cmake --version
   # Should output: cmake version 3.x.x
   ```

---

### Step 5: Install Build Tools

Choose ONE of these options:

#### Option A: Visual Studio Build Tools (Recommended)

1. **Download Visual Studio Build Tools**
   - Visit: https://visualstudio.microsoft.com/downloads/
   - Scroll to "All Downloads" → "Tools for Visual Studio"
   - Download "Build Tools for Visual Studio 2022"

2. **Install Build Tools**
   ```
   - Run the installer
   - Select "Desktop development with C++"
   - Make sure these are checked:
     ✅ MSVC v143 - VS 2022 C++ x64/x86 build tools
     ✅ Windows 10/11 SDK
     ✅ C++ CMake tools for Windows
   - Click "Install" (will download ~6GB)
   ```

3. **Verify Installation**
   ```powershell
   # Open "Developer Command Prompt for VS 2022"
   nmake /?
   # Should show NMake help
   ```

#### Option B: Ninja Build (Lightweight Alternative)

1. **Download Ninja**
   - Visit: https://github.com/ninja-build/ninja/releases
   - Download: `ninja-win.zip`

2. **Install Ninja**
   ```powershell
   # Extract to C:\Program Files\Ninja
   # Add to PATH:
   $env:Path += ";C:\Program Files\Ninja"
   [System.Environment]::SetEnvironmentVariable('Path', $env:Path, 'User')
   ```

3. **Verify Installation**
   ```powershell
   ninja --version
   # Should output: 1.x.x
   ```

---

### Step 6: Install Pico SDK

1. **Clone the Pico SDK**
   ```powershell
   # Create a directory for development tools
   cd C:\
   mkdir Pico
   cd Pico
   
   # Clone the SDK
   git clone https://github.com/raspberrypi/pico-sdk.git
   cd pico-sdk
   
   # Initialize submodules (IMPORTANT!)
   git submodule update --init
   ```

2. **Set Environment Variable**
   ```powershell
   # Set PICO_SDK_PATH permanently
   [System.Environment]::SetEnvironmentVariable('PICO_SDK_PATH', 'C:\Pico\pico-sdk', 'User')
   
   # Verify it's set (close and reopen PowerShell first)
   $env:PICO_SDK_PATH
   # Should output: C:\Pico\pico-sdk
   ```

---

## 🎯 Install VS Code (Optional but Recommended)

### Step 1: Install VS Code

1. **Download VS Code**
   - Visit: https://code.visualstudio.com/
   - Download for Windows

2. **Install VS Code**
   ```
   - Run installer with default settings
   - ✅ Check "Add to PATH"
   - ✅ Check "Register Code as an editor for supported file types"
   ```

### Step 2: Install VS Code Extensions

Open VS Code and install these extensions:

1. **C/C++ Extension**
   ```
   - Press Ctrl+Shift+X (Extensions)
   - Search: "C/C++"
   - Install: "C/C++" by Microsoft
   ```

2. **CMake Tools**
   ```
   - Search: "CMake Tools"
   - Install: "CMake Tools" by Microsoft
   ```

3. **CMake Language Support**
   ```
   - Search: "CMake"
   - Install: "CMake" by twxs
   ```

4. **Serial Monitor** (for debugging)
   ```
   - Search: "Serial Monitor"
   - Install: "Serial Monitor" by Microsoft
   ```

5. **Raspberry Pi Pico** (optional)
   ```
   - Search: "Raspberry Pi Pico"
   - Install: "Raspberry Pi Pico" by Raspberry Pi
   ```

### Step 3: Configure VS Code

1. **Open Settings** (Ctrl+,)

2. **Set CMake Generator** (if using Visual Studio):
   ```json
   {
     "cmake.generator": "NMake Makefiles",
     "cmake.configureSettings": {
       "PICO_SDK_PATH": "C:/Pico/pico-sdk"
     }
   }
   ```

3. **Or for Ninja**:
   ```json
   {
     "cmake.generator": "Ninja",
     "cmake.configureSettings": {
       "PICO_SDK_PATH": "C:/Pico/pico-sdk"
     }
   }
   ```

---

## 🏗️ Building the Project

Now you're ready to build the Symbion Station 8 project!

### Using PowerShell (Command Line)

1. **Navigate to Project**
   ```powershell
   cd C:\Users\Daniel\Desktop\symbion_station8
   ```

2. **Verify SDK Path**
   ```powershell
   # Check if environment variable is set
   $env:PICO_SDK_PATH
   
   # If not set, set it for this session:
   $env:PICO_SDK_PATH = "C:\Pico\pico-sdk"
   ```

3. **Create Build Directory**
   ```powershell
   mkdir build
   cd build
   ```

4. **Configure with CMake**
   
   **For Visual Studio (NMake):**
   ```powershell
   # Open "Developer Command Prompt for VS 2022" or run:
   # "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
   
   cmake -G "NMake Makefiles" ..
   ```
   
   **For Ninja:**
   ```powershell
   cmake -G "Ninja" ..
   ```

5. **Build the Project**
   
   **For NMake:**
   ```powershell
   nmake
   ```
   
   **For Ninja:**
   ```powershell
   ninja
   ```

6. **Check Output**
   ```powershell
   # You should see:
   # symbion_station8.uf2
   # symbion_station8.elf
   # symbion_station8.bin
   
   ls *.uf2
   ```

---

### Using VS Code

1. **Open Project**
   ```
   - File → Open Folder
   - Select: C:\Users\Daniel\Desktop\symbion_station8
   ```

2. **Configure CMake**
   ```
   - Press Ctrl+Shift+P
   - Type: "CMake: Configure"
   - Select your kit (arm-none-eabi-gcc)
   - Wait for configuration to complete
   ```

3. **Build Project**
   ```
   - Press Ctrl+Shift+P
   - Type: "CMake: Build"
   - OR click "Build" button in status bar
   - Wait for build to complete
   ```

4. **Find Output**
   ```
   - Output file: build/symbion_station8.uf2
   - Right-click → Reveal in File Explorer
   ```

---

## 📲 Flashing the Pico

### Method 1: Drag and Drop (Easiest)

1. **Put Pico in Boot Mode**
   ```
   - Hold the BOOTSEL button on the Pico
   - While holding, plug in the USB cable
   - Release the BOOTSEL button
   - Pico appears as a USB drive (RPI-RP2)
   ```

2. **Copy UF2 File**
   ```
   - Open File Explorer
   - Navigate to: C:\Users\Daniel\Desktop\symbion_station8\build
   - Find: symbion_station8.uf2
   - Copy file to the RPI-RP2 drive
   - Pico will automatically reboot and run the program
   ```

### Method 2: Using Picotool (Advanced)

1. **Install Picotool**
   ```powershell
   # Clone picotool
   cd C:\Pico
   git clone https://github.com/raspberrypi/picotool.git
   cd picotool
   mkdir build
   cd build
   
   # Build picotool
   cmake ..
   nmake  # or ninja
   
   # Add to PATH
   $env:Path += ";C:\Pico\picotool\build"
   ```

2. **Flash with Picotool**
   ```powershell
   # Pico in BOOTSEL mode
   picotool load symbion_station8.uf2
   picotool reboot
   ```

---

## 📡 Serial Monitor Setup

To see debug output from the Pico:

### Method 1: PuTTY (Free, Easy)

1. **Download PuTTY**
   - Visit: https://www.putty.org/
   - Download and install

2. **Find COM Port**
   ```powershell
   # List serial ports
   [System.IO.Ports.SerialPort]::getportnames()
   # Example output: COM3, COM4
   # Pico is usually the highest numbered port
   ```

3. **Connect with PuTTY**
   ```
   - Open PuTTY
   - Connection type: Serial
   - Serial line: COM3 (or your port)
   - Speed: 115200
   - Click "Open"
   ```

### Method 2: VS Code Serial Monitor

1. **Open Serial Monitor**
   ```
   - Press Ctrl+Shift+P
   - Type: "Serial Monitor: Focus on Serial Monitor View"
   ```

2. **Connect**
   ```
   - Select COM port (e.g., COM3)
   - Set baud rate: 115200
   - Click "Start Monitoring"
   ```

### Method 3: PowerShell (Built-in)

```powershell
# Install if needed
Install-Module -Name SerialPort

# Connect (replace COM3 with your port)
$port = new-Object System.IO.Ports.SerialPort COM3,115200,None,8,one
$port.Open()

# Read data
while ($port.IsOpen) {
    if ($port.BytesToRead -gt 0) {
        $port.ReadExisting()
    }
}
```

---

## 🔍 Troubleshooting

### "PICO_SDK_PATH not set"

```powershell
# Set it permanently:
[System.Environment]::SetEnvironmentVariable('PICO_SDK_PATH', 'C:\Pico\pico-sdk', 'User')

# Close and reopen PowerShell

# Verify:
$env:PICO_SDK_PATH
```

### "arm-none-eabi-gcc not found"

```powershell
# Find where it's installed
Get-Command arm-none-eabi-gcc

# If not found, add to PATH:
$toolchainPath = "C:\Program Files (x86)\Arm GNU Toolchain arm-none-eabi\13.2 Rel1\bin"
$env:Path += ";$toolchainPath"
[System.Environment]::SetEnvironmentVariable('Path', $env:Path, 'User')
```

### "CMake Error: Could not find CMAKE_MAKE_PROGRAM"

**For NMake:**
```powershell
# Must run from Developer Command Prompt
# Or manually:
& "C:\Program Files\Microsoft Visual Studio\2022\BuildTools\Common7\Tools\VsDevCmd.bat"
```

**For Ninja:**
```powershell
# Make sure Ninja is in PATH
ninja --version

# If not found, download and add to PATH
```

### "Cannot find pico_sdk_import.cmake"

```powershell
# Make sure you're in the project directory
cd C:\Users\Daniel\Desktop\symbion_station8

# Verify file exists
ls pico_sdk_import.cmake

# If missing, copy from SDK:
copy C:\Pico\pico-sdk\external\pico_sdk_import.cmake .
```

### Build Errors About Missing Headers

```powershell
# SDK submodules not initialized
cd C:\Pico\pico-sdk
git submodule update --init --recursive
```

### Pico Not Recognized as USB Drive

```
1. Try a different USB cable (must be data cable, not just power)
2. Try a different USB port
3. Hold BOOTSEL longer (5+ seconds) while plugging in
4. Check Device Manager for "RP2 Boot" device
```

### Serial Monitor Shows Gibberish

```
- Wrong baud rate - must be 115200
- Wrong COM port selected
- USB cable is power-only (no data)
```

---

## 📚 Quick Reference

### Essential Commands

```powershell
# Check installations
python --version
git --version
cmake --version
arm-none-eabi-gcc --version
ninja --version  # or nmake /?

# Check environment
$env:PICO_SDK_PATH

# Build project
cd C:\Users\Daniel\Desktop\symbion_station8
mkdir build; cd build
cmake -G "NMake Makefiles" ..  # or "Ninja"
nmake  # or ninja

# Find COM ports
[System.IO.Ports.SerialPort]::getportnames()
```

### Directory Structure After Setup

```
C:\
├── Pico\
│   ├── pico-sdk\              ← SDK installation
│   │   ├── src\
│   │   ├── lib\
│   │   └── ...
│   └── picotool\              ← Optional tools
│
└── Users\Daniel\Desktop\
    └── symbion_station8\       ← Your project
        ├── build\              ← Build output
        │   └── symbion_station8.uf2
        ├── include\
        ├── lib\
        ├── main.cpp
        └── CMakeLists.txt
```

---

## ✅ Verification Checklist

Before building your project, verify everything:

- [ ] Git installed and working (`git --version`)
- [ ] Python installed and in PATH (`python --version`)
- [ ] ARM toolchain installed (`arm-none-eabi-gcc --version`)
- [ ] CMake installed (`cmake --version`)
- [ ] Build tools installed (`nmake /?` or `ninja --version`)
- [ ] Pico SDK cloned and submodules initialized
- [ ] `PICO_SDK_PATH` environment variable set
- [ ] VS Code installed with C++ extensions (optional)
- [ ] Can see Pico as USB drive when in BOOTSEL mode
- [ ] Serial monitor working (PuTTY or VS Code)

---

## 🎓 Next Steps

1. **Build the test version first:**
   ```powershell
   # Rename to use test version
   cd C:\Users\Daniel\Desktop\symbion_station8
   mv main.cpp main_full.cpp
   mv main_test_no_rail.cpp main.cpp
   ```

2. **Follow the build steps above**

3. **Flash to Pico**

4. **Connect serial monitor** to see output

5. **Test components** as described in `QUICK_START.md`

6. **When you get the rail**, switch back to full version and rebuild

---

## 📖 Additional Resources

- [Pico SDK Documentation](https://raspberrypi.github.io/pico-sdk-doxygen/)
- [Getting Started with Pico](https://datasheets.raspberrypi.com/pico/getting-started-with-pico.pdf)
- [Pico C/C++ SDK Book](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)
- [RP2040 Datasheet](https://datasheets.raspberrypi.com/rp2040/rp2040-datasheet.pdf)

---

## 🎉 You're Ready!

Your development environment is now set up. You can:
- ✅ Build Pico projects
- ✅ Flash firmware to Pico
- ✅ Debug with serial monitor
- ✅ Modify and rebuild code

**Happy coding!** 🚀

---

*Windows Development Setup Guide - Symbion Station 8*  
*Last Updated: November 20, 2025*
