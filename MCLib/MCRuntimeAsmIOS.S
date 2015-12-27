//
//  MCRuntimeAsmIOS.s
//  monkcGame
//
//  Created by SunYuLi on 5/21/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

/* for iOS simulator <= iPhone5 */
#if defined(__i386__)

.text
.globl __push_jump
.p2align 4, 0x90
__push_jump:
	cmpl $0, 0(%esp)
	je 0f					#; confirm return address not nil
	jmp *0(%esp)
0:
	ret

#;int mc_atomic_get_integer(volatile int* target);
#;void* mc_atomic_get_pointer(volatile void** target);
#;int mc_atomic_set_integer(volatile int* target, volatile int old, volatile int value);
#;int mc_atomic_set_pointer(volatile void** target, volatile void* old, volatile void* value);

.text
.globl	_mc_atomic_get_integer
.p2align 4, 0x90
_mc_atomic_get_integer:
	pushl %ebp				
	movl %esp, %ebp
								#; 8(%ebp) addr
	xorl %eax, %eax
	movl 8(%ebp), %edx
	movl (%edx), %eax
	movl %ebp, %esp				
	popl %ebp
	ret


.text
.globl	_mc_atomic_get_pointer
.p2align 4, 0x90
_mc_atomic_get_pointer:
	pushl %ebp				
	movl %esp, %ebp
								#; 8(%ebp) addr
	xorl %eax, %eax
	movl 8(%ebp), %edx
	movl (%edx), %eax
	movl %ebp, %esp				
	popl %ebp
	ret


.text
.globl	_mc_atomic_set_integer
.p2align 4, 0x90
_mc_atomic_set_integer:
	pushl %ebp				
	movl %esp, %ebp
	 							#; 8(%ebp)  addr
								#; 12(%ebp) oldval
								#; 16(%ebp) newval
	xorl %eax, %eax
	movl 8(%ebp), %edx			#; dest addr in edx
	movl 12(%ebp), %eax			#; old value in eax
	movl 16(%ebp), %ecx			#; new value in ecx

	lock cmpxchgl %ecx, (%edx) 	#; atomic compare and swap
	xorl %eax, %eax				#; clear eax to 0
	jne	0b
	movl %ebp, %esp				#; successed return 0
	popl %ebp
	ret
0:
	movl $1, %eax				#; failed return 1
	movl %ebp, %esp				
	popl %ebp
	ret


.text
.globl	_mc_atomic_set_pointer
.p2align 4, 0x90
_mc_atomic_set_pointer:
	pushl %ebp				
	movl %esp, %ebp
	 							#; 8(%ebp)  addr
								#; 12(%ebp) oldval
								#; 16(%ebp) newval
	xorl %eax, %eax
	movl 8(%ebp), %edx			#; dest addr in edx
	movl 12(%ebp), %eax			#; old value in eax
	movl 16(%ebp), %ecx			#; new value in ecx

	lock cmpxchgl %ecx, (%edx) 	#; atomic compare and swap
	xorl %eax, %eax				#; clear eax to 0
	jne	0b
	movl %ebp, %esp				#; successed return 0
	popl %ebp
	ret
0:
	movl $1, %eax				#; failed return 1
	movl %ebp, %esp				
	popl %ebp
	ret
#endif

/* for iOS simulator >= iPhone6 */
#if defined(__x86_64__)
#;void* _push_jump(id const obj, void* addr, ...);

.text
.globl __push_jump
.p2align 4, 0x90
__push_jump:
	cmpq $0, %rdi		#; confirm return address not nil
	je 0f
	jmp *%rdi
0:
	ret

#;define int_arg1 %rdi
#;define int_arg2 %rsi
#;define int_arg3 %rdx

#;int mc_atomic_get_integer(volatile int* target);
#;void* mc_atomic_get_pointer(volatile void** target);
#;int mc_atomic_set_integer(volatile int* target, volatile int old, volatile int value);
#;int mc_atomic_set_pointer(volatile void** target, volatile void* old, volatile void* value);

.text
.globl	_mc_atomic_get_integer
.p2align 4, 0x90
_mc_atomic_get_integer:
	pushq %rbp				
	movq %rsp, %rbp
								#; %rdi addr
	xorq %rax, %rax
	movq (%rdi), %rax
	movq %rbp, %rsp				
	popq %rbp
	ret


.text
.globl	_mc_atomic_get_pointer
.p2align 4, 0x90
_mc_atomic_get_pointer:
	pushq %rbp				
	movq %rsp, %rbp
								#; %rdi addr
	xorq %rax, %rax
	movq (%rdi), %rax
	movq %rbp, %rsp				
	popq %rbp
	ret


.text
.globl	_mc_atomic_set_integer
.p2align 4, 0x90
_mc_atomic_set_integer:
	pushq %rbp				
	movq %rsp, %rbp
	 							#; %rdi addr
								#; %rsi oldval
								#; %rdx newval
	xorq %rax, %rax
	movq %rsi, %rax

	lock cmpxchgq %rdx, (%rdi) 	#; atomic compare and swap
	xorq %rax, %rax				#; clear rax to 0
	jne	0b
	movq %rbp, %rsp				#; successed return 0
	popq %rbp
	ret
0:
	movq $1, %rax				#; failed return 1
	movq %rbp, %rsp				
	popq %rbp
	ret


.text
.globl	_mc_atomic_set_pointer
.p2align 4, 0x90
_mc_atomic_set_pointer:
	pushq %rbp				
	movq %rsp, %rbp
	 							#; %rdi addr
								#; %rsi oldval
								#; %rdx newval
	xorq %rax, %rax
	movq %rsi, %rax

	lock cmpxchgq %rdx, (%rdi) 	#; atomic compare and swap
	xorq %rax, %rax				#; clear rax to 0
	jne	0b
	movq %rbp, %rsp				#; successed return 0
	popq %rbp
	ret
0:
	movq $1, %rax				#; failed return 1
	movq %rbp, %rsp				
	popq %rbp
	ret
#endif

/*
infos about ARM 32 platform:

stack-align: 	public(8byte) non-public(4byte)
frame-pointer:  fp is r11 in ARM mode / r7 in thumb mode
keep-fp:		-mapcs-frame will keep the fp not to be optimized out

__armv7__
*/

#ifdef xxxxx
.text
.globl __push_jump
.p2align 3 			
__push_jump:
	cmp r0, #0
	beq 0f
	bx r0
0:
	bx lr

/*
int mc_atomic_get_integer(volatile int* target);
void* mc_atomic_get_pointer(volatile void** target);
int mc_atomic_set_integer(volatile int* target, volatile int old, volatile int value);
int mc_atomic_set_pointer(volatile void** target, volatile void* old, volatile void* value);

a1 addr
a2 oldval
a3 newval
return 0 success
*/

.text
.globl	_mc_atomic_get_integer
.p2align 3, 0x90
_mc_atomic_get_integer:
	ldrex v1, [r0]
	mov r0, v1
	bx lr

.text
.globl	_mc_atomic_get_pointer
.p2align 3, 0x90
_mc_atomic_get_pointer:
	ldrex v1, [r0]
	mov r0, v1
	bx lr

.text
.globl	_mc_atomic_set_integer
.p2align 3, 0x90
_mc_atomic_set_integer:
	strex v1, r2, [r0]
	mov r0, v1
	bx lr

.text
.globl	_mc_atomic_set_pointer
.p2align 3, 0x90
_mc_atomic_set_pointer:
	strex v1, r2, [r0]
	mov r0, v1
	bx lr
#endif

/*
infos about ARM 64 platform:

stack-align: 	public(16byte) non-public(16byte)
frame-pointer: 	fp is r11 in ARM mode / r7 in thumb mode
keep-fp:		-mapcs-frame will keep the fp not to be optimized out

r0 r1 r2 r3 r4 r5 r6 r7 r8 r9 r10 r11   r30
w0 w1 w2 w3 w4 w5 w6 w7 w8 w9 w10 w11   w30 (32bit context)
x0 x1 x2 x3 x4 x5 x6 x7 x8 x9 x10 x11   x30 (64bit context)
a1 a2 a3 a4 v1 v2 v3 v4 v5 v6 v7  v8    lr

.p2align 4 : 16-byte aligned
*/

#ifdef __arm64__
.text
.globl __push_jump
.p2align 4
__push_jump:
	cmp x0, #0
	beq 0f
	br x0
0:
	ret

/*
int mc_atomic_get_integer(volatile int* target);
void* mc_atomic_get_pointer(volatile void** target);
int mc_atomic_set_integer(volatile int* target, volatile int old, volatile int value);
int mc_atomic_set_pointer(volatile void** target, volatile void* old, volatile void* value);

a1 addr
a2 oldval
a3 newval
return 0 success

r0 r1 r2 r3 r4 r5 r6 r7 r8 r9 r10 r11
a1 a2 a3 a4 v1 v2 v3 v4 v5 v6 v7  v8
*/

.text
.globl	_mc_atomic_get_integer
.p2align 4, 0x90
_mc_atomic_get_integer:
	ldxr w4, [x0,#0]
	mov x0, x4
	blr lr

.text
.globl	_mc_atomic_get_pointer
.p2align 4, 0x90
_mc_atomic_get_pointer:
	ldxr w4, [x0,#0]
	mov x0, x4
	blr lr

.text
.globl	_mc_atomic_set_integer
.p2align 4, 0x90
_mc_atomic_set_integer:
	stxr w2, w4, [x0,#0]
	mov x0, x4
	blr lr

.text
.globl	_mc_atomic_set_pointer
.p2align 4, 0x90
_mc_atomic_set_pointer:
	stxr w2, w4, [x0,#0]
	mov x0, x4
	blr lr
#endif