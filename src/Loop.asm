(FUNCTION_LOOP) #4
@4 
D=A //D = 4
@CONT
D;JNE
@ENDFUNCTIONLOOP
0;JMP
(CONT)
@SP
M=M-1
D=D-1
@CONT
D;JNE
(ENDFUNCTIONLOOP)
