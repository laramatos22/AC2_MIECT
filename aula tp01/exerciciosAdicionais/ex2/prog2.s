#define STR_MAX_SIZE 20
#
#char *strcat(char *, char *);
#char *strcpy(char *, char *);
#int strlen(char *);
#
#int main(void)
#{
#	static char str1[STR_MAX_SIZE + 1];
#	static char str2[STR_MAX_SIZE + 1];
#	static char str3[2 * STR_MAX_SIZE + 1];
#	
#	printStr("Introduza 2 strings: ");
#	readStr( str1, STR_MAX_SIZE );
#	readStr( str2, STR_MAX_SIZE );
#	printStr("Resultados:\n");
#	prinInt( strlen(str1), 10 );
#	prinInt( strlen(str2), 10 );
#	strcpy(str3, str1);
#	printStr( strcat(str3, str2) );
#	printInt10( strcmp(str1, str2) );
#	return 0;
#}
#
#Cálculo da dimensão de uma string.
#int strlen(char *s)
#{
#	int len;
#	for( len = 0; *s != '\0'; len++, s++ );
#	return len;
#}
#
#Cópia de uma string.
#char *strcpy(char *dst, char *src)
#{
#	char *p = dst;
#	for( ; ( *dst = *src ) != '\0'; dst++, src++ );
#	return p;
#}
#
#Concatenação de 2 strings.
#char *strcat(char *dst, char *src)
#{
#	char *p = dst;
#	for( ; *dst != '\0'; dst++ );
#	strcpy( dst, src );
#	return p;
#}
#
#Comparação alfabética de 2 strings.
#// Returned value is:
#// < 0 string "s1" is "less than" string "s2"
#// = 0 string "s1" is equal to string "s2"
#// > 0 string "s1" is "greater than" string "s2"
#int strcmp(char *s1, char *s2)
#{
#	for( ; (*s1 == *s2) && (*s1 != '\0'); s1++, s2++ );
#	return( *s1 - *s2 );
#}


	.data
	
	.equ	printInt, 7
	.equ	printStr, 8
	.equ	readStr, 9
	.equ 	printInt10, 7
	.equ 	STR_MAX_SIZE, 20

msg1:	.asciiz "Introduzir Primeira String: "
msg2:	.asciiz "Introduzir Segunda String: "
msg3:	.asciiz "Resultados:\n"
msg4:	.asciiz "Length First String: "
msg5:	.asciiz "Length Second String: "

str1:	.space 21
str2:	.space 21
str3:	.space 41

	.text
	.globl main
	
main:
	addiu $sp, $sp, -4		# Reserva espaço na stack
	sw 	$ra, 0($sp)		# salvaguarda o valor de $ra
	
	li 	$v0, printStr
	la 	$a0, msg1
	syscall			# printStr("Introduzir Primeira String: ");
	
	li 	$v0, readStr
	la 	$a0, str1
	ori $a1, $0, 20
	syscall			# readStr( str1, STR_MAX_SIZE );
	
	li 	$v0, printStr
	la 	$a0, msg2
	syscall			# printStr("Introduzir Segunda String: ");
	
	li 	$v0, readStr
	la 	$a0, str2
	ori $a1, $0, 20
	syscall			# readStr( str2, STR_MAX_SIZE );
	
	li 	$v0, printStr
	la 	$a0, msg3
	syscall			# printStr("Resultados:\n");
	
	li 	$v0, printStr
	la 	$a0, msg4		# printStr("Length First String: ");
	
	la 	$a0, str1
	jal	strlen			# strlen( str1 )
	
	move 	$a0, $v0
	li 	$a1, 10
	li 	$v0, printInt
	syscall			# prinInt( strlen(str1), 10 );
	
	li 	$v0, printStr
	la 	$a0, msg5
	syscall			# printStr("Length Second String: ");
	
	la 	$a0, str2
	jal 	strlen			# strlen ( str2 )
	
	move 	$a0, $v0
	li	$a1, 10
	li 	$v0, printInt		# prinInt( strlen(str2), 10 );
	
	la 	$a0, str3
	la	$a1, str1
	jal 	strcpy			# strcpy(str3, str1);
	
	la 	$a0, str3
	la 	$a1, str2
	jal	strcat			# strcat(str3, str2)
	
	move 	$a0, $v0
	li	$v0, printStr
	syscall			# printStr( strcat(str3, str2) );
	
	la	$a0, str1
	la	$a1, str2
	jal	strcmp			# strcmp(str1, str2)
	
	move	$a0, $v0
	li	$v0, printInt10
	syscall			# printInt10( strcmp(str1, str2) );
	
	lw 	$ra, 0($sp)
	addiu $sp, $sp, 4
	
	li	$v0, 0			# return 0;
	jr	$ra

	
# ------------ FUNÇÃO STRLEN ------------- #

strlen: 
	li 	$v0, 0			# int len = 0;
	
lenFor:
	lb	$t0, 0($a0)		# criação do ponteiro s*
	
	beq $t0, 0, endLenFor		# *s != '\0'
	addi $a0, $a0, 1		# dst++
	addi $v0, $v0, 1		# src++
	j lenFor
	
endLenFor:
	jr 	$ra


# ----------- FUNÇÃO STRCPY ---------- #

strcpy:
	ori $t0, $0, 0			# $t0 = p
	move  	$a0, $v0
	
doCpy:
	lb $t1, 0($a1)			# *p = dst;
	lb $t1, 0($a0)			# *dst = *src
	addiu $a0, $a0, 1		# dst++
	addiu $a1, $a1, 1		# src++
	addiu $t0, $t0, 1
	
	beq $t0, '\0', doCpy
	
	jr 	$ra	
	

# ---------- FUNÇÃO STRCAT ----------- #

strcat:
	addiu $sp, $sp, -8
	sw	$ra, 0($sp)
	sw 	$s0, 4($sp)
	
catFor:
	lw	$s0, 0($s0)		# *p = dst;
	
	beq $s0, '\0', catEndFor	# *dst != '\0'
	addi $s0, $a0, 1		# dst++
	j catFor
	
catEndFor:
	jal strcpy			# strcpy( dst, src );
	
	lw 	$ra, 0($sp)
	lw 	$s0, 4($sp)
	addiu $sp, $sp, 8
	jr 	$ra


# ---------- FUNÇÃO STRCMP ---------- #

strcmp:

cmpFor:
	lb	$t0, 0($a0)
	lb	$t1, 0($a1)
	bne $t0, $t1, endCmpFor	# (*s1 == *s2)
	beq $t0, '\0', endCmpFor	# (*s1 != '\0')
	addiu $a0, $a0, 1		# s1++
	addiu $a1, $a1, 1		# s2++
	j	cmpFor
	
endCmpFor:
	sub $v0, $t0, $t1		# return( *s1 - *s2 );
	jr	$ra


