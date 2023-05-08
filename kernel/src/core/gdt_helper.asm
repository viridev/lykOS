GLOBAL gdt_flush

gdt_flush:
    ; mov rax, [rsp+4]  ; Get the pointer to the GDT, passed as a parameter.
    lgdt [rdi]        ; Load the new GDT pointer

    push 0x08
    lea rax, [rel flush]
    push rax
    retfq

flush:
    mov ax, 0x10      ; 0x10 is the offset in the GDT to our data segment
    mov ds, ax        ; Load all data segment selectors
    mov es, ax
    mov fs, ax
    mov gs, ax
    mov ss, ax
    ret