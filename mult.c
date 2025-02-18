#include <stdio.h>
#include <stdint.h> //to use stndint

// TODO: [x] - Print into 16 bit
// TODO: [x] - print hex to binary in 16 bit

void printBin(int data, unsigned char data_width)
{
    for (int i = data_width - 1; i >= 0; i--)
    {
        printf("%d", (data >> i) & 1);
    }
}
// 0x88,0x85,0x01

uint8_t twosComplement(uint8_t num)
{
    return ~num + 1;
}

int main(void)
{
    //TEST 1
    uint8_t opperand01 = 0xC0;
    uint8_t opperand11 = 0x0A;
    
    //TEST 2
    // uint8_t opperand01 = 0x46;
    // uint8_t opperand11 = 0x02;
    

    printf("***************************** \n");
    printf("Fetching opperands.... \n");

    printf("OP1: ");
    printBin(opperand01, 8); // expected: 1100 0000
    printf("\nOP2: ");
    printBin(opperand11, 8); // expected: 0000 0010

    // Initialize registers for Booth's multiplication
    int product = 0;
    int A = 0;                    // Accumulator
    unsigned char Q = opperand11; // Multiplier (Q)
    unsigned char M = opperand01; // Multiplicand (M)
    int Q_1 = 0;                  // Q-1 bit
    int count = 8;                // Number of bits

    if (M & 0x80)
    {                       // If negative
        M = -(twosComplement(M)); // Convert to negative value
    }
    else
    {
        M = M;
    }
    printf("\nBooth's Multiplication Algorithm Steps:\n");
    printf("A             Q       Q_1     M   Q[0]     n\n");
    printf("------------------------------------------------\n");

    // LOOP count until 0
    for (int i = 0; i < 8; i++)
    {
        int qscreen = Q & 0x01; // Current Q[0]
        count = count - 1;

        // Display current state
        printBin(A, 8);
        printf("    ");
        printBin(Q, 8);
        printf("    ");
        printf("%d", Q_1);
        printf("    ");
        printBin(M, 8);
        printf("    ");
        printf("%d", qscreen);
        printf("    ");
        printf("%d", count);
        printf("\n");

        // Booth's algorithm cases
        if ((Q & 0x01) == 1 && Q_1 == 0) // Case 10
        {
            A = A - M;
        }
        else if ((Q & 0x01) == 0 && Q_1 == 1) // Case 01
        {
            A = A + M;
        }

        // Store Q[0] in Q_1 before shift
        Q_1 = Q & 1;

        // Arithmetic right shift for A and Q
        Q = (Q >> 1) | ((A & 0x01) << 7);
        A = (A >> 1);
        if (A & 0x40)
            A |= 0x80; // Sign extension for negative numbers
    }

    printf("\nFinal Result:\n");
    printf("A: ");
    printBin(A, 8);
    printf("\nQ: ");
    printBin(Q, 8);
    printf("\nProduct in binary: ");
    product = (A << 8) | Q;
    printBin(product, 16);
    printf("\nProduct in decimal: %d\n", product);

    return 0;
}
