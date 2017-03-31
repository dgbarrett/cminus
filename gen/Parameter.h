#ifndef __PARAMETER_H__
#define __PARAMETER_H__

#define REGISTER 1
#define MEMORY 0

typedef struct _Parameter {
	int isRegister;
	int addr;
} FunctionParameter;

FunctionParameter ** createParameterMap(int numParams);
void ParameterMap_addParam(FunctionParameter ** params, int index, int isRegister, int addr);

#endif