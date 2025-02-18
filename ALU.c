// Team 5 Members:
// - Nino Angelo Balagtas
// - Mac Dylan Balagtas
#include <stdio.h>
#include <stdlib.h>

char *identifyOperation(unsigned char control_signal)
{
    switch (control_signal)
    {
    case 0x01:
        return "ADD";
    case 0x02:
        return "SUB";
    case 0x03:
        return "MUL";
    case 0x04:
        return "AND";
    case 0x05:
        return "OR";
    case 0x06:
        return "NOT";
    case 0x07:
        return "XOR";
    case 0x08:
        return "Shift Right (logical)";
    case 0x09:
        return "Shift Left (logical)";
    default:
        return "Unknown Operation";
    }
}

int validateOperation(unsigned char input)
{
    unsigned char valid_operation[] = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09};
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
    char *binary_str = (char *)malloc(data_width + 1);
    if (binary_str == NULL)
    {
        exit(1);
    }

    // loop through each bit from most significant to least significant
    for (int i = data_width - 1; i >= 0; i--)
    {
        // fill the binary string with 0 or 1
        binary_str[data_width - 1 - i] = ((data >> i) & 1) + '0';
    }

    binary_str[data_width] = '\0'; // null terminator - common in c for strings

    return binary_str;
}

// 2's complement
unsigned char twosComp(unsigned char operand)
{
    printf("2's complement operand \n");
    return (~operand) + 1;
}

// ADD operation
unsigned int addSubOperation(unsigned char operand1, unsigned char operand2)
{
    return operand1 + operand2;
}

// MUL operation
unsigned int mulOperation(unsigned char operand1, unsigned char operand2)
{
    // Initialize registers for Booth's multiplication
    int product = 0;
    int A = 0;                  // Accumulator
    unsigned char Q = operand1; // Multiplier (Q)
    unsigned char M = operand2; // Multiplicand (M)
    int Q_1 = 0;                // Q-1 bit
    int count = 8;              // Number of bits

    if (M & 0x80)
    {                       // If negative
        M = -(twosComp(M)); // Convert to negative value
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
        char *A_bin = printBin(A, 8);
        printf("%s    ", A_bin);

        char *Q_bin = printBin(Q, 8);
        printf("%s    ", Q_bin);

        printf("%d    ", Q_1);

        char *M_bin = printBin(M, 8);
        printf("%s    ", M_bin);

        printf("%d    ", qscreen);
        printf("%d\n", count);

        // Free allocated memory
        free(A_bin);
        free(Q_bin);
        free(M_bin);

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
    }
    product = (A << 8) | Q;
    return product;
}

unsigned char bitwiseAND(unsigned char operand1, unsigned char operand2)
{
    return operand1 & operand2;
}

unsigned char bitwiseOR(unsigned char operand1, unsigned char operand2)
{
    return operand1 | operand2;
}

unsigned char bitwiseNOT(unsigned char operand1, unsigned char operand2)
{
    return (~operand1) & (~operand2);
}

unsigned char bitwiseXOR(unsigned char operand1, unsigned char operand2)
{
    return operand1 ^ operand2;
}

int ALU(unsigned char operand1, unsigned char operand2, unsigned char control_signal)
{
    static unsigned int ACC;
    unsigned char temp_OP1 = operand1, temp_OP2 = operand2;

    ACC = 0x0000;

    switch (control_signal)
    {
    case 0x01: // ADD
        ACC = addSubOperation(temp_OP1, temp_OP2);
        break;
    case 0x02: // SUB
        temp_OP2 = twosComp(operand2);

        ACC = addSubOperation(temp_OP1, temp_OP2);
        break;
    case 0x03: // MUL
        ACC = mulOperation(operand1, operand2);
        break;
    case 0x04: // AND
        ACC = bitwiseAND(operand1, operand2);
        return ACC;
    case 0x05: // OR
        ACC = bitwiseOR(operand1, operand2);
        return ACC;
    case 0x06: // NOT
        ACC = bitwiseNOT(operand1, operand2);
        return ACC;
    case 0x07: // XOR
        ACC = bitwiseXOR(operand1, operand2);
        return ACC;
    case 0x08: // Shift Right (logical)
        return ACC;
    case 0x09: // Shift Left (logical)
        return ACC;
    default:
        printf("Invalid operation\n");
        break;
    }

    return ACC;
}

/* This function sets the flags after the arithmetic or logical operation */
void setFlags(unsigned int ACC)
{
    int SF = 0, CF = 0, ZF = 0, OF = 0; // sf - sign flag, cf - carry flag, of - overflow flag, zf - zero flag

    if (ACC == 0x0000) // check if value of ACC is equal to 0
    {
        ZF = 1;
    }
    if ((ACC & 0x0080) == 0x0080) // check if sign (8th bit) of ACC is 1 (negative)
    {
        SF = 1;
    }
    if (ACC > 0x7F || ACC < 0x80)
    {
        OF = 1;
    }
    if (ACC > 0xFF)
    {
        CF = 1;
    }

    printf("ZF=%d, CF=%d, SF=%d, OF=%d\n", ZF, CF, SF, OF);
}

void main()
{
    unsigned char operand1, operand2, control_signal;
    unsigned char ACC;

    printf("Enter OP1 in hexadecimal (e.g. 0x05): ");
    scanf("%hhx", &operand1);

    printf("Enter OP2 in hexadecimal (e.g. 0x03): ");
    scanf("%hhx", &operand2);

    printf("Enter control signal in hexadecimal\n ");
    printf("0x01 - ADD\n 0x02 - SUB\n 0x03 - MUL\n 0x04 - AND\n 0x05 - OR\n 0x06 - NOT\n 0x07 - XOR\n 0x08 - Shift Right\n 0x09 - Shift Left \n: ");
    scanf("%hhx", &control_signal);

    // identify and validate operation
    int op_signal = validateOperation(control_signal);

    if (op_signal == 0)
    {
        printf("Invalid operation!\n");
        exit(1);
    }

    printf("\n************************************** \n");
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

    char *acc_binary = printBin(ACC, sizeof(ACC) * 16);
    printf("ACC = %s \n", acc_binary);

    // set the flags
    setFlags(ACC);
}