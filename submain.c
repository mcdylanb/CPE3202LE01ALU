
#include <stdio.h>
#include <stdint.h> //to use stndint

// TODO: [x] - Print into 16 bit
// TODO: [x] - print hex to binary in 16 bit
// Function prototypes
unsigned char twosComp(unsigned char data);
unsigned char setFlags(unsigned int ACC);
void printBin(int data, unsigned char data_width);

// Helper function to calculate 2's complement
unsigned char twosComp(unsigned char data) {
    return ~data + 1;
}

void printBin(int data, unsigned char data_width) {
    for(int i = data_width - 1; i >= 0; i--) {
        printf("%d", (data >> i) & 1);
    }
}

int ALU(unsigned char operand1, unsigned char operand2, unsigned char control_signals){
    printf("\nOperation details:\n");
    printf("Operand 1: ");
    printBin(operand1, 8);
    printf(" (0x%02X)\n", operand1);
    printf("Operand 2: ");
    printBin(operand2, 8);
    printf(" (0x%02X)\n", operand2);

    unsigned char result = 0x00;
    return result;
}
int main(void){
    uint8_t opperand01 = 0xC0;
    uint8_t opperand11 = 0x0A;
    ALU(opperand01,opperand11,opperand01);
    return 0;
}
