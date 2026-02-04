	.file	"ex2.c"
	.intel_syntax noprefix
	.text
	.section	.rodata.str1.1,"aMS",@progbits,1
.LC3:
	.string	"x = %f, y = %f, time = %f s\n"
	.section	.text.startup,"ax",@progbits
	.p2align 4
	.globl	main
	.type	main, @function
main:
.LFB22:
	.cfi_startproc
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	push	rbx
	sub	rsp, 24
	.cfi_offset 3, -24
	call	clock@PLT
	movsd	xmm0, QWORD PTR .LC1[rip]
	pxor	xmm1, xmm1
	mov	rbx, rax
	mov	eax, 100000000
	.p2align 4
	.p2align 4
	.p2align 3
.L2:
	addsd	xmm1, xmm0
	addsd	xmm1, xmm0
	sub	eax, 2
	jne	.L2
	movsd	QWORD PTR -24[rbp], xmm1
	call	clock@PLT
	movsd	xmm1, QWORD PTR -24[rbp]
	pxor	xmm2, xmm2
	lea	rdi, .LC3[rip]
	sub	rax, rbx
	cvtsi2sd	xmm2, rax
	movapd	xmm0, xmm1
	mov	eax, 3
	divsd	xmm2, QWORD PTR .LC2[rip]
	call	printf@PLT
	add	rsp, 24
	xor	eax, eax
	pop	rbx
	pop	rbp
	.cfi_def_cfa 7, 8
	ret
	.cfi_endproc
.LFE22:
	.size	main, .-main
	.section	.rodata.cst8,"aM",@progbits,8
	.align 8
.LC1:
	.long	1374389535
	.long	1073028792
	.align 8
.LC2:
	.long	0
	.long	1093567616
	.ident	"GCC: (Debian 15.2.0-7) 15.2.0"
	.section	.note.GNU-stack,"",@progbits
