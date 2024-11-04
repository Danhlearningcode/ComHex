#include <stdio.h>
#include <stdlib.h>

#define BUFFER_SIZE 10 // Kích thước cố định của buffer

typedef struct {
    char buffer[BUFFER_SIZE]; // Mảng tĩnh để lưu trữ dữ liệu
    size_t head;              // Chỉ số cho phần tử tiếp theo sẽ được thêm vào
    size_t tail;              // Chỉ số cho phần tử tiếp theo sẽ được lấy ra
    size_t currentSize;       // Kích thước hiện tại của buffer
} RingBuffer;

// Khởi tạo ring buffer
void initBuffer(RingBuffer *rb) {
    rb->head = 0;
    rb->tail = 0;
    rb->currentSize = 0;
}

// Kiểm tra xem buffer có đầy không
int isFull(RingBuffer *rb) {
    return rb->currentSize == BUFFER_SIZE;
}

// Kiểm tra xem buffer có rỗng không
int isEmpty(RingBuffer *rb) {
    return rb->currentSize == 0;
}

// Thêm một phần tử vào buffer
void put(RingBuffer *rb, char value) {
    if (!isFull(rb)) {
        rb->buffer[rb->head] = value;
        rb->head = (rb->head + 1) % BUFFER_SIZE;
        rb->currentSize++;
    } else {
        printf("Buffer is full! Cannot add %c\n", value);
    }
}

// Lấy một phần tử ra khỏi buffer
char get(RingBuffer *rb) {
    if (!isEmpty(rb)) {
        char value = rb->buffer[rb->tail];
        rb->tail = (rb->tail + 1) % BUFFER_SIZE;
        rb->currentSize--;
        return value;
    }
    printf("Buffer is empty!\n");
    return '\0'; // Trả về ký tự null nếu buffer rỗng
}

int main() {
    RingBuffer rb;
    initBuffer(&rb); 
    // Mảng dữ liệu cần gửi (dữ liệu bạn đã cung cấp)
    char sampleData[] = {0x70, 0x06, 0x00, 0x20, 0x89, 0x01, 0x00, 0x08, 0x91, 0x01};
    return 0;
}
