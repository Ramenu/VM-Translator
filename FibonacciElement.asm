
@261
D=A
@SP
M=D

functionSys.init0
-----------------------------------

(Sys.init)
@0
D=A
@Sys.init.init
D;JNE
@Sys.init.init.end
0;JMP
(Sys.init.init)
@SP
A=M
M=0
@SP
M=M+1
D=D-1
@Sys.init.init
D;JNE
(Sys.init.init.end)

pushconstant4
-----------------------------------

@4
D=A
@SP
A=M
M=D
@SP
M=M+1

callMain.fibonacci1//computesthe4'thfibonaccielement
-----------------------------------

@67
D=A
@SP
A=M
M=D
@LCL
D=M
@SP
M=M+1
A=M
M=D
@ARG
D=M
@SP
M=M+1
A=M
M=D
@THIS
D=M
@SP
M=M+1
A=M
M=D
@THAT
D=M
@SP
M=M+1
A=M
M=D
@SP
M=M+1
D=M
@LCL
M=D
@5
D=D-A
@1
D=D-A
@ARG
M=D
@Main.fibonacci
0;JMP
(67)

labelWHILE
-----------------------------------

(WHILE)

gotoWHILE//loopsinfinitely
-----------------------------------

@WHILE
0;JMP

functionMain.fibonacci0
-----------------------------------

(Main.fibonacci)
@0
D=A
@Main.fibonacci.init
D;JNE
@Main.fibonacci.init.end
0;JMP
(Main.fibonacci.init)
@SP
A=M
M=0
@SP
M=M+1
D=D-1
@Main.fibonacci.init
D;JNE
(Main.fibonacci.init.end)

pushargument0
-----------------------------------

@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

pushconstant2
-----------------------------------

@2
D=A
@SP
A=M
M=D
@SP
M=M+1

lt//checksifn<2
-----------------------------------

@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
D=M-D
M=-1
@CHECKTRUE1
D;JLT
@SP
A=M
M=0
(CHECKTRUE1)
@SP
M=M+1

if-gotoIF_TRUE
-----------------------------------

@SP
M=M-1
A=M
D=M
@IF_TRUE
D;JNE

gotoIF_FALSE
-----------------------------------

@IF_FALSE
0;JMP

labelIF_TRUE//ifn<2,returnn
-----------------------------------

(IF_TRUE)

pushargument0
-----------------------------------

@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

return
-----------------------------------

@LCL
D=M
@13
M=D
@5
A=D-A
D=M
@14
M=D
@SP
A=M-1
D=M
@ARG
A=M
M=D
@ARG
D=M
@SP
M=D+1
@13
A=M-1
D=M
@THAT
M=D
@2
D=A
@13
A=M-D
D=M
@THIS
M=D
@3
D=A
@13
A=M-D
D=M
@ARG
M=D
@4
D=A
@13
A=M-D
D=M
@LCL
M=D
@14
A=M
0;JMP

labelIF_FALSE//ifn>=2,returnsfib(n-2)+fib(n-1)
-----------------------------------

(IF_FALSE)

pushargument0
-----------------------------------

@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

pushconstant2
-----------------------------------

@2
D=A
@SP
A=M
M=D
@SP
M=M+1

sub
-----------------------------------

@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
M=M-D
@SP
M=M+1

callMain.fibonacci1//computesfib(n-2)
-----------------------------------

@251
D=A
@SP
A=M
M=D
@LCL
D=M
@SP
M=M+1
A=M
M=D
@ARG
D=M
@SP
M=M+1
A=M
M=D
@THIS
D=M
@SP
M=M+1
A=M
M=D
@THAT
D=M
@SP
M=M+1
A=M
M=D
@SP
M=M+1
D=M
@LCL
M=D
@5
D=D-A
@1
D=D-A
@ARG
M=D
@Main.fibonacci
0;JMP
(251)

pushargument0
-----------------------------------

@ARG
D=M
@0
A=D+A
D=M
@SP
A=M
M=D
@SP
M=M+1

pushconstant1
-----------------------------------

@1
D=A
@SP
A=M
M=D
@SP
M=M+1

sub
-----------------------------------

@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
M=M-D
@SP
M=M+1

callMain.fibonacci1//computesfib(n-1)
-----------------------------------

@320
D=A
@SP
A=M
M=D
@LCL
D=M
@SP
M=M+1
A=M
M=D
@ARG
D=M
@SP
M=M+1
A=M
M=D
@THIS
D=M
@SP
M=M+1
A=M
M=D
@THAT
D=M
@SP
M=M+1
A=M
M=D
@SP
M=M+1
D=M
@LCL
M=D
@5
D=D-A
@1
D=D-A
@ARG
M=D
@Main.fibonacci
0;JMP
(320)

add//returnsfib(n-1)+fib(n-2)
-----------------------------------

@SP
M=M-1
A=M
D=M
@SP
M=M-1
A=M
M=M+D
@SP
M=M+1

return
-----------------------------------

@LCL
D=M
@13
M=D
@5
A=D-A
D=M
@14
M=D
@SP
A=M-1
D=M
@ARG
A=M
M=D
@ARG
D=M
@SP
M=D+1
@13
A=M-1
D=M
@THAT
M=D
@2
D=A
@13
A=M-D
D=M
@THIS
M=D
@3
D=A
@13
A=M-D
D=M
@ARG
M=D
@4
D=A
@13
A=M-D
D=M
@LCL
M=D
@14
A=M
0;JMP

(END)
@END
0;JMP