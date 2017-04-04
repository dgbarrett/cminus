0000:          LD       6,      0,      0			;Loading SP with DMEM_MAX.
0001:          ST       0,      0,      0			;Zeroing out the top of DMem.
0002:         LDA       6,     -5,      6			;Allocating space on the stack for (1) global variables.
0003:         LDA       4,      3,      7			;Loading return address for call to "main" into REGISTER4.
0004:          ST       4,      0,      6			;Pushing return address for call to "main" to the stack.
0005:         LDA       6,     -1,      6			;SP++.
0006:         LDA       7,     36,      7			;Jumping to "main".
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
0020:        HALT       0,      0,      0			;Exit point for program (tests/other/arr.tm).
0021:         LDC       0, -55555,      0			;Start of internal function "HANDLE_EXCEPTION_DIV_BY_ZERO".
0022:         OUT       0,      0,      0
0023:         LDA       7,    -16,      7			;Jump to program exit routine.
0024:         LDC       0, -44444,      0			;Start of internal function "HANDLE_EXCEPTION_DMEM".
0025:         OUT       0,      0,      0
0026:         LDA       7,    -19,      7			;Jump to program exit routine.
0027:         LDC       0, -33333,      0			;Start of internal function "HANDLE_EXCEPTION_IMEM".
0028:         OUT       0,      0,      0
0029:         LDA       7,    -22,      7			;Jump to program exit routine.
0030:          ST       0,      0,      6			;Start of callable function "input".
0031:         LDA       6,     -1,      6
0032:          IN       0,      0,      0
0033:          ST       0,      3,      6
0034:          LD       0,      1,      6
0035:         LDA       6,      1,      6
0036:          LD       7,      1,      6			;Returning from function "input".
0037:          ST       0,      0,      6			;Start of callable function "output".
0038:         LDA       6,     -1,      6
0039:          LD       0,      3,      6
0040:         OUT       0,      0,      0
0041:         LDA       6,      1,      6
0042:          LD       7,      1,      6
0043:          ST       0,      0,      6			;Start callable function "main". Saving registers on stack.
0044:         LDA       6,     -1,      6
0045:          ST       1,      0,      6
0046:         LDA       6,     -1,      6
0047:          ST       2,      0,      6
0048:         LDA       6,     -1,      6
0049:          ST       3,      0,      6
0050:         LDA       6,     -1,      6
0051:          ST       4,      0,      6
0052:         LDA       6,     -1,      6
0053:          ST       5,      0,      6
0054:         LDA       6,     -1,      6			;Done saving registers on the stack.
0055:         LDA       5,      0,      6			;Saving frame pointer for call to "main" into FP (R5).
0056:         LDC       0,      0,      0			;REGISTER0 = &x
0057:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 0 for call to "printArr".
0058:         LDA       6,     -1,      6			;SP++.
0059:         LDA       4,      3,      7			;Loading return address for call to "printArr" into REGISTER4.
0060:          ST       4,      0,      6			;Pushing return address for call to "printArr" to the stack.
0061:         LDA       6,     -1,      6			;SP++.
0062:         LDA       7,     24,      7			;Jumping to "printArr".
0063:         LDA       6,      2,      6			;Cleanup from function call.
0064:         LDC       0,      0,      0			;REGISTER0 = &x
0065:         LDA       0,      1,      0			;Moving to correct positon in array.
0066:          LD       0,      0,      0			;Loading array element value.
0067:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 0 for call to "output".
0068:         LDA       6,     -1,      6			;SP++.
0069:         LDA       4,      3,      7			;Loading return address for call to "output" into REGISTER4.
0070:          ST       4,      0,      6			;Pushing return address for call to "output" to the stack.
0071:         LDA       6,     -1,      6			;SP++.
0072:         LDA       7,    -36,      7			;Jumping to "output".
0073:         LDA       6,      2,      6			;Cleanup from function call.
0074:          LD       5,      1,      6			;Start of finale for "main". Restoring registers.
0075:         LDA       6,      1,      6
0076:          LD       4,      1,      6
0077:         LDA       6,      1,      6
0078:          LD       3,      1,      6
0079:         LDA       6,      1,      6
0080:          LD       2,      1,      6
0081:         LDA       6,      1,      6
0082:          LD       1,      1,      6
0083:         LDA       6,      1,      6
0084:          LD       0,      1,      6
0085:         LDA       6,      1,      6			;Done restoring registers from stack.
0086:          LD       7,      1,      6			;Returning from function "main".
0087:          ST       0,      0,      6			;Start callable function "printArr". Saving registers on stack.
0088:         LDA       6,     -1,      6
0089:          ST       1,      0,      6
0090:         LDA       6,     -1,      6
0091:          ST       2,      0,      6
0092:         LDA       6,     -1,      6
0093:          ST       3,      0,      6
0094:         LDA       6,     -1,      6
0095:          ST       4,      0,      6
0096:         LDA       6,     -1,      6
0097:          ST       5,      0,      6
0098:         LDA       6,     -1,      6			;Done saving registers on the stack.
0099:         LDA       5,      0,      6			;Saving frame pointer for call to "printArr" into FP (R5).
0100:          LD       0,      8,      5			;REGISTER0 = &arr
0101:         LDC       1,      1,      0			;REGISTER1 = 1
0102:         ADD       0,      0,      1			;REGISTER0 += REGISTER1
0103:         LDC       1,     12,      0			;REGISTER1 = 12
0104:          ST       1,      0,      0			;arr[1] = REGISTER1
0105:         LDA       0,      0,      1			;REGISTER0 = arr[1]. Saving result of expression.
0106:          LD       5,      1,      6			;Start of finale for "printArr". Restoring registers.
0107:         LDA       6,      1,      6
0108:          LD       4,      1,      6
0109:         LDA       6,      1,      6
0110:          LD       3,      1,      6
0111:         LDA       6,      1,      6
0112:          LD       2,      1,      6
0113:         LDA       6,      1,      6
0114:          LD       1,      1,      6
0115:         LDA       6,      1,      6
0116:          LD       0,      1,      6
0117:         LDA       6,      1,      6			;Done restoring registers from stack.
0118:          LD       7,      1,      6			;Returning from function "printArr".
