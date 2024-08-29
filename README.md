# Access Control System

A project designed to manage door access using various authentication methods.


## Technologies Used

### Backend

- **C#**
  - The backend is developed in C#, handling communication between the user interfaces (Discord and REST API) and the core access control logic.

### Core

- **ESP32**
  - The core of the system is built around the ESP32 microcontroller, programmed in C++ using PlatformIO. The ESP32 manages the physical access control mechanisms, such as door locking and unlocking.

## Instructions

1- **Clone the Repository:**  
- Clone this repository to your local machine

### Backend
2.1- Open the [Solution File](https://github.com/bora-sy/AccessControlSystem/blob/main/src/ACSBackend/ACSBackend.sln).

2.2- Configure the application using environment variables & database records.

2.3- Build & Run the application.

### Core

3.1- **Install PlatformIO:**  
   - Make sure you have PlatformIO installed in your IDE (VS Code recommended). If not, follow the installation guide [here](https://platformio.org/install).

3.2- **Open in VS Code:**  
   - Open the [src/ACS_Core](https://github.com/bora-sy/AccessControlSystem/tree/main/src/ACS_Core) folder in VS Code.
   - PlatformIO will automatically detect the environment.

3.3- **Connect your Board:**  
   Connect an ESP32 Board (or any board with any ESP32 module) to your computer via USB.

3.3- **Select the Environment:**  
   Make sure the correct environment is selected in `platformio.ini`. You can modify this file if needed.

3.4- **Upload the Code:**  
   - Click on the PlatformIO icon in the sidebar.
   - Under "PROJECT TASKS," select your environment.
   - Click on "Upload" to compile and upload the code to your board.
  
## Preview


<img src="https://github.com/user-attachments/assets/ceb44142-67ef-447c-af3a-9eb8cb440538" width="500">

*The device is not mounted yet. I'll update the images once it's mounted.*
