#ifndef __GEN_C__
#define __GEN_C__

#include "../parse/ast.h"
#include "Instruction.h"

TMCode * generateCode(ASTNode * root, char * fname);
void printTMCode(TMCode * tm);

#endif