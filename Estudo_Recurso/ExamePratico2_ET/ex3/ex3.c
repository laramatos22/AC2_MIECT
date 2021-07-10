#include<detpic32.h>

void initUART();
void putc(char c);
void puts(char *str);

volatile DS4, DS3, DS2, DS1;    

int main(void) {
    
    initUART();

    // --------- INTERRUPT SETUP ----------- //
    U2STAbits.URXISEL = 0;              // Select interrupt only for receiver
    IPC8bits.U2IP = 1;                  // UART2 Priority
    IEC1bits.U2RXIE = 1;                // U2RX Interrupt Enable
    IFS1bits.U2RXIF = 0;                // U2RX Interrupt Flag

    // ---------- CONFIGURATION ---------- //
    TRISB = TRISB | 0x000F;             // configure RB[0..3] as inputs
    TRISEbits.TRISE4 = 0;               // configure RE4 as output
    LATEbits.LATE4 = 1;                 // usado para escrever valores num porto de saída

    EnableInterrupts();

    while (1)
    {
        DS4 = (PORTB & 0x0008) >> 3;    // 0x0000 0000 0000 1000 -> ler valor no porto de entrada RB3
        DS3 = (PORTB & 0X0004) >> 2;    // 0x0000 0000 0000 0100 -> ler valor no porto de entrada RB2
        DS2 = (PORTB & 0x0002) >> 1;    // 0x0000 0000 0000 0010 -> ler valor no porto de entrada RB1
        DS1 = (PORTB & 0x0001);         // 0x0000 0000 0000 0001 -> ler valor no porto de entrada RB0
    }
    
    return 0;

}

// O programador acede ao buffer através do registo U2RXREG

// ----- ROTINA DE SERVIÇO À INTERRUPÇAO ----- //
void _int_(32) isr_uart(void) {

    if (IFS1bits.U2RXIF == 1)
    {
        while(U2STAbits.URXDA == 0);
        char c = U2RXREG;
        putChar(c);
        if(c == 'T'){
            LATEbits.LATE4 = !LATEbits.LATE4;
        } else if (c == 'P'){
            puts("DipSwitch=");
            putc(DS4 + '0');
            putc(DS3 + '0');
            putc(DS2 + '0');
            putc(DS1 + '0');
            putc('\n');
        }
    }
    IFS1bits.U2RXIF = 0;
    
}

void putc(char c) { // -> put char 
    // wait while UTXBF 00 1
    while(U2STAbits.UTXBF == 1);

    // Copy c to the U2TXREG register
    U2TXREG = c;
}

void puts(char *str){ // -> put string
    // use putc() function to send each character ('\0' shoould not be sent)
    while(*str != '\0'){
        putc(*str);
        str++;
    }
}

// ------------ CONFIGURACAO UART ---------------- //
void initUART() {

    U2BRG = ((PBCLK + 8 * 9600) / (16 * 9600)) - 1;
    U2MODEbits.BRGH = 0;    // O relogio que determina a taxa de recação é o mesmo que é usado no módulo de transmissao
                            // e é obtido através de uma divisao por 4 ou por 16 (bit BRGH do registo U2MODE) do relogio
                            // gerado pelo modulo BRG.
                            // 1- configurar o gerador de baudrate de acordo com a taxa de transmissao / rececao
                            //    pretendida

    U2MODEbits.PDSEL = 2;   // 2- configurar os parâmetros da trama: dimensao da palavra a transmitir (nº de data bits) e tipo de paridade
    U2MODEbits.STSEL = 1;   //    e o número de stop bits

    U2STAbits.UTXEN = 1;    // ativar os módulos de 
    U2STAbits.URXEN = 1;    // transmissão e de receção
    U2MODEbits.ON = 1;      // ativar a UART

}

