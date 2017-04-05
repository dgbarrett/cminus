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
0019:        HALT       0,      0,      0			;Exit point for program (tests/other/mod.tm).
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
0055:         LDA       6,     -1,      6			;Allocating space for return value from "input".
0056:         LDA       4,      3,      7			;Loading return address for call to "input" into REGISTER4.
0057:          ST       4,      0,      6			;Pushing return address for call to "input" to the stack.
0058:         LDA       6,     -1,      6			;SP++.
0059:         LDA       7,    -31,      7			;Jumping to "input".
0060:          LD       0,      2,      6			;Loading return value from call to "input" into REGISTER0.
0061:         LDA       6,      2,      6			;Cleanup from function call.
0062:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 0 for call to "isOdd".
0063:         LDA       6,     -1,      6			;SP++.
0064:         LDA       6,     -1,      6			;Allocating space for return value from "isOdd".
0065:         LDA       4,      3,      7			;Loading return address for call to "isOdd" into REGISTER4.
0066:          ST       4,      0,      6			;Pushing return address for call to "isOdd" to the stack.
0067:         LDA       6,     -1,      6			;SP++.
0068:         LDA       7,     22,      7			;Jumping to "isOdd".
0069:          LD       0,      2,      6			;Loading return value from call to "isOdd" into REGISTER0.
0070:         LDA       6,      3,      6			;Cleanup from function call.
0071:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 0 for call to "output".
0072:         LDA       6,     -1,      6			;SP++.
0073:         LDA       4,      3,      7			;Loading return address for call to "output" into REGISTER4.
0074:          ST       4,      0,      6			;Pushing return address for call to "output" to the stack.
0075:         LDA       6,     -1,      6			;SP++.
0076:         LDA       7,    -41,      7			;Jumping to "output".
0077:         LDA       6,      2,      6			;Cleanup from function call.
0078:          LD       5,      1,      6			;Start of finale for "main". Restoring registers.
0079:         LDA       6,      1,      6
0080:          LD       4,      1,      6
0081:         LDA       6,      1,      6
0082:          LD       3,      1,      6
0083:         LDA       6,      1,      6
0084:          LD       2,      1,      6
0085:         LDA       6,      1,      6
0086:          LD       1,      1,      6
0087:         LDA       6,      1,      6
0088:          LD       0,      1,      6
0089:         LDA       6,      1,      6			;Done restoring registers from stack.
0090:          LD       7,      1,      6			;Returning from function "main".
0091:          ST       0,      0,      6			;Start callable function "isOdd". Saving registers on stack.
0092:         LDA       6,     -1,      6
0093:          ST       1,      0,      6
0094:         LDA       6,     -1,      6
0095:          ST       2,      0,      6
0096:         LDA       6,     -1,      6
0097:          ST       3,      0,      6
0098:         LDA       6,     -1,      6
0099:          ST       4,      0,      6
0100:         LDA       6,     -1,      6
0101:          ST       5,      0,      6
0102:         LDA       6,     -1,      6			;Done saving registers on the stack.
0103:         LDA       5,      0,      6			;Saving frame pointer for call to "isOdd" into FP (R5).
0104:         LDA       6,     -1,      6			;Allocating space on the stack for (1) local variables.
0105:         LDA       0,      0,      5			;REGISTER0 = &temp
0106:          LD       1,      9,      5			;REGISTER1 = x
0107:         LDC       2,      2,      0			;REGISTER2 = 2
0108:         JEQ       2,    -89,      7			;Divide by zero check.
0109:         DIV       1,      1,      2			;REGISTER1 /= REGISTER2
0110:          ST       1,      0,      0			;temp = REGISTER1
0111:         LDA       0,      0,      1			;REGISTER0 = temp. Saving result of expression.
0112:         LDA       0,      0,      5			;REGISTER0 = &temp
0113:          LD       1,      0,      5			;REGISTER1 = temp
0114:         LDC       2,      2,      0			;REGISTER2 = 2
0115:         MUL       1,      1,      2			;REGISTER1 *= REGISTER2
0116:          ST       1,      0,      0			;temp = REGISTER1
0117:         LDA       0,      0,      1			;REGISTER0 = temp. Saving result of expression.
0118:          LD       0,      9,      5			;REGISTER0 = x
0119:          LD       1,      0,      5			;REGISTER1 = temp
0120:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0121:          ST       0,      8,      5			;Storing value returned from "isOdd" on stack.
0122:         LDA       7,      0,      7			;Jumping to finale for "isOdd".
0123:         LDA       6,      1,      6			;Start of finale for "isOdd". Deallocating local vars.
0124:          LD       5,      1,      6
0125:         LDA       6,      1,      6
0126:          LD       4,      1,      6
0127:         LDA       6,      1,      6
0128:          LD       3,      1,      6
0129:         LDA       6,      1,      6
0130:          LD       2,      1,      6
0131:         LDA       6,      1,      6
0132:          LD       1,      1,      6
0133:         LDA       6,      1,      6
0134:          LD       0,      1,      6
0135:         LDA       6,      1,      6			;Done restoring registers from stack.
0136:          LD       7,      1,      6			;Returning from function "isOdd".
