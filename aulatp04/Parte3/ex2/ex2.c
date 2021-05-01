#include<detpic32.h>

// Parte 3 - ex2. A frequência de incremento deverá ser 1 Hz e os
// valores devem ser mostrados em decimal. A conversão para decimal pode ser feita, de
// forma simplificada e desde que o valor de entrada seja representável em decimal com dois
// dígitos (00-99), pela seguinte função:

unsigned char toBcd(unsigned char value)
{
    return ((value / 10) << 4) + (value % 10);
}

// função que envie um byte (8 bits)
// ou seja dois algarismos hexadecimais para os dois displays, fazendo corresponder os 4 bits
// menos significativos ao display low e os 4 bits mais significativos ao display high.

void delay(unsigned int ms)
{
    for (; ms < 0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

//3.a) Reescreva a função send2displays() de modo a que, sempre que for invocada,
// envie, de forma alternada, apenas um dos dois dígitos para o sistema de visualização.
// Isto é, em chamadas sucessivas à função, o comportamento deverá ser: enviar
// "dígit_low", enviar "dígit_high", enviar "digit_low", enviar "dígit_high", ...

void send2displays(unsigned char value)
{
    static const char display7Scodes[] = {0x37,     //0
                                         0x06,      //1
                                         0x5B,      //2
                                         0x4F,      //3
                                         0x66,      //4
                                         0x6D,      //5
                                         0x7D,      //6
                                         0x07,      //7
                                         0x7F,      //8
                                         0x6F,      //9
                                         0x77,      //A
                                         0x7C,      //B
                                         0x39,      //C
                                         0x5E,      //D
                                         0x79,      //E
                                         0x71};     //F

    static char displayFlag = 0;    // static variable: doesn't loose its
                                    // value between calls to function

    // send digit_high (dh) to display_high: dh = value >> 4
    unsigned char digit_high = value >> 4;                          // Get the index of the decimal part
    // send digit_low (dl) to display_low: dl = value & 0x0F
    unsigned char digit_low = value & 0x0F;                         // Get the index of the unitary part

    // Get the correct hexadecimal code for the number
    digit_high = display7Scodes[digit_high];
    digit_low = display7Scodes[digit_low];

    // if "displayFlag" is 0 then send digit_low to display_low
    if (displayFlag == 0)
    {
        LATD = (LATD | 0x0040) & 0xFFDF;                                // Displya High active and Display Low inactive
        LATB = (LATB & 0x80FF) | (unsigned int)(digit_low) << 8;        // Clean display and set the right value
    }
    // else send digit_high to didplay_high
    else
    {
        LATD = (LATD | 0x0020) & 0xFFBF;                                // Displya High inactive and Display Low active
        LATB = (LATB & 0x80FF) | (unsigned int)(digit_low) << 8;        // Clean display and set the right value
    }      
    // toggle "displayFlag" variable
    displayFlag = !displayFlag;

}

int main (void)
{
    TRISB = TRISB & 0x80FF;             // Configure RB14 to RB8 as output
    TRISD = TRISD & 0xFF9F;             // Configure RD6 and RD5 as output -> for digit_high and digit_low

    unsigned char count = 0;
    int i=0;

    // 2. O contador deve ser incrementado com uma frequência de 5 Hz e o seu valor deve ser enviado, ao mesmo
    // ritmo, para os displays através da função send2displays().
    // 3.b) invocar a função send2displays() com uma frequência de 20 Hz (i.e., a cada 50 ms),
    // continuando a usar a função delay() para determinar as frequências de
    // refrescamento (20 Hz) e de contagem (5 Hz).
    while (1)
    {
        i++;          
        send2displays(toBcd(count));
        delay(10);                      // delay(10 ms) -> 100Hz
        if (i % 100 == 0)               // update count at 1 Hz = 100 ms
        {   
            if (++count == 60)          // contador em módulo 60
            {
                count = 0;
            }
        }     
    }
    return 0;

}

/**
int main(void)
{
    // declare variables
    // initialize ports
    counter = 0;
    while(1)
    {
        i = 0;
        do
        {
            send2displays( counter );
            // wait 50 ms
        } while(++i < 4);
        // increment counter (mod 256)
    }
    return 0;
}*/

// ----------- OU -----------------

/**
int main(void)
{
    unsigned int i;
    ...
    counter = 0;
    i = 0;
    while(1)
    {
        i++;
        send2displays(counter);
        // wait 50 ms
        if(i % 4 == 0)
            // increment counter
    }
    return 0;
}*/