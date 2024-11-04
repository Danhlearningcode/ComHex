#include "HexReader.h"
#include "FindHexFile.h"
#include "ComUart.h"

int main(){
    DirectoryEntry entries[256];    
    int entry_count = 0,i;
    Read_Direction(".", 0, entries, &entry_count);
    const char* portName = "\\\\.\\COM3";
    int baudRate = 115200;
    char buffer[256];
    unsigned long bytesRead, bytesWritten;

    printf("List hex: \n");
    for (int i=0; i < entry_count; i++) {
        printf("%d. %s - %s\n",i, entries[i].name, entries[i].full_path);
    }
    printf("FILE HEX muốn lấy data: ");
    scanf("%d",&i);
    printf("dang doc file: %s\n", entries[i].full_path);
    FILE *fp = fopen(entries[i].full_path, "r");        
    if(fp == NULL){
        printf("Loi khi mo file");
    }
    else{
        printf("du lieu tu file %s: \n", entries[i].full_path);
        GetHexData(fp);
        fclose(fp);

        for (uint16_t i = 0; i < CurrentNumberData; i++) {
            if (openPort(portName, baudRate)) {
                for (uint32_t j = 0; j < CurrentNumberData; j++) {
                    if (!writeData((const char*)&HexData[j], 1, &bytesWritten)) {
                        printf("Lỗi khi gửi dữ liệu qua UART\n");
                        break; 
                    }   
                }
            }
            // if (writeData("Hello COM port", 14, &bytesWritten)) {
            //     printf("Data written: %ld bytes\n", bytesWritten);
            // }
            //while (1) {
            //    if (readData(buffer, sizeof(buffer), &bytesRead)) {
            //         printf("Data read: %ld bytes, Content: %s\n", bytesRead, buffer);
            //     }
            //     Sleep(100); 
            // }
            // closePort();
            // } else {
            //     printf("Failed to open port %s\n", portName);
            // }

        printf("%02x ", HexData[i]);
        }
        CurrentNumberData = 0;
        free(HexData);
        printf("\n");
    }
    return 0;
}
// gcc -c main.c -o main.o
// gcc -c HexReader.c -o HexReader.o
// gcc -c FindHexFile.c -o FindHexFile.o
// gcc -c ComUart.c -o ComUart.o
// gcc main.o HexReader.o FindHexFile.o ComUart.o -o main