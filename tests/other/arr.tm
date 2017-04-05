0000:          LD       6,      0,      0			;Loading SP with DMEM_MAX.
0001:          ST       0,      0,      0			;Zeroing out the top of DMem.
0002:         LDA       4,      3,      7			;Loading return address for call to "main" into REGISTER4.
0003:          ST       4,      0,      6			;Pushing return address for call to "main" to the stack.
0004:         LDA       6,     -1,      6			;SP++.
0005:         LDA       7,     33,      7			;Jumping to "main".
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
0019:        HALT       0,      0,      0			;Exit point for program (tests/other/arr.tm).
0020:         LDC       0,-5555555,      0			;Start of internal function "HANDLE_EXCEPTION_DIV_BY_ZERO".
0021:         OUT       0,      0,      0
0022:         LDA       7,    -16,      7			;Jump to program exit routine.
0023:         LDC       0,-4444444,      0			;Start of internal function "HANDLE_EXCEPTION_ARRAY_OOB".
0024:         OUT       0,      0,      0
0025:         LDA       7,    -19,      7			;Jump to program exit routine.
0026:          ST       0,      0,      6			;Start of callable function "input".
0027:         LDA       6,     -1,      6
0028:          IN       0,      0,      0
0029:          ST       0,      3,      6
0030:          LD       0,      1,      6
0031:         LDA       6,      1,      6
0032:          LD       7,      1,      6			;Returning from function "input".
0033:          ST       0,      0,      6			;Start of callable function "output".
0034:         LDA       6,     -1,      6
0035:          LD       0,      3,      6
0036:         OUT       0,      0,      0
0037:         LDA       6,      1,      6
0038:          LD       7,      1,      6
0039:          ST       0,      0,      6			;Start callable function "main". Saving registers on stack.
0040:         LDA       6,     -1,      6
0041:          ST       1,      0,      6
0042:         LDA       6,     -1,      6
0043:          ST       2,      0,      6
0044:         LDA       6,     -1,      6
0045:          ST       3,      0,      6
0046:         LDA       6,     -1,      6
0047:          ST       4,      0,      6
0048:         LDA       6,     -1,      6
0049:          ST       5,      0,      6
0050:         LDA       6,     -1,      6			;Done saving registers on the stack.
0051:         LDA       5,      0,      6			;Saving frame pointer for call to "main" into FP (R5).
0052:         LDA       6,     -6,      6			;Allocating space on the stack for (2) local variables.
0053:         LDA       0,     -5,      5			;REGISTER0 = &y
0054:         LDC       1,      5,      0			;REGISTER1 = 5
0055:          ST       1,      0,      0			;y = REGISTER1
0056:         LDA       0,      0,      1			;REGISTER0 = y. Saving result of expression.
0057:         LDA       0,      0,      5			;REGISTER0 = &x
0058:          LD       1,     -5,      5			;REGISTER1 = y
0059:         LDC       2,      5,      0			;REGISTER2 = len(x)
0060:         SUB       2,      2,      1			;REGISTER2 -= REGISTER1
0061:         JLE       2,    -39,      7			;Jumping to "HANDLE_EXCEPTION_ARRAY_OOB".
0062:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0063:         LDC       1,     10,      0			;REGISTER1 = 10
0064:          ST       1,      0,      0			;x[y] = REGISTER1
0065:         LDA       0,      0,      1			;REGISTER0 = x[y]. Saving result of expression.
0066:         LDA       0,      0,      5			;REGISTER0 = &x
0067:          LD       1,     -5,      5			;REGISTER1 = y
0068:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0069:          LD       0,      0,      0			;REGISTER0 = x[y]
0070:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 0 for call to "output".
0071:         LDA       6,     -1,      6			;SP++.
0072:         LDA       4,      3,      7			;Loading return address for call to "output" into REGISTER4.
0073:          ST       4,      0,      6			;Pushing return address for call to "output" to the stack.
0074:         LDA       6,     -1,      6			;SP++.
0075:         LDA       7,    -43,      7			;Jumping to "output".
0076:         LDA       6,      2,      6			;Cleanup from function call.
0077:         LDA       6,      6,      6			;Start of finale for "main". Deallocating local vars.
0078:          LD       5,      1,      6
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
