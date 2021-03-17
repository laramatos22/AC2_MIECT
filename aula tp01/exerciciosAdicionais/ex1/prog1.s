#void wait(int);
#
#int main(void)
#{
#	int s = 0;
#	int cnt = 0;
#	char c;
#	do
#	{
#		putChar('\r');	// Carriage return character
#		printInt( cnt, 10 | 3 << 16 ); // 0x0003000A: decimal w/ 3 digits
#		putChar('\t');	// Tab character
#		printInt( cnt, 2 | 8 << 16 ); // 0x00080002: binary w/ 8 bits
#		wait(5);
#		c = inkey();
#		if( c == '+' )
#			s = 0;
#		if( c == '-' )
#			s = 1;
#		if( s == 0 )
#		cnt = (cnt + 1) & 0xFF;
#		else
#			cnt = (cnt - 1) & 0xFF;
#	} while( c != 'q' );
#	return 0;
#}
#
#void wait(int ts)
#{
#	int i;
#	for( i=0; i < 515000 * ts; i++ );
#}


	.data
	
	.equ	printInt, 6
	.equ	inkey, 1
	.equ	putChar, 3
	
	.text
	.globl main

main:	li	$t0, 0		# $t0 = int cnt = 0
	li 	$t1, 0		# $t1 = int s = 0

do:	li 	$v0, putChar
	la 	$a0, '\r'
	syscall		# putChar('\r');

	li 	$v0, printInt
	li 	$t2, 3		# $t2 = 3
	sll 	$t3, $t2, 4	# $t3 = 3 << 16
	ori 	$t4, $t3, 10	# $t4 = 10 | 3 << 16
	move 	$a0, $t0	# $a0 = $t0 = cnt
	move 	$a1, $t4	# $a1 = $t4
	syscall		# printInt( cnt, 10 | 3 << 16 );
	
	li 	$v0, putChar
	la 	$a0, '\t'
	syscall		# putChar('\t');

	li 	$v0, printInt
	li 	$t2, 8		# $t2 = 8
	sll 	$t3, $t2, 4	# $t3 = 8 << 16
	ori 	$t4, $t3, 2	# $t4 = 2 | 8 << 16
	move 	$a0, $t0	# $a0 = $t0 = cnt
	move 	$a1, $t4	# $a1 = $t4
	syscall		# printInt( cnt, 2 | 8 << 16 );

	li 	$a0, 5
	jal wait		# chamada à sub-rotina wait(5);
	
	li 	$v0, inkey
	syscall		# inkey()
	move 	$t0, $v0
	
if1:	bne $t0, '+', if2	# if( c == '+' )
	li 	$t1, 0		# s = 0;
	
if2:	bne $t0, '-', if3	# if( c == '-' )
	li	$t6, 1		# s = 1;
	
if3:	bne $t0, 0, else	# if( s == 0 )
	add $t0, $t0, 1	# cnt + 1
	and $t0, $t0, 0xFF	# cnt = (cnt + 1) & 0xFF;
	
else:	sub $t0, $t0, 1
	and $t0, $t0, 0xFF	# cnt = (cnt - 1) & 0xFF;
	
while:	beq $t0, 'q', do	# while( c != 'q' );

	li	$v0, 0		# return 0;
	jr 	$ra


#--------- FUNÇÃO WAIT ----------#
	
wait:	li	$t0, 0		# i=0
	li	$t2, 515000
	mult	$a0, $t2	# 515000 * ts
	
for:	bge $t0, $t1, endfor	# i < 515000 * ts
	addi $t0, $t0, 1	# i++
	j for
	
endfor:
	jr $ra


