#int main(void)
#{
#	char c;
#	do
#	{
#		c = getChar();
#		if( c != '\n' )
#			putChar( c );
#		} while( c != '\n' );
#	return 0;
#}

#Mapa de registos:
#$a0: c

	.data
	.equ getChar, 2
	.equ putChar, 3
	.text
	.globl main
	
main:	li	$v0, getChar		# $v0 = getChar
	syscall			# getChar()
	
	move 	$a0, $v0		# $a0 = c = getChar();
	
if:	beq 	$a0, '0', while
	li 	$v0, putChar		# $v0 = putChar(c);
	syscall			# putChar(c);
	
while:	bne $a0, '\n', main		# while( c != '\n' );
	
	li $v0, 0			#return 0;
	jr $ra



