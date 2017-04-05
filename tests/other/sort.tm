0000:          LD       6,      0,      0			;Loading SP with DMEM_MAX.
0001:          ST       0,      0,      0			;Zeroing out the top of DMem.
0002:         LDA       6,    -10,      6			;Allocating space on the stack for (1) global variables.
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
0020:        HALT       0,      0,      0			;Exit point for program (tests/other/sort.tm).
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
0053:         LDA       6,     -1,      6			;Allocating space on the stack for (1) local variables.
0054:         LDA       0,      0,      5			;REGISTER0 = &i
0055:         LDC       1,      0,      0			;REGISTER1 = 0
0056:          ST       1,      0,      0			;i = REGISTER1
0057:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0058:          LD       0,      0,      5			;REGISTER0 = i
0059:         LDC       1,     10,      0			;REGISTER1 = 10
0060:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0061:         JGE       0,      2,      7			;if (REGISTER0 >= 0) jump to PC + 2.
0062:         LDC       0,      1,      0			;REGISTER0 = 1
0063:         LDA       7,      1,      7			;Jump to PC + 1.
0064:         LDC       0,      0,      0			;REGISTER0 = 0
0065:         JEQ       0,     22,      7			;if (REGISTER0 == 0) Jump to instruction after loop body (PC + 22)
0066:         LDC       0,   1023,      0			;REGISTER0 = &x
0067:          LD       1,      0,      5			;REGISTER1 = i
0068:         LDC       2,     10,      0			;REGISTER2 = len(x)
0069:         SUB       2,      2,      1			;REGISTER2 -= REGISTER1
0070:         JLE       2,    -47,      7			;Jumping to "HANDLE_EXCEPTION_ARRAY_OOB".
0071:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0072:         LDA       6,     -1,      6			;Allocating space for return value from "input".
0073:         LDA       4,      3,      7			;Loading return address for call to "input" into REGISTER4.
0074:          ST       4,      0,      6			;Pushing return address for call to "input" to the stack.
0075:         LDA       6,     -1,      6			;SP++.
0076:         LDA       7,    -50,      7			;Jumping to "input".
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
0087:         LDA       7,    -30,      7			;Jump to top of loop (PC - 30).
0088:         LDC       0,   1023,      0			;REGISTER0 = &x
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
0100:         LDA       7,    130,      7			;Jumping to "sort".
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
0113:         JEQ       0,     18,      7			;if (REGISTER0 == 0) Jump to instruction after loop body (PC + 18)
0114:         LDC       0,   1023,      0			;REGISTER0 = &x
0115:          LD       1,      0,      5			;REGISTER1 = i
0116:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0117:          LD       0,      0,      0			;REGISTER0 = x[i]
0118:          ST       0,      0,      6			;Pushing REGISTER0 to stack as param 0 for call to "output".
0119:         LDA       6,     -1,      6			;SP++.
0120:         LDA       4,      3,      7			;Loading return address for call to "output" into REGISTER4.
0121:          ST       4,      0,      6			;Pushing return address for call to "output" to the stack.
0122:         LDA       6,     -1,      6			;SP++.
0123:         LDA       7,    -90,      7			;Jumping to "output".
0124:         LDA       6,      2,      6			;Cleanup from function call.
0125:         LDA       0,      0,      5			;REGISTER0 = &i
0126:          LD       1,      0,      5			;REGISTER1 = i
0127:         LDC       2,      1,      0			;REGISTER2 = 1
0128:         ADD       1,      1,      2			;REGISTER1 += REGISTER2
0129:          ST       1,      0,      0			;i = REGISTER1
0130:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0131:         LDA       7,    -26,      7			;Jump to top of loop (PC - 26).
0132:         LDA       6,      1,      6			;Start of finale for "main". Deallocating local vars.
0133:          LD       5,      1,      6
0134:         LDA       6,      1,      6
0135:          LD       4,      1,      6
0136:         LDA       6,      1,      6
0137:          LD       3,      1,      6
0138:         LDA       6,      1,      6
0139:          LD       2,      1,      6
0140:         LDA       6,      1,      6
0141:          LD       1,      1,      6
0142:         LDA       6,      1,      6
0143:          LD       0,      1,      6
0144:         LDA       6,      1,      6			;Done restoring registers from stack.
0145:          LD       7,      1,      6			;Returning from function "main".
0146:          ST       0,      0,      6			;Start callable function "minloc". Saving registers on stack.
0147:         LDA       6,     -1,      6
0148:          ST       1,      0,      6
0149:         LDA       6,     -1,      6
0150:          ST       2,      0,      6
0151:         LDA       6,     -1,      6
0152:          ST       3,      0,      6
0153:         LDA       6,     -1,      6
0154:          ST       4,      0,      6
0155:         LDA       6,     -1,      6
0156:          ST       5,      0,      6
0157:         LDA       6,     -1,      6			;Done saving registers on the stack.
0158:         LDA       5,      0,      6			;Saving frame pointer for call to "minloc" into FP (R5).
0159:         LDA       6,     -3,      6			;Allocating space on the stack for (3) local variables.
0160:         LDA       0,     -2,      5			;REGISTER0 = &k
0161:          LD       1,     10,      5			;REGISTER1 = low
0162:          ST       1,      0,      0			;k = REGISTER1
0163:         LDA       0,      0,      1			;REGISTER0 = k. Saving result of expression.
0164:         LDA       0,     -1,      5			;REGISTER0 = &x
0165:          LD       1,     11,      5			;*REGISTER1 = &a
0166:          LD       2,     10,      5			;REGISTER2 = low
0167:         SUB       1,      1,      2			;REGISTER1 -= REGISTER2
0168:          LD       1,      0,      1			;REGISTER1 = a[low]
0169:          ST       1,      0,      0			;x = REGISTER1
0170:         LDA       0,      0,      1			;REGISTER0 = x. Saving result of expression.
0171:         LDA       0,      0,      5			;REGISTER0 = &i
0172:          LD       1,     10,      5			;REGISTER1 = low
0173:         LDC       2,      1,      0			;REGISTER2 = 1
0174:         ADD       1,      1,      2			;REGISTER1 += REGISTER2
0175:          ST       1,      0,      0			;i = REGISTER1
0176:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0177:          LD       0,      0,      5			;REGISTER0 = i
0178:          LD       1,      9,      5			;REGISTER1 = high
0179:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0180:         JGE       0,      2,      7			;if (REGISTER0 >= 0) jump to PC + 2.
0181:         LDC       0,      1,      0			;REGISTER0 = 1
0182:         LDA       7,      1,      7			;Jump to PC + 1.
0183:         LDC       0,      0,      0			;REGISTER0 = 0
0184:         JEQ       0,     29,      7			;if (REGISTER0 == 0) Jump to instruction after loop body (PC + 29)
0185:          LD       0,     11,      5			;*REGISTER0 = &a
0186:          LD       1,      0,      5			;REGISTER1 = i
0187:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0188:          LD       0,      0,      0			;REGISTER0 = a[i]
0189:          LD       1,     -1,      5			;REGISTER1 = x
0190:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0191:         JGE       0,      2,      7			;if (REGISTER0 >= 0) jump to PC + 2.
0192:         LDC       0,      1,      0			;REGISTER0 = 1
0193:         LDA       7,      1,      7			;Jump to PC + 1.
0194:         LDC       0,      0,      0			;REGISTER0 = 0
0195:         JEQ       0,     11,      7			;if (REGISTER0 == 0) Jump to instruction after if statement (PC + 11).
0196:         LDA       0,     -1,      5			;REGISTER0 = &x
0197:          LD       1,     11,      5			;*REGISTER1 = &a
0198:          LD       2,      0,      5			;REGISTER2 = i
0199:         SUB       1,      1,      2			;REGISTER1 -= REGISTER2
0200:          LD       1,      0,      1			;REGISTER1 = a[i]
0201:          ST       1,      0,      0			;x = REGISTER1
0202:         LDA       0,      0,      1			;REGISTER0 = x. Saving result of expression.
0203:         LDA       0,     -2,      5			;REGISTER0 = &k
0204:          LD       1,      0,      5			;REGISTER1 = i
0205:          ST       1,      0,      0			;k = REGISTER1
0206:         LDA       0,      0,      1			;REGISTER0 = k. Saving result of expression.
0207:         LDA       0,      0,      5			;REGISTER0 = &i
0208:          LD       1,      0,      5			;REGISTER1 = i
0209:         LDC       2,      1,      0			;REGISTER2 = 1
0210:         ADD       1,      1,      2			;REGISTER1 += REGISTER2
0211:          ST       1,      0,      0			;i = REGISTER1
0212:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0213:         LDA       7,    -37,      7			;Jump to top of loop (PC - 37).
0214:          LD       0,     -2,      5			;REGISTER0 = k
0215:          ST       0,      8,      5			;Storing value returned from "minloc" on stack.
0216:         LDA       7,      0,      7			;Jumping to finale for "minloc".
0217:         LDA       6,      3,      6			;Start of finale for "minloc". Deallocating local vars.
0218:          LD       5,      1,      6
0219:         LDA       6,      1,      6
0220:          LD       4,      1,      6
0221:         LDA       6,      1,      6
0222:          LD       3,      1,      6
0223:         LDA       6,      1,      6
0224:          LD       2,      1,      6
0225:         LDA       6,      1,      6
0226:          LD       1,      1,      6
0227:         LDA       6,      1,      6
0228:          LD       0,      1,      6
0229:         LDA       6,      1,      6			;Done restoring registers from stack.
0230:          LD       7,      1,      6			;Returning from function "minloc".
0231:          ST       0,      0,      6			;Start callable function "sort". Saving registers on stack.
0232:         LDA       6,     -1,      6
0233:          ST       1,      0,      6
0234:         LDA       6,     -1,      6
0235:          ST       2,      0,      6
0236:         LDA       6,     -1,      6
0237:          ST       3,      0,      6
0238:         LDA       6,     -1,      6
0239:          ST       4,      0,      6
0240:         LDA       6,     -1,      6
0241:          ST       5,      0,      6
0242:         LDA       6,     -1,      6			;Done saving registers on the stack.
0243:         LDA       5,      0,      6			;Saving frame pointer for call to "sort" into FP (R5).
0244:         LDA       6,     -2,      6			;Allocating space on the stack for (2) local variables.
0245:         LDA       0,      0,      5			;REGISTER0 = &i
0246:          LD       1,      9,      5			;REGISTER1 = low
0247:          ST       1,      0,      0			;i = REGISTER1
0248:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0249:          LD       0,      0,      5			;REGISTER0 = i
0250:          LD       1,      8,      5			;REGISTER1 = high
0251:         LDC       2,      1,      0			;REGISTER2 = 1
0252:         SUB       1,      1,      2			;REGISTER1 -= REGISTER2
0253:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0254:         JGE       0,      2,      7			;if (REGISTER0 >= 0) jump to PC + 2.
0255:         LDC       0,      1,      0			;REGISTER0 = 1
0256:         LDA       7,      1,      7			;Jump to PC + 1.
0257:         LDC       0,      0,      0			;REGISTER0 = 0
0258:         JEQ       0,     50,      7			;if (REGISTER0 == 0) Jump to instruction after loop body (PC + 50)
0259:         LDA       6,     -1,      6			;Allocating space on the stack for (1) local variables.
0260:         LDA       0,     -1,      5			;REGISTER0 = &k
0261:          LD       1,     10,      5			;*REGISTER1 = &a
0262:          ST       1,      0,      6			;Pushing REGISTER1 to stack as param 0 for call to "minloc".
0263:         LDA       6,     -1,      6			;SP++.
0264:          LD       1,      0,      5			;REGISTER1 = i
0265:          ST       1,      0,      6			;Pushing REGISTER1 to stack as param 1 for call to "minloc".
0266:         LDA       6,     -1,      6			;SP++.
0267:          LD       1,      8,      5			;REGISTER1 = high
0268:          ST       1,      0,      6			;Pushing REGISTER1 to stack as param 2 for call to "minloc".
0269:         LDA       6,     -1,      6			;SP++.
0270:         LDA       6,     -1,      6			;Allocating space for return value from "minloc".
0271:         LDA       4,      3,      7			;Loading return address for call to "minloc" into REGISTER4.
0272:          ST       4,      0,      6			;Pushing return address for call to "minloc" to the stack.
0273:         LDA       6,     -1,      6			;SP++.
0274:         LDA       7,   -129,      7			;Jumping to "minloc".
0275:          LD       1,      2,      6			;Loading return value from call to "minloc" into REGISTER1.
0276:         LDA       6,      5,      6			;Cleanup from function call.
0277:          ST       1,      0,      0			;k = REGISTER1
0278:         LDA       0,      0,      1			;REGISTER0 = k. Saving result of expression.
0279:         LDA       0,     -2,      5			;REGISTER0 = &t
0280:          LD       1,     10,      5			;*REGISTER1 = &a
0281:          LD       2,     -1,      5			;REGISTER2 = k
0282:         SUB       1,      1,      2			;REGISTER1 -= REGISTER2
0283:          LD       1,      0,      1			;REGISTER1 = a[k]
0284:          ST       1,      0,      0			;t = REGISTER1
0285:         LDA       0,      0,      1			;REGISTER0 = t. Saving result of expression.
0286:          LD       0,     10,      5			;*REGISTER0 = &a
0287:          LD       1,     -1,      5			;REGISTER1 = k
0288:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0289:          LD       1,     10,      5			;*REGISTER1 = &a
0290:          LD       2,      0,      5			;REGISTER2 = i
0291:         SUB       1,      1,      2			;REGISTER1 -= REGISTER2
0292:          LD       1,      0,      1			;REGISTER1 = a[i]
0293:          ST       1,      0,      0			;a[k] = REGISTER1
0294:         LDA       0,      0,      1			;REGISTER0 = a[k]. Saving result of expression.
0295:          LD       0,     10,      5			;*REGISTER0 = &a
0296:          LD       1,      0,      5			;REGISTER1 = i
0297:         SUB       0,      0,      1			;REGISTER0 -= REGISTER1
0298:          LD       1,     -2,      5			;REGISTER1 = t
0299:          ST       1,      0,      0			;a[i] = REGISTER1
0300:         LDA       0,      0,      1			;REGISTER0 = a[i]. Saving result of expression.
0301:         LDA       0,      0,      5			;REGISTER0 = &i
0302:          LD       1,      0,      5			;REGISTER1 = i
0303:         LDC       2,      1,      0			;REGISTER2 = 1
0304:         ADD       1,      1,      2			;REGISTER1 += REGISTER2
0305:          ST       1,      0,      0			;i = REGISTER1
0306:         LDA       0,      0,      1			;REGISTER0 = i. Saving result of expression.
0307:         LDA       6,      1,      6			;Start of finale for "sort". Deallocating local vars.
0308:         LDA       7,    -60,      7			;Jump to top of loop (PC - 60).
0309:         LDA       6,      2,      6			;Start of finale for "sort". Deallocating local vars.
0310:          LD       5,      1,      6
0311:         LDA       6,      1,      6
0312:          LD       4,      1,      6
0313:         LDA       6,      1,      6
0314:          LD       3,      1,      6
0315:         LDA       6,      1,      6
0316:          LD       2,      1,      6
0317:         LDA       6,      1,      6
0318:          LD       1,      1,      6
0319:         LDA       6,      1,      6
0320:          LD       0,      1,      6
0321:         LDA       6,      1,      6			;Done restoring registers from stack.
0322:          LD       7,      1,      6			;Returning from function "sort".
