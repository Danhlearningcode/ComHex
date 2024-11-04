#include <windows.h>
#include <stdio.h>

extern HANDLE hComm;  // Handle cho cổng COM
extern int statusPort; // Biến trạng thái cổng COM (0: đóng, 1: mở)

int closePort();
int openPort(const char* portName, int baudRate);
int readData(char* buffer, unsigned int bufferSize, unsigned long* bytesRead);
int writeData(const char* data, unsigned int dataSize, unsigned long* bytesWritten);
int getStatusPort();