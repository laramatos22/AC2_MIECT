#programa de modo a que quando por premida a tecla ' A ', a frequência de
#incremento dos contadores passe para o dobro.
#
#Premindo a tecla' N ', a frequência de incremento dos contadores deve voltar ao valor normal. 
#Para a leitura do carater utilize o system call inkey() .

# Mapa de registos:
# $t0: cnt1
# $t1: cnt5
# $t2: cnt10
# $t3: temp
# $t4: key
# $t5: delay

	.data
	
	.equ 	READ_CORE_TIMER, 11
	.equ 	RESET_CORE_TIMER, 12
	.equ 	PUT_CHAR, 3
	.equ 	PRINT_INT, 6
	.equ 	INKEY, 1
	
	.text
	.globl main
	
main:
	li 	$t0, 0			# cnt1 = 0
	li 	$t1, 0			# cnt5 = 0
	li	$t2, 0			# cnt10 = 0
	li 	$t5, 100		# delay = 100
	
while:
	move 	$a0, $t5
	jal 	delay			# delay(100)
	
	li	$v0, INKEY
	syscall			# inkey() -> leitura do caracter
	
	move 	$t4, $v0
	
	bne	$t4, 'A', clickA	# key == 'A' vai para clickA
	div	$t5, $t5, 2		# senão: delay/2
	
clickA:
	bne	$t4, 'N', clickN	# se key == 'N'
	mul	$t5, $t5, 2		# volta ao nomral / como estava antes
	
clickN:
	rem	$t3, $t2, 2
	bnez	$t3, click1
	addiu	$t1, $t1, 1
	
click1:
	rem	$t3, $t2, 10
	bnez	$t3, click2
	addiu	$t0, $t0, 1
	
click2:
	li	$a1, 0x0004000A
	
	#print 1Hz
	move	$a0, $t0
	li	$v0, PRINT_INT
	syscall
	
	# print tab
	li	$a0, '\t'
	li	$v0, PUT_CHAR
	syscall
	
	#print 5Hz
	move	$a0, $t1
	li	$v0, PRINT_INT
	syscall
	
	# print tab
	li	$a0, '\t'
	li	$v0, PUT_CHAR
	syscall
	
	#print 10Hz
	move	$a0, $t2
	li	$v0, PRINT_INT
	syscall

	li	$a0, '\r'
	li	$v0, PUT_CHAR
	syscall
	
	addiu 	$t2, $t2, 1
	
	j while
	
endWhile:
	jr	$ra

#------------------ FUNÇÃO DELAY ------------------#

#$a0: ms

delay:
for:	
	ble 	$a0, 0, endFor			# for(ms > 0)
	sub 	$a0, $a0, 1			# ms--
	li 	$v0, RESET_CORE_TIMER
	syscall				# reset_core_timer()
	
delayWhile:
	li 	$v0, READ_CORE_TIMER
	bge	$v0, 20000, endDWhile		# while(readCoreTimer() < K);
	j	delayWhile
	
endDWhile:
	j 	for
	
endFor:
	jr 	$ra
        

