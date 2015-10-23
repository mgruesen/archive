	.text
	.file	"strcpy.ll"
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

	.globl	strcpy
	.align	16, 0x90
	.type	strcpy,@function
strcpy:                                 # @strcpy
	.cfi_startproc
# BB#0:
	pushq	%r14
.Ltmp1:
	.cfi_def_cfa_offset 16
	pushq	%rbx
.Ltmp2:
	.cfi_def_cfa_offset 24
	pushq	%rax
.Ltmp3:
	.cfi_def_cfa_offset 32
.Ltmp4:
	.cfi_offset %rbx, -24
.Ltmp5:
	.cfi_offset %r14, -16
	movq	%rsi, %r14
	movq	%rdi, %rbx
                                        # kill: RDI<def> RBX<kill>
	callq	strlen
	movl	$0, 4(%rsp)
	jmp	.LBB1_1
	.align	16, 0x90
.LBB1_2:                                # %strcpy.body
                                        #   in Loop: Header=BB1_1 Depth=1
	movslq	4(%rsp), %rcx
	movb	(%rbx,%rcx), %dl
	movb	%dl, (%r14,%rcx)
	leal	1(%rcx), %ecx
	movl	%ecx, 4(%rsp)
.LBB1_1:                                # %strcpy.condit
                                        # =>This Inner Loop Header: Depth=1
	cmpl	%eax, 4(%rsp)
	jl	.LBB1_2
# BB#3:                                 # %strcpy.end
	movl	4(%rsp), %eax
	incl	%eax
	cltq
	movb	$0, (%r14,%rax)
	movb	$1, %al
	addq	$8, %rsp
	popq	%rbx
	popq	%r14
	retq
.Ltmp6:
	.size	strcpy, .Ltmp6-strcpy
	.cfi_endproc

	.globl	main
	.align	16, 0x90
	.type	main,@function
main:                                   # @main
	.cfi_startproc
# BB#0:
	pushq	%rax
.Ltmp7:
	.cfi_def_cfa_offset 16
	movl	$src, %edi
	movl	$dst, %esi
	callq	strcpy
	testb	$1, %al
	je	.LBB2_2
# BB#1:                                 # %succ
	movl	$suc_msg, %edi
	xorl	%eax, %eax
	callq	printf
	movl	$src, %edi
	xorl	%eax, %eax
	callq	printf
	movl	$dst, %edi
	xorl	%eax, %eax
	callq	printf
	xorl	%eax, %eax
	popq	%rdx
	retq
.LBB2_2:                                # %fail
	movl	$err_msg, %edi
	xorl	%eax, %eax
	callq	printf
	movl	$1, %eax
	popq	%rdx
	retq
.Ltmp8:
	.size	main, .Ltmp8-main
	.cfi_endproc

	.type	src,@object             # @src
	.section	.rodata,"a",@progbits
src:
	.asciz	"some string"
	.size	src, 12

	.type	dst,@object             # @dst
	.local	dst
	.comm	dst,12,1
	.type	suc_msg,@object         # @suc_msg
suc_msg:
	.asciz	"Copy passed\n"
	.size	suc_msg, 13

	.type	err_msg,@object         # @err_msg
err_msg:
	.asciz	"Copy failed\n"
	.size	err_msg, 13


	.section	".note.GNU-stack","",@progbits
