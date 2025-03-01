#include <iostream>
#include <vector>
#include <chrono>
#include <thread>
#include <fstream>
#include <windows.h>

#define BAUDRATE 57600
#define DEBUGOUTPUT 0

// checksum variables
unsigned char generatedChecksum = 0;
unsigned char checksum = 0;
int payloadLength = 0;
std::vector<unsigned char> payloadData(64, 0);
unsigned char poorQuality = 0;
unsigned char attention = 0;
unsigned char meditation = 0;
int16_t rawData = 0;

// system variables
long lastReceivedPacket = 0;
bool bigPacket = false;

HANDLE hSerial;

bool openSerialPort(const char* portName) {
    hSerial = CreateFile(portName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        std::cerr << "Error opening serial port" << std::endl;
        return false;
    }
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);
    if (!GetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error getting serial port state" << std::endl;
        return false;
    }
    dcbSerialParams.BaudRate = BAUDRATE;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        std::cerr << "Error setting serial port parameters" << std::endl;
        return false;
    }
    return true;
}

unsigned char ReadOneByte() {
    unsigned char byteRead;
    DWORD bytesRead;
    ReadFile(hSerial, &byteRead, 1, &bytesRead, NULL);
    return byteRead;
}

int main() {
    if (!openSerialPort("COM6")) {
        return 1;
    }

    auto startTime = std::chrono::steady_clock::now();
    while (std::chrono::steady_clock::now() - startTime < std::chrono::seconds(20)) {
        if (ReadOneByte() == 170) {
            if (ReadOneByte() == 170) {
                payloadLength = ReadOneByte();
                if (payloadLength > 169) continue;

                generatedChecksum = 0;
                for (int i = 0; i < payloadLength; i++) {
                    payloadData[i] = ReadOneByte();
                    generatedChecksum += payloadData[i];
                }

                checksum = ReadOneByte();
                generatedChecksum = 255 - generatedChecksum;

                if (checksum == generatedChecksum) {
                    poorQuality = 200;
                    attention = 0;
                    meditation = 0;

                    for (int i = 0; i < payloadLength; i++) {
                        switch (payloadData[i]) {
                            case 2:
                                i++;
                                poorQuality = payloadData[i];
                                bigPacket = true;
                                break;
                            case 4:
                                i++;
                                attention = payloadData[i];
                                break;
                            case 5:
                                i++;
                                meditation = payloadData[i];
                                break;
                            case 0x80:
                                i++;
                                rawData = ((int8_t)payloadData[i + 1] << 8) | payloadData[i + 2];
                                std::cout << "Raw Data: " << rawData << std::endl;
                                i += 2;
                                break;
                            case 0x83:
                                i += 24;
                                break;
                            default:
                                break;
                        }
                    }

                    if (bigPacket) {
                        lastReceivedPacket = std::chrono::duration_cast<std::chrono::milliseconds>(
                              std::chrono::system_clock::now().time_since_epoch()).count();
                    }
                    bigPacket = false;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }
    
    CloseHandle(hSerial);
    return 0;
}
