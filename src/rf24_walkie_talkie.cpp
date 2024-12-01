#include <ctime>
#include <iostream>
#include <string>
#include <thread>
#include <atomic>
#include <csignal>
#include <RF24/RF24.h>

using namespace std;

// Global variables for graceful shutdown
RF24* globalRadio = nullptr;
thread* globalReceiveThread = nullptr;
atomic<bool> keepRunning(true);
atomic<bool> isListening(true);

// Signal handler for clean exit
void signalHandler(int signum) {
    cout << "\nInterrupt signal (" << signum << ") received." << endl;
    
    // Set flag to stop main loop
    keepRunning = false;
    
    // Wait a moment to allow cleanup
    this_thread::sleep_for(chrono::milliseconds(100));
    
    // Cleanup radio if it exists
    if (globalRadio) {
        // Stop listening and power down the radio
        globalRadio->stopListening();
        globalRadio->powerDown();
        
        // Delete the radio object
        delete globalRadio;
        globalRadio = nullptr;
    }
    
    // Join receive thread if it exists
    if (globalReceiveThread) {
        if (globalReceiveThread->joinable()) {
            globalReceiveThread->join();
        }
        delete globalReceiveThread;
        globalReceiveThread = nullptr;
    }
    
    // Exit the program
    exit(signum);
}

// Function to handle receiving messages
void receiveMessages(RF24& radio, const uint8_t* address) {
    while (keepRunning) {
        if (isListening) {
            if (radio.available()) {
                // Prepare a buffer to store the incoming message
                char message[32] = {0};  // Adjust buffer size as needed
                
                // Read the message
                radio.read(&message, sizeof(message));
                
                // Print the received message
                cout << "\nReceived message: " << message << endl;
                cout << "Press Enter to send a message or wait to continue listening..." << endl;
            }
            
            // Small delay to prevent tight looping
            this_thread::sleep_for(chrono::milliseconds(100));
        }
    }
}

int main() {
    // Register signal handlers
    signal(SIGINT, signalHandler);   // Ctrl+C
    signal(SIGTERM, signalHandler);  // Termination signal
    
    // Initialize the radio
    RF24 radio(RF24_CE_PIN, RF24_CSN_PIN);
    globalRadio = &radio;
    
    // Radio pipe addresses for communication
    const uint8_t address[6] = "00001";
    
    // Initialize the radio
    if (!radio.begin()) {
        cout << "Radio hardware not responding!" << endl;
        return 1;
    }
    
    // Set the PA Level low to prevent power supply related issues
    radio.setPALevel(RF24_PA_LOW);
    
    // Start in listening mode
    radio.openReadingPipe(0, address);
    radio.startListening();
    
    // Start a thread for receiving messages
    thread receiveThread(receiveMessages, ref(radio), address);
    globalReceiveThread = &receiveThread;
    
    // Main interaction loop
    while (keepRunning) {
        // Wait for user to press Enter to switch to transmit mode
        cout << "Listening mode: Press Enter to send a message..." << endl;
        cin.get();  // Wait for Enter key
        
        if (!keepRunning) break;
        
        // Switch to transmit mode
        isListening = false;
        radio.stopListening();
        radio.openWritingPipe(address);
        
        // Prompt for message
        cout << "Enter message to transmit: ";
        string message;
        getline(cin, message);
        
        if (!keepRunning) break;
        
        // Send the message
        if (radio.write(message.c_str(), message.length() + 1)) {
            cout << "Message sent successfully: " << message << endl;
        } else {
            cout << "Message transmission failed" << endl;
        }
        
        // Switch back to listening mode
        radio.startListening();
        radio.openReadingPipe(0, address);
        isListening = true;
    }
    
    // Cleanup (will be called if normal exit is possible)
    signalHandler(0);
    
    return 0;
}
