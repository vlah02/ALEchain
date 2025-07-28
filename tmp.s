.section data
.word global1
.word local_label
.word global2+4
.word local_label+8
.word missing
.word global1-1

.ascii "Text"
label_in_data:
.word 42

.section txt
start:
call global1
call local_label
call missing
jmp global2+4
.section data
.word global1
.word local_label
.word global2+4
.word local_label+8
.word missing
.word global1-1

.ascii "Text"
label_in_data:
.word 42

.section txt
start:
call global1
call local_label
call missing
jmp global2+4

local_label:
halt

.section data
.word start
.word label_in_data

.global global1
.global global2
.section data
.word global1
.word local_label
.word global2+4
.word local_label+8
.word missing
.word global1-1

.ascii "Text"
label_in_data:
.word 42

.section txt
start:
call global1
call local_label
call missing
jmp global2+4

local_label:
halt

.section data
.word start
.word label_in_data

.global global1
.global global2
