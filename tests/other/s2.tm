0000:          ST       0,      0,      0			;Zeroing out the top of DMem.
0001:         LDA       6,     10,      6			;Allocating space on the stack for (1) global variables.
0002:         LDA       4,      3,      7			;Loading return address for call to "main" into REGISTER4.
0003:          ST       4,      0,      6			;Pushing return address for call to "main" to the stack.
0004:         LDA       6,      1,      6			;SP++.
0005:         LDA       7,     36,      7			;Jumping to "main".
0006:         LDA       6,     -1,      6			;Cleanup from function call.
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
0019:        HALT       0,      0,      0			;Exit point for program (tests/other/sort.tm).
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
0030:         LDA       6,      1,      6
0031:          IN       0,      0,      0
0032:          ST       0,     -3,      6
0033:          LD       0,     -1,      6
0034:         LDA       6,     -1,      6
0035:          LD       7,     -1,      6			;Returning from function "input".
0036:          ST       0,      0,      6			;Start of callable function "output".
0037:         LDA       6,      1,      6
0038:          LD       0,     -3,      6
0039:         OUT       0,      0,      0
0040:         LDA       6,     -1,      6
0041:          LD       7,     -1,      6
0042:          ST       0,      0,      6			;Start callable function "main". Saving registers on stack.
0043:         LDA       6,      1,      6
0044:          ST       1,      0,      6
0045:         LDA       6,      1,      6
0046:          ST       2,      0,      6
0047:         LDA       6,      1,      6
0048:          ST       3,      0,      6
0049:         LDA       6,      1,      6
0050:          ST       4,      0,      6
0051:         LDA       6,      1,      6
0052:          ST       5,      0,      6
0053:         LDA       6,      1,      6			;Done saving registers on the stack.
0054:         LDA       5,      0,      6			;Saving frame pointer for call to "main" into FP (R5).
0055:         LDA       6,      1,      6			;Allocating space on the stack for (1) local variables.
0056:         LDA       0,      0,      5			;REGISTER0 = &i
0057:         LDC       1,      0,      0			;REGISTER1 = 0
0058:          ST       1,      0,      0			;i = REGISTER1
0059:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0060:          LD       0,      0,      5			;REGISTER0 = i
0061:         LDC       1,     10,      0			;REGISTER1 = 10
0062:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0063:         JGE       0,      2,      7			;if (REGISTER0 >= 0) jump to PC + 2.
0064:         LDC       0,      1,      0			;REGISTER0 = 1
0065:         LDA       7,      1,      7			;Jump to PC + 1.
0066:         LDC       0,      0,      0			;REGISTER0 = 0
0067:         JEQ       0,     19,      7			;if (REGISTER0 == 0) Jump to instruction after loop body (PC + 19)
0068:         LDC       0,      0,      0			;REGISTER0 = &x
0069:          LD       1,      0,      5			;REGISTER1 = i
0070:         ADD       0,      0,      1			;REGISTER0 += REGISTER1
0071:         LDA       6,      1,      6			;Allocating space for return value from "input".
0072:         LDA       4,      3,      7			;Loading return address for call to "input" into REGISTER4.
0073:          ST       4,      0,      6			;Pushing return address for call to "input" to the stack.
0074:         LDA       6,      1,      6			;SP++.
0075:         LDA       7,    -47,      7			;Jumping to "input".
0076:          LD       1,     -2,      6			;Loading return value from call to "input" into REGISTER1.
0077:         LDA       6,     -2,      6			;Cleanup from function call.
0078:          ST       1,      0,      0			;x[i] = REGISTER1
0079:         LDA       0,      0,      1			;REGISTER0 = x[i]. Saving result of expression.
0080:         LDA       0,      0,      5			;REGISTER0 = &i
0081:          LD       1,      0,      5			;REGISTER1 = i
0082:         LDC       2,      1,      0			;REGISTER2 = 1
0083:         ADD       1,      1,      2			;REGISTER1 += REGISTER2
0084:          ST       1,      0,      0			;i = REGISTER1
0085:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0086:         LDA       7,    -27,      7			;Jump to top of loop (PC - 27).
0087:         LDC       0,      0,      0			;REGISTER0 = &x
0088:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 0 for call to "sort".
0089:         LDA       6,      1,      6			;SP++.
0090:         LDC       0,      0,      0			;REGISTER0 = 0
0091:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 1 for call to "sort".
0092:         LDA       6,      1,      6			;SP++.
0093:         LDC       0,     10,      0			;REGISTER0 = 10
0094:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 2 for call to "sort".
0095:         LDA       6,      1,      6			;SP++.
0096:         LDA       4,      3,      7			;Loading return address for call to "sort" into REGISTER4.
0097:          ST       4,      0,      6			;Pushing return address for call to "sort" to the stack.
0098:         LDA       6,      1,      6			;SP++.
0099:         LDA       7,    126,      7			;Jumping to "sort".
0100:         LDA       6,     -4,      6			;Cleanup from function call.
0101:         LDA       0,      0,      5			;REGISTER0 = &i
0102:         LDC       1,      0,      0			;REGISTER1 = 0
0103:          ST       1,      0,      0			;i = REGISTER1
0104:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0105:          LD       0,      0,      5			;REGISTER0 = i
0106:         LDC       1,     10,      0			;REGISTER1 = 10
0107:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0108:         JGE       0,      2,      7			;if (REGISTER0 >= 0) jump to PC + 2.
0109:         LDC       0,      1,      0			;REGISTER0 = 1
0110:         LDA       7,      1,      7			;Jump to PC + 1.
0111:         LDC       0,      0,      0			;REGISTER0 = 0
0112:         JEQ       0,     17,      7			;if (REGISTER0 == 0) Jump to instruction after loop body (PC + 17)
0113:         LDC       0,      0,      0			;REGISTER0 = &x
0114:          LD       1,      0,      5			;REGISTER1 = i
0115:          LD       0,      0,      1			;Loading array element value.
0116:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 0 for call to "output".
0117:         LDA       6,      1,      6			;SP++.
0118:         LDA       4,      3,      7			;Loading return address for call to "output" into REGISTER4.
0119:          ST       4,      0,      6			;Pushing return address for call to "output" to the stack.
0120:         LDA       6,      1,      6			;SP++.
0121:         LDA       7,    -86,      7			;Jumping to "output".
0122:         LDA       6,     -2,      6			;Cleanup from function call.
0123:         LDA       0,      0,      5			;REGISTER0 = &i
0124:          LD       1,      0,      5			;REGISTER1 = i
0125:         LDC       2,      1,      0			;REGISTER2 = 1
0126:         ADD       1,      1,      2			;REGISTER1 += REGISTER2
0127:          ST       1,      0,      0			;i = REGISTER1
0128:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0129:         LDA       7,    -25,      7			;Jump to top of loop (PC - 25).
0130:         LDA       6,     -1,      6			;Start of finale for "main". Deallocating local vars.
0131:          LD       5,     -1,      6
0132:         LDA       6,     -1,      6
0133:          LD       4,     -1,      6
0134:         LDA       6,     -1,      6
0135:          LD       3,     -1,      6
0136:         LDA       6,     -1,      6
0137:          LD       2,     -1,      6
0138:         LDA       6,     -1,      6
0139:          LD       1,     -1,      6
0140:         LDA       6,     -1,      6
0141:          LD       0,     -1,      6
0142:         LDA       6,     -1,      6			;Done restoring registers from stack.
0143:          LD       7,     -1,      6			;Returning from function "main".
0144:          ST       0,      0,      6			;Start callable function "minloc". Saving registers on stack.
0145:         LDA       6,      1,      6
0146:          ST       1,      0,      6
0147:         LDA       6,      1,      6
0148:          ST       2,      0,      6
0149:         LDA       6,      1,      6
0150:          ST       3,      0,      6
0151:         LDA       6,      1,      6
0152:          ST       4,      0,      6
0153:         LDA       6,      1,      6
0154:          ST       5,      0,      6
0155:         LDA       6,      1,      6			;Done saving registers on the stack.
0156:         LDA       5,      0,      6			;Saving frame pointer for call to "minloc" into FP (R5).
0157:         LDA       6,      3,      6			;Allocating space on the stack for (3) local variables.
0158:         LDA       0,      2,      5			;REGISTER0 = &k
0159:          LD       1,    -10,      5			;REGISTER1 = low
0160:          ST       1,      0,      0			;k = REGISTER1
0161:         LDA       0,      0,      1			;REGISTER0 = k. Saving result of expression.
0162:         LDA       0,      1,      5			;REGISTER0 = &x
0163:          LD       1,    -11,      5			;REGISTER1 = &a
0164:          LD       2,    -10,      5			;REGISTER2 = low
0165:          LD       1,      0,      2			;Loading array element value.
0166:          ST       1,      0,      0			;x = REGISTER1
0167:         LDA       0,      0,      1			;REGISTER0 = x. Saving result of expression.
0168:         LDA       0,      0,      5			;REGISTER0 = &i
0169:          LD       1,    -10,      5			;REGISTER1 = low
0170:         LDC       2,      1,      0			;REGISTER2 = 1
0171:         ADD       1,      1,      2			;REGISTER1 += REGISTER2
0172:          ST       1,      0,      0			;i = REGISTER1
0173:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0174:          LD       0,      0,      5			;REGISTER0 = i
0175:          LD       1,     -9,      5			;REGISTER1 = high
0176:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0177:         JGE       0,      2,      7			;if (REGISTER0 >= 0) jump to PC + 2.
0178:         LDC       0,      1,      0			;REGISTER0 = 1
0179:         LDA       7,      1,      7			;Jump to PC + 1.
0180:         LDC       0,      0,      0			;REGISTER0 = 0
0181:         JEQ       0,     27,      7			;if (REGISTER0 == 0) Jump to instruction after loop body (PC + 27)
0182:          LD       0,    -11,      5			;REGISTER0 = &a
0183:          LD       1,      0,      5			;REGISTER1 = i
0184:          LD       0,      0,      1			;Loading array element value.
0185:          LD       1,      1,      5			;REGISTER1 = x
0186:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0187:         JGE       0,      2,      7			;if (REGISTER0 >= 0) jump to PC + 2.
0188:         LDC       0,      1,      0			;REGISTER0 = 1
0189:         LDA       7,      1,      7			;Jump to PC + 1.
0190:         LDC       0,      0,      0			;REGISTER0 = 0
0191:         JEQ       0,     10,      7			;if (REGISTER0 == 0) Jump to instruction after if statement (PC + 10).
0192:         LDA       0,      1,      5			;REGISTER0 = &x
0193:          LD       1,    -11,      5			;REGISTER1 = &a
0194:          LD       2,      0,      5			;REGISTER2 = i
0195:          LD       1,      0,      2			;Loading array element value.
0196:          ST       1,      0,      0			;x = REGISTER1
0197:         LDA       0,      0,      1			;REGISTER0 = x. Saving result of expression.
0198:         LDA       0,      2,      5			;REGISTER0 = &k
0199:          LD       1,      0,      5			;REGISTER1 = i
0200:          ST       1,      0,      0			;k = REGISTER1
0201:         LDA       0,      0,      1			;REGISTER0 = k. Saving result of expression.
0202:         LDA       0,      0,      5			;REGISTER0 = &i
0203:          LD       1,      0,      5			;REGISTER1 = i
0204:         LDC       2,      1,      0			;REGISTER2 = 1
0205:         ADD       1,      1,      2			;REGISTER1 += REGISTER2
0206:          ST       1,      0,      0			;i = REGISTER1
0207:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0208:         LDA       7,    -35,      7			;Jump to top of loop (PC - 35).
0209:          LD       0,      2,      5			;REGISTER0 = k
0210:          ST       0,     -8,      5			;Storing value returned from "minloc" on stack.
0211:         LDA       7,      0,      7			;Jumping to finale for "minloc".
0212:         LDA       6,     -3,      6			;Start of finale for "minloc". Deallocating local vars.
0213:          LD       5,     -1,      6
0214:         LDA       6,     -1,      6
0215:          LD       4,     -1,      6
0216:         LDA       6,     -1,      6
0217:          LD       3,     -1,      6
0218:         LDA       6,     -1,      6
0219:          LD       2,     -1,      6
0220:         LDA       6,     -1,      6
0221:          LD       1,     -1,      6
0222:         LDA       6,     -1,      6
0223:          LD       0,     -1,      6
0224:         LDA       6,     -1,      6			;Done restoring registers from stack.
0225:          LD       7,     -1,      6			;Returning from function "minloc".
0226:          ST       0,      0,      6			;Start callable function "sort". Saving registers on stack.
0227:         LDA       6,      1,      6
0228:          ST       1,      0,      6
0229:         LDA       6,      1,      6
0230:          ST       2,      0,      6
0231:         LDA       6,      1,      6
0232:          ST       3,      0,      6
0233:         LDA       6,      1,      6
0234:          ST       4,      0,      6
0235:         LDA       6,      1,      6
0236:          ST       5,      0,      6
0237:         LDA       6,      1,      6			;Done saving registers on the stack.
0238:         LDA       5,      0,      6			;Saving frame pointer for call to "sort" into FP (R5).
0239:         LDA       6,      2,      6			;Allocating space on the stack for (2) local variables.
0240:         LDA       0,      0,      5			;REGISTER0 = &i
0241:          LD       1,     -9,      5			;REGISTER1 = low
0242:          ST       1,      0,      0			;i = REGISTER1
0243:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0244:          LD       0,      0,      5			;REGISTER0 = i
0245:          LD       1,     -8,      5			;REGISTER1 = high
0246:         LDC       2,      1,      0			;REGISTER2 = 1
0247:         SUB       1,      1,      2			;REGISTER1 -= REGISTER2
0248:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0249:         JGE       0,      2,      7			;if (REGISTER0 >= 0) jump to PC + 2.
0250:         LDC       0,      1,      0			;REGISTER0 = 1
0251:         LDA       7,      1,      7			;Jump to PC + 1.
0252:         LDC       0,      0,      0			;REGISTER0 = 0
0253:         JEQ       0,     48,      7			;if (REGISTER0 == 0) Jump to instruction after loop body (PC + 48)
0254:         LDA       6,      1,      6			;Allocating space on the stack for (1) local variables.
0255:         LDA       0,      1,      5			;REGISTER0 = &k
0256:          LD       1,    -10,      5			;REGISTER1 = &a
0257:          ST       1,      0,      6			;Pushing REGISTER1 to stack as param 0 for call to "minloc".
0258:         LDA       6,      1,      6			;SP++.
0259:          LD       1,      0,      5			;REGISTER1 = i
0260:          ST       1,      0,      6			;Pushing REGISTER1 to stack as param 1 for call to "minloc".
0261:         LDA       6,      1,      6			;SP++.
0262:          LD       1,     -8,      5			;REGISTER1 = high
0263:          ST       1,      0,      6			;Pushing REGISTER1 to stack as param 2 for call to "minloc".
0264:         LDA       6,      1,      6			;SP++.
0265:         LDA       6,      1,      6			;Allocating space for return value from "minloc".
0266:         LDA       4,      3,      7			;Loading return address for call to "minloc" into REGISTER4.
0267:          ST       4,      0,      6			;Pushing return address for call to "minloc" to the stack.
0268:         LDA       6,      1,      6			;SP++.
0269:         LDA       7,   -126,      7			;Jumping to "minloc".
0270:          LD       1,     -2,      6			;Loading return value from call to "minloc" into REGISTER1.
0271:         LDA       6,     -5,      6			;Cleanup from function call.
0272:          ST       1,      0,      0			;k = REGISTER1
0273:         LDA       0,      0,      1			;REGISTER0 = k. Saving result of expression.
0274:         LDA       0,      2,      5			;REGISTER0 = &t
0275:          LD       1,    -10,      5			;REGISTER1 = &a
0276:          LD       2,      1,      5			;REGISTER2 = k
0277:          LD       1,      0,      2			;Loading array element value.
0278:          ST       1,      0,      0			;t = REGISTER1
0279:         LDA       0,      0,      1			;REGISTER0 = t. Saving result of expression.
0280:          LD       0,    -10,      5			;REGISTER0 = &a
0281:          LD       1,      1,      5			;REGISTER1 = k
0282:         ADD       0,      0,      1			;REGISTER0 += REGISTER1
0283:          LD       1,    -10,      5			;REGISTER1 = &a
0284:          LD       2,      0,      5			;REGISTER2 = i
0285:          LD       1,      0,      2			;Loading array element value.
0286:          ST       1,      0,      0			;a[k] = REGISTER1
0287:         LDA       0,      0,      1			;REGISTER0 = a[k]. Saving result of expression.
0288:          LD       0,    -10,      5			;REGISTER0 = &a
0289:          LD       1,      0,      5			;REGISTER1 = i
0290:         ADD       0,      0,      1			;REGISTER0 += REGISTER1
0291:          LD       1,      2,      5			;REGISTER1 = t
0292:          ST       1,      0,      0			;a[i] = REGISTER1
0293:         LDA       0,      0,      1			;REGISTER0 = a[i]. Saving result of expression.
0294:         LDA       0,      0,      5			;REGISTER0 = &i
0295:          LD       1,      0,      5			;REGISTER1 = i
0296:         LDC       2,      1,      0			;REGISTER2 = 1
0297:         ADD       1,      1,      2			;REGISTER1 += REGISTER2
0298:          ST       1,      0,      0			;i = REGISTER1
0299:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0300:         LDA       6,     -1,      6			;Start of finale for "sort". Deallocating local vars.
0301:         LDA       7,    -58,      7			;Jump to top of loop (PC - 58).
0302:         LDA       6,     -2,      6			;Start of finale for "sort". Deallocating local vars.
0303:          LD       5,     -1,      6
0304:         LDA       6,     -1,      6
0305:          LD       4,     -1,      6
0306:         LDA       6,     -1,      6
0307:          LD       3,     -1,      6
0308:         LDA       6,     -1,      6
0309:          LD       2,     -1,      6
0310:         LDA       6,     -1,      6
0311:          LD       1,     -1,      6
0312:         LDA       6,     -1,      6
0313:          LD       0,     -1,      6
0314:         LDA       6,     -1,      6			;Done restoring registers from stack.
0315:          LD       7,     -1,      6			;Returning from function "sort".
