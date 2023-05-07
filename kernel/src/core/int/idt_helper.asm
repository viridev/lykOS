EXTERN isr_handler

%macro isr_stub 1
global isr_stub_%1
isr_stub_%1:
    push 0
    push %1
    push rax
    push rbx
    push rcx
    push rdx
    push rbp
    push rdi
    push rsi
    push r8
    push r9
    push r10
    push r11
    push r12
    push r13
    push r14
    push r15

    mov rdi, rsp
    call isr_handler

    pop r15
    pop r14
    pop r13
    pop r12
    pop r11
    pop r10
    pop r9
    pop r8
    pop rsi
    pop rdi
    pop rbp
    pop rdx
    pop rcx
    pop rbx
    pop rax

    add rsp, 16

    add rsp, 8 ; remove the pushed byte from the stack
    iretq
%endmacro

%assign i 0
%rep 32
    isr_stub i
    %assign i i+1
%endrep