#int main(void)
#{
#	int value;
#	while(1)
#	{
#		printStr("\nIntroduza um numero (sinal e módulo): ");
#		value = readInt10();
#		printStr("\nValor lido, em base 2: ");
#		printInt(value, 2);
#		printStr("\nValor lido, em base 16: ");
#		printInt(value, 16);
#		printStr("\nValor lido, em base 10 (unsigned): ");
#		printInt(value, 10);
#		printStr("\nValor lido, em base 10 (signed): ");
#		printInt10(value);
#	}
#	return 0;
#}

	.data
	.equ 	printStr, 8
	.equ 	printInt, 6
	.equ 	printInt10, 7
	.equ 	readInt10, 5
	
str1:	.asciiz "\nIntroduza um numero (sinal e módulo): "
str2:	.asciiz "\nValor lido, em base 2: "
str3:	.asciiz "\nValor lido, em base 16: "
str4:	.asciiz "\nValor lido, em base 10 (unsigned): "
str5:	.asciiz "\nValor lido, em base 10 (signed): "

	.text
	.globl main
	
main:

while:	li 	$v0, printStr	
	la 	$a0, str1
	syscall			# printStr("\nIntroduza um numero (sinal e módulo): ");
	
	li 	$v0, readInt10
	syscall
	move $t0, $v0		# $t0 = value = readInt10();
	
	li 	$v0, printStr
	la $a0, str2
	syscall			# printStr("\nValor lido, em base 2: ");
	
	li 	$v0, printInt
	move 	$a0, $t0
	li 	$a1, 2
	syscall			# printInt(value, 2);
	
	li	$v0, printStr
	la 	$a0, str3
	syscall			# printStr("\nValor lido, em base 16: ");
	
	li 	$v0, printInt
	move 	$a0, $t0
	li 	$a1, 16
	syscall			# printInt(value, 16);
	
	li	$v0, printStr
	la 	$a0, str4
	syscall			# printStr("\nValor lido, em base 10 (unsigned): ");
	
	li	$v0, printInt
	move 	$a0, $t0
	li 	$a1, 10
	syscall			# printInt(value, 10);
	
	li 	$v0, printStr
	la 	$a0, str5
	syscall			# printStr("\nValor lido, em base 10 (signed): ");
	
	li 	$v0, printInt10
	move 	$a0, $t0
	syscall			# printInt10(value);
	
	j 	while
	
	li 	$v0, 0
	jr 	$ra
	


