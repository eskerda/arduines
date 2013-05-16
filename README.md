# ArduiNES #

It's time to bring nostalgia back by converting your NES gamepad into a 
compilant USB HID game device.

This project contains a firmware based on one of 
[Darran's Hunt examples][dhunt_firmware] on emulating USB devices 
using [LUFA][LUFA] and an sketch file to read the input from a NES gamepad.


## Turn your NES gamepad into an USB game device ##
![Finished product][mvp]

You'll need:

* An Arduino
* Some solid cables
* A NES gamepad
* [dfu-programmer][dfu] on Linux

## Step 1: Connect your NES gamepad to the Arduino ##

    NES gamepad controller pinout

        o 4    1. +5VDC Supply
    1 o o 5    2. ?
    2 o o 6    3. ?
    3 o o 7    4. GND
               5. CLK
               6. LATCH
               7. DATA

* NES pin 1 (+5V input) with the +5V output on the Arduino.
* NES pin 4 (GND) with the GND on the Arduino.
* NES pin 5 (CLK) with the input #2 on the Arduino.
* NES pin 6 (LATCH) with the input #3 on the Arduino.
* NES pin 7 (DATA) with the input #4 on the Arduino.

To test that everything works wonderfully, upload the nintendo-test sketch on
the Arduino. You should see the state of the buttons being outputted on the 
serial interface. If everything seems fine, proceed to the next step.

## Step 2: Upload the arduines sketch ##

Before flashing your board with a new firmware, you must upload the sketch 
first. The Gamepad.hex firmware does not allow uploading sketches onto memory. 

If you need to upload a new sketch after flashing the firmware, you'll have 
to re-flash again the board with the default Arduino firmware.

## Step 3: Put your Arduino into DFU mode ##

Instead of using an specific purpose USB-to-serial chip, the Arduino comes 
with an ATmega programmed to act as an USB to serial device (so you can 
upload compiled bytecode from sketches into the EEPROM). That's what gets 
detected when you connect the Arduino into the computer. In better words:

> The Uno differs from all preceding boards in that it does not use the FTDI 
> USB-to-serial driver chip. Instead, it features theAtmega16U2 (Atmega8U2 up 
> to version R2) programmed as a USB-to-serial converter.
> 
> Source: http://arduino.cc/en/Main/arduinoBoardUno

If you want the Arduino to appear as a different device, you need to flash a 
new firmware onto it, by putting it into DFU (direct firmware upgrade) mode 
which, depending on your board, is done with more or less messy ways which 
for older versions may require soldering.

Hopefully my board is an R3, which can be cleanly put into DFU mode by jumping
the RESET and GND pins of the Atmega16U2 together for a fraction, using a 
jumper or, if feeling wild, just a piece of copper.

        ____________________
       | o       ···········
    RESET ---> + · ·
    GND -----> + · ·
      _|____
     |      |
     | USB  |
     |______|
       |
       |

Now an lsusb will show you "03eb:2fef Atmel Corp". Now, using the 
dfu-programmer you can interface with the atmega chip and flash firmware 
upgrades.

For instance, just to know everything is going as it should you can test it 
safely by

    dfu-programmer <your chip> get bootloader-version

In the UNO R3, your chip is an atmega16u2. With other revisions, I guess it is
an atmega8u2. The latest version of the dfu-programmer comes with the 
atmega16u2 included as a target with its device ID (0x04000). If you get 
"dfu-programmer: no device present" odds are you are using an outdated version
of the dfu-programmer, or your chip is not in DFU mode.

## Step 5: Flash a new firmware ##

    dfu-programmer <your chip> flash Gamepad.hex

## Step 4: Plug & Play ##

Reconnect the arduino, and notice that it is recognized as a USB HID device. 
Time to test the input using your system settings, outputting /dev/input/js0 
or directly playing some games!

## Bonus: Compiling the firmware ##

Follow [this] [darran_build] post on Darran's blog about setting up the 
environment to compile LUFA based firmwares. Note that you will need 
LUFA-100807 in order to properly build firmwares for the Arduino. This is what
I did in arch:

    pacman -S binutils gmp mpfr libmpc gcc avr-libc
    cd arduines
    wget http://lufa-lib.googlecode.com/files/LUFA-100807.zip
    unzip LUFA-100807.zip
    ln -s LUFA\ 100807/LUFA LUFA
    cd arduino-gamepad
    # Edit the makefile to reflect your setup
    # Set your MCU to atmega16u2 or atmega8u2
    # check the LUFA_PATH
    vim makefile
    make clean; make

If everything compiles cleanly, a Gamepad.hex file will be generated on the 
same folder.

[mvp]: http://i.imgur.com/TNCoS8el.jpg "Finished product"
[dfu]: http://dfu-programmer.sourceforge.net/ "dfu-programmer's website"
[darran_build]: http://hunt.net.nz/users/darran/weblog/a4091/Building_Arduino_Atmega8u2_firmware_on_Linux.html "A guide on building arduino firmwares on Linux"
[dhunt_firmware]: http://hunt.net.nz/users/darran/weblog/a3599/Arduino_UNO_Joystick_HID_firmware.html "Post including the examples"
[LUFA]: http://www.fourwalledcubicle.com/LUFA.php "LUFA website"

