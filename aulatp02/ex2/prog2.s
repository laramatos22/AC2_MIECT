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
#
#void delay(int ms)
#{
#	for(; ms > 0; ms--)
#	{
#		resetCoreTimer();
#		while(readCoreTimer() < K);
#	}
#}
#

	.equ 	READ_CORE_TIMER,11
        .equ 	RESET_CORE_TIMER,12
        .equ 	PUT_CHAR,3
        .equ 	PRINT_INT,6
        .data
        .text
        .globl main
        
main:  
	addiu 	$sp, $sp, -8
	sw 	$ra, 0($sp)
	sw	$s0, 4($sp)

	li 	$t0,0                    	#int counter = 0;

while:         
        li 	$a0, 100
        jal delay
      
        addi 	$t0,$t0,1              		# counter++
        move 	$a0, $t0
        li 	$a1,0x0004000A           	#10 | 4 << 16 = 0x0004000A
        li 	$v0, PRINT_INT           	# printInt(++counter, 10 | 4 << 16);
        syscall
        
        li 	$a0, '\r'
        li 	$v0, PUT_CHAR            	#putChar('\r'); 
        syscall
        
        j 	while

endWhile:
	lw	$ra, 0($sp)
	lw 	$s0, 4($sp)
	addiu 	$sp, $sp, 8

	li 	$v0, 0			
        jr 	$ra                      	#termina o programa
        
        
#------------------ FUNÇÃO DELAY ------------------#

#$a0: ms

delay:
for:	
	ble 	$a0, 0, endFor			# for(ms > 0)
	sub 	$a0, $a0, 1			# ms--
	li 	$v0, RESET_CORE_TIMER
	syscall					# reset_core_timer()
	
delayWhile:
	li 	$v0, READ_CORE_TIMER
	bge	$v0, 20000, endDWhile		# while(readCoreTimer() < K);
	j	delayWhile
	
endDWhile:
	j 	for
	
endFor:
	jr 	$ra
        
  
