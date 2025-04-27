section .text

global HtableOptFind

HtableOptFind:
    xor eax, eax

    ;-----------------------------
    ; CRC32 FIELD                |
    ;-----------------------------

    mov     rdx,    qword [rdi+16]
    crc32   rax,    qword [rsi]
    crc32   rax,    qword [rsi+8]
    crc32   rax,    qword [rsi+16]
    crc32   rax,    qword [rsi+24]

    ;----------------------------|

    and     eax,    1023                ;
    mov     rax,    qword [rdx+rax*8]   ; int bin = icrc32(string) % 128
    test    rax,    rax                 ;
    je      .FinishProg
    xor     edi,    edi
    jmp     .HtableCycle

.HtableStopCycle:
    mov rax, qword [rax+32]
    test rax, rax
    je .FinishProg

.HtableCycle:                           ; for (List * lst = tab->table[bin]; lst; lst = lst->nxt)
    mov rcx, qword [rax]
    mov edx, edi

    vmovdqu ymm0, yword [rsi]           ; Moving string to YMM
    vptest  ymm0, yword [rcx]           ; ~(s1 & s2)
    setc    dl                          ;  Set dl if CF == 1 (s1 equals s2)

    test edx, edx
    je .HtableStopCycle
    mov eax, 6                          ; return HTABLE_FOUND;

    vzeroupper
    ret

.FinishProg:
    mov eax, 7                          ; return HTABLE_NOT_FOUND;
    ret
