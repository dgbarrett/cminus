#ifndef __LEX_H__
#define __LEX_H__

#include "tokens.h"

Token * getNextToken();
int getToken();
void initScanner();

#endif