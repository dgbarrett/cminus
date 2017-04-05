0000:          LD       6,      0,      0			;Loading SP with DMEM_MAX.
0001:          ST       0,      0,      0			;Zeroing out the top of DMem.
0002:         LDA       6,    -11,      6			;Allocating space on the stack for (2) global variables.
0003:         LDA       4,      3,      7			;Loading return address for call to "main" into REGISTER4.
0004:          ST       4,      0,      6			;Pushing return address for call to "main" to the stack.
0005:         LDA       6,     -1,      6			;SP++.
0006:         LDA       7,     33,      7			;Jumping to "main".
0007:         LDA       6,      1,      6			;Cleanup from function call.
0008:         LDC       0,      0,      0			;Start of program exit routine.
0009:         LDC       1,      0,      0			;Zeroing out lower 6 registers.
0010:         LDC       2,      0,      0
0011:         LDC       3,      0,      0
0012:         LDC       4,      0,      0
0013:         LDC       5,      0,      0
0014:         LDC       6,   1023,      0			;Zeroing out DMem. SP = DMEM_MAX.
0015:         JLT       6,      3,      7			;Jump to zero out SP if done clearing DMem.
0016:          ST       0,      0,      6			;Register 0 is always 0 after above zeroing of registers.
0017:         LDA       6,     -1,      6			;SP--.
0018:         LDA       7,     -4,      7			;Jump back to top of DMem clearing loop.
0019:         LDC       6,      0,      0			;Zeroing out SP.
0020:        HALT       0,      0,      0			;Exit point for program (tests/other/global.tm).
0021:         LDC       0, -55555,      0			;Start of internal function "HANDLE_EXCEPTION_DIV_BY_ZERO".
0022:         OUT       0,      0,      0
0023:         LDA       7,    -16,      7			;Jump to program exit routine.
0024:         LDC       0, -44444,      0			;Start of internal function "HANDLE_EXCEPTION_ARRAY_OOB".
0025:         OUT       0,      0,      0
0026:         LDA       7,    -19,      7			;Jump to program exit routine.
0027:          ST       0,      0,      6			;Start of callable function "input".
0028:         LDA       6,     -1,      6
0029:          IN       0,      0,      0
0030:          ST       0,      3,      6
0031:          LD       0,      1,      6
0032:         LDA       6,      1,      6
0033:          LD       7,      1,      6			;Returning from function "input".
0034:          ST       0,      0,      6			;Start of callable function "output".
0035:         LDA       6,     -1,      6
0036:          LD       0,      3,      6
0037:         OUT       0,      0,      0
0038:         LDA       6,      1,      6
0039:          LD       7,      1,      6
0040:          ST       0,      0,      6			;Start callable function "main". Saving registers on stack.
0041:         LDA       6,     -1,      6
0042:          ST       1,      0,      6
0043:         LDA       6,     -1,      6
0044:          ST       2,      0,      6
0045:         LDA       6,     -1,      6
0046:          ST       3,      0,      6
0047:         LDA       6,     -1,      6
0048:          ST       4,      0,      6
0049:         LDA       6,     -1,      6
0050:          ST       5,      0,      6
0051:         LDA       6,     -1,      6			;Done saving registers on the stack.
0052:         LDA       5,      0,      6			;Saving frame pointer for call to "main" into FP (R5).
0053:         LDC       0,   1013,      0			;REGISTER0 = &itr
0054:         LDC       1,      3,      0			;REGISTER1 = 3
0055:          ST       1,      0,      0			;itr = REGISTER1
0056:         LDA       0,      0,      1			;REGISTER0 = itr. Saving result of expression.
0057:         LDC       0,   1023,      0			;REGISTER0 = &ints
0058:         LDC       1,   1013,      0			;REGISTER1 = &itr
0059:          LD       1,      0,      1			;REGISTER1 = &itr
0060:         LDC       2,     10,      0			;REGISTER2 = len(ints)
0061:         SUB       2,      2,      1			;REGISTER2 -= REGISTER1
0062:         JLE       2,    -39,      7			;Jumping to "HANDLE_EXCEPTION_ARRAY_OOB".
0063:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0064:         LDC       1,     33,      0			;REGISTER1 = 33
0065:          ST       1,      0,      0			;ints[itr] = REGISTER1
0066:         LDA       0,      0,      1			;REGISTER0 = ints[itr]. Saving result of expression.
0067:         LDC       0,   1023,      0			;REGISTER0 = &ints
0068:         LDC       1,      5,      0			;REGISTER1 = 5
0069:         LDC       2,     10,      0			;REGISTER2 = len(ints)
0070:         SUB       2,      2,      1			;REGISTER2 -= REGISTER1
0071:         JLE       2,    -48,      7			;Jumping to "HANDLE_EXCEPTION_ARRAY_OOB".
0072:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0073:         LDC       1,     77,      0			;REGISTER1 = 77
0074:          ST       1,      0,      0			;ints[5] = REGISTER1
0075:         LDA       0,      0,      1			;REGISTER0 = ints[5]. Saving result of expression.
0076:         LDC       0,   1023,      0			;REGISTER0 = &ints
0077:         LDC       1,      6,      0			;REGISTER1 = 6
0078:         LDC       2,     10,      0			;REGISTER2 = len(ints)
0079:         SUB       2,      2,      1			;REGISTER2 -= REGISTER1
0080:         JLE       2,    -57,      7			;Jumping to "HANDLE_EXCEPTION_ARRAY_OOB".
0081:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0082:         LDC       1,    777,      0			;REGISTER1 = 777
0083:          ST       1,      0,      0			;ints[6] = REGISTER1
0084:         LDA       0,      0,      1			;REGISTER0 = ints[6]. Saving result of expression.
0085:         LDC       0,   1023,      0			;REGISTER0 = &ints
0086:         LDC       1,      9,      0			;REGISTER1 = 9
0087:         LDC       2,     10,      0			;REGISTER2 = len(ints)
0088:         SUB       2,      2,      1			;REGISTER2 -= REGISTER1
0089:         JLE       2,    -66,      7			;Jumping to "HANDLE_EXCEPTION_ARRAY_OOB".
0090:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0091:         LDC       1,     39,      0			;REGISTER1 = 39
0092:          ST       1,      0,      0			;ints[9] = REGISTER1
0093:         LDA       0,      0,      1			;REGISTER0 = ints[9]. Saving result of expression.
0094:         LDC       0,   1023,      0			;REGISTER0 = &ints
0095:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 0 for call to "printArr".
0096:         LDA       6,     -1,      6			;SP++.
0097:         LDA       4,      3,      7			;Loading return address for call to "printArr" into REGISTER4.
0098:          ST       4,      0,      6			;Pushing return address for call to "printArr" to the stack.
0099:         LDA       6,     -1,      6			;SP++.
0100:         LDA       7,     14,      7			;Jumping to "printArr".
0101:         LDA       6,      2,      6			;Cleanup from function call.
0102:          LD       5,      1,      6			;Start of finale for "main". Restoring registers.
0103:         LDA       6,      1,      6
0104:          LD       4,      1,      6
0105:         LDA       6,      1,      6
0106:          LD       3,      1,      6
0107:         LDA       6,      1,      6
0108:          LD       2,      1,      6
0109:         LDA       6,      1,      6
0110:          LD       1,      1,      6
0111:         LDA       6,      1,      6
0112:          LD       0,      1,      6
0113:         LDA       6,      1,      6			;Done restoring registers from stack.
0114:          LD       7,      1,      6			;Returning from function "main".
0115:          ST       0,      0,      6			;Start callable function "printArr". Saving registers on stack.
0116:         LDA       6,     -1,      6
0117:          ST       1,      0,      6
0118:         LDA       6,     -1,      6
0119:          ST       2,      0,      6
0120:         LDA       6,     -1,      6
0121:          ST       3,      0,      6
0122:         LDA       6,     -1,      6
0123:          ST       4,      0,      6
0124:         LDA       6,     -1,      6
0125:          ST       5,      0,      6
0126:         LDA       6,     -1,      6			;Done saving registers on the stack.
0127:         LDA       5,      0,      6			;Saving frame pointer for call to "printArr" into FP (R5).
0128:         LDC       0,   1013,      0			;REGISTER0 = &itr
0129:          LD       0,      0,      0			;REGISTER0 = &itr
0130:         LDC       1,     10,      0			;REGISTER1 = 10
0131:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0132:         JGE       0,      2,      7			;if (REGISTER0 >= 0) jump to PC + 2.
0133:         LDC       0,      1,      0			;REGISTER0 = 1
0134:         LDA       7,      1,      7			;Jump to PC + 1.
0135:         LDC       0,      0,      0			;REGISTER0 = 0
0136:         JEQ       0,     20,      7			;if (REGISTER0 == 0) Jump to instruction after loop body (PC + 20)
0137:          LD       0,      8,      5			;*REGISTER0 = &arr
0138:         LDC       1,   1013,      0			;REGISTER1 = &itr
0139:          LD       1,      0,      1			;REGISTER1 = &itr
0140:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0141:          LD       0,      0,      0			;REGISTER0 = arr[itr]
0142:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 0 for call to "output".
0143:         LDA       6,     -1,      6			;SP++.
0144:         LDA       4,      3,      7			;Loading return address for call to "output" into REGISTER4.
0145:          ST       4,      0,      6			;Pushing return address for call to "output" to the stack.
0146:         LDA       6,     -1,      6			;SP++.
0147:         LDA       7,   -114,      7			;Jumping to "output".
0148:         LDA       6,      2,      6			;Cleanup from function call.
0149:         LDC       0,   1013,      0			;REGISTER0 = &itr
0150:         LDC       1,   1013,      0			;REGISTER1 = &itr
0151:          LD       1,      0,      1			;REGISTER1 = &itr
0152:         LDC       2,      1,      0			;REGISTER2 = 1
0153:         ADD       1,      1,      2			;REGISTER1 += REGISTER2
0154:          ST       1,      0,      0			;itr = REGISTER1
0155:         LDA       0,      0,      1			;REGISTER0 = itr. Saving result of expression.
0156:         LDA       7,    -29,      7			;Jump to top of loop (PC - 29).
0157:          LD       5,      1,      6			;Start of finale for "printArr". Restoring registers.
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
0169:          LD       7,      1,      6			;Returning from function "printArr".
