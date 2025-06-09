# Description  
Parking management system based on the ESP32 development board, featuring spot counting, automatic exit control, a web server interface, and additional functionalities.

# Use Case  
- When a user attempts to access the parking lot with a valid RFID card, the RFID software component verifies the card ID. Upon successful validation, the EntryRamp task is triggered, and a personalized greeting ("Hello [Name]") is displayed.  
- When a vehicle parks in a slot, the distance sensor detects occupancy, and the count of available parking spots is decremented by one. The display component updates to show the current number of free spots.

# Components  
- RFID card scanner — PN532  
- Display module - SH1107 
- 2 x Servo motor - SG90  
- 5 x Distance sensor - VL6180X
- ESP32

# Hardware shematic
![image](https://github.com/user-attachments/assets/ce9c6b4a-8880-40ce-a895-f7415027547d)

# Final product
### Parking model
![image](https://github.com/user-attachments/assets/6a775c56-751f-4f36-961c-eec7329709a7)
![image](https://github.com/user-attachments/assets/345d0359-b080-4574-aeb4-666e82b8e8d1)
### Web UI
![image](https://github.com/user-attachments/assets/1bdcd3f3-f2c9-45f7-8588-b76c1201493e)
![image](https://github.com/user-attachments/assets/02ad1149-8eda-4d3c-aed9-9e5ad0c3bfed)
### User manager app
![image](https://github.com/user-attachments/assets/97e58ba6-f783-4cd6-9659-82ae9a13bee3)

