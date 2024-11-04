#include <windows.h>
#include <stdio.h>

HANDLE hComm;  // Handle cho cổng COM
int statusPort = 0; // Biến trạng thái cổng COM (0: đóng, 1: mở)

int closePort() {
    if (statusPort == 1) {
        if (!CloseHandle(hComm)) {
            printf("Error: Port closing failed\n");
            return 0;
        }
        statusPort = 0;
        hComm = INVALID_HANDLE_VALUE; // Đặt lại giá trị của hComm
        printf("Port closed successfully\n");
        Sleep(100); // Chờ 100ms để đảm bảo tài nguyên được giải phóng
        return 1;
    }
    return 0;
}

// Hàm mở cổng COM và thiết lập các thông số
int openPort(const char* portName, int baudRate) {
    // Nếu cổng COM đã mở, đóng trước khi mở lại
    if (hComm != INVALID_HANDLE_VALUE) {
        closePort();
    }

    hComm = CreateFileA(
        portName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL
    );

    if (hComm == INVALID_HANDLE_VALUE) {
        printf("Error: Unable to open %s\n", portName);
        return 0;
    }

    // Cấu hình cổng COM như trước
    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hComm, &dcbSerialParams)) {
        printf("Error: GetCommState failed\n");
        CloseHandle(hComm);
        return 0;
    }

    dcbSerialParams.BaudRate = baudRate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity = NOPARITY;

    if (!SetCommState(hComm, &dcbSerialParams)) {
        PurgeComm(hComm, PURGE_RXCLEAR | PURGE_TXCLEAR);
        printf("Error: SetCommState failed. Trying to purge...\n");
        if (!SetCommState(hComm, &dcbSerialParams)) {
            printf("Error: SetCommState still failed after purge.\n");
            CloseHandle(hComm);
            return 0;
        }
    }


    // Thiết lập thời gian chờ
    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 3;
    timeouts.ReadTotalTimeoutConstant = 2;
    timeouts.ReadTotalTimeoutMultiplier = 3;
    timeouts.WriteTotalTimeoutConstant = 2;
    timeouts.WriteTotalTimeoutMultiplier = 3;

    if (!SetCommTimeouts(hComm, &timeouts)) {
        printf("Error: SetCommTimeouts failed\n");
        CloseHandle(hComm);
        return 0;
    }

    statusPort = 1;
    printf("%s opened successfully\n", portName);
    return 1;
}



// Hàm đọc dữ liệu từ cổng COM
int readData(char* buffer, unsigned int bufferSize, unsigned long* bytesRead) {
    if (!ReadFile(hComm, buffer, bufferSize, bytesRead, NULL)) {
        printf("Error: Read from COM port failed\n");
        return 0;
    }
    if (*bytesRead > 0) {
        buffer[*bytesRead] = '\0'; // Kết thúc chuỗi
    }
    return 1;
}

// Hàm ghi dữ liệu vào cổng COM
int writeData(const char* data, unsigned int dataSize, unsigned long* bytesWritten) {
    if (!WriteFile(hComm, data, dataSize, bytesWritten, NULL)) {
        printf("Error: Write to COM port failed\n");
        return 0;
    }
    return 1;
}

// Hàm kiểm tra trạng thái cổng COM
int getStatusPort() {
    return statusPort;
}
