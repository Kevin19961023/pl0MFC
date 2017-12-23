#define norw 13
#define txmax 100
#define nmax 14
#define al 10
#define amax 2047
#define levmax 3
#define cxmax 200
#define err_semi 0
#define err_comma 1


#define getsymdo                              if(-1==getsym())return -1
#define getchdo                               if(-1==getch())return -1
#define testdo(a,b,c)                         if(-1==test(a,b,c))return -1
#define gendo(a,b,c)                          if(-1==gen(a,b,c))return -1
#define expressiondo(a,b,c,d)                   if(-1==expression(a,b,c,d))return -1
#define factordo(a,b,c,d)                       if(-1==factor(a,b,c,d))return -1
#define termdo(a,b,c,d)                         if(-1==term(a,b,c,d))return -1
#define conditiondo(a,b,c,d)                    if(-1==condition(a,b,c,d))return -1
#define statementdo(a,b,c,d)                    if(-1==statement(a,b,c,d))return -1
#define constdeclarationdo(a,b,c)             if(-1==constdeclaration(a,b,c))return -1
#define vardeclarationdo(a,b,c)               if(-1==vardeclaration(a,b,c))return -1

#include <stdio.h>
#include "string"
#include <sstream>
#include <iostream>
using namespace std;

enum symbol{
    nul,    ident,     number,     plus,     minus,
	times,   slash,   oddsym,   eql,     neq,
	lss,     leq,      gtr,     geq,     lparen,
	rparen,  comma,   semicolon,period,  becomes,
	beginsym, endsym, ifsym,    thensym,  whilesym,
	writesym, readsym,  dosym,  callsym,  constsym,
	varsym,  procsym, negIdt, gotoIdt
};

#define symnum 42

enum object{
	constant,
	variable,
	procedur,
};

enum fct{
	lit,opr,lod,sto,cal,inte,jmp,jpc,
};

struct f_instruction
{
	enum symbol f;
	int op1;
	int op2;
	int re;
};

struct f_e
{
	int adr;//变量地址/codebegin
	int val;//数字值/false
};

#define fctnum 8

struct instruction
{
	enum fct f;
	int l;
	int a;
};

struct tablestruct
{
	char name[al];
	enum object kind;
	int val;
	int level;
	int adr;
	int size;
};



#pragma once
class CPl0
{
public:
	bool tableswitch;
	bool listswitch;
	char ch;
	enum symbol sym;
	char id[al+1];
	int num;
	int cc,ll;
	int cx;
	int f_cx;//四元式指针
	int tempNum;//记录临时变量地址  起始cxmax便于区分
	char line[81];
	char a[al+1];
	struct instruction code[cxmax];
	struct f_instruction f_code[cxmax];
	int tempData[cxmax];//存放局部变量
	struct f_e e;//四元式中间变量
	char word[norw][al];
	enum symbol wsym[norw];
	enum symbol ssym[256];
	char mnemonic[fctnum][5];
	bool declbegsys[symnum];
	bool statbegsys[symnum];
	bool facbegsys[symnum];

	int err_commaorsemi;
	int rr;//记录行号

	string content;//存放源程序
	int contentIndex;

	ostringstream tableStr;//存放符号表
	ostringstream errorStr;

	struct tablestruct table[txmax];
	char fname[al];
	int err;
public:
	CPl0(void);
	~CPl0(void);
	void error(int n);
	int getch();
	int getsym();
	void init();
	int gen(enum fct x,int y,int z);
	int test(bool* s1,bool *s2,int n);
	int inset(int e, bool *s);
	int addset(bool* sr,bool* s1,bool *s2,int n);
	int subset(bool* sr,bool* s1,bool *s2,int n);
	int mulset(bool* sr,bool* s1,bool *s2,int n);
	int block(int lev,int tx,bool* fsys);
	void interpret();
	int factor(bool* fsys,int *ptx,int lev,f_e *le);
	int term(bool* fsys,int *ptx,int lev,f_e *le);
	int condition(bool* fsys,int *ptx,int lev,f_e *le);
	int expression(bool* fsys,int *ptx,int lev,f_e *le);
	int statement(bool* fsys,int *ptx,int lev,f_e *le);
	void listcode(int cx0);
	int vardeclaration(int *ptx,int lev,int *pdx);
	int constdeclaration(int *ptx,int lev,int *pdx);
	int position(char* idt,int tx);
	void enter(enum object k,int* ptx,int lev, int* pdx);
	int base(int l,int* s,int b);
	int decodePl0();
	void getCharFromString();

	void initf_e(f_e *e);
	void emit(enum symbol f,int a,int b,int c);
	int getplace(f_e e);
	string fdisplay();
	string tdisplay();
	string wdisplay();
	string transfer(enum symbol t);
};

