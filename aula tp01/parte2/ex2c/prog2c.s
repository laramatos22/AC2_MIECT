#int main(void)
#{
#	char c;
#	do
#	{
#		while( (c = inkey()) == 0 );
#		if( c != '\n' )
#			putChar( c );
#	} while( c != '\n' );
#	return 0;
#}

	.data
	.equ	inkey, 1
	.equ	putChar, 3
	.text
	.globl main
	
main:

do:	li 	$v0, inkey
	syscall
	beqz	$v0, do
	
if:	move 	$a0, $v0
	beq 	$a0, '\n', while
	
	li 	$v0, putChar
	syscall
	
while: 	bne $a0, '\n', do
	
	li $v0, 0
	jr $ra
	

