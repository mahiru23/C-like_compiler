.data
program_a: .word 4
program_b: .word 4
program_c: .word 4
program_i: .word 4
program_j: .word 4
main_a: .word 4
main_b: .word 4
main_c: .word 4
T0: .word 4
T1: .word 4
T2: .word 4
T3: .word 4
T4: .word 4
T5: .word 4
T6: .word 4
T7: .word 4
T8: .word 4
T9: .word 4
T10: .word 4
T11: .word 4
T12: .word 4
T13: .word 4
T14: .word 4
T15: .word 4
T16: .word 4
T17: .word 4
T18: .word 4
T19: .word 4
T20: .word 4
T21: .word 4
T22: .word 4
T23: .word 4
T24: .word 4
T25: .word 4
T26: .word 4
T27: .word 4
T28: .word 4
T29: .word 4
T30: .word 4
T31: .word 4
T32: .word 4
T33: .word 4
T34: .word 4
T35: .word 4
T36: .word 4
T37: .word 4
T38: .word 4
T39: .word 4
T40: .word 4
T41: .word 4
T42: .word 4
T43: .word 4
T44: .word 4
T45: .word 4
SP: .word 0x10020000
.text
lw $1 SP
addi $sp $1 0
j main
program:
addi $5 $1 0
addi $6 $2 0
addi $7 $3 0
addi $8 $4 0
addi $9 $31 0
lw $2 0($sp)
sw $2 program_a
lw $2 4($sp)
sw $2 program_b
lw $2 8($sp)
sw $2 program_c
lui $2 1
srl $2 $2 16
sw $2 T0
lw $2 T0
sw $2 T1
lw $2 T1
sw $2 program_i
lw $2 program_a
sw $2 T2
lw $2 T2
sw $2 T3
lw $2 program_b
sw $2 T4
lw $2 program_c
sw $2 T5
lw $2 T4
lw $3 T5
add $4 $2 $3
sw $4 T6
lw $2 T6
sw $2 T7
lw $2 T7
sw $2 T8
lw $2 T8
sw $2 T9
lw $2 T3
lw $3 T9
slt $1 $3 $2
bnez $1 label1
j label2
label1:
lw $2 program_a
sw $2 T10
lw $2 program_b
lw $3 program_c
mul $4 $2 $3
sw $4 T11
lw $2 T11
sw $2 T12
lui $2 1
srl $2 $2 16
sw $2 T13
lw $2 T12
lw $3 T13
add $4 $2 $3
sw $4 T14
lw $2 T14
sw $2 T15
lw $2 T15
sw $2 T16
lw $2 T10
lw $3 T16
add $4 $2 $3
sw $4 T17
lw $2 T17
sw $2 T18
lw $2 T18
sw $2 program_j
j label3
label2:
lw $2 program_a
sw $2 T19
lw $2 T19
sw $2 T20
lw $2 T20
sw $2 program_j
label3:
lw $2 program_i
sw $2 T21
lw $2 T21
sw $2 T22
lui $2 100
srl $2 $2 16
sw $2 T23
lw $2 T23
sw $2 T24
lw $2 T22
lw $3 T24
slt $1 $3 $2
beqz $1 label4
j label5
label4:
lw $2 program_i
addi $3 $0 2
mul $4 $2 $3
sw $4 T25
lw $2 T25
sw $2 T26
lw $2 T26
sw $2 T27
lw $2 T27
sw $2 program_i
j label3
label5:
lw $2 program_i
sw $2 T28
lw $2 T28
sw $2 T29
lw $2 T29
sw $2 0($sp)
addi $1 $5 0
addi $2 $6 0
addi $3 $7 0
addi $4 $8 0
addi $31 $9 0
jr $31
main:
addi $5 $1 0
addi $6 $2 0
addi $7 $3 0
addi $8 $4 0
addi $9 $31 0
lui $2 6
srl $2 $2 16
sw $2 T31
lw $2 T31
sw $2 T32
lw $2 T32
sw $2 main_a
lui $2 2
srl $2 $2 16
sw $2 T33
lw $2 T33
sw $2 T34
lw $2 T34
sw $2 main_b
lui $2 3
srl $2 $2 16
sw $2 T35
lw $2 T35
sw $2 T36
lw $2 T36
sw $2 main_c
lw $2 main_a
sw $2 T37
lw $2 T37
sw $2 T38
lw $2 T38
sw $2 0($sp)
lw $2 main_b
sw $2 T39
lw $2 T39
sw $2 T40
lw $2 T40
sw $2 4($sp)
lw $2 main_c
sw $2 T41
lw $2 T41
sw $2 T42
lw $2 T42
sw $2 8($sp)
jal program
lw $2 0($sp)
sw $2 T43
lw $2 T43
sw $2 T44
lw $2 T44
sw $2 0($sp)
addi $1 $5 0
addi $2 $6 0
addi $3 $7 0
addi $4 $8 0
addi $31 $9 0
