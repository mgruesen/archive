	.text
	.file	"strlen.ll"
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

	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:
	pushq	%rbx
.Ltmp1:
	.cfi_def_cfa_offset 16
.Ltmp2:
	.cfi_offset %rbx, -16
	movl	$globstr, %edi
	callq	strlen
	movl	%eax, %ebx
	movl	$globstr, %edi
	xorl	%eax, %eax
	callq	printf
	movl	$.Lprint_int, %edi
	xorl	%eax, %eax
	movl	%ebx, %esi
	callq	printf
	xorl	%eax, %eax
	popq	%rbx
	retq
.Ltmp3:
	.size	main, .Ltmp3-main
	.cfi_endproc

	.type	globstr,@object         # @globstr
	.data
	.globl	globstr
globstr:
	.asciz	"input string"
	.size	globstr, 13

	.type	.Lprint_int,@object     # @print_int
	.section	.rodata,"a",@progbits
.Lprint_int:
	.ascii	"%d\n"
	.size	.Lprint_int, 3


	.section	".note.GNU-stack","",@progbits
