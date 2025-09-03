%include "head.include"

your_if:
    mov eax, [a1]
    cmp eax, 40
    jge ge40
    cmp eax, 18
    jge ge18
    shl eax, 5
    mov [if_flag], eax

    jmp your_while

ge40:
    add eax, 3
    mov edx, 0
    mov ecx, 5
    idiv ecx
    mov [if_flag], eax
    
    jmp your_while

ge18:
    mov ecx, eax
    shl ecx, 1
    sub ecx, 80
    neg ecx
    mov [if_flag], ecx

    jmp your_while

your_while:
    mov ebx, dword[a2]
    cmp ebx, 25         
    jge end_while      

    call my_random      

    mov esi, [while_flag]             
    mov [esi + 2 * ebx], eax  
    inc ebx
    mov dword[a2], ebx 
    jmp your_while      

end_while:

%include "end.include"

your_function:
    mov esi, [your_string]
    mov eax, 0
loop:
    movzx ecx, byte[esi + eax]
    cmp ecx, 0
    je loopend
    pushad
    add ecx, 9
    push ecx
    call print_a_char
    pop ecx
    popad
    inc eax
    jmp loop;

loopend:
    ret
