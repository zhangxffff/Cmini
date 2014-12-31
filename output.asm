extern printf
extern scanf


section	.data
num_printed	dd	8
str_print_ln	db	10, 0
str_print_num	db	"%d", 9, 9, 0
str_input_num	db	"%d", 0


var0	dd	0
var1	dd	0
var2	dd	0
var3	dd	0
var4	dd	0
var5	dd	0
var6	dd	0
var7	dd	0
var8	dd	0
var9	dd	0
var10	dd	0



section	.text
print_num:
	push dword eax
	push dword str_print_num
	call printf
	add esp, 8
	dec dword [num_printed]
	jnz R
	mov dword [num_printed], 8
	push dword str_print_ln
	call printf
	add esp, 4
R:	ret


input_num:
	push dword eax
	push dword str_input_num
	call scanf
	add esp, 8
	ret


	global main
main:

	mov eax, var2
	call input_num


	mov dword [var0], 0

	mov dword [var1], 1

	mov eax, [var0]
	call print_num


	mov eax, [var1]
	call print_num


L0:
	mov eax, [var2]
	mov [var3], eax
	mov dword [var4], 2
	mov eax, [var4]
	sub eax, [var3]
	mov dword [var3], 0
	jns L2
	mov dword [var3], 1
L2:
	mov eax, dword [var3]
	cmp eax, 0
	jz L1
	mov eax, [var0]
	mov [var5], eax
	mov eax, [var1]
	mov [var6], eax
	mov eax, [var6]
	add [var5], eax
	mov eax, [var5]
	mov [var1], eax

	mov eax, [var1]
	mov [var7], eax
	mov eax, [var0]
	mov [var8], eax
	mov eax, [var8]
	sub [var7], eax
	mov eax, [var7]
	mov [var0], eax

	mov eax, [var1]
	call print_num


	mov eax, [var2]
	mov [var9], eax
	mov dword [var10], 1
	mov eax, [var10]
	sub [var9], eax
	mov eax, [var9]
	mov [var2], eax


	jmp L0
L1:


	push dword str_print_ln
	call printf
	add esp, 4
