# ESP32-ReqMaker
A simple program that allows you to make requests from the ESP32 over UART

## Usage
First, send the network SSID and then the LF character. After that, send the network password and the LF character again. The ESP32 shoudl turn its internal led on, connect to the network, turn the internal led off and respond with `connected\n`. 

To make a request, send JSON data in this format in one line:
```json
{
  "method": "GET | POST",
  "url": "https://example.com",
  "headers": {
    "Header-1": "value1",
    "Header-2": "value2",
    ...
  },
  "body": "Hello World!"
}
```
and end it with an LF character. The ESP32 should respond with something like this:
```json
{
  "status": 200,
  "body": "Hello World again!",
  "headers": {
    "Ping": "pong",
    "Thisisa": "header",
    ...
  }
}
```
and then the LF character.
