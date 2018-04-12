#include "manyvars.h"

struct eValue {
	float value;
	float diff;
};

union val{
	float f;
	char op;
	int n;
};

struct expr{
	char type;
	union val v;
};


float autoDiff(struct expr * r, int varIndex, ...){
	printf("Zaczynamy rozniczkowac\n");
	struct eValue * vStack = malloc(sizeof(struct eValue));
	int vsSize = 0;	
	int varNumb = r->v.n;
	va_list values;
	va_start(values,varIndex);
	float * varTab = malloc(sizeof(float)*(varNumb));
	for(int i = 0; i<varNumb; i++){
		varTab[i]=(float)va_arg(values,double);
		printf("%.2f\n",varTab[i]);
	}
	va_end(values);
	float mainVar = varTab[varIndex-1];
	printf("Mainvar to: %.2f\n",mainVar);
	r++;
	float tmpV;
	float tmpD;
	for(r; r->type!='\0'; r++){
		switch(r->type){
			case 'n':
				printf("Trafiamy na numer\n");
				vsSize++;
				vStack = realloc(vStack,sizeof(struct eValue)*vsSize);
				vStack[vsSize-1].value = r->v.f;
				vStack[vsSize-1].diff = 0;
				break;
			case 'v':
				printf("Trafiamy na zmienna\n");
				if(varIndex == r->v.n){
					vsSize++;
					vStack = realloc(vStack,sizeof(struct eValue)*vsSize);
					vStack[vsSize-1].value = mainVar;
					vStack[vsSize-1].diff = 1.0;
				}
				else{
					vsSize++;
					vStack = realloc(vStack,sizeof(struct eValue)*vsSize);
					vStack[vsSize-1].value = varTab[r->v.n-1];
					vStack[vsSize-1].diff = 0.0;
				}

				break;
			case 'o':
				printf("Operator %c\n",r->v.op);
				switch(r->v.op){
					case '+':
						tmpV = vStack[vsSize-1].value + vStack[vsSize-2].value;
						tmpD = vStack[vsSize-1].diff + vStack[vsSize-2].diff;
						vsSize -= 1;
						vStack = realloc(vStack,sizeof(float)*vsSize);	
						vStack[vsSize-1].value = tmpV;
						vStack[vsSize-1].diff = tmpD;
						break;
					case '-':
						tmpV = vStack[vsSize-2].value - vStack[vsSize-1].value;
						tmpD = vStack[vsSize-2].diff - vStack[vsSize-1].diff;
						vsSize -= 1;
						vStack = realloc(vStack,sizeof(float)*vsSize);	
						vStack[vsSize-1].value = tmpV;
						vStack[vsSize-1].diff = tmpD;
						break;
					case '*':
						tmpV = vStack[vsSize-1].value*vStack[vsSize-2].value;
						tmpD = vStack[vsSize-1].value*vStack[vsSize-2].diff + vStack[vsSize-2].value*vStack[vsSize-1].diff;
						vsSize -= 1;
						vStack = realloc(vStack,sizeof(float)*vsSize);	
						vStack[vsSize-1].value = tmpV;
						vStack[vsSize-1].diff = tmpD;
						break;
					case '/':
						tmpV = vStack[vsSize-2].value/vStack[vsSize-1].value;
						tmpD = (vStack[vsSize-2].diff*vStack[vsSize-1].value-vStack[vsSize-2].value*vStack[vsSize-1].value)/(vStack[vsSize].value*vStack[vsSize].value);
						vsSize -= 1;
						vStack = realloc(vStack,sizeof(float)*vsSize);	
						vStack[vsSize-1].value = tmpV;
						vStack[vsSize-1].diff = tmpD;
						break;

				}

				break;
			case 'f':
				printf("Funkcja %c\n",r->v.op);
				switch(r->v.op){
					case 'S':
						tmpV = (float)sin(vStack[vsSize-1].value);
						tmpD = (float)(cos(vStack[vsSize-1].value))*vStack[vsSize-1].diff;
						vStack[vsSize-1].value = tmpV;
						vStack[vsSize-1].diff = tmpD;
						break;
					case 'C':
						tmpV = (float)cos(vStack[vsSize-1].value);
						tmpD = (float)((-1)*sin(vStack[vsSize-1].value))*vStack[vsSize-1].diff;
						vStack[vsSize-1].value = tmpV;
						vStack[vsSize-1].diff = tmpD;
						break;
				}

				break;
			case '\0':
				tmpV = vStack[vsSize-1].diff;
				free(vStack);
				return tmpV;
		}
	printf("Po obiegu na szczycie stosu sa: v=%.2f,d=%.2f\n",vStack[vsSize-1].value,vStack[vsSize-1].diff);
	}

	tmpV = vStack[vsSize-1].diff;	
	free(vStack);
	return tmpV;

}
