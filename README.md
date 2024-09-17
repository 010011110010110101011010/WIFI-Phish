# 🚨🚨🚨 Disclaimer: For Educational Purposes Only 🚨🚨🚨

- This project and all associated content are intended for educational and research purposes only. The techniques demonstrated here are meant to enhance understanding of network security and should never be used for unauthorized or malicious activities.

- Users are responsible for ensuring that they comply with all applicable laws and regulations in their respective jurisdictions. The authors of this project disclaim any responsibility for misuse or illegal activity carried out with the information provided.

# 🚨🚨🚨 Always obtain explicit permission before conducting any network-related tests on systems or devices you do not own. 🚨🚨🚨

<div align="center">
  <h1>🎣 <strong>WIFIPhish</strong> 🌐</h1>
  <img src="https://github.com/user-attachments/assets/b21fd05c-4c14-4e58-83aa-063700af7ebf">

</div>


# 🎣 **WiFi Phishing Project** 🛰️

![Arduino](https://img.shields.io/badge/Arduino-IDE-red?style=flat-square&logo=arduino)
![License](https://img.shields.io/badge/License-MIT-red?style=flat-square)

## 📖 **Project Overview**
This project is a **WiFi Phishing** system designed for educational purposes, utilizing the **ESP8266** microcontroller. It creates a fake WiFi network to lure unsuspecting users into connecting and potentially capturing their login credentials using a **Google Login Portal** And Redirection message. The system includes functionality for setting up a fake network, handling connections, and collecting data.

---

## 🌟 **Features**
- ✅ Create a Fake WiFi Network (SSID)
- ✅ Capture Connected Device Data
- ✅ Configure Network Settings
- ✅ Handle User Connections
- ✅ Store Data Login Credentails in EEPROM

---

<h2>📂 Project Structure</h2>

## ⚙️ **Installation Guide**

### 1. Clone the Repository

```bash
git clone https://github.com/010011110010110101011010/WIFIPhish.git
cd WIFIPhish/
```

### 2. Set Up Your Development Environment

1. **Install Arduino IDE:** Download and install the Arduino IDE from <a href="https://www.arduino.cc/en/software" target="_blank">Arduino's official website.</a>


2. **Install Required Libraries:** Open the Arduino IDE and install the following libraries:
- ESP8266WiFi
- DNSServer


3. **Upload the Code**
- **Open the Project:** Open main.cpp in the Arduino IDE.
- **Select Board:** Choose ESP8266 as your board in the Arduino IDE.
- **Upload Code:** Upload the code to your ESP8266 board.

4. **Configure WiFi Settings**
- **Set SSID and Password:** Modify the following lines in <code>Script.ino</code> to set your fake network details:
   	 ```bash
   	 const char* SSID_NAME = "Your_Custom_Network_Name";
	 const String WPass = "Your_Custom_Network_Password";
     ```
<h2>🛠 Usage Guide</h2> <ol> <li> <strong>Network Setup</strong><br> The ESP8266 will broadcast the fake WiFi network with the SSID set in `Script.ino`. Connect to this network to start the phishing process. </li> <li> <strong>Data Collection</strong><br> The Attacker Visits the <code>IP_Router_Address/pass</code> In the Browser And Monitor the connected devices and captured data using the serial monitor in the Arduino IDE Or in The <code>/pass</code> Webpage. </li> <li> <strong>Configuration</strong><br> Adjust network settings and password in the `Script.ino` file as needed. </li> </ol> <h2>💻 Tech Stack</h2> <table> <thead> <tr> <th>Component</th> <th>Technology</th> </tr> </thead> <tbody> <tr> <td><strong>Hardware</strong></td> <td>ESP8266</td> </tr> <tr> <td><strong>Software</strong></td> <td>Arduino IDE, C++</td> </tr> <tr> <td><strong>Libraries</strong></td> <td>ESP8266WiFi, DNSServer</td> </tr> <tr> <td><strong>Version Control</strong></td> <td>Git</td> </tr> </tbody> </table>

### 📅 Future Enhancements
 - 🌐 Improve data capture methods and security. 
 - 🔍 Add more sophisticated phishing techniques. 
 - 📊 Integrate logging and reporting features.

### 📝 License
This project is licensed under the MIT License.

### 🎯 Screenshots

## The Captive Portal :
<img width="1012" alt="Screenshot 2024-09-17 at 19 23 35" src="https://github.com/user-attachments/assets/58f3557f-180c-4872-b96a-cfc67ab9c8cb">

<img width="1012" alt="Screenshot 2024-09-17 at 19 23 48" src="https://github.com/user-attachments/assets/4c6d44a5-beea-4bbc-9e65-563f37e815e5">


## The Data Extraction Page :
<img width="1280" alt="Screenshot 2024-09-17 at 19 26 05" src="https://github.com/user-attachments/assets/9131e124-a6dc-40d3-a118-a6540816e8b0">

## ® Project By 010011110010110101011010 ®
