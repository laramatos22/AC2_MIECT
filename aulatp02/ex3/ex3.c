#include <detpic32.h>

void delay(int ms);

int main(void) 
{
    int cont1 = 0;
    int cont5 = 0;
    int cont10 = 0;

    while (1)
    {
        delay(100);

        putChar('\r');
        printInt(cont1, 0x00050002);    // printInt(cont1, 10 | 5 <<16);

        putChar(' ');
        printInt(cont5, 0x00050002);    // printInt(cont5, 10 | 5 <<16);

        putChar(' ');
        printInt(cont10, 0x00050002);   // printInt(cont10, 10 | 5 <<16);

        cont10++;

        if (cont10 % 2 == 0)
        {
            cont5++;
        }

        if (cont10 % 10 == 0)
        {
            cont1++;
        }
    }

    return 0;
    
}

void delay(int ms)
{
    for (; ms > 0; ms--)
    {
        resetCoreTimer();
        while (readCoreTimer() < 20000)
        
    }
    
}