section .text

global HtableOptFind

HtableOptFind:
    xor eax, eax
    xor edx, edx

    ;-----------------------------
    ; CRC32 FIELD                |
    ;-----------------------------

    mov     rdx,    qword [rdi+16]
    crc32   rax,    qword [rsi]
    crc32   rax,    qword [rsi+8]
    crc32   rax,    qword [rsi+16]
    crc32   rax,    qword [rsi+24]

    ;----------------------------|

    ;----------------------------|
    ; BloomFilter Field          |
    ; ---------------------------|
                                ;|
    movzx edi, al               ;| bloom[0] check
    cmp byte [rdx+rdi], 0       ;|
    je .BloomReject             ;|
    movzx edi, ah               ;| bloom[1] check
    cmp byte [rdx + rdi], 0     ;|
    je .BloomReject             ;|
    mov edi, eax                ;| bloom[2] check
    shr edi, 16                 ;|
    movzx edi, dil              ;|
    cmp byte [rdx + rdi], 0     ;|
    je .BloomReject             ;|
    mov rdi, rax                ;| bloom[3] check
    shr rdi, 24                 ;|
    cmp byte [rdx + rdi], 0     ;|
    je .BloomReject             ;|
                                ;|
    mov eax, 7                  ;|
    ret                         ;|
; -------------------------------|

.BloomReject:

    and     eax,    127                 ;
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
    mov eax, 7                          ; return HTABLE_FOUND;

    vzeroupper
    ret

.FinishProg:
    mov eax, 8                          ; return HTABLE_NOT_FOUND;
    ret
