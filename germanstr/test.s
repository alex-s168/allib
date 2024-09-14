	.text
	.intel_syntax noprefix
	.file	"test.c"
	.globl	get1                            # -- Begin function get1
	.p2align	4, 0x90
	.type	get1,@function
get1:                                   # @get1
	.cfi_startproc
# %bb.0:
	mov	rax, rdi
	shr	rax, 40
                                        # kill: def $al killed $al killed $rax
	ret
.Lfunc_end0:
	.size	get1, .Lfunc_end0-get1
	.cfi_endproc
                                        # -- End function
	.globl	get2                            # -- Begin function get2
	.p2align	4, 0x90
	.type	get2,@function
get2:                                   # @get2
	.cfi_startproc
# %bb.0:
	mov	qword ptr [rsp - 16], rdi
	mov	qword ptr [rsp - 8], rsi
	mov	eax, edx
	add	rsi, rax
	lea	rax, [rsp + rax - 12]
	cmp	edx, 4
	cmovb	rsi, rax
	cmp	edi, 13
	cmovb	rsi, rax
	movzx	eax, byte ptr [rsi]
	ret
.Lfunc_end1:
	.size	get2, .Lfunc_end1-get2
	.cfi_endproc
                                        # -- End function
	.globl	set1                            # -- Begin function set1
	.p2align	4, 0x90
	.type	set1,@function
set1:                                   # @set1
	.cfi_startproc
# %bb.0:
	cmp	dword ptr [rdi], 13
	jae	.LBB2_2
# %bb.1:
	add	rdi, 5
	mov	byte ptr [rdi], 97
	ret
.LBB2_2:
	mov	byte ptr [rdi + 5], 97
	mov	rdi, qword ptr [rdi + 8]
	inc	rdi
	mov	byte ptr [rdi], 97
	ret
.Lfunc_end2:
	.size	set1, .Lfunc_end2-set1
	.cfi_endproc
                                        # -- End function
	.globl	set2                            # -- Begin function set2
	.p2align	4, 0x90
	.type	set2,@function
set2:                                   # @set2
	.cfi_startproc
# %bb.0:
	cmp	dword ptr [rdi], 13
	jae	.LBB3_2
# %bb.1:
	mov	eax, esi
	lea	rax, [rdi + rax + 4]
	mov	byte ptr [rax], 97
	ret
.LBB3_2:
	mov	eax, esi
	cmp	esi, 4
	jae	.LBB3_4
# %bb.3:
	mov	byte ptr [rdi + rax + 4], 97
.LBB3_4:
	add	rax, qword ptr [rdi + 8]
	mov	byte ptr [rax], 97
	ret
.Lfunc_end3:
	.size	set2, .Lfunc_end3-set2
	.cfi_endproc
                                        # -- End function
	.ident	"clang version 17.0.6"
	.section	".note.GNU-stack","",@progbits
	.addrsig
