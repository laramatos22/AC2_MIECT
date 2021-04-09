# PARTE 1 - EXERCICIO 2

# escrever  no  porto  de  saída  o  valor  lido  do  porto  de entrada, negado (i.e., RE0 = RB0\).

	.data

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
	lui     $t1, SFR_BASE_HI     #     

	# CONFIGURAR O PORTO RE0 COMO SAÍDA   
	lw      $t2, TRISE($t1)      # READ  (Mem_addr = 0xBF880000 + 0x6100)       
	andi    $t2, $t2, 0xFFF6     # MODIFY (bit0=bit3=0 (0 means OUTPUT)) -> 0x1111 1111 1111 0110 
	sw      $t2, TRISE($t1)      # WRITE (Write TRISE register)

	# CONFIGURAR O PORTO RB0 COMO ENTRADA
	lw      $t2, TRISB($t1)      # READ  (Mem_addr = 0xBF880000 + 0x6100)       
	ori    $t2, $t2, 0x0009     # MODIFY (bit1=bit3=1 (1 means INPUT)) -> 0x0000 0000 0000 1001       
	sw      $t2, TRISB($t1)      # WRITE (Write TRISE register)

while: 
	lw      $t2, PORTB($t1)      # READ valor do porto de entrada B    
    not     $t2, $t2             # negar o valor
    andi    $t2, $t2, 0x0001
    lw      $t3, LATE($t1)       # READ (Read LATE register)       
	andi    $t3, $t3, 0xFFFE     # MODIFY (bit0 = 0)       
	or      $t2, $t2, $t3        # MODIFY (bit3 = 1) -> 0x0000 0000 0000 1000     
	sw      $t2, LATE($t1)       # WRITE (Write LATE register) 
	j while

jr $ra
