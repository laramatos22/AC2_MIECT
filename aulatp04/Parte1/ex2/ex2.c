#include <detpic32.h>

// um contador crescente, atualizado a uma frequência de 4Hz.
// O resultado deverá ser observando nos 4 LEDs já montados na sua placa e ligados aos
// portos RE0 a RE3.

void delay(int ms)
{
    for(; ms > 0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

int main(void)
{
    LATE = (LATE & 0xFFF0);     
    TRISE =(TRISE & 0xFFF0);    // outputs configurados de TRISE.RE3 A TRISE.RE0

    int count = 0;

    while(1)
    {
        delay(250);             // frequencia de 4Hz
        LATE = (LATE & 0xFFF0)|count;
        count++;
        
    }
    return 0;
}