#ifndef _HEX_READER_H 
#define _HEX_READER_H
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>

typedef enum{
    START_CODE  = 0U,
    BYTE_COUNT  = 1U,  // Trường ByteCount 
    ADDRESS     = 2U,
    RECORD_TYPE = 3U, // Trường RecordType 
    DATA        = 4U, // Trường Data 
    CHECK_SUM   = 5U,
}HEXField;

typedef struct 
{
    uint8_t Byte_Count; //ByteCount của dòng hex
    uint8_t Address[2];
    uint8_t Record_Type;//RecordType của dòng hex
    uint8_t Data[16];
    uint8_t Check_Sum;
}HEXFormData;

extern uint8_t *HexData; // Mảng lưu data
extern uint32_t CurrentNumberData; // Số lượng data hiện tại
// extern char LineData[50]; // Mảng chứa giá trị đọc được từ file hex
// extern int LineHexData[50]; // Mảng chứa giá trị đọc từ file hex đã chuyển qua dạng hex
extern HEXFormData hexStruct; 

uint8_t GetHexData(FILE *fp); // Khai báo hàm lấy data
char ReadLine(FILE *fp, char *fLineData); // khai báo hàm đọc dữ liệu từ 1 dòng
void ConvertStringToHex(char *buff,int *fLineHexData,uint8_t length); // khai báo hàm chuyển đổi string sang hex

#endif // HEX_READER_H