Fridge buzzer
=============

Our fridge doesn't give an audible warning when the door is left open 
for too long, so I made this embarrassingly trivial thing to fix this.

![Photo of a approx. 5×5cm PCA in a silver 3D printed plastic frame 
glued onto a wooden door. On the PCB, there's a black piezo buzzer on 
the left side and CR2032 button cell on the right side. In the lower 
portion of the PCB, there are a 6-Pin tag-connect footprint, an MSP430 
MCU in a TSSOP-14 package, an inductor with chipped off ferrite and a 
pushbutton.](media/pcb.jpg)

# Hardware

[Schematics](fridge-buzzer/output/schematic.pdf)

The fridge buzzer is based on an MSP430G2232 low-power MCU connected to 
a SI7201 hall effect switch and a pizeo buzzer. The whole thing is powered 
from CR2032 button cell.

An inductor is connected in parallel to the piezo buzzer to increase 
its volume.

For programming the MSP430, there's a 6-pin Tag-Connect footprint on 
the board.

As usual, this board is made with [Horizon 
EDA](https://horizon-eda.org/).

# Firmware

The firmware is less than 200 lines of spaghetti-ish C to be compiled 
with msp430-elf-gcc. By making use of the MSP430's sleep modes and 
configuring peripherals the right way, the 
idle power consumption is less than 3&nbsp;µA, resulting in a projected 
battery life of about 9 years on a 235&nbsp;mAh battery.

After inserting the battery, the device is in setup mode for about 25 
seconds and beeps if it detects the magnet. After that, it enters 
normal operation. If the door's been open for longer than 90 seconds, 
it starts beeping in a not-so-annoying low-pitched tone. After 120 
seconds, it starts beeping in a higher pitch. To avoid annoying people 
in case of a malfunction such a fallen off magnet, beeping stops after 
5 minutes.

Pressing the button on the board mutes the beep until the door is 
closed again.

# Mechanical construction

The PCB slides into in a 3D-printed holder that's glued to the fridge door 
with mounting tape. The magnet that actuates the hall-effect switch 
is pressed into a stick that sits in a 3D-printed holder that's glued to the 
fridge cabinet. By adjusting the position of the stick, one can tune 
the sensitivity to make sure a slightly ajar door is detected as open.

All parts were designed in solvespace.

![Photo of a 3D-printed silver cylindrical stick that sits inside a 
roughly triangular holder. The holder is glued to the inside of a white 
cabinet.](media/magnet.jpg)

# License

Hardware and mechanical parts CERN Open Hardware Licence v1.2, Firmware 
GPLv3
