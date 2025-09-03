org 0x7c00
[bits 16]
xor ax, ax ; eax = 0
; 初始化段寄存器, 段地址全部设为0
mov ds, ax
mov ss, ax
mov es, ax
mov fs, ax
mov gs, ax

; 初始化栈指针
mov sp, 0x7c00
mov ax, 1                ; 逻辑扇区号第0~15位
mov cx, 0                ; 逻辑扇区号第16~31位
mov bx, 0x7e00           ; bootloader的加载地址
    call asm_read_hard_disk  ; 读取硬盘  
jmp 0x0000:0x7e00        ; 跳转到bootloader

jmp $ ; 死循环

asm_read_hard_disk:
    ; 保存LBA号到di
    mov di, ax

    ; 计算S = (LBA + 1) % 63
    xor dx, dx
    inc ax          ;AX = LBA + 1
    mov bx, 63
    div bx          ; AX = (LBA + 1)//63, DX = (LBA+1)%63
    mov si, dx      ; SI =  S


    ; 计算H和C: AX = (LBA+1)//63, DX = H (余数)
    xor dx, dx
    mov bx, 18
    div bx          ; AX = C, DX = H

    ; 设置CHS参数
    mov cx, si
    mov ch, al      ; CH = 柱面低8位 (C ≤ 255)
 
    ; 设置DH=磁头号，DL=80h
    mov dh, dl      ; DH = H
    mov dl, 0x80    ; DL = 驱动器号

    ; 设置功能号及读取参数
    mov ax, 0x0205  ; AH=02h读扇区，AL=5个扇区
    mov bx, 0x7e00  

    int 0x13        ; 调用BIOS中断
      
    ret

times 510 - ($ - $$) db 0
db 0x55, 0xaa