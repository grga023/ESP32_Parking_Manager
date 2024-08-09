# Description
Parking based on ESP32 development board with counting left spots, automatic exit, web server, and other functionality. 
## Use case
* When someone tries to access parking with a valid RFID card, his ID in the RFID SW component will be checked, and after confirming his identity, the EntryRamp task will be triggered, and his name and "Hello" message will be displayed.
* After someone parks his car in a parking slot, the Distance sensor will be triggered and free parking slots will be decremented by one. Display component should be triggered to display the current state of slots.

# Components
* RFID card scanner - PN532
* Display
* 2 x Servo motor - SG90
* 5 x Distance sensor - VL6810

# Hardver schematic

# SW component schematic
