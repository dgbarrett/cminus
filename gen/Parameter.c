#include "Parameter.h"

#include <stdlib.h>

FunctionParameter * new_Parameter(char * name, int isRegister, int addr) {
	FunctionParameter * p = malloc(sizeof(*p));

	p -> isRegister = isRegister;
	p -> addr = addr;
	p -> name = name;

	return p;
}

FunctionParameter ** createParameterMap(int numParams) {
	FunctionParameter ** params = calloc(numParams + 1, sizeof(*params));
	return params;
}

void ParameterMap_addParam(FunctionParameter ** params, int index, char * paramName, int isRegister, int addr) {
	if (params) {
		params[index] = new_Parameter(paramName, isRegister, addr);
	}
}