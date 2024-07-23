#include <stdint.h>
#include <stdlib.h>
#include <ctype.h>

uint8_t getDeviceNumber(const char* input, const char** afterDeviceSpec) {
    *afterDeviceSpec = input; // Default to start of input if no device spec is found

    // Check if the string starts with 'n' or 'N'
    if (input[0] == 'n' || input[0] == 'N') {
        // Check if the next character is a colon, which means it's device number 1
        if (input[1] == ':') {
            *afterDeviceSpec = &input[2]; // Update pointer to after "N:"
            return 1;
        }
        // Check if the next character is a digit and followed by a colon
        else if (isdigit(input[1]) && input[2] == ':') {
            *afterDeviceSpec = &input[3]; // Update pointer to after "Nx:"
            return (uint8_t) (input[1] - '0'); // Convert the digit character to its integer value
        }
    }
    // If none of the above conditions are met, return device number 1
    // but do not update the pointer as there's no device spec to skip
    return 1;
}