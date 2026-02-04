	.file	"ex2.c"
	.text
	.section	.rodata
.LC4:
	.string	"x = %f, y = %f, time = %f s\n"
	.text
	.globl	main
	.type	main, @function
main:
.LFB6:
	.cfi_startproc
	pushq	%rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	movq	%rsp, %rbp
	.cfi_def_cfa_register 6
	subq	$64, %rsp
	movsd	.LC0(%rip), %xmm0
	movsd	%xmm0, -32(%rbp)
	movsd	.LC1(%rip), %xmm0
	movsd	%xmm0, -40(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -8(%rbp)
	pxor	%xmm0, %xmm0
	movsd	%xmm0, -16(%rbp)
	call	clock@PLT
	movq	%rax, -48(%rbp)
	movl	$0, -20(%rbp)
	jmp	.L2
.L3:
	movsd	-32(%rbp), %xmm0
	mulsd	-40(%rbp), %xmm0
	movsd	-8(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -8(%rbp)
	movsd	-32(%rbp), %xmm0
	mulsd	-40(%rbp), %xmm0
	movsd	-16(%rbp), %xmm1
	addsd	%xmm1, %xmm0
	movsd	%xmm0, -16(%rbp)
	addl	$1, -20(%rbp)
.L2:
	cmpl	$99999999, -20(%rbp)
	jle	.L3
	call	clock@PLT
	movq	%rax, -56(%rbp)
	movq	-56(%rbp), %rax
	subq	-48(%rbp), %rax
	pxor	%xmm0, %xmm0
	cvtsi2sdq	%rax, %xmm0
	movsd	.LC3(%rip), %xmm1
	movapd	%xmm0, %xmm2
	divsd	%xmm1, %xmm2
	movsd	-16(%rbp), %xmm0
	movq	-8(%rbp), %rax
	leaq	.LC4(%rip), %rdx
	movapd	%xmm0, %xmm1
	movq	%rax, %xmm0
	movq	%rdx, %rdi
	movl	$3, %eax
	call	printf@PLT
	movl	$0, %eax
	leave
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE6:
	.size	main, .-main
	.section	.rodata
	.align 8
.LC0:
	.long	-1717986918
	.long	1072798105
	.align 8
.LC1:
	.long	858993459
	.long	1072902963
	.align 8
.LC3:
	.long	0
	.long	1093567616
	.ident	"GCC: (Debian 15.2.0-7) 15.2.0"
	.section	.note.GNU-stack,"",@progbits
