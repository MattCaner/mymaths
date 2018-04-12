#include "mathparser.h"

union val{
	float f;
	char op;
	int n;
};

struct expr{
	char type;
	union val v;	
};


int precedence(char operator){
	switch(operator){
		case '(':
			return 0;
		case '+':
			return 1;
		case '-':
			return 1;
		case '*':
			return 2;
		case '/':
			return 2;
		case '^':
			return 3;
		default:
			return 4;
	}
}


struct expr * parseToRPN (const char * chain, ...){
	struct expr * const out = malloc(sizeof(struct expr)*(strlen(chain)+2)); //0th element of the table is reserved for the number of variables
	char * opst = malloc(sizeof(char)*strlen(chain));
	opst[0] = 0; //kij wam w oko
	char * vtable = malloc(sizeof(char));	
	vtable[0] = 0; //kij wam w oko jeszcze bardziej

	int outsize = 0;

	char c=chain[0];
	int cp = 0;
	int i = 0;
	while(c!='\0'){
		c = chain[i];
		if(c>='0'&&c<='9')
		{
			//printf("znaleziono cyferke %c\n",c);
			outsize++;
			//out
			for(cp = i; chain[cp]=='.'||((chain[cp]>='0'&&chain[cp]<='9'));cp++) /*printf ("Znaleziono cyferkoznaczek %c\n",chain[cp])*/;
			char * tmpstr = malloc(sizeof(char)*(cp-i+2));
			memcpy(tmpstr,chain+i,sizeof(char)*(cp-i+1));
			tmpstr[cp-i+1] = '\0';
			out[outsize].type = 'n';
			out[outsize].v.f = (float)(atof(tmpstr));
			//printf("Dodano wartosc %.2f\n",out[outsize].v.f);
			free(tmpstr);
			i=cp-1;
		}

		if(c=='['){
			outsize++;
			out[outsize].type = 'v';
			int check = 0;
			for(int j = 1; j<=(int)vtable[(int)vtable[0]]; j++){
				if(chain[i+1] == vtable[j]) check = j;
			}
			if(check==0){
				vtable[0]++;
				vtable = realloc(vtable,(int)vtable[0]);
				vtable[vtable[0]] = chain[i+1];
				out[outsize].v.n = (int)vtable[0];
			}
			else{
				out[outsize].v.n = check;
			}	
			i+=2;
		}


		if(c=='+'||c=='*'||c=='/'||c=='-'||c=='^'){
			//printf("znaleziono znaczek %c\n",c);
			if(opst[0]!=0&&precedence(opst[opst[0]])>=precedence(c)){
			//	printf("if\n");
				while(opst[0]!=0&&precedence(opst[opst[0]])>=precedence(c)){
					outsize++;
					out[outsize].v.op = opst[opst[0]];
					out[outsize].type = 'o';
					opst[0]--;
				}
			opst[0]++;
			opst[opst[0]] = c;			
			}
			else{
			//	printf("else\n");
				opst[0]++;
				opst[opst[0]]=c;
			}

		}
		if(c=='l'||c=='s'||c=='c'||c=='t'||c=='a'){
			cp = i;
			char tmpf;
			switch(c){
				case 'l':
					cp++;
					if(chain[cp]=='n'){
						tmpf = 'l';	
					}
					else{
						printf("Expression error. Terminating application.\n");
						exit(1);
					}
					i+=1;
					break;
				case 's':
					i+=2;
					tmpf = 'S';
					break;
				case 'c':
					i+=2;
					tmpf = 'C';
					break;
				case 't':
					i+=1;
					tmpf = 'T';
					break;
				case 'a':
					i+=3;
					switch(chain[i]){
						case 's':
							i+=2;
							tmpf = 's';
							break;
						case 'c':
							i+=2;
							tmpf = 'c';
							break;
						case 't':
							i+=1;
							tmpf = 't';					
					}
				break;
			}
			opst[0]++;
			opst[opst[0]]=tmpf;
		}
		if(c=='('){
			//printf("nawiasik (\n");
			opst[0]++;
			opst[opst[0]]='(';
		}
		if(c==')'){
			//printf("nawiasik )\n");
			while(opst[opst[0]]!='('){
				outsize++;
				out[outsize].v.op = opst[opst[0]];
				out[outsize].type = 'o';
				opst[0]--;
			}
			opst[0]--;
			char tmpc = opst[opst[0]];
			if(tmpc=='s'||tmpc=='S'||tmpc=='c'||tmpc=='C'||tmpc=='t'||tmpc=='T'||tmpc=='l'){
				outsize++;
				out[outsize].type = 'f';
				out[outsize].v.op = opst[opst[0]];
				opst[0]--;
			}
		}	
		i++;
	}

	if(c=='\0'){
		//printf("Koniec lancucha, dlugosc outsize %d, dlugosc opst %d\n",outsize,opst[0]);
		while(opst[opst[0]]!=0){
			outsize++;
			out[outsize].v.op = opst[opst[0]];
			out[outsize].type = 'o';
			//printf("Zrobione wepchniecie\n");
			opst[0]--;
		}
	}

	out[outsize+1].type = '\0';
	out[0].v.n = (int)vtable[0];
	free(vtable);	//JESTES WOLNY, ZGREDKU!
	free(opst);
	//printf("YOLO\n");
	return out;
}

void printFromRPN(struct expr * r){
	//printf("Uruchomiono wypisywajke r jest rowny %p\n",r);
	r++;
	while(r->type!='\0'){
		if(r->type=='n') printf("%.2f ",r->v.f);
		if(r->type=='o') printf("%c ",r->v.op);
		if(r->type=='f') printf("%c ",r->v.op);
		if(r->type=='v') printf("[%d] ",r->v.n);
		r++;
	}
	printf("\n");
}

float countInRPN(struct expr * r, ...){
	float * sf = malloc(sizeof(float));		//na poczatku byl stakfloutuf, a potem stala sie swiatlosc
	int ssize = 0;
	int argsize = r->v.n;
	float * vars = malloc(sizeof(float)*(argsize+1));
	va_list arguments;
	va_start(arguments,r);
	for(int i = 1; i<=argsize; i++){
		vars[i] = (float)va_arg(arguments,double);
	}
	va_end(arguments);
	r++;
	while(r->type!='\0'){
		if(r->type=='n'){
			ssize++;
			sf = realloc(sf,sizeof(float)*(ssize));
			sf[ssize] = r->v.f;
			//printf("Liczba %.2f trafia na stos\n",sf[ssize]);
		}
		if(r->type=='v'){
			ssize++;
			sf = realloc(sf,sizeof(float)*ssize);
			sf[ssize] = vars[r->v.n];
		}
		if(r->type=='o'){
			float tmp;
			switch(r->v.op){
				case '+':
					tmp = sf[ssize-1]+sf[ssize];
					ssize--;
					sf = realloc(sf,sizeof(float)*(ssize));
					sf[ssize] = tmp;
				break;
				case '-':
					tmp = sf[ssize-1]-sf[ssize];
					ssize--;
					sf = realloc(sf,sizeof(float)*(ssize));
					sf[ssize] = tmp;
				break;
				case '*':
					tmp = sf[ssize-1]*sf[ssize];
					ssize--;
					sf = realloc(sf,sizeof(float)*(ssize));
					sf[ssize] = tmp;
				break;
				case '/':
					tmp = sf[ssize-1]/sf[ssize];
					ssize--;
					sf = realloc(sf,sizeof(float)*(ssize));
					sf[ssize] = tmp;
				break;
				case '^':
					tmp = pow(sf[ssize-1],sf[ssize]);
					ssize--;
					sf = realloc(sf,sizeof(float)*(ssize));
					sf[ssize] = tmp;
				break;
			}
			//printf("Po obliczeniach Liczba %.2f trafia na stos\n",sf[ssize]);
		}
		if(r->type=='f'){
			switch(r->v.op){
				case 'l':
					sf[ssize] = (float)log(sf[ssize]);
					break;			
				case 'S':
					sf[ssize] = (float)sin(sf[ssize]);
					break;	
				case 's':
					sf[ssize] = (float)asin(sf[ssize]);
					break;	
				case 'C':
					sf[ssize] = (float)cos(sf[ssize]);
					break;	
				case 'c':
					sf[ssize] = (float)acos(sf[ssize]);
					break;	
				case 'T':
					sf[ssize] = (float)tan(sf[ssize]);
					break;	
				case 't':
					sf[ssize] = (float)atan(sf[ssize]);
					break;	
			}
		}

		r++;
	}
	float toreturn = sf[ssize];
	free(sf);
	free(vars);
	return toreturn;
}
