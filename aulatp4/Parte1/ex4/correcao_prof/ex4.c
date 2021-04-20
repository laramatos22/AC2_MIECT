#include <detpic32.h>

void delay(int ms)
{
    for(; ms > 0; ms--)
    {
        resetCoreTimer();
        while(readCoreTimer() < 20000);
    }
}

void sendSegment(char letra) 
{
    LATB = LATB & 0x00FF;

    switch (letra)
        {
        //o segmento A está ligado ao porto RB8.
        case 'a':
            LATBbits.LATB8 = 1; 
            break;

        case 'b':
            LATBbits.LATB9 = 1; 
            break;

        case 'c':
            LATBbits.LATB10 = 1; 
            break;
        
        case 'd':
            LATBbits.LATB11 = 1; 
            break;
        
        case 'e':
            LATBbits.LATB12 = 1; 
            break;
        
        case 'f':
            LATBbits.LATB13 = 1; 
            break;
        
        case 'g':
            LATBbits.LATB14 = 1; 
            break;
        
        default:
            break;
    }
}

int main(void)
{
    unsigned char segment;
    int i;

    LATDbits.LATD6 = 1; // display high active
    LATDbits.LATD5 = 0; // display low inactive
    
    LATB = LATB & 0x00FF;                       
    TRISB = TRISB & 0x00FF; // configure RB8-RB14 as outputs
    TRISD = TRISD & 0xFF9F; // configure RD5-RD6 as outputs

    while(1)
    {
        LATDbits.LATD6 = !LATDbits.LATD6; //
        LATDbits.LATD5 = !LATDbits.LATD5; // toggle display selection
        segment = 'a';   //segment = 1;

        for(i=0; i < 7; i++)
        {
            sendSegment(segment);   // send "segment" value to display
            delay(500);             // wait 0.5 second
            segment++; //segment = segment << 1;
        }
    }
    return 0;
}
    
