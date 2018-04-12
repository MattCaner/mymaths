#ifndef _MATHPARSER_H
#define _MATHPARSER_H
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<stdarg.h>

/* Created by Surt Ignir Magnis the Incinerator, son of Svartr, son of Borr, who was grandson of Ymir,
 * count of Thunder's Edge, High King of Musspellheim, Leader of The Forces of Ragnarok,
 * Lord Of Fire, Captain of Nagflar, The One That Shall Burn Yggdrasil, Killer of Gods,
 * etc. etc. etc. [a.k.a. student 297927]
 *
 * uses a very naive approach to Shunting Yard Algorithm (firstly I thought Shunting and Yard were surnames, so hehe and haha and generally lol)
 * and even more naive approach to RPN parsing, so use wisely!
 *
 *  The following math functions are (or at least should be) supported:	
 *  x+y, x-y, x*y, x/y, x^y, sin(x), cos(x), tg(x), arcsin(x), arccos(x), arctg(x), ln(x)
 *  Any function that receives only one argument (like sin) must have this argument in brackets. si(10) is ok, sin 10 will break the parser.
 *  The parser does not understand multiplicating two neighbouring expressions as default. 2.2*sin(7) is ok, 2.2sin(7) is not.
 *
 *  Variable can be represented by any single char put into square brackets, for example [x], [0], [W], [@].
 *  [\n], [\0], and other control chars ARE NOT recommended and risk of breaking the parser is QUITE HIGH.
 *
 *  Example syntax:
 *  
 *  7.23*sin([x]/2)+2.13^[y]*arctg(0.1+[h])
 *
 *  Function countInRPN given wrong number of arguments will break.
 *
 *  Currently parser is very delicate and any syntax error is likely to break it. The plans are to change it.
 *  
 *  The algorithm tends to waste some space (size of one float in parseToRPN and size of one float in countInRPN). I know it.
 *  I still think if changing it is a good idea, because rewriting algorithm to use space more efficiently would slow it down.
 *	
 *  U know bro, there are many better parsing algorithms, better google and download them, using this one may be dangerous
 *
 */

union val;
struct expr; //expr is a structure used in creation of RPN arrays. Each element has a type (operator, function, explicit value, variable or end of array).
////First element of array holds number of variables stored in the array, while the last element is represented by '\n' type.


struct expr * parseToRPN (const char * , ...);	//Parses a character chain, creating an array of structs, representing a mathematical expression in RPN.
void printFromRPN(struct expr * r);		//Takes a pointer to the first element of RPN array, printfs it in the console.
float countInRPN(struct expr * r, ...);		//Counts the numerical value of an expression array, given values of variables as additional arguments.

int precedence(char);	//Utility function used to check operator precedence in parser.

#endif
