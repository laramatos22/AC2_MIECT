#include<detpic32.h>

void config();
void setPWM(int dutycycle);
void delay(int micros); //micros = microsegundos

int main(void) {

    config();

    /**
     * Para deixar de haver dúvidas:
     * ->Aula prática 4:
     *      TRISE = (TRISE & 0xFFF0) | 0x0001;  // RE3 a RE1 configurado com saídas (&)
     *                                          // RE0 configurada como entrada (|)
     */

    TRISB = TRISB | 0x0009; //0x0000 0000 0000 1111 -> set RB3 a RB0 as input

    while (1)
    {
        int dipVal = PORTB & 0xFFF0;    //0x1111 1111 1111 0000
        if (dipVal == 1)
        {
            //dutycyle = 25%
            setPWM(25);  
        } else if (dipVal == 8)
        {
            //dutycycle = 70%
            setPWM(70);
        }
        
        // T = 250 microsegundos = 250x10*-6 seg
        // f = 1/T = 1/(250*10⁻6) = 4000 Hz = 4MHz
        delay(250); //delay de 250 microsegundos
        
    }

    return 0;     

}

void config() {

    T2CONbits.TCKPS = 2;    //1:2 prescaler (i.e. fout_presc = 5MHz)
    //Kprescaler = ceil(fPBCLK / ((65535 + 1) * fout))
    //fout = (fPBCLK/Kprescaler) / (PR2 + 1) = 20MHz / (4 * (17856 + 1)) = 280 Hz
    //PR2 = (fout_presc/fout) - 1 = (5MHz / 280Hz) - 1 = 17856
    PR2 = 17856;
    TMR2 = 0;               //Reset timer T2 count register
    T2CONbits.TON = 1;     //Enable timer T2

    IPC2bits.T2IP = 2;      // Interrupt priority (must be in range [1..6])
    IEC0bits.T2IE = 1;      // Enable timer T2 interrupts
    IFS0bits.T2IF = 0;      // Reset timer T2 interrupt flag

}

void setPWM(int dutycycle) {

    OC2CONbits.OCM = 6;                             //PWM mode on OC2; fault pin disabled
    OC2CONbits.OCTSEL =0;                           //Use timer T2 as the time base for PWM generation
    OC2RS = ((PR2 + 1) * dutycycle) / 100;          //Ton constant
    OC2CONbits.ON = 1;                              //Enable OC1 module

}

void delay(int micros){
    for(;micros > 0; micros--){
        resetCoreTimer();
        while(readCoreTimer() < 5000);
    }
}

