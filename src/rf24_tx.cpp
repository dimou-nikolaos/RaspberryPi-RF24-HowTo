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
    const uint8_t address[6] = "00001";

    // Initialize the radio
    if (!radio.begin()) {
        cout << "Radio hardware not responding!" << endl;
        return 1;
    }

    // Set the PA Level low to prevent power supply related issues
    radio.setPALevel(RF24_PA_LOW);

    // Open a writing pipe
    radio.openWritingPipe(address);

    // Stop listening to start transmitting
    radio.stopListening();

    // Message to send
    const char message[] = "Hello, RF24!";

    // Send the message
    if (radio.write(&message, sizeof(message))) {
        cout << "Message sent successfully: " << message << endl;
    } else {
        cout << "Message transmission failed" << endl;
    }

    return 0;
}
