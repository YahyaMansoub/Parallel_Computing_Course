	.file	"ex2.c"
	.intel_syntax noprefix
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
	push	rbp
	.cfi_def_cfa_offset 16
	.cfi_offset 6, -16
	mov	rbp, rsp
	.cfi_def_cfa_register 6
	sub	rsp, 64
	movsd	xmm0, QWORD PTR .LC0[rip]
	movsd	QWORD PTR -32[rbp], xmm0
	movsd	xmm0, QWORD PTR .LC1[rip]
	movsd	QWORD PTR -40[rbp], xmm0
	pxor	xmm0, xmm0
	movsd	QWORD PTR -8[rbp], xmm0
	pxor	xmm0, xmm0
	movsd	QWORD PTR -16[rbp], xmm0
	call	clock@PLT
	mov	QWORD PTR -48[rbp], rax
	mov	DWORD PTR -20[rbp], 0
	jmp	.L2
.L3:
	movsd	xmm0, QWORD PTR -32[rbp]
	mulsd	xmm0, QWORD PTR -40[rbp]
	movsd	xmm1, QWORD PTR -8[rbp]
	addsd	xmm0, xmm1
	movsd	QWORD PTR -8[rbp], xmm0
	movsd	xmm0, QWORD PTR -32[rbp]
	mulsd	xmm0, QWORD PTR -40[rbp]
	movsd	xmm1, QWORD PTR -16[rbp]
	addsd	xmm0, xmm1
	movsd	QWORD PTR -16[rbp], xmm0
	add	DWORD PTR -20[rbp], 1
.L2:
	cmp	DWORD PTR -20[rbp], 99999999
	jle	.L3
	call	clock@PLT
	mov	QWORD PTR -56[rbp], rax
	mov	rax, QWORD PTR -56[rbp]
	sub	rax, QWORD PTR -48[rbp]
	pxor	xmm0, xmm0
	cvtsi2sd	xmm0, rax
	movsd	xmm1, QWORD PTR .LC3[rip]
	movapd	xmm2, xmm0
	divsd	xmm2, xmm1
	movsd	xmm0, QWORD PTR -16[rbp]
	mov	rax, QWORD PTR -8[rbp]
	lea	rdx, .LC4[rip]
	movapd	xmm1, xmm0
	movq	xmm0, rax
	mov	rdi, rdx
	mov	eax, 3
	call	printf@PLT
	mov	eax, 0
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
