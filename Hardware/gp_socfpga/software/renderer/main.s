	.file	"main.c"
	.section	.text.startup,"ax",@progbits
	.align	2
	.globl	main
	.type	main, @function
main:
	add	sp,sp,-48
	sw	s2,32(sp)
	lui	s2,%hi(.L7)
	sw	s0,40(sp)
	sw	s1,36(sp)
	sw	s3,28(sp)
	sw	ra,44(sp)
	sw	s4,24(sp)
	add	s2,s2,%lo(.L7)
	lui	s3,%hi(copy_counter)
	li	s0,2097152
	li	s1,7
.L2:
	lhu	a4,2(s0)
	lhu	a5,0(s0)
	beq	a4,a5,.L2
	lhu	s4,0(s0)
	lhu	a5,2(s0)
	beq	s4,a5,.L2
.L14:
	sll	a5,s4,2
	add	a5,a5,s4
	sll	a5,a5,1
	add	a5,s0,a5
	lhu	a4,4(a5)
	sh	a4,4(sp)
	lhu	a4,6(a5)
	sh	a4,6(sp)
	lhu	a4,8(a5)
	sh	a4,8(sp)
	lhu	a4,10(a5)
	sh	a4,10(sp)
	lhu	a5,12(a5)
	sh	a5,12(sp)
	lbu	a5,4(sp)
	and	a5,a5,0xff
	bgtu	a5,s1,.L5
	sll	a5,a5,2
	add	a5,s2,a5
	lw	a5,0(a5)
	jr	a5
	.section	.rodata
	.align	2
	.align	2
.L7:
	.word	.L5
	.word	.L6
	.word	.L5
	.word	.L8
	.word	.L9
	.word	.L5
	.word	.L10
	.word	.L11
	.section	.text.startup
.L10:
	lbu	a2,5(sp)
	lw	a0,%lo(copy_counter)(s3)
	add	a1,sp,6
	and	a2,a2,0xff
	call	vid_copy
	lbu	a4,5(sp)
	lw	a5,%lo(copy_counter)(s3)
	and	a4,a4,0xff
	add	a5,a5,a4
	sw	a5,%lo(copy_counter)(s3)
.L5:
	lhu	a5,0(s0)
	add	s4,s4,1
	and	s4,s4,127
	add	a5,a5,1
	and	a5,a5,127
	sh	a5,0(s0)
	lhu	a5,2(s0)
	bne	a5,s4,.L14
	j	.L2
.L9:
	lhu	a0,6(sp)
	lhu	a1,8(sp)
	lhu	a2,10(sp)
	lhu	a3,12(sp)
	lhu	a4,14(sp)
	call	vid_fill_rect
	j	.L5
.L8:
	lhu	a0,6(sp)
	lhu	a1,8(sp)
	lhu	a2,10(sp)
	lhu	a3,12(sp)
	call	vid_fill_line
	j	.L5
.L6:
	lhu	a5,8(sp)
	lhu	a0,6(sp)
	sll	a5,a5,16
	or	a0,a5,a0
	call	vid_setbuffer
	li	a0,0
	call	vid_clear
	j	.L5
.L11:
	sw	zero,%lo(copy_counter)(s3)
	j	.L5
	.size	main, .-main
	.section	.sbss,"aw",@nobits
	.align	2
	.type	copy_counter, @object
	.size	copy_counter, 4
copy_counter:
	.zero	4
	.ident	"GCC: (GNU) 6.1.0"
