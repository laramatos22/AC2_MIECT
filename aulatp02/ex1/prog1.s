#int main(void)
#{
#	int counter = 0;
#	while(1)
#	{
#		resetCoreTimer();
#		while(readCoreTimer() < 10000000);
#		printInt(++counter, 10 | 4 << 16);	// Ver nota 1
#		putChar('\r');				// cursor regressa ao início da linha
#		}
#	return 0;
#}

	.equ 	READ_CORE_TIMER,11
        .equ 	RESET_CORE_TIMER,12
        .equ 	PUT_CHAR,3
        .equ 	PRINT_INT,6
        .data
        .text
        .globl main
        
main:  
	li $t0,0                    	#int counter = 0;

while:  
	li $v0, RESET_CORE_TIMER    	#resetCoreTimer(); 
        syscall
        
        li $v0,READ_CORE_TIMER      	# while (1) {
        syscall    
                         #
        blt $v0,10000000,while       	#while(readCoreTimer() < 10000000);
       
        addi $t0,$t0,1              	# counter++
        move $a0, $t0
        li $a1,0x0004000A           	#10 | 4 << 16 = 0x0004000A
        li $v0, PRINT_INT           	# printInt(++counter, 10 | 4 << 16);
        syscall
        
        li $a0, '\r'
        li $v0, PUT_CHAR            	#putChar('\r'); 
        syscall
        
        j while
	
	li $v0, 0			
        jr $ra                      	#termina o programa		
	

