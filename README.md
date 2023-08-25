# Gate Status Sensor with Email Alerts

![Gate_sensor](image.png)

## Introduction

This project involves creating a gate status sensor using an ESP8266 module. The sensor detects whether the gate is open or closed. If the gate remains open for an extended period, the system sends an email alert to the specified recipient. This project is designed to help you monitor the status of your gate remotely.

**Project Repository:** [Gate Status Sensor](https://github.com/lukas-kramer07/Gate-Status-Sensor)

## Features

- Real-time status monitoring: Check if your gate is open or closed in real-time.
- Email notifications: Receive email alerts if the gate is open for an extended period.
- User-friendly interface: Access the status through a web interface on your local network.

## Technologies Used

This project utilizes the following technologies and libraries:

- ESP8266WiFi: WiFi library for connecting to the local network.
- ESPAsyncTCP and ESPAsyncWebServer: Libraries for creating an asynchronous web server.
- ESP_Mail_Client: A library for sending email notifications.
- HTML and CSS: Used for creating the web interface.

## Installation

To set up the Gate Status Sensor, follow these steps:

1. Clone this repository to your development environment.
2. Connect the ultrasonic sensor to your ESP8266 module according to the pin configuration in the `GateSensor.ino` file.
3. Configure the network settings in the `network.h` file, including your WiFi SSID and password.
4. Set up email notifications by entering your email credentials in the `Email.h` file.
5. Upload the `GateSensor.ino` sketch to your ESP8266 module using the Arduino IDE.
6. Connect the ESP8266 module to your gate.
7. Power on the ESP8266 module.

## Usage

To use the Gate Status Sensor, follow these steps:

1. Access the web interface by opening a web browser and entering the IP address of your ESP8266 module on your local network.
2. The interface will display the current status of the gate (open or closed) and the time it has been in that state.
3. If the gate remains open for a predefined period (default: 20 minutes), an email notification will be sent to the specified recipient.

## Customization

You can customize this project in various ways:

- Change the appearance of the web interface by modifying the HTML and CSS in the `html_code.h` file.
- Adjust the time interval for email notifications by changing the `timeToMail` variable in the `GateSensor.ino` file.
- Customize the email content and recipient in the `GateSensor.ino` file.
- Extend the project by adding additional sensors or features.

## Credits

- ESP Mail code adapted from [mobizt/ESP-Mail-Client](https://github.com/mobizt/ESP-Mail-Client).

## License

This project is open-source and available under the [MIT License](LICENSE).

Feel free to contribute, share, or modify this project according to your needs. If you encounter any issues or have suggestions for improvement, please create an issue on the GitHub repository.

