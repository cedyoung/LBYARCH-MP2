section .text
global daxpy_asm

; Function: daxpy_asm
; Description: Performs the Z = A*X + Y operation using scalar double-precision
; Parameters (Microsoft x64 Calling Convention):
;   1. n (long long)       - RCX
;   2. A (double)          - XMM1
;   3. X (double pointer)  - R8
;   4. Y (double pointer)  - R9
;   5. Z (double pointer)  - [RSP + 40] (Stack)

daxpy_asm:
    ; We use R11 as a temporary holder for the destination address.
    mov r11, [rsp + 40]
    xor rdx, rdx

process_vector:
    cmp rdx, rcx
    jge done

    ; Operation: Z[i] = (A * X[i]) + Y[i]
    
    ; We use XMM5 to ensure no conflict with argument registers.
    ; Offset calculation: Index * 8 (since doubles are 8 bytes).
    movsd xmm5, [r8 + rdx*8]

    mulsd xmm5, xmm1

    addsd xmm5, [r9 + rdx*8]

    movsd [r11 + rdx*8], xmm5

    inc rdx
    
    jmp process_vector

done:
    ret