#include <stdio.h>
#include <stdlib.h>

char *identifyOperation(unsigned char control_signal)
{
    if (control_signal == 0x01)
    {
        return "ADD";
    }
    else if (control_signal == 0x02)
    {
        return "SUB";
    }
}

int validateOperation(unsigned char input)
{
    unsigned char valid_operation[] = {0x01, 0x02}; // add, sub
    int size = sizeof(valid_operation) / sizeof(valid_operation[0]);

    for (int i = 0; i < size; i++)
    {
        if (input == valid_operation[i])
        {
            return 1;
        }
    }

    return 0;
}

unsigned char *printBin(unsigned char data, unsigned char data_width)
{
    // Allocate memory for the binary string (data_width + 1 for the null terminator)
    char *binary_str = (char *)malloc(data_width + 1); // +1 for the null terminator
    if (binary_str == NULL)
    {
        printf("Memory allocation failed!\n");
        exit(1); // Exit if memory allocation fails
    }

    // Loop through each bit from most significant to least significant
    for (int i = data_width - 1; i >= 0; i--)
    {
        // Fill the binary string with '0' or '1'
        binary_str[data_width - 1 - i] = ((data >> i) & 1) + '0';
    }

    binary_str[data_width] = '\0'; // Null-terminate the string

    return binary_str; // Return the binary string
}

unsigned char twosComp(unsigned char operand2)
{
    printf("2's complement OP2 \n");
    return (~operand2) + 1;
}

/* This function represents the ALU */
int ALU(unsigned char operand1, unsigned char operand2, unsigned char control_signal)
{
    static unsigned int ACC;
    unsigned char temp_OP1 = 0x00, temp_OP2 = 0x00;

    /* Setting ACC and flags to initial values */
    ACC = 0x0000;
    int SF = 0, CF = 0, ZF = 0, OF = 0;

    if (control_signal == 0x01 || control_signal == 0x02) // ADD or SUB
    {
        /* Sign and Operation Check Logic */
        temp_OP1 = operand1;
        if (control_signal == 0x02)        // check if operation is SUB
            temp_OP2 = twosComp(operand2); // 2’s complement operand2
        else
            temp_OP2 = operand2;

        /* 8-bit Adder */
        printf("Adding OP1 & OP2... \n");
        ACC = temp_OP1 + temp_OP2;
    }
    else if (control_signal == 0x02) // MUL
    {
    }
    // setFlags(ACC); // set the flags

    return ACC;
}

/* This function sets the flags after the arithmetic or logical operation */
// void setFlags(unsigned int ACC)
// {
//     if (ACC == 0x0000) // check if value of ACC is equal to 0
//     {
//         int ZF = 1;
//     }
//     if ((ACC & 0x0080) == 0x0080) // check if sign (8th bit) of ACC is 1 (negative)
//     {
//     }
// }

void main()
{
    unsigned char operand1, operand2, control_signal;
    unsigned char ACC;

    printf("************************************** \n");
    printf("Enter OP1: ");
    scanf("%hhx", &operand1);

    printf("Enter OP2: ");
    scanf("%hhx", &operand2);

    printf("Enter control signal: ");
    scanf("%hhx", &control_signal);

    // identify and validate operation
    int op_signal = validateOperation(control_signal);

    if (op_signal == 0)
    {
        printf("Invalid operation!\n");
        exit(1);
    }

    printf("Fetching operands... \n");
    // convert binary op1
    char *operand1_binary = printBin(operand1, sizeof(operand1) * 8);
    printf("OP1 = %s \n", operand1_binary);

    // convert binary op2
    char *operand2_binary = printBin(operand2, sizeof(operand2) * 8);
    printf("OP2 = %s \n", operand2_binary);

    char *opsign = identifyOperation(control_signal);
    printf("Operation = %s \n", opsign);

    // ALU calculation
    ACC = ALU(operand1, operand2, control_signal);
    // printf("ACC = 0x%02X \n", ACC);

    char *acc_binary = printBin(ACC, sizeof(ACC) * 16); // 16-bit magnitude
    printf("ACC = %s \n", acc_binary);
    printf("************************************** \n");
}