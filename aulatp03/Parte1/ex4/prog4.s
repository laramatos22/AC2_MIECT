# PARTE 1 - EXERCICIO 4

# void main(void) {      
#    int v = 0;      
#    
#    TRISE0 = 0;    // Configura o porto RE0 como saída      
#    while(1)      
#    {         
#        LATE0 = v;  // Escreve v no bit 0 do porto E         
#        delay(500); // Atraso de 500ms         
#        v ^= 1;     // complementa o bit 0 de v (v = v xor 1)      
#    } 
# }

	.data

    .equ READ_CORE_TIMER, 11
    .equ RESET_CORE_TIMER, 12

	.equ SFR_BASE_HI, 0xBF88      # 16 MSbits of SFR area       
	.equ TRISE, 0x6100            # TRISE address is 0xBF886100      
	.equ PORTE, 0x6110            # PORTE address is 0xBF886110      
	.equ LATE,  0x6120            # LATE  address is 0xBF886120

	.equ TRISB, 0x6040            # TRISB address is 0xBF886040      
	.equ PORTB, 0x6050            # PORTB address is 0xBF886050      
	.equ LATB,  0x6060            # LATB  address is 0xBF886060

	.text
	.globl main

main:
    addiu   $sp, $sp, -16        # reserva espaço na stack
    sw      $ra, 0($sp)          # salvaguarda os valores dos registos
    sw      $s0, 4($sp)
    sw      $s1, 8($sp)
    sw      $s2, 12($sp)

    li      $s0, 0               # int v = 0;

	lui     $s1, SFR_BASE_HI     #     

	# CONFIGURAR O PORTO RE0 COMO SAÍDA   
	lw      $s2, TRISE($s1)      # READ  (Mem_addr = 0xBF880000 + 0x6100)       
	andi    $s2, $s2, 0xFFF6     # MODIFY (bit0=bit3=0 (0 means OUTPUT)) -> 0x1111 1111 1111 0110 
	sw      $s2, TRISE($s1)      # WRITE (Write TRISE register)

	
while:      
	lw      $s2, LATE($s1)       # READ (Read LATE register)       
	andi    $s2, $s2, 0xFFFE     # MODIFY (bit0 = 0)       
	or     	$s2, $s2, $s0    
	sw      $s2, LATE($s1)       # WRITE (Write LATE register) 

 #   li      $a0, 500
 #   li      $a0, 20
    li      $a0, 10
    jal delay

    xor     $s0, $s0, 0x0001 

	j while

    lw      $ra, 0($sp)			# Devolve os valores aos registos
    lw      $s0, 4($sp)
    lw      $s1, 8($sp)
    lw      $s2, 12($sp)
	addiu 	$sp, $sp, 16		# Liberta espaço da stack

    jr $ra

#------------------ FUNÇÃO DELAY ------------------#

#$a0: ms

delay:
for:	
	ble     $a0, 0, endFor			# for(ms > 0)
	sub 	$a0, $a0, 1			    # ms--
	li 	$v0, RESET_CORE_TIMER
	syscall				            # reset_core_timer()
	
delayWhile:
	li 	$v0, READ_CORE_TIMER
	syscall
	bge	$v0, 20000, endDWhile		# while(readCoreTimer() < K);
	j	delayWhile
	
endDWhile:
	j 	for
	
endFor:
	jr 	$ra

