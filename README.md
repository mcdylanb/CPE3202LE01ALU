# CPE3202LE01ALU


Sample function to Print Binary
```c
void printBinary(unsigned char num) {
    for(int i = 7; i >= 0; i--) {
        printf("%d", (num >> i) & 1);
    }
    printf("\n");
}

printBinary(0x85);
```

### Arithmetic Notes
- C can comprehend hexcode base on `0x<num><num>` 
- and can do addition
- sample code 
```c
#include <stdio.h>

int main(void)
{
    unsigned char opperand0 = 0x88;
    unsigned char opperand1 = 0x85;
    unsigned char result = opperand0 + opperand1;
    printf("0x%02X \n",result);
}
```

- how to initialize hexcode 
```c
unsigned char opperand0 = 0x88;
```
- how to call hexcode
```c
    printf("0x%02X \n",result);
```