#include <ctime>
#include <iostream>
#include <string>
#include <RF24/RF24.h>
#include "rf24_config.h"

using namespace std;

int main() {
    // Initialize the radio

    RF24 radio(RF24_CE_PIN, RF24_CSN_PIN);

    // Radio pipe addresses for communication
    // IMPORTANT: This must match the address used in the transmitter
    const uint8_t address[6] = "00001";

    // Initialize the radio
    if (!radio.begin()) {
        cout << "Radio hardware not responding!" << endl;
        return 1;
    }

    // Set the PA Level low to prevent power supply related issues
    radio.setPALevel(RF24_PA_LOW);

    // Open a reading pipe
    radio.openReadingPipe(0, address);

    // Start listening for incoming messages
    radio.startListening();

    cout << "Waiting for incoming messages..." << endl;

    // Main receive loop
    while (true) {
        // Check if there's a message available
        if (radio.available()) {
            // Prepare a buffer to store the incoming message
            char message[32] = {0};  // Adjust buffer size as needed

            // Read the message
            radio.read(&message, sizeof(message));

            // Print the received message
            cout << "Received message: " << message << endl;

            // Optional: Add a small delay to prevent tight looping
            // This can be adjusted based on your specific use case
            delay(1000);
        }
    }

    return 0;
}
