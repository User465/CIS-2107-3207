#program to put deadbeef bit pattern into register 1

        .text
        .globl main

main:
        ori     $2, $0, 0xdead0000  #puts dead in 2
        ori     $1, $2, 0x0000beef  #puts beef in 1

#End of file
