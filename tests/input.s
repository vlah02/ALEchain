.section txt
start:
    halt
    int
    call label1
    jmp label2
    beq %r1, %r2, label1
label1:
    ret
label2:
    not %r3
