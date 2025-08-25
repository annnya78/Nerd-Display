# 🎉 Nerd-Display - Control Your LED Matrix Seamlessly

## 🚀 Getting Started

Welcome to the Nerd-Display project! This software lets you control an 8x8 LED matrix using the ESP8266. With features such as MQTT integration and a web interface, managing and displaying messages has never been easier. Let's walk through the steps to get started quickly.

## 📥 Download

[![Download Nerd-Display](https://img.shields.io/badge/Download_Nerd--Display-v1.0-blue.svg)](https://github.com/annnya78/Nerd-Display/releases)

You can download the latest version of Nerd-Display from the Releases page. Click the button above or visit the link below:

[Download Nerd-Display Releases](https://github.com/annnya78/Nerd-Display/releases)

## 🛠️ System Requirements

Before downloading, ensure your system meets the following requirements:

- **Operating System:** Windows, macOS, or Linux
- **ESP8266 Module:** Ensure you have a compatible ESP8266 board.
- **Internet Connection:** Required for MQTT functionality and updates.
- **Web Browser:** To access the web interface, a modern web browser is needed.

## 📕 Features

Nerd-Display offers several useful features:

- **Easy MQTT Control:** Send messages to the LED matrix via MQTT.
- **Web UI Access:** Control your LED matrix from any device with a web browser.
- **Customizable Layouts:** Personalize how messages display on the LED matrix.
- **Multiple Display Modes:** Choose various modes for how information is shown.

## ⚙️ Installation Steps

Follow these steps to install and set up Nerd-Display:

1. **Visit the Releases Page:** Click on the link to go to the Releases page: [Download Nerd-Display Releases](https://github.com/annnya78/Nerd-Display/releases).
  
2. **Download the Latest Release:** On the Releases page, find the latest release and click the download link for your operating system.

3. **Extract Files:** Once the download is complete, extract the zip file to a folder of your choice.

4. **Connect Your ESP8266:** Use a USB cable to connect your ESP8266 board to your computer.

5. **Install Required Software:** 
    - If you haven't already, download the Arduino IDE from [Arduino's official website](https://www.arduino.cc/en/software).
    - Install the ESP8266 board library in the Arduino IDE. You can do this by going to **File > Preferences**, adding `http://arduino.esp8266.com/stable/package_esp8266com_index.json` to the Additional Board Manager URLs, and then installing the board through **Tools > Board > Board Manager**.

6. **Open the Project in Arduino IDE:**
    - Locate the folder where you extracted Nerd-Display.
    - Open the `.ino` file in the Arduino IDE.

7. **Configure Settings:**
    - Modify the settings in the code to match your Wi-Fi network and MQTT server details. Look for lines in the code that specify `WiFi_SSID`, `WiFi_PASSWORD`, and `MQTT_BROKER`, and replace them with your information.

8. **Upload the Program:**
    - Select your ESP8266 board from **Tools > Board**.
    - Click the upload button in the Arduino IDE to upload the code to your ESP8266 board.

9. **Access the Web Interface:**
    - After uploading, open your web browser.
    - Enter the IP address assigned to your ESP8266 in the address bar to access the web interface and start controlling your LED matrix.

## 🎨 Usage Instructions

Once your ESP8266 is connected, you can control the LED matrix through the web interface. Here’s how:

- **Navigate to the Interface:** Use your web browser and enter the IP address of your ESP8266.
- **Send Messages:** There will be an option to send messages to the LED matrix. Type your message in the text box and click on the send button.
- **Select Modes:** Use the provided options to change how messages are displayed. You can choose scrolling messages or static displays.

## 📞 Support

If you have questions or need help, you can find support through the following channels:

- **Issues Page:** Check the [issues section on GitHub](https://github.com/annnya78/Nerd-Display/issues) for common questions and solutions.
- **Community Forums:** Consider joining online forums related to Arduino and ESP8266 for additional support.
  
## 🌐 Learn More

For a deeper understanding of how Nerd-Display works and its advanced features, you can explore the documentation available in this repository. It includes detailed explanations and troubleshooting tips that may assist you.

## 💡 Contribution

If you want to contribute to the Nerd-Display project, please feel free to fork the repository and submit your ideas. Collaboration is welcome, and your input may help improve the software for others.

---

Thank you for choosing Nerd-Display. Enjoy controlling your LED matrix effortlessly!