# ESP32 Projects
A collection of projects employing the IOT potential of an Esp32 module with Display. 

<br>

### Board Used:
TTGO T-Display ESP32

#### Board available on <a href='https://www.amazon.com/dp/B07X1W16QS?' target="_blank" rel="noopener noreferrer">Amazon</a>

![TTGO-T](https://github.com/user-attachments/assets/4eb4b439-1c52-4fb3-bfba-b0c439b13e63)

<br>

## Setup
### Platformio Project Setup
Board: Espressif ESP32 Dev Module  
Framework: Arduino  
### secrets.h
Add a <b>secrets.h</b> file to the /src folder with WIFI ssid/password and API Key. 
### Editable Platformio.ini File specifications  
monitor_speed = 921600 ; //Speed reduced for performance reliability.  
board_build.f_cpu = 80000000 ; //Frequency reduced from 240000000 to prevent overheating.
### Uploading to Board
Once your files are complete, you can build the project and upon success upload to the TTGO-T display board. The board should reboot automatically and initiate the program.

If you encounter any problems setting up this board please feel free to reach out and I will do my best to assist. 


