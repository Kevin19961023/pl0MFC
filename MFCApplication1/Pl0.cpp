#include "stdafx.h"
#include "Pl0.h"


#define stacksize 500

CPl0::CPl0(void)
{
}


CPl0::~CPl0(void)
{
}


int CPl0::decodePl0()
{
	rr = 0;
	err_commaorsemi = -1;
	bool nxtlev[symnum];
        tableswitch=1;
		init();
		err = 0;
		cc = cx = ll = 0;
		
		f_cx = 0;
		e.adr = -1;
		e.val = -1;
		tempNum = cxmax;
		ch = ' ';
		if (-1 != getsym())
		{
			addset(nxtlev,declbegsys,statbegsys,symnum);
			nxtlev[period] = true;
			if(-1==block(0,0,nxtlev))           /*调用编译程序*/
            {
                printf("\n");
                return 0;
            }
             if(sym!=period)
            {
                error(9);
            }
            if(err==0)
            {
                //fa2=fopen("fa2.tmp", "w");
                //interpret();
                //fclose(fa2);
            }
            else
            {
                printf("Errors in pl/0 program");
            }
		}

	//	fdisplay();

	
	printf("\n");

	printf("出错总数为%d\n",err);
	getchar();
	getchar();
	
	return 0;
}

void CPl0::init()
{
	for (int i = 0; i <= 255; i++)
	{
		ssym[i] = nul;
	}
	ssym['+'] = plus;
	ssym['-'] = minus;
	ssym['*'] = times;
	ssym['/'] = slash;
	ssym['('] = lparen;
	ssym[')'] = rparen;
	ssym['='] = eql;
	ssym[','] = comma;
	ssym['.'] = period;
	ssym['#'] = neq;
	ssym[';'] = semicolon;
	strcpy_s(&(word[0][0]),10,"begin");
	strcpy_s(&(word[0][0]),10,"begin");
	strcpy_s(&(word[1][0]),10,"call");
	strcpy_s(&(word[2][0]),10,"const");
	strcpy_s(&(word[3][0]),10,"do");
	strcpy_s(&(word[4][0]),10,"end");
	strcpy_s(&(word[5][0]),10,"if");
	strcpy_s(&(word[6][0]),10,"odd");
	strcpy_s(&(word[7][0]),10,"procedure");
	strcpy_s(&(word[8][0]),10,"read");
	strcpy_s(&(word[9][0]),10,"then");
	strcpy_s(&(word[10][0]),10,"var");
	strcpy_s(&(word[11][0]),10,"while");
	strcpy_s(&(word[12][0]),10,"write");

	wsym[0] = beginsym;
	wsym[1] = callsym;
	wsym[2] = constsym;
	wsym[3] = dosym;
	wsym[4] = endsym;
	wsym[5] = ifsym;
	wsym[6] = oddsym;
	wsym[7] = procsym;
	wsym[8] = readsym;
	wsym[9] = thensym;
	wsym[10] = varsym;
	wsym[11] = whilesym;
	wsym[12] = writesym;

	strcpy_s(&(mnemonic[lit][0]),5,"lit");
    strcpy_s(&(mnemonic[opr][0]),5,"opr");
    strcpy_s(&(mnemonic[lod][0]),5,"lod");
    strcpy_s(&(mnemonic[sto][0]),5,"sto");
    strcpy_s(&(mnemonic[cal][0]),5,"cal");
    strcpy_s(&(mnemonic[inte][0]),5,"int");
    strcpy_s(&(mnemonic[jmp][0]),5,"jmp");
    strcpy_s(&(mnemonic[jpc][0]),5,"jpc");

	for (int i = 0; i < symnum; i++)
	{
		declbegsys[i] = false;
		statbegsys[i] = false;
		facbegsys[i] = false;
	}

	declbegsys[constsym] = true;
	declbegsys[varsym] = true;
	declbegsys[procsym] = true;
	
	
	statbegsys[beginsym] = true;
	statbegsys[callsym] = true;
	statbegsys[ifsym] = true;
	statbegsys[whilesym] = true;


	facbegsys[ident] = true;
	facbegsys[number] = true;
	facbegsys[lparen] = true;

}

int CPl0::inset(int e, bool* s)
{
	return s[e];
}

int CPl0::addset(bool* sr,bool* s1,bool* s2,int n)
{
	for (int i = 0; i < n; i++)
	{
		sr[i] = s1[i]||s2[i];
	}
	return 0;
}

int CPl0::subset(bool* sr,bool* s1,bool* s2,int n)
{
	for (int i = 0; i < n; i++)
	{
		sr[i] = s1[i]&&(!s2[i]);
	}
	return 0;
}

int CPl0::mulset(bool* sr,bool* s1,bool* s2,int n)
{
	for (int i = 0; i < n; i++)
	{
		sr[i] = s1[i]&&s2[i];
	}
	return 0;
}

void CPl0::error(int n)
{               
	if (err_commaorsemi == err_semi)
	{
		printf("-------出错位置第%d行\n",rr-1);
		errorStr <<"   出错位置第" << rr-1 << "行   ";
	}
	else
	{
		printf("-------出错位置第%d行\n",rr);
		errorStr <<"   出错位置第" << rr << "行   ";
	}
	printf("-------错误符号%c错误编号%d\n",ch,n);
	errorStr <<"   错误编号" << n << "\r\n";
	err_commaorsemi = -1;
    err++;
}

int CPl0::getch()
{
	if (cc == ll)//读一行到缓冲区
	{
		if (contentIndex == content.size())
		{
			printf("程序不完整");
			return -1;
		}
		ll = 0;
		cc = 0;
		printf("%d ",rr+1);
        ch=' ';
		while (ch != 10)
		{
			if (contentIndex == content.size())
			{
				line[ll] = 0;
				break;
			}
			
			printf("%c",ch);
			ch = content[contentIndex];
			if (ch != '\r')
			{
				line[ll] = ch;
				ll++;
				
			}
			contentIndex++;
			
		}
		printf("\n");
		rr++;
	}
	ch = line[cc];
	cc++;
	return 0;
}

int CPl0::getsym()
{
	int i,j,k;
	while ( (ch == ' ')||(ch == 10)||(ch == 9) )//9---TAB
	{
		getchdo;
	}
	if(ch >= 'a' && ch <= 'z')
	{
		k = 0;
		do
		{
			if (k < al)
			{
				a[k] = ch;
				k++;
			}
			getchdo;
		} while( (ch>='a'&&ch<='z')||(ch>='0'&&ch<='9') );
		a[k] = 0;
		strcpy_s(id,11,a);
		i = 0;
		j = norw - 1;
		do
		{
			k = (i+j)/2;
			if (strcmp(id,word[k])<=0)
			{
				j = k-1;
			}
			if (strcmp(id,word[k])>=0)
			{
				i = k+1;
			}
		} while (i <= j);
		if(i-1>j)
        {
            sym=wsym[k];
        }
        else
        {
            sym=ident;
        }

	}
	else if (ch >= '0' && ch <= '9')
	{
		k = 0;
		num = 0;
		sym = number;
		do
		{
			num = 10*num + ch - '0';
			k++;
			getchdo;
		} while (ch >= '0' && ch <= '9');
		k--;
		if (k > nmax)
		{
			error(30);
		}
	}
	else if (ch == ':')
	{
		getchdo;
        if(ch=='=')
        {
			sym=becomes;
			getchdo;    
		}
        else
        {
			sym=nul;            /*不能识别的符号*/
            
		}
	}
	else if (ch == '<')
	{
		getchdo;
        if(ch=='=')
        {
			sym=leq;
			getchdo;    
		}
        else
        {
			sym=lss;           
            
		}
	}
	else if (ch == '>')
	{
		getchdo;
        if(ch=='=')
        {
			sym=geq;
			getchdo;    
		}
        else
        {
			sym=gtr;
            
		}
	}
	else       
	{     
		sym = ssym[ch];
		if (sym != period)
		{
			getchdo;
		}
	}
	return 0;
}

int CPl0::gen(enum fct x,int y,int z)
{
	if (cx >= cxmax)
	{
		printf("程序过长");
		return -1;
	}
	code[cx].f = x;
	code[cx].l = y;
	code[cx].a = z;
	cx++;
	return 0;
}

int CPl0::test(bool* s1,bool* s2,int n)
{
	if (!inset(sym,s1))
	{
		//error(n);
		while ((!inset(sym,s1))&&(!inset(sym,s2)))
		{
			getsymdo;
		}
	}
	return 0;
}

void CPl0::enter(enum object k,int* ptx, int lev, int* pdx)
{
	(*ptx)++;
	strcpy_s(table[(*ptx)].name,11,id);
	table[(*ptx)].kind = k;
	switch (k)
	{
		case constant:
			if (num > amax)
			{
				error(31);
				num = 0;
			}
			table[(*ptx)].val = num;
			break;
		case variable:
			table[(*ptx)].adr = (*pdx);
			table[(*ptx)].level = lev;
			(*pdx)++;
			break;
		case procedur:
			table[(*ptx)].level = lev;
			break;
	}
}

int CPl0::position(char* idt,int tx)
{
	int i;
	strcpy_s(table[0].name,11,idt);
	i = tx;
	while (strcmp(table[i].name, idt) != 0)
	{
		i--;
	}
	return i;
}

int CPl0::constdeclaration(int *ptx,int lev,int *pdx)
{
	if (sym == ident)
	{
		getsymdo;
		if (sym == eql || sym == becomes)
		{
			if (sym == becomes)
			{
				error(1);
			}
			getsymdo;
			if (sym == number)
			{
				enter(constant,ptx,lev,pdx);
				getsymdo;
			}
			else
			{
				error(2);
			}
		}
		else
		{
			error(3);
		}
	}
	else
	{
		error(4);
	}
	return 0;
}

int CPl0::vardeclaration(int *ptx,int lev,int *pdx)
{
	if (sym == ident)
	{
		enter(variable,ptx,lev,pdx);
		getsymdo;
	}
	else
	{
		error(4);
	}
	return 0;
}

void CPl0::listcode(int cx0)
{
    int i;
    if (listswitch)
    {
        for(i=cx0;i<cx;i++)
        {
            printf("%d %s %d %d\n",i,mnemonic[code[i].f],code[i].l,code[i].a);
        }
     }
}

int CPl0::statement(bool* fsys,int *ptx,int lev,f_e *le)
{
	int i,cx1,cx2;
	bool nxtlev[symnum];
	if (sym == ident)
	{
		i = position(id, *ptx);
		if (i == 0)
		{
			error(11);
			getsymdo;
		}
		else
		{
			if (table[i].kind != variable)
			{
				error(12);
				i = 0;
			}
			else
			{

				getsymdo;
				if (sym == becomes)
				{
					getsymdo;
				}
				else
				{
					error(13);
				}
				memcpy(nxtlev,fsys,sizeof(bool)*symnum);
				f_e sub_e;
				initf_e(&sub_e);
				expressiondo(nxtlev,ptx,lev,&sub_e);
				if (i != 0)
				{
					gendo(sto,lev - table[i].level,table[i].adr);
					emit(becomes,getplace(sub_e),-1,i);
				}
			}
		}
	}
	else if (sym == readsym)
	{
		getsymdo;
		if (sym != lparen)
		{
			error(34);
		}
		else
		{
				do
				{
					getsymdo;
					if (sym == ident)
					{
						i = position(id,*ptx);
					}
					else
					{
						i = 0;
					}
					if (i == 0)
					{
						error(35);
					}
					else
					{
						gendo(opr,0,16);
						gendo(sto,lev - table[i].level,table[i].adr);
						emit(readsym,i,-1,-1);
					}
					getsymdo;
				} while (sym == comma);
		}
		if (sym != rparen)
		{
			error(33);
			while (!inset(sym,fsys))
			{
				getsymdo;
			}
		}
		else
		{
			getsymdo;
		}
		
	}
	else if(sym == writesym)
	{
		getsymdo;
		if (sym == lparen)
		{
			do
			{
				getsymdo;
				memcpy(nxtlev,fsys,sizeof(bool)*symnum);
				nxtlev[rparen] = true;
				nxtlev[comma] = true;
				f_e sub_e;
				initf_e(&sub_e);
				expressiondo(nxtlev,ptx,lev,&sub_e);
				gendo(opr,0,14);
				emit(writesym,getplace(sub_e),-1,-1);

			} while (sym == comma);
			if (sym != rparen)
			{
				error(33);
			}
			else
			{
				getsymdo;
			}
		}
		gendo(opr,0,15);
	}
	else if (sym == callsym)
	{
		getsymdo;
		if (sym != ident)
		{
			error(14);
		}
		else
		{
			i = position(id, *ptx);
			if (i == 0)
			{
				error(11);
			}
			else
			{
				if (table[i].kind == procedur)
				{
					gendo(cal,lev - table[i].level,table[i].adr);
					emit(callsym,i,-1,-1);

				}
				else
				{
					error(15);
				}
			}
			getsymdo;
		}
	}
	else if (sym == ifsym)
	{
		getsymdo;
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[thensym] = true;
		nxtlev[dosym] = true;
		f_e sub_e;
		initf_e(&sub_e);
		
		conditiondo(nxtlev,ptx,lev,&sub_e);
		if (sym == thensym)
		{
			getsymdo;
		}
		else
		{
			error(16);
		}
		cx1 = cx;
		gendo(jpc,0,0);
		f_e sub_e2;
		initf_e(&sub_e2);
		statementdo(fsys,ptx,lev,&sub_e2);
		code[cx1].a = cx;
		f_code[sub_e.val].re = f_cx;
	}
	else if (sym == beginsym)
	{
		getsymdo;
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[semicolon] = true;
		nxtlev[endsym] = true;
		f_e sub_e;
		initf_e(&sub_e);
		statementdo(nxtlev,ptx,lev,&sub_e);
		while (inset(sym,statbegsys) || sym == semicolon)//如果分析完一句后遇到分号或语句开始符循环分析下一句语句
		{
			if (sym == semicolon)
			{
				getsymdo;
			}
			else
			{
				error(10);
			}
			f_e temp_e;
			initf_e(&temp_e);
			statementdo(nxtlev,ptx,lev,&temp_e);
		}
		if (sym == endsym)
		{
			getsymdo;
		}
		else
		{
			error(17);
		}
	}
	else if (sym == whilesym)
	{
		f_e sub_e;
		initf_e(&sub_e);
		cx1 = cx;
		getsymdo;
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[dosym] = true;
		conditiondo(nxtlev,ptx,lev,&sub_e);
		cx2 = cx;
		gendo(jpc,0,0);
		if(sym == dosym)
		{
			getsymdo;
		}
		else
		{
			error(18);
		}
		f_e sub_e2;
		initf_e(&sub_e2);
		statementdo(fsys,ptx,lev,&sub_e2);
		gendo(jmp,0,cx1);
		emit(gotoIdt,-1,-1,sub_e.adr);
		code[cx2].a = cx; 
		f_code[sub_e.val].re = f_cx;
	}
	memcpy(nxtlev,fsys,sizeof(bool)*symnum);
	testdo(fsys,nxtlev,19);
	
	return 0;
}

int CPl0::expression(bool* fsys,int *ptx,int lev,f_e *le)
{
	enum symbol addop;
	int flag = -1;//用于区分不同情况
	bool nxtlev[symnum];
	f_e sub_e1,sub_e2,sub_e3;
	initf_e(&sub_e1);initf_e(&sub_e2);initf_e(&sub_e3);
	if (sym == plus || sym == minus)
	{
		addop = sym;
		getsymdo;
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[plus] = true;
		nxtlev[minus] = true;
		termdo(nxtlev,ptx,lev,&sub_e1);
		if (addop == minus)
		{
			gendo(opr,0,1);
			emit(negIdt,getplace(sub_e1),-1,tempNum);
			sub_e1.adr = tempNum;
			tempNum++;
			flag = 0;
		}
		else
		{
			flag = 1;
		}
		
	}
	else
	{
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[plus] = true;
		nxtlev[minus] = true;
		termdo(nxtlev,ptx,lev,&sub_e1);
		flag = 1;
	}
	sub_e3 = sub_e1;
	while (sym == plus || sym == minus)
	{
		addop = sym;
		getsymdo;
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[plus] = true;
		nxtlev[minus] = true;
		sub_e2 = sub_e3;
		termdo(nxtlev,ptx,lev,&sub_e3);
		if (addop == plus)
		{
			gendo(opr,0,2);
			emit(plus,getplace(sub_e2),getplace(sub_e3),tempNum);
		}
		else
		{
			gendo(opr,0,3);
			emit(minus,getplace(sub_e2),getplace(sub_e3),tempNum);
		}
		sub_e3.adr = tempNum;
		tempNum++;
		flag = 2;
	}
	if (flag == 1)
	{
		(*le).adr = getplace(sub_e1);
	}
	else if(flag == 0 || flag == 2)
	{
		(*le).adr = tempNum - 1;
	}
	return 0;

}

int CPl0::term(bool* fsys,int *ptx,int lev,f_e *le)
{
	int flag = 0;
	enum symbol mulop;
	f_e sub_e1,sub_e2,sub_e3;
	initf_e(&sub_e1);initf_e(&sub_e2);initf_e(&sub_e3);
	bool nxtlev[symnum];
	memcpy(nxtlev,fsys,sizeof(bool)*symnum);
	nxtlev[times] = true;
	nxtlev[slash] = true;
	factordo(nxtlev,ptx,lev,&sub_e1);
	sub_e3 = sub_e1;
	while (sym == times || sym == slash)
	{
		sub_e2 = sub_e3;
		mulop = sym;
		getsymdo;
		factordo(nxtlev,ptx,lev,&sub_e3);
		if (mulop == times)
		{
			gendo(opr,0,4);
			emit(times,getplace(sub_e2),getplace(sub_e3),tempNum);
		}
		else
		{
			gendo(opr,0,5);
			emit(slash,getplace(sub_e2),getplace(sub_e3),tempNum);
		}
		sub_e3.adr = tempNum;
		tempNum++;
		flag = 1;
	}
	if (flag == 1)
	{
		(*le).adr = tempNum - 1;
	}
	else if(flag == 0)
	{
		(*le).adr = getplace(sub_e1);
	}
	return 0;
}

int CPl0::factor(bool* fsys,int *ptx,int lev,f_e *le)
{
	int i;
	bool nxtlev[symnum];
	testdo(facbegsys,fsys,24);
	while (inset(sym,facbegsys))
	{
		if (sym == ident)
		{
			i = position(id,*ptx);
			if (i == 0)
			{
				error(11);
			}
			else
			{
				switch (table[i].kind)
				{
					case constant:gendo(lit,0,table[i].val);
						(*le).adr = i;
						(*le).val = -1;
						break;
					case variable:gendo(lod,lev - table[i].level,table[i].adr);
						(*le).adr = i;
						(*le).val = -1;
						break;
					case procedur:error(21);break; 
				}

			}
			getsymdo;
		}
		else
		{
			if (sym == number)
			{
				if (num > amax)
				{
					error(31);
					num = 0;
				}
				(*le).adr = -1;
				(*le).val = num+1000;
				gendo(lit,0,num);
				getsymdo;
			}
			else
			{
				if (sym == lparen)
				{
					getsymdo;
					memcpy(nxtlev,fsys,sizeof(bool)*symnum);
					nxtlev[rparen] = true;
					f_e sub_e;
					initf_e(&sub_e);
					expressiondo(nxtlev,ptx,lev,&sub_e);
					*le = sub_e;
					if (sym == rparen)
					{
						getsymdo;
					}
					else
					{
						error(22);
					}
				}
				testdo(fsys,facbegsys,23);
			}
		}
		
	}
	return 0;
}

int CPl0::condition(bool* fsys,int *ptx,int lev,f_e *le)
{
	enum symbol relop;
	bool nxtlev[symnum];
	f_e sub_e1,sub_e2;
	initf_e(&sub_e1);initf_e(&sub_e2);
	if (sym == oddsym)
	{
		getsymdo;
		expressiondo(fsys,ptx,lev,&sub_e1);
		(*le).adr = f_cx;
		(*le).val = f_cx+1;
		emit(oddsym,getplace(sub_e1),-1,f_cx+2);
		emit(gotoIdt,-1,-1,-1);
		gendo(opr,0,6);
	}
	else
	{
		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
        nxtlev[eql]=true;
        nxtlev[neq]=true;
        nxtlev[lss]=true;
        nxtlev[leq]=true;
        nxtlev[gtr]=true;
        nxtlev[geq]=true;
		expressiondo(nxtlev,ptx,lev,&sub_e1);
		if (sym != eql && sym != neq && sym != lss && sym != leq && sym != gtr && sym != geq)
		{
			error(20);
		}
		else
		{
			relop = sym;
			getsymdo;
			expressiondo(fsys,ptx,lev,&sub_e2);
			switch (relop)
			{
			
				case eql:
					gendo(opr,0,8);
					(*le).adr = f_cx;
					(*le).val = f_cx+1;
					emit(eql,getplace(sub_e1),getplace(sub_e2),f_cx+2);
					emit(gotoIdt,-1,-1,-1);
					break;
				case neq:
					(*le).adr = f_cx;
					(*le).val = f_cx+1;
					emit(neq,getplace(sub_e1),getplace(sub_e2),f_cx+2);
					emit(gotoIdt,-1,-1,-1);
					gendo(opr,0,9);
					break;
				case lss:
					(*le).adr = f_cx;
					(*le).val = f_cx+1;
					emit(lss,getplace(sub_e1),getplace(sub_e2),f_cx+2);
					emit(gotoIdt,-1,-1,-1);
					gendo(opr,0,10);
					break;
				case leq:
					(*le).adr = f_cx;
					(*le).val = f_cx+1;
					emit(leq,getplace(sub_e1),getplace(sub_e2),f_cx+2);
					emit(gotoIdt,-1,-1,-1);
					gendo(opr,0,13);
					
					break;
				case gtr:
					(*le).adr = f_cx;
					(*le).val = f_cx+1;
					emit(gtr,getplace(sub_e1),getplace(sub_e2),f_cx+2);
					emit(gotoIdt,-1,-1,-1);
					gendo(opr,0,12);
					
					break;
				case geq:
					(*le).adr = f_cx;
					(*le).val = f_cx+1;
					emit(geq,getplace(sub_e1),getplace(sub_e2),f_cx+2);
					emit(gotoIdt,-1,-1,-1);
					gendo(opr,0,11);
					
					break;
			
			}
		}
	}
	return 0;
}

int CPl0::block(int lev,int tx,bool* fsys)
{
	int i;
	int dx;
	int tx0;
	int cx0;
	bool nxtlev[symnum];
	dx = 3;
	tx0 = tx;
	table[tx].adr = cx;
	gendo(jmp,0,0);
	if (lev > levmax)
	{
		error(32);
	}
	do
	{
		if (sym == constsym)
		{
			getsymdo;
			do
			{
				constdeclarationdo(&tx,lev,&dx);
				while (sym == comma)
				{
					getsymdo;
					constdeclarationdo(&tx,lev,&dx);
				}
				if (sym == semicolon)
				{
					getsymdo;
				}
				else
				{
					err_commaorsemi = err_semi;
					error(5);
				}
			} while (sym == ident);
		}
		if (sym == varsym)
		{
			getsymdo;
			do
			{
				vardeclarationdo(&tx,lev,&dx);
				while (sym == comma)
				{
					getsymdo;
					vardeclarationdo(&tx,lev,&dx);
				}
				if (sym == semicolon)
				{
					getsymdo;
				}
				else
				{
					err_commaorsemi = err_semi;
					error(5);
				}
			} while (sym == ident);
		}
 		while (sym == procsym)
		{
			getsymdo;
			if (sym == ident)
			{
				enter(procedur,&tx,lev,&dx);
				getsymdo;
			}
			else
			{
				error(4);
			}
			if (sym == semicolon)
			{
				getsymdo;
			}
			else
			{
				err_commaorsemi = err_semi;
				error(5);
			}
			memcpy(nxtlev,fsys,sizeof(bool)*symnum);
            nxtlev[semicolon]=true;
            if(-1==block(lev+1,tx,nxtlev))
            {
                return -1;/*递归调用*/
            }
			if (sym == semicolon)
			{
				getsymdo;
				memcpy(nxtlev,fsys,sizeof(bool)*symnum);
				nxtlev[ident]=true;
				nxtlev[procsym]=true;
				testdo(nxtlev,fsys,6);
			}
			else
			{
				err_commaorsemi = err_semi;
				error(5);
			}

		}

		memcpy(nxtlev,fsys,sizeof(bool)*symnum);
		nxtlev[ident]=true;
		nxtlev[period]=true;
		testdo(nxtlev,declbegsys,7);
	} while (inset(sym,declbegsys));
	code[table[tx0].adr].a = cx;
	table[tx0].adr = cx;
	table[tx0].size = dx;

	cx0 = cx;
	gendo(inte,0,dx);
	if (tableswitch)
	{
		printf("TABLE:\n");
        if(tx0+1>tx)
        {
            printf("NULL\n");
        }
        for(i=tx0+1;i<=tx;i++)
        {
            switch(table[i].kind)
            {
                case constant:
                    printf("%d    const    %s    ",i,table[i].name);
                    printf("val=%d\n",table[i].val);
					tableStr << i << "   const    | " << table[i].name << " | val="<< table[i].val<< "\r\n";
                    break;
                case variable:
                    printf("%d    var   %s    ",i,table[i].name);
                    printf("lev=%d    addr=%d\n",table[i].level,table[i].adr);
					tableStr << i << "   var    | " << table[i].name << " | lev="<< table[i].level<< " | addr="<< table[i].adr<< "\r\n";
                    
                    break;
                case procedur:
                    printf("%d    proc    %s    ",i,table[i].name);
                    printf("lev=%d    addr=%d    size=%d\n",table[i].level,table[i].adr,table[i].size);
					tableStr << i << "   proc    | " << table[i].name << " | lev="<< table[i].level<< " | addr="<< table[i].adr<< " | size="<< table[i].size<< "\r\n";
                    
                    break;
            }
		}
		printf("\n");
	}
 	memcpy(nxtlev,fsys,sizeof(bool)*symnum);
	nxtlev[semicolon]=true;
	nxtlev[endsym]=true;
	f_e init_e;
	initf_e(&init_e);
	statementdo(nxtlev,&tx,lev,&init_e);
	gendo(opr,0,0);
	memset(nxtlev,0,sizeof(bool)*symnum);
	test(fsys,nxtlev,8);
	//listcode(cx0);
	return 0;
}

int CPl0::base(int l,int* s,int b)
{
	int b1;
	b1 = b;
	while (l > 0)
	{
		b1 = s[b1];
		l--;
	}
	return b1;

}

void CPl0::initf_e(f_e *e)
{
	(*e).adr = (*e).val = -1;
}

void CPl0::emit(enum symbol f,int a,int b,int c)
{
	f_code[f_cx].f = f;
	f_code[f_cx].op1 = a;
	f_code[f_cx].op2 = b;
	f_code[f_cx].re = c;
	f_cx++;
}

int CPl0::getplace(f_e e)
{
	if(e.adr == -1) return e.val;
	else if (e.val == -1)
	{
		return e.adr;
	}
	else
	{
		return -1;
	}
}

string CPl0::fdisplay()
{
	string resultStr = "";
	ostringstream s1;
	
	for (int i = 0; i < f_cx; i++)
	{
		string t = transfer(f_code[i].f);

		s1 << "  " << i << "    | " << t << " | "<< f_code[i].op1 << " | " << f_code[i].op2 << " | " << f_code[i].re << "\r\n";
		
		transfer(f_code[i].f);
		printf(" | ");
		printf("%d",f_code[i].op1);
		printf(" | ");
		printf("%d",f_code[i].op2);
		printf(" | ");
		printf("%d",f_code[i].re);
		printf("\n");
	}
	string s2 = s1.str();
	resultStr += s2;
	return resultStr;
}

string CPl0::tdisplay()
{
	string resultStr = "";
	
	string s2 = tableStr.str();
	resultStr += s2;
	return resultStr;
}

string CPl0::wdisplay()
{
	string resultStr = "";
	
	string s2 = errorStr.str();
	resultStr += s2;
	return resultStr;
}

string CPl0::transfer(enum symbol t)
{
	string re;
	//strcpy(re,"123");
	//printf(re);
	//getchar();
	switch (t)
	{
	case plus:re = "plus";
		break;
	case minus:re = "minus";
		break;
	case times:re = "times";
		break;
	case slash:re = "slash";
		break;
	case oddsym:re = "oddsym";
		break;
	case eql:re = "eql";
		break;
	case neq:re = "neq";
		break;
	case lss:re = "lss";
		break;
	case leq:re = "leq";
		break;
	case gtr:re = "gtr";
		break;
	case geq:re = "geq";
		break;
	case becomes:re = "becomes";
		break;
	case whilesym:re = "whilesym";
		break;
	case writesym:re = "writesym";
		break;
	case readsym:re = "readsym";
		break;
	case dosym:re = "dosym";
		break;
	case callsym:re = "callsym";
		break;
	case procsym:re = "procsym";
		break;
	case negIdt:re = "negIdt";
		break;
	case gotoIdt:re = "gotoIdt";
		break;
	default:re = "wrong";
		break;
	}
	return re;
}

  
