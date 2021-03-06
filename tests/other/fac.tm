0000:          LD       6,      0,      0			;Loading SP with DMEM_MAX.
0001:          ST       0,      0,      0			;Zeroing out the top of DMem.
0002:         LDA       4,      3,      7			;Loading return address for call to "main" into REGISTER4.
0003:          ST       4,      0,      6			;Pushing return address for call to "main" to the stack.
0004:         LDA       6,     -1,      6			;SP++.
0005:         LDA       7,     36,      7			;Jumping to "main".
0006:         LDA       6,      1,      6			;Cleanup from function call.
0007:         LDC       0,      0,      0			;Start of program exit routine.
0008:         LDC       1,      0,      0			;Zeroing out lower 6 registers.
0009:         LDC       2,      0,      0
0010:         LDC       3,      0,      0
0011:         LDC       4,      0,      0
0012:         LDC       5,      0,      0
0013:         LDC       6,   1023,      0			;Zeroing out DMem. SP = DMEM_MAX.
0014:         JLT       6,      3,      7			;Jump to zero out SP if done clearing DMem.
0015:          ST       0,      0,      6			;Register 0 is always 0 after above zeroing of registers.
0016:         LDA       6,     -1,      6			;SP--.
0017:         LDA       7,     -4,      7			;Jump back to top of DMem clearing loop.
0018:         LDC       6,      0,      0			;Zeroing out SP.
0019:        HALT       0,      0,      0			;Exit point for program (tests/other/fac.tm).
0020:         LDC       0, -55555,      0			;Start of internal function "HANDLE_EXCEPTION_DIV_BY_ZERO".
0021:         OUT       0,      0,      0
0022:         LDA       7,    -16,      7			;Jump to program exit routine.
0023:         LDC       0, -44444,      0			;Start of internal function "HANDLE_EXCEPTION_DMEM".
0024:         OUT       0,      0,      0
0025:         LDA       7,    -19,      7			;Jump to program exit routine.
0026:         LDC       0, -33333,      0			;Start of internal function "HANDLE_EXCEPTION_IMEM".
0027:         OUT       0,      0,      0
0028:         LDA       7,    -22,      7			;Jump to program exit routine.
0029:          ST       0,      0,      6			;Start of callable function "input".
0030:         LDA       6,     -1,      6
0031:          IN       0,      0,      0
0032:          ST       0,      3,      6
0033:          LD       0,      1,      6
0034:         LDA       6,      1,      6
0035:          LD       7,      1,      6			;Returning from function "input".
0036:          ST       0,      0,      6			;Start of callable function "output".
0037:         LDA       6,     -1,      6
0038:          LD       0,      3,      6
0039:         OUT       0,      0,      0
0040:         LDA       6,      1,      6
0041:          LD       7,      1,      6
0042:          ST       0,      0,      6			;Start callable function "main". Saving registers on stack.
0043:         LDA       6,     -1,      6
0044:          ST       1,      0,      6
0045:         LDA       6,     -1,      6
0046:          ST       2,      0,      6
0047:         LDA       6,     -1,      6
0048:          ST       3,      0,      6
0049:         LDA       6,     -1,      6
0050:          ST       4,      0,      6
0051:         LDA       6,     -1,      6
0052:          ST       5,      0,      6
0053:         LDA       6,     -1,      6			;Done saving registers on the stack.
0054:         LDA       5,      0,      6			;Saving frame pointer for call to "main" into FP (R5).
0055:         LDA       6,     -3,      6			;Allocating space on the stack for (3) local variables.
0056:         LDA       0,      0,      5			;REGISTER0 = &x
0057:         LDA       6,     -1,      6			;Allocating space for return value from "input".
0058:         LDA       4,      3,      7			;Loading return address for call to "input" into REGISTER4.
0059:          ST       4,      0,      6			;Pushing return address for call to "input" to the stack.
0060:         LDA       6,     -1,      6			;SP++.
0061:         LDA       7,    -33,      7			;Jumping to "input".
0062:          LD       1,      2,      6			;Loading return value from call to "input" into REGISTER1.
0063:         LDA       6,      2,      6			;Cleanup from function call.
0064:          ST       1,      0,      0			;x = REGISTER1
0065:         LDA       0,      0,      1			;REGISTER0 = x. Saving result of expression.
0066:         LDA       0,     -2,      5			;REGISTER0 = &fac
0067:         LDC       1,      1,      0			;REGISTER1 = 1
0068:          ST       1,      0,      0			;fac = REGISTER1
0069:         LDA       0,      0,      1			;REGISTER0 = fac. Saving result of expression.
0070:          LD       0,      0,      5			;REGISTER0 = x
0071:         LDC       1,      1,      0			;REGISTER1 = 1
0072:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0073:         JLE       0,      2,      7			;if (REGISTER0 <= 0) jump to PC + 2.
0074:         LDC       0,      1,      0			;REGISTER0 = 1
0075:         LDA       7,      1,      7			;Jump to PC + 1.
0076:         LDC       0,      0,      0			;REGISTER0 = 0
0077:         JEQ       0,     13,      7			;if (REGISTER0 == 0) Jump to instruction after loop body (PC + 13)
0078:         LDA       0,     -2,      5			;REGISTER0 = &fac
0079:          LD       1,     -2,      5			;REGISTER1 = fac
0080:          LD       2,      0,      5			;REGISTER2 = x
0081:         MUL       1,      1,      2			;REGISTER1 *= REGISTER2
0082:          ST       1,      0,      0			;fac = REGISTER1
0083:         LDA       0,      0,      1			;REGISTER0 = fac. Saving result of expression.
0084:         LDA       0,      0,      5			;REGISTER0 = &x
0085:          LD       1,      0,      5			;REGISTER1 = x
0086:         LDC       2,      1,      0			;REGISTER2 = 1
0087:         SUB       1,      1,      2			;REGISTER1 -= REGISTER2
0088:          ST       1,      0,      0			;x = REGISTER1
0089:         LDA       0,      0,      1			;REGISTER0 = x. Saving result of expression.
0090:         LDA       7,    -21,      7			;Jump to top of loop (PC - 21).
0091:          LD       0,     -2,      5			;REGISTER0 = fac
0092:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 0 for call to "output".
0093:         LDA       6,     -1,      6			;SP++.
0094:         LDA       4,      3,      7			;Loading return address for call to "output" into REGISTER4.
0095:          ST       4,      0,      6			;Pushing return address for call to "output" to the stack.
0096:         LDA       6,     -1,      6			;SP++.
0097:         LDA       7,    -62,      7			;Jumping to "output".
0098:         LDA       6,      2,      6			;Cleanup from function call.
0099:         LDA       6,      3,      6			;Start of finale for "main". Deallocating local vars.
0100:          LD       5,      1,      6
0101:         LDA       6,      1,      6
0102:          LD       4,      1,      6
0103:         LDA       6,      1,      6
0104:          LD       3,      1,      6
0105:         LDA       6,      1,      6
0106:          LD       2,      1,      6
0107:         LDA       6,      1,      6
0108:          LD       1,      1,      6
0109:         LDA       6,      1,      6
0110:          LD       0,      1,      6
0111:         LDA       6,      1,      6			;Done restoring registers from stack.
0112:          LD       7,      1,      6			;Returning from function "main".
