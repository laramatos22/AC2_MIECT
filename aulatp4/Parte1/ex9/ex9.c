#include <detpic32.h>

int main(void)
{
    //int display7Scodes[] = {0x37, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};
      int display7Scodes[] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x77, 0x7C, 0x39, 0x5E, 0x79, 0x71};

    TRISB = TRISB & 0x000F;        // configure RB0 to RB3 as inputs
    TRISB = TRISB & 0x80FF;        // configure RB8 to RB14 and RD5 to RD6 as outputs 
    TRISD = TRISD & 0xFF9F;

    LATDbits.LATD5 = 0;            // Select display low
    LATDbits.LATD6 = 1;		       // Disconnect Display High

    int index;
    int code;

    while(1)
    {
        index = PORTB & 0x000F;    // read dip-switch
        code = display7Scodes[index];   // convert to 7 segments code
        
        /**O envio do código para os displays passa por limpar os bits da LATB que 
        estão ligados aos segmentos (RB14 a RB8) e pegar no code, fazer 8 shifts à 
        esquerda para ficar a apontar para esses bits e fazer o Or.*/
        LATB = (LATB & 0x80FF) | (code << 8);          // send to display

    }

    return 0;

}
