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
0019:        HALT       0,      0,      0			;Exit point for program (tests/other/gcd.tm).
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
0055:         LDA       6,     -2,      6			;Allocating space on the stack for (2) local variables.
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
0066:         LDA       0,     -1,      5			;REGISTER0 = &y
0067:         LDA       6,     -1,      6			;Allocating space for return value from "input".
0068:         LDA       4,      3,      7			;Loading return address for call to "input" into REGISTER4.
0069:          ST       4,      0,      6			;Pushing return address for call to "input" to the stack.
0070:         LDA       6,     -1,      6			;SP++.
0071:         LDA       7,    -43,      7			;Jumping to "input".
0072:          LD       1,      2,      6			;Loading return value from call to "input" into REGISTER1.
0073:         LDA       6,      2,      6			;Cleanup from function call.
0074:          ST       1,      0,      0			;y = REGISTER1
0075:         LDA       0,      0,      1			;REGISTER0 = y. Saving result of expression.
0076:          LD       0,      0,      5			;REGISTER0 = x
0077:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 0 for call to "gcd".
0078:         LDA       6,     -1,      6			;SP++.
0079:          LD       0,     -1,      5			;REGISTER0 = y
0080:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 1 for call to "gcd".
0081:         LDA       6,     -1,      6			;SP++.
0082:         LDA       6,     -1,      6			;Allocating space for return value from "gcd".
0083:         LDA       4,      3,      7			;Loading return address for call to "gcd" into REGISTER4.
0084:          ST       4,      0,      6			;Pushing return address for call to "gcd" to the stack.
0085:         LDA       6,     -1,      6			;SP++.
0086:         LDA       7,     23,      7			;Jumping to "gcd".
0087:          LD       0,      2,      6			;Loading return value from call to "gcd" into REGISTER0.
0088:         LDA       6,      4,      6			;Cleanup from function call.
0089:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 0 for call to "output".
0090:         LDA       6,     -1,      6			;SP++.
0091:         LDA       4,      3,      7			;Loading return address for call to "output" into REGISTER4.
0092:          ST       4,      0,      6			;Pushing return address for call to "output" to the stack.
0093:         LDA       6,     -1,      6			;SP++.
0094:         LDA       7,    -59,      7			;Jumping to "output".
0095:         LDA       6,      2,      6			;Cleanup from function call.
0096:         LDA       6,      2,      6			;Start of finale for "main". Deallocating local vars.
0097:          LD       5,      1,      6
0098:         LDA       6,      1,      6
0099:          LD       4,      1,      6
0100:         LDA       6,      1,      6
0101:          LD       3,      1,      6
0102:         LDA       6,      1,      6
0103:          LD       2,      1,      6
0104:         LDA       6,      1,      6
0105:          LD       1,      1,      6
0106:         LDA       6,      1,      6
0107:          LD       0,      1,      6
0108:         LDA       6,      1,      6			;Done restoring registers from stack.
0109:          LD       7,      1,      6			;Returning from function "main".
0110:          ST       0,      0,      6			;Start callable function "gcd". Saving registers on stack.
0111:         LDA       6,     -1,      6
0112:          ST       1,      0,      6
0113:         LDA       6,     -1,      6
0114:          ST       2,      0,      6
0115:         LDA       6,     -1,      6
0116:          ST       3,      0,      6
0117:         LDA       6,     -1,      6
0118:          ST       4,      0,      6
0119:         LDA       6,     -1,      6
0120:          ST       5,      0,      6
0121:         LDA       6,     -1,      6			;Done saving registers on the stack.
0122:         LDA       5,      0,      6			;Saving frame pointer for call to "gcd" into FP (R5).
0123:          LD       0,      9,      5			;REGISTER0 = v
0124:         LDC       1,      0,      0			;REGISTER1 = 0
0125:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0126:         JNE       0,      2,      7			;if (REGISTER0 != 0) jump to PC + 2.
0127:         LDC       0,      1,      0			;REGISTER0 = 1
0128:         LDA       7,      1,      7			;Jump to PC + 1.
0129:         LDC       0,      0,      0			;REGISTER0 = 0
0130:         JEQ       0,      4,      7			;if (REGISTER0 == 0) Jump to else body at PC + 4.
0131:          LD       0,     10,      5			;REGISTER0 = u
0132:          ST       0,      8,      5			;Storing value returned from "gcd" on stack.
0133:         LDA       7,     23,      7			;Jumping to finale for "gcd".
0134:         LDA       7,     22,      7			;Jump to instruction after if/else statement (PC + 22).
0135:          LD       0,      9,      5			;REGISTER0 = v
0136:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 0 for call to "gcd".
0137:         LDA       6,     -1,      6			;SP++.
0138:          LD       0,     10,      5			;REGISTER0 = u
0139:          LD       1,     10,      5			;REGISTER1 = u
0140:          LD       2,      9,      5			;REGISTER2 = v
0141:         JEQ       2,   -122,      7			;Divide by zero check.
0142:         DIV       1,      1,      2			;REGISTER1 /= REGISTER2
0143:          LD       2,      9,      5			;REGISTER2 = v
0144:         MUL       1,      1,      2			;REGISTER1 *= REGISTER2
0145:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0146:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 1 for call to "gcd".
0147:         LDA       6,     -1,      6			;SP++.
0148:         LDA       6,     -1,      6			;Allocating space for return value from "gcd".
0149:         LDA       4,      3,      7			;Loading return address for call to "gcd" into REGISTER4.
0150:          ST       4,      0,      6			;Pushing return address for call to "gcd" to the stack.
0151:         LDA       6,     -1,      6			;SP++.
0152:         LDA       7,    -43,      7			;Jumping to "gcd".
0153:          LD       0,      2,      6			;Loading return value from call to "gcd" into REGISTER0.
0154:         LDA       6,      4,      6			;Cleanup from function call.
0155:          ST       0,      8,      5			;Storing value returned from "gcd" on stack.
0156:         LDA       7,      0,      7			;Jumping to finale for "gcd".
0157:          LD       5,      1,      6			;Start of finale for "gcd". Restoring registers.
0158:         LDA       6,      1,      6
0159:          LD       4,      1,      6
0160:         LDA       6,      1,      6
0161:          LD       3,      1,      6
0162:         LDA       6,      1,      6
0163:          LD       2,      1,      6
0164:         LDA       6,      1,      6
0165:          LD       1,      1,      6
0166:         LDA       6,      1,      6
0167:          LD       0,      1,      6
0168:         LDA       6,      1,      6			;Done restoring registers from stack.
0169:          LD       7,      1,      6			;Returning from function "gcd".
