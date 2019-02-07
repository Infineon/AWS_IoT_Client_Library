# wifi-middleware

Repo for housing the WiFi middleware created for ModusToolbox and mbed OS.

- code_examples        - Reference applications for WiFi middleware
    - AWS_Greengrass
        - publisher
        - subscriber
    - AWS_IoT_client
        - publisher
        - subscriber
- libraries
    - protocols         - WiFi middleware protocol libraries
        - AWS_IoT_client
        - HTTP_client
        - MQTT
    - utilities         - Helper utilities
	

# Device side configuration for running AWS IOT applications.

* Follow steps to create AWS IOT thing and thing certificate.
  [ https://docs.aws.amazon.com/iot/latest/developerguide/iot-gs.html ]

* Load/Modify created thing certificate, private key and RootCA certificate in aws_config.h file.

* Modify thing name & topic name accordingly in aws_config.h file.

* Compile & run the application
