#include <Arduino.h>


namespace arra {
// Define the maximum message content length
const size_t MAX_CONTENT_LENGTH = 16;

// Structure for holding a message
struct Message {
  byte id;
  char content[MAX_CONTENT_LENGTH];
};

// Enum for command types
enum CommandType {
  CALIBRATE_1 = 1,
  CALIBRATE_2 = 2,
  CONFIG_1 = 3,
  CONFIG_2 = 4
};

// Command handler class
class CommandHandler {
public:
  // Constructor
  CommandHandler() {}

  // Method to handle the received command
  void handleCommand(const Message& message) {
    switch (message.id) {
      case CALIBRATE_1:
        handleCalibrate1();
        break;
      case CALIBRATE_2:
        handleCalibrate2();
        break;
      case CONFIG_1:
        handleConfig1();
        break;
      case CONFIG_2:
        handleConfig2();
        break;
      default:
        // Invalid or unrecognized command
        break;
    }
  }

private:
  // Command execution methods
  void handleCalibrate1() {
    // Business logic for CALIBRATE_1 command
  }

  void handleCalibrate2() {
    // Business logic for CALIBRATE_2 command
  }

  void handleConfig1() {
    // Business logic for CONFIG_1 command
  }

  void handleConfig2() {
    // Business logic for CONFIG_2 command
  }
};

// // Function to send a message
// void sendMessage(const Message& message) {
//   const byte* messageBytes = reinterpret_cast<const byte*>(&message);
//   Serial.write(messageBytes, sizeof(Message));
// }

// // Function to receive a message
// void receiveMessage(Message& message) {
//   byte* messageBytes = reinterpret_cast<byte*>(&message);
//   Serial.readBytes(messageBytes, sizeof(Message));
// }

// Function to set the content of a message
void setMessageContent(Message& message, const char* content) {
  strncpy(message.content, content, MAX_CONTENT_LENGTH);
}

// Function to get the content of a message
const char* getMessageContent(const Message& message) {
  return message.content;
}

}