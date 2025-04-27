section .text

global HtableOptFind

HtableOptFind:
    xor eax, eax
    mov     rdx,    qword [rdi+16]
    crc32   rax,    qword [rsi]
    crc32   rax,    qword [rsi+8]
    crc32   rax,    qword [rsi+16]
    crc32   rax,    qword [rsi+24]

    and     eax,    127
    mov     rax,    qword [rdx+rax*8]
    test    rax,    rax
    je      .FinishProg
    xor     edi,    edi
    jmp     .HtableCycle

.HtableStopCycle:
    mov rax, qword [rax+32]
    test rax, rax
    je .FinishProg

.HtableCycle:
    mov rcx, qword [rax]
    mov edx, edi
    ; xor eax, eax
    vmovdqu ymm0, yword [rsi]
    vptest  ymm0, yword [rcx]
    setc    dl

    test edx, edx
    je .HtableStopCycle
    mov eax, 6

    vzeroupper
    ret

.FinishProg:
    mov eax, 7
    ret
