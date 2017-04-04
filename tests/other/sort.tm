0000:          LD       6,      0,      0			;Loading SP with DMEM_MAX.
0001:          ST       0,      0,      0			;Zeroing out the top of DMem.
0002:         LDA       6,    -10,      6			;Allocating space on the stack for (1) global variables.
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
0020:        HALT       0,      0,      0			;Exit point for program (tests/other/sort.tm).
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
0056:         LDA       6,     -1,      6			;Allocating space on the stack for (1) local variables.
0057:         LDA       0,      0,      5			;REGISTER0 = &i
0058:         LDC       1,      0,      0			;REGISTER1 = 0
0059:          ST       1,      0,      0			;i = REGISTER1
0060:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0061:          LD       0,      0,      5			;REGISTER0 = i
0062:         LDC       1,     10,      0			;REGISTER1 = 10
0063:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0064:         JGE       0,      2,      7			;if (REGISTER0 >= 0) jump to PC + 2.
0065:         LDC       0,      1,      0			;REGISTER0 = 1
0066:         LDA       7,      1,      7			;Jump to PC + 1.
0067:         LDC       0,      0,      0			;REGISTER0 = 0
0068:         JEQ       0,     19,      7			;if (REGISTER0 == 0) Jump to instruction after loop body (PC + 19)
0069:         LDC       0,      0,      0			;REGISTER0 = &x
0070:          LD       1,      0,      5			;REGISTER1 = i
0071:         ADD       0,      0,      1			;REGISTER0 += REGISTER1
0072:         LDA       6,     -1,      6			;Allocating space for return value from "input".
0073:         LDA       4,      3,      7			;Loading return address for call to "input" into REGISTER4.
0074:          ST       4,      0,      6			;Pushing return address for call to "input" to the stack.
0075:         LDA       6,     -1,      6			;SP++.
0076:         LDA       7,    -47,      7			;Jumping to "input".
0077:          LD       1,      2,      6			;Loading return value from call to "input" into REGISTER1.
0078:         LDA       6,      2,      6			;Cleanup from function call.
0079:          ST       1,      0,      0			;x[i] = REGISTER1
0080:         LDA       0,      0,      1			;REGISTER0 = x[i]. Saving result of expression.
0081:         LDA       0,      0,      5			;REGISTER0 = &i
0082:          LD       1,      0,      5			;REGISTER1 = i
0083:         LDC       2,      1,      0			;REGISTER2 = 1
0084:         ADD       1,      1,      2			;REGISTER1 += REGISTER2
0085:          ST       1,      0,      0			;i = REGISTER1
0086:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0087:         LDA       7,    -27,      7			;Jump to top of loop (PC - 27).
0088:         LDC       0,      0,      0			;REGISTER0 = &x
0089:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 0 for call to "sort".
0090:         LDA       6,     -1,      6			;SP++.
0091:         LDC       0,      0,      0			;REGISTER0 = 0
0092:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 1 for call to "sort".
0093:         LDA       6,     -1,      6			;SP++.
0094:         LDC       0,     10,      0			;REGISTER0 = 10
0095:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 2 for call to "sort".
0096:         LDA       6,     -1,      6			;SP++.
0097:         LDA       4,      3,      7			;Loading return address for call to "sort" into REGISTER4.
0098:          ST       4,      0,      6			;Pushing return address for call to "sort" to the stack.
0099:         LDA       6,     -1,      6			;SP++.
0100:         LDA       7,    126,      7			;Jumping to "sort".
0101:         LDA       6,      4,      6			;Cleanup from function call.
0102:         LDA       0,      0,      5			;REGISTER0 = &i
0103:         LDC       1,      0,      0			;REGISTER1 = 0
0104:          ST       1,      0,      0			;i = REGISTER1
0105:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0106:          LD       0,      0,      5			;REGISTER0 = i
0107:         LDC       1,     10,      0			;REGISTER1 = 10
0108:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0109:         JGE       0,      2,      7			;if (REGISTER0 >= 0) jump to PC + 2.
0110:         LDC       0,      1,      0			;REGISTER0 = 1
0111:         LDA       7,      1,      7			;Jump to PC + 1.
0112:         LDC       0,      0,      0			;REGISTER0 = 0
0113:         JEQ       0,     17,      7			;if (REGISTER0 == 0) Jump to instruction after loop body (PC + 17)
0114:         LDC       0,      0,      0			;REGISTER0 = &x
0115:          LD       1,      0,      5			;REGISTER1 = i
0116:          LD       0,      0,      1			;Loading array element value.
0117:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 0 for call to "output".
0118:         LDA       6,     -1,      6			;SP++.
0119:         LDA       4,      3,      7			;Loading return address for call to "output" into REGISTER4.
0120:          ST       4,      0,      6			;Pushing return address for call to "output" to the stack.
0121:         LDA       6,     -1,      6			;SP++.
0122:         LDA       7,    -86,      7			;Jumping to "output".
0123:         LDA       6,      2,      6			;Cleanup from function call.
0124:         LDA       0,      0,      5			;REGISTER0 = &i
0125:          LD       1,      0,      5			;REGISTER1 = i
0126:         LDC       2,      1,      0			;REGISTER2 = 1
0127:         ADD       1,      1,      2			;REGISTER1 += REGISTER2
0128:          ST       1,      0,      0			;i = REGISTER1
0129:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0130:         LDA       7,    -25,      7			;Jump to top of loop (PC - 25).
0131:         LDA       6,      1,      6			;Start of finale for "main". Deallocating local vars.
0132:          LD       5,      1,      6
0133:         LDA       6,      1,      6
0134:          LD       4,      1,      6
0135:         LDA       6,      1,      6
0136:          LD       3,      1,      6
0137:         LDA       6,      1,      6
0138:          LD       2,      1,      6
0139:         LDA       6,      1,      6
0140:          LD       1,      1,      6
0141:         LDA       6,      1,      6
0142:          LD       0,      1,      6
0143:         LDA       6,      1,      6			;Done restoring registers from stack.
0144:          LD       7,      1,      6			;Returning from function "main".
0145:          ST       0,      0,      6			;Start callable function "minloc". Saving registers on stack.
0146:         LDA       6,     -1,      6
0147:          ST       1,      0,      6
0148:         LDA       6,     -1,      6
0149:          ST       2,      0,      6
0150:         LDA       6,     -1,      6
0151:          ST       3,      0,      6
0152:         LDA       6,     -1,      6
0153:          ST       4,      0,      6
0154:         LDA       6,     -1,      6
0155:          ST       5,      0,      6
0156:         LDA       6,     -1,      6			;Done saving registers on the stack.
0157:         LDA       5,      0,      6			;Saving frame pointer for call to "minloc" into FP (R5).
0158:         LDA       6,     -3,      6			;Allocating space on the stack for (3) local variables.
0159:         LDA       0,     -2,      5			;REGISTER0 = &k
0160:          LD       1,     10,      5			;REGISTER1 = low
0161:          ST       1,      0,      0			;k = REGISTER1
0162:         LDA       0,      0,      1			;REGISTER0 = k. Saving result of expression.
0163:         LDA       0,     -1,      5			;REGISTER0 = &x
0164:          LD       1,     11,      5			;*REGISTER1 = &a
0165:          LD       2,     10,      5			;REGISTER2 = low
0166:          LD       1,      0,      2			;Loading array element value.
0167:          ST       1,      0,      0			;x = REGISTER1
0168:         LDA       0,      0,      1			;REGISTER0 = x. Saving result of expression.
0169:         LDA       0,      0,      5			;REGISTER0 = &i
0170:          LD       1,     10,      5			;REGISTER1 = low
0171:         LDC       2,      1,      0			;REGISTER2 = 1
0172:         ADD       1,      1,      2			;REGISTER1 += REGISTER2
0173:          ST       1,      0,      0			;i = REGISTER1
0174:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0175:          LD       0,      0,      5			;REGISTER0 = i
0176:          LD       1,      9,      5			;REGISTER1 = high
0177:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0178:         JGE       0,      2,      7			;if (REGISTER0 >= 0) jump to PC + 2.
0179:         LDC       0,      1,      0			;REGISTER0 = 1
0180:         LDA       7,      1,      7			;Jump to PC + 1.
0181:         LDC       0,      0,      0			;REGISTER0 = 0
0182:         JEQ       0,     27,      7			;if (REGISTER0 == 0) Jump to instruction after loop body (PC + 27)
0183:          LD       0,     11,      5			;*REGISTER0 = &a
0184:          LD       1,      0,      5			;REGISTER1 = i
0185:          LD       0,      0,      1			;Loading array element value.
0186:          LD       1,     -1,      5			;REGISTER1 = x
0187:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0188:         JGE       0,      2,      7			;if (REGISTER0 >= 0) jump to PC + 2.
0189:         LDC       0,      1,      0			;REGISTER0 = 1
0190:         LDA       7,      1,      7			;Jump to PC + 1.
0191:         LDC       0,      0,      0			;REGISTER0 = 0
0192:         JEQ       0,     10,      7			;if (REGISTER0 == 0) Jump to instruction after if statement (PC + 10).
0193:         LDA       0,     -1,      5			;REGISTER0 = &x
0194:          LD       1,     11,      5			;*REGISTER1 = &a
0195:          LD       2,      0,      5			;REGISTER2 = i
0196:          LD       1,      0,      2			;Loading array element value.
0197:          ST       1,      0,      0			;x = REGISTER1
0198:         LDA       0,      0,      1			;REGISTER0 = x. Saving result of expression.
0199:         LDA       0,     -2,      5			;REGISTER0 = &k
0200:          LD       1,      0,      5			;REGISTER1 = i
0201:          ST       1,      0,      0			;k = REGISTER1
0202:         LDA       0,      0,      1			;REGISTER0 = k. Saving result of expression.
0203:         LDA       0,      0,      5			;REGISTER0 = &i
0204:          LD       1,      0,      5			;REGISTER1 = i
0205:         LDC       2,      1,      0			;REGISTER2 = 1
0206:         ADD       1,      1,      2			;REGISTER1 += REGISTER2
0207:          ST       1,      0,      0			;i = REGISTER1
0208:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0209:         LDA       7,    -35,      7			;Jump to top of loop (PC - 35).
0210:          LD       0,     -2,      5			;REGISTER0 = k
0211:          ST       0,      8,      5			;Storing value returned from "minloc" on stack.
0212:         LDA       7,      0,      7			;Jumping to finale for "minloc".
0213:         LDA       6,      3,      6			;Start of finale for "minloc". Deallocating local vars.
0214:          LD       5,      1,      6
0215:         LDA       6,      1,      6
0216:          LD       4,      1,      6
0217:         LDA       6,      1,      6
0218:          LD       3,      1,      6
0219:         LDA       6,      1,      6
0220:          LD       2,      1,      6
0221:         LDA       6,      1,      6
0222:          LD       1,      1,      6
0223:         LDA       6,      1,      6
0224:          LD       0,      1,      6
0225:         LDA       6,      1,      6			;Done restoring registers from stack.
0226:          LD       7,      1,      6			;Returning from function "minloc".
0227:          ST       0,      0,      6			;Start callable function "sort". Saving registers on stack.
0228:         LDA       6,     -1,      6
0229:          ST       1,      0,      6
0230:         LDA       6,     -1,      6
0231:          ST       2,      0,      6
0232:         LDA       6,     -1,      6
0233:          ST       3,      0,      6
0234:         LDA       6,     -1,      6
0235:          ST       4,      0,      6
0236:         LDA       6,     -1,      6
0237:          ST       5,      0,      6
0238:         LDA       6,     -1,      6			;Done saving registers on the stack.
0239:         LDA       5,      0,      6			;Saving frame pointer for call to "sort" into FP (R5).
0240:         LDA       6,     -2,      6			;Allocating space on the stack for (2) local variables.
0241:         LDA       0,      0,      5			;REGISTER0 = &i
0242:          LD       1,      9,      5			;REGISTER1 = low
0243:          ST       1,      0,      0			;i = REGISTER1
0244:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0245:          LD       0,      0,      5			;REGISTER0 = i
0246:          LD       1,      8,      5			;REGISTER1 = high
0247:         LDC       2,      1,      0			;REGISTER2 = 1
0248:         SUB       1,      1,      2			;REGISTER1 -= REGISTER2
0249:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0250:         JGE       0,      2,      7			;if (REGISTER0 >= 0) jump to PC + 2.
0251:         LDC       0,      1,      0			;REGISTER0 = 1
0252:         LDA       7,      1,      7			;Jump to PC + 1.
0253:         LDC       0,      0,      0			;REGISTER0 = 0
0254:         JEQ       0,     48,      7			;if (REGISTER0 == 0) Jump to instruction after loop body (PC + 48)
0255:         LDA       6,     -1,      6			;Allocating space on the stack for (1) local variables.
0256:         LDA       0,     -1,      5			;REGISTER0 = &k
0257:          LD       1,     10,      5			;*REGISTER1 = &a
0258:          ST       1,      0,      6			;Pushing REGISTER1 to stack as param 0 for call to "minloc".
0259:         LDA       6,     -1,      6			;SP++.
0260:          LD       1,      0,      5			;REGISTER1 = i
0261:          ST       1,      0,      6			;Pushing REGISTER1 to stack as param 1 for call to "minloc".
0262:         LDA       6,     -1,      6			;SP++.
0263:          LD       1,      8,      5			;REGISTER1 = high
0264:          ST       1,      0,      6			;Pushing REGISTER1 to stack as param 2 for call to "minloc".
0265:         LDA       6,     -1,      6			;SP++.
0266:         LDA       6,     -1,      6			;Allocating space for return value from "minloc".
0267:         LDA       4,      3,      7			;Loading return address for call to "minloc" into REGISTER4.
0268:          ST       4,      0,      6			;Pushing return address for call to "minloc" to the stack.
0269:         LDA       6,     -1,      6			;SP++.
0270:         LDA       7,   -126,      7			;Jumping to "minloc".
0271:          LD       1,      2,      6			;Loading return value from call to "minloc" into REGISTER1.
0272:         LDA       6,      5,      6			;Cleanup from function call.
0273:          ST       1,      0,      0			;k = REGISTER1
0274:         LDA       0,      0,      1			;REGISTER0 = k. Saving result of expression.
0275:         LDA       0,     -2,      5			;REGISTER0 = &t
0276:          LD       1,     10,      5			;*REGISTER1 = &a
0277:          LD       2,     -1,      5			;REGISTER2 = k
0278:          LD       1,      0,      2			;Loading array element value.
0279:          ST       1,      0,      0			;t = REGISTER1
0280:         LDA       0,      0,      1			;REGISTER0 = t. Saving result of expression.
0281:          LD       0,     10,      5			;*REGISTER0 = &a
0282:          LD       1,     -1,      5			;REGISTER1 = k
0283:         ADD       0,      0,      1			;REGISTER0 += REGISTER1
0284:          LD       1,     10,      5			;*REGISTER1 = &a
0285:          LD       2,      0,      5			;REGISTER2 = i
0286:          LD       1,      0,      2			;Loading array element value.
0287:          ST       1,      0,      0			;a[k] = REGISTER1
0288:         LDA       0,      0,      1			;REGISTER0 = a[k]. Saving result of expression.
0289:          LD       0,     10,      5			;*REGISTER0 = &a
0290:          LD       1,      0,      5			;REGISTER1 = i
0291:         ADD       0,      0,      1			;REGISTER0 += REGISTER1
0292:          LD       1,     -2,      5			;REGISTER1 = t
0293:          ST       1,      0,      0			;a[i] = REGISTER1
0294:         LDA       0,      0,      1			;REGISTER0 = a[i]. Saving result of expression.
0295:         LDA       0,      0,      5			;REGISTER0 = &i
0296:          LD       1,      0,      5			;REGISTER1 = i
0297:         LDC       2,      1,      0			;REGISTER2 = 1
0298:         ADD       1,      1,      2			;REGISTER1 += REGISTER2
0299:          ST       1,      0,      0			;i = REGISTER1
0300:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0301:         LDA       6,      1,      6			;Start of finale for "sort". Deallocating local vars.
0302:         LDA       7,    -58,      7			;Jump to top of loop (PC - 58).
0303:         LDA       6,      2,      6			;Start of finale for "sort". Deallocating local vars.
0304:          LD       5,      1,      6
0305:         LDA       6,      1,      6
0306:          LD       4,      1,      6
0307:         LDA       6,      1,      6
0308:          LD       3,      1,      6
0309:         LDA       6,      1,      6
0310:          LD       2,      1,      6
0311:         LDA       6,      1,      6
0312:          LD       1,      1,      6
0313:         LDA       6,      1,      6
0314:          LD       0,      1,      6
0315:         LDA       6,      1,      6			;Done restoring registers from stack.
0316:          LD       7,      1,      6			;Returning from function "sort".
