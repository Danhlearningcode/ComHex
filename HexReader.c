#include "HexReader.h"

uint8_t *HexData = NULL; // Định nghĩa mảng lưu data
uint32_t CurrentNumberData = 0; // Định nghĩa số lượng data hiện tại
// char LineData[50]; // Định nghĩa mảng chứa giá trị đọc được từ file hex
// int LineHexData[50]; // Định nghĩa mảng chứa giá trị đọc từ file hex đã chuyển qua dạng hex
HEXFormData hexStruct; // Định nghĩa cấu trúc hexStruct

 /******************************************************************************************
 * @brief   Lấy data từ 1 file Lưu Data vào mảng HexData
 * @details Kiểm tra thông tin của 1 dòng hex,
 *          Gán Data vào mảng nếu RecordType của dòng hex là 0x00
 *          Lưu Data vào mảng HexData
 * @param   *fp Con trỏ tới file 
 * @return  uint8_t trả về 1 nếu lấy được data, 0 nếu có lỗi xảy ra
 *******************************************************************************************/
uint8_t GetHexData(FILE *fp){
    HEXField State;
    char LineData[50]; // mảng chứa data của 1 hàng
    int LineHexData[50]; // mảng chứa data của 1 hàng đã được chuyển về dạng hex
    uint32_t Count = 0;
    uint32_t DataCount = 0;
    uint8_t CalcCheckSum=0;
    uint16_t CurrentSize = 256;
    HexData = (uint8_t *)malloc(CurrentSize * sizeof(uint8_t));
    while ( ReadLine(fp,LineData) ){
        State = START_CODE; // gán state về START
        for ( Count = 0; Count < strlen(LineData); Count++){ // Lặp tới khi hết 1 dòng HEX
            switch (State) 
            {
            case START_CODE:
                if(LineData[Count] != ':'){
                    free(HexData);
                    return 0;
                }
                else{
                    State = BYTE_COUNT;
                }
                break;
            case BYTE_COUNT:
                ConvertStringToHex(&LineData[Count],LineHexData,2); // Chuyển đổi 2 phần tử tiếp theo của dòng hex từ string sang hex
                hexStruct.Byte_Count = LineHexData[0]; // gán giá trị cho Byte_count
                Count=2; // Gán Count = 2
                State = ADDRESS;// chuyển tới Case tiếp theo
                break;
            case ADDRESS:
                ConvertStringToHex(&LineData[Count],LineHexData,4);
                hexStruct.Address[0] = LineHexData[0];
                hexStruct.Address[1] = LineHexData[1];
                Count=6;
                State = RECORD_TYPE;
                break;
            case RECORD_TYPE:
                ConvertStringToHex(&LineData[Count],LineHexData,2); // Chuyển đổi 2 phần tử tiếp theo của dòng hex từ string sang hex
                hexStruct.Record_Type = LineHexData[0]; // gán giá trị cho Record_type
                Count=8;// Gán Count = 8
                State = DATA;// chuyển tới Case tiếp theo
                break;  
            case DATA:
                ConvertStringToHex(&LineData[Count],LineHexData,hexStruct.Byte_Count*2);
                for (DataCount = 0; DataCount < hexStruct.Byte_Count; DataCount++)
                {  
                    hexStruct.Data[DataCount] = LineHexData[DataCount];
                    if(hexStruct.Record_Type == 0x00){
                        if(CurrentNumberData >= CurrentSize){
                            CurrentSize = CurrentSize + hexStruct.Byte_Count;
                            HexData = (uint8_t *)realloc(HexData, sizeof(uint8_t) * CurrentSize);
                            }
                        HexData[CurrentNumberData] = LineHexData[DataCount];
                        CurrentNumberData++;
                    } 
                }
                Count = 8 + hexStruct.Byte_Count*2;
                State = CHECK_SUM;
                break;
            case CHECK_SUM:
                ConvertStringToHex(&LineData[Count],LineHexData,2);
                hexStruct.Check_Sum = LineHexData[0];
                CalcCheckSum = hexStruct.Byte_Count + hexStruct.Address[0] + hexStruct.Address[1] + hexStruct.Record_Type;
                for (DataCount = 0; DataCount < hexStruct.Byte_Count; DataCount++)
                {
                    CalcCheckSum = CalcCheckSum + hexStruct.Data[DataCount];
                }
                CalcCheckSum = ~CalcCheckSum + 1;
                if (CalcCheckSum != hexStruct.Check_Sum){
                    free(HexData);
                    return 0;
                }
                Count = strlen(LineData)+1;
                break;        
            default:
                free(HexData);
                return 0;
            }
        } 
    }
    // CurrentNumberData = 0;
    return 1;
}
 /******************************************************************************************
 * @brief   Hàm này lấy giá trị của một dòng hex trong file HEX rồi gán vào mảng data
 * @details Hàm này lấy giá trị của từng phần tử trên 1 dòng rồi gán vào mảng data
 *          lặp lại cho tới khi kết thúc file
 * @param   *fLineData Con trỏ tới mảng chứa data
 * @param   *fp Con trỏ tới file 
 * @return  char Trả về 0 nếu kết thúc file, 1 nếu chưa kết thúc file
 *******************************************************************************************/
char ReadLine(FILE *fp, char *fLineData){
    // khởi tạo các giá trị
    char fData;
    uint8_t Count=0;

    while((fData = fgetc(fp)) != '\n' && fData != EOF){ //Lặp và gán fData tới khi gặp ký tự \n hoặc EOF
        fLineData[Count] = fData; // gán giá trị lấy được từ file vào mảng chứa data
        Count++;
    } 
    if (fData == EOF) {
        return 0; //EndFile
    }
    return 1; 
}
 /******************************************************************************************
 * @brief   Chuyển đổi từ String sang HEX
 * @details Hàm này thực hiện kiểm tra các phần tử trong mảng với độ dài biết trước
 *          thuộc từ 1 tới 9, từ A tới F sau đó chuyển đổi String sang Hex 
 * @param   *buff  Con trỏ tới mảng cần chuyển đổi
 * @param   length Số lượng phần tử trong mảng cần chuyển đổi
 * @param   *fLineHexData Con trỏ tới mảng chứa giá trị chuyển đổi
 * @return  void
 *******************************************************************************************/
void ConvertStringToHex(char *buff,int *fLineHexData,uint8_t length){
    // khởi tạo các giá trị
    uint32_t Count = 0; 
    uint32_t Count1 = 0; 
    uint32_t fbuff[50]; 

    for(Count = 0; Count < length; Count++){
        if(buff[Count] >= '0' && buff[Count] <= '9'){ // kiểm tra xem index của mảng có nằm trong khoảng từ 1 tới 9
            fbuff[Count] = buff[Count] - 48; // nếu có thì thực hiện chuyển đổi thành Dec
        }
        else if(buff[Count] >= 'A' && buff[Count] <= 'F'){// kiểm tra xem index của mảng có nằm trong khoảng từ A tới F
            fbuff[Count] = buff[Count] - 65 + 10; // nếu có thì thực hiện chuyển đổi thành Dec
        }
        else{
            return; // nếu không nằm trong khoảng từ 1 tới 9 hoặc A tới F thì thoát vòng lặp
        }
        if(Count%2 != 0){ // kiểm tra xem số lượng phần tử đã chuyển đổi có phải là 2 chưa
            fLineHexData[Count1] = ((fbuff[Count-1] << 4) | (fbuff[Count])); //thực hiện chuyển đổi
            Count1++;//index +1
        }
    }
}