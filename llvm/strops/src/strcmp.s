	.text
	.file	"strcmp.ll"
	.globl	strlen
	.align	16, 0x90
	.type	strlen,@function
strlen:                                 # @strlen
	.cfi_startproc
# BB#0:
	movl	$0, -4(%rsp)
	jmp	.LBB0_1
	.align	16, 0x90
.LBB0_2:                                # %strlen.body
                                        #   in Loop: Header=BB0_1 Depth=1
	incl	-4(%rsp)
.LBB0_1:                                # %strlen.condit
                                        # =>This Inner Loop Header: Depth=1
	movslq	-4(%rsp), %rax
	cmpb	$0, (%rdi,%rax)
	jne	.LBB0_2
# BB#3:                                 # %strlen.end
	movl	-4(%rsp), %eax
	retq
.Ltmp0:
	.size	strlen, .Ltmp0-strlen
	.cfi_endproc

	.globl	strcmp
	.align	16, 0x90
	.type	strcmp,@function
strcmp:                                 # @strcmp
	.cfi_startproc
# BB#0:
	pushq	%rbp
.Ltmp1:
	.cfi_def_cfa_offset 16
	pushq	%r14
.Ltmp2:
	.cfi_def_cfa_offset 24
	pushq	%rbx
.Ltmp3:
	.cfi_def_cfa_offset 32
	subq	$16, %rsp
.Ltmp4:
	.cfi_def_cfa_offset 48
.Ltmp5:
	.cfi_offset %rbx, -32
.Ltmp6:
	.cfi_offset %r14, -24
.Ltmp7:
	.cfi_offset %rbp, -16
	movq	%rsi, %r14
	movq	%rdi, %rbx
	movq	%rdx, %rdi
	callq	puts
	movq	%rbx, %rdi
	callq	strlen
	movl	%eax, %ebp
	movq	%r14, %rdi
	callq	strlen
	movl	$0, 12(%rsp)
	cmpl	%eax, %ebp
	jne	.LBB1_4
	.align	16, 0x90
.LBB1_1:                                # %strcmp.body
                                        # =>This Inner Loop Header: Depth=1
	movslq	12(%rsp), %rax
	movzbl	(%r14,%rax), %ecx
	movzbl	(%rbx,%rax), %eax
	cmpl	%ecx, %eax
	jne	.LBB1_4
# BB#2:                                 # %strcmp.increment
                                        #   in Loop: Header=BB1_1 Depth=1
	movl	12(%rsp), %eax
	incl	%eax
	movl	%eax, 12(%rsp)
	cmpl	%ebp, %eax
	jle	.LBB1_1
# BB#3:                                 # %strcmp.pass
	movl	$eq_str, %edi
	callq	puts
	movb	$1, %al
	jmp	.LBB1_5
.LBB1_4:                                # %strcmp.fail
	movl	$neq_str, %edi
	callq	puts
	xorl	%eax, %eax
.LBB1_5:                                # %strcmp.fail
	addq	$16, %rsp
	popq	%rbx
	popq	%r14
	popq	%rbp
	retq
.Ltmp8:
	.size	strcmp, .Ltmp8-strcmp
	.cfi_endproc

	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:
	pushq	%rax
.Ltmp9:
	.cfi_def_cfa_offset 16
	movl	$string_a, %edi
	movl	$string_b, %esi
	movl	$a_cmp_b, %edx
	callq	strcmp
	movl	$string_b, %edi
	movl	$string_c, %esi
	movl	$b_cmp_c, %edx
	callq	strcmp
	movl	$string_a, %edi
	movl	$string_c, %esi
	movl	$a_cmp_c, %edx
	callq	strcmp
	xorl	%eax, %eax
	popq	%rdx
	retq
.Ltmp10:
	.size	main, .Ltmp10-main
	.cfi_endproc

	.type	string_a,@object        # @string_a
	.section	.rodata,"a",@progbits
string_a:
	.asciz	"stringa"
	.size	string_a, 8

	.type	string_b,@object        # @string_b
string_b:
	.asciz	"stringb"
	.size	string_b, 8

	.type	string_c,@object        # @string_c
string_c:
	.asciz	"stringa"
	.size	string_c, 8

	.type	a_cmp_b,@object         # @a_cmp_b
	.align	16
a_cmp_b:
	.asciz	"'stringa' == 'stringb'\n"
	.size	a_cmp_b, 24

	.type	b_cmp_c,@object         # @b_cmp_c
	.align	16
b_cmp_c:
	.asciz	"'stringb' == 'stringa'\n"
	.size	b_cmp_c, 24

	.type	a_cmp_c,@object         # @a_cmp_c
	.align	16
a_cmp_c:
	.asciz	"'stringa' == 'stringa'\n"
	.size	a_cmp_c, 24

	.type	eq_str,@object          # @eq_str
eq_str:
	.asciz	"Equal\n"
	.size	eq_str, 7

	.type	neq_str,@object         # @neq_str
neq_str:
	.asciz	"Not equal\n"
	.size	neq_str, 11


	.section	".note.GNU-stack","",@progbits
