//
//  LexAnalysis.cpp
//  CPSC323_HW3
//
//  Created by Liren on 4/20/18.
//  Copyright © 2018 Liren. All rights reserved.
//
//LexAnalysis.cpp
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <vector>
#include <iomanip>
#include "LexAnalysis.h"

using namespace std;

int leftSmall = 0;//(
int rightSmall = 0;//)
int leftMiddle = 0;//[
int rightMiddle = 0;//]
int leftBig = 0;//{
int rightBig = 0;//}
int lineBra[6][1000] = {0};
int static_iden_number = 0;

NormalNode * normalHead;
NormalNode * normalTail;

struct ErrorNode
{
    char content[30];
    char describe[30];
    int type;
    int line;
    ErrorNode * next;
};

ErrorNode * errorHead;

struct IdentiferNode
{
    char content[30];
    char describe[30];
    int type;
    int addr;
    int line;
    IdentiferNode * next;
    IdentiferNode * prev;
};
IdentiferNode * idenHead;
IdentiferNode * idenTail;

vector<pair<const char *,int> > keyMap;
vector<pair<const char *,int> > operMap;
vector<pair<const char *,int> > limitMap;

void initKeyMapping()
{
    keyMap.clear();
    keyMap.push_back(make_pair("auto",AUTO));
    keyMap.push_back(make_pair("break",BREAK));
    keyMap.push_back(make_pair("case",CASE));
    keyMap.push_back(make_pair("char",CHAR));
    keyMap.push_back(make_pair("const",CONST));
    keyMap.push_back(make_pair("continue",CONTINUE));
    keyMap.push_back(make_pair("default",DEFAULT));
    keyMap.push_back(make_pair("do",DO));
    keyMap.push_back(make_pair("double",DOUBLE));
    keyMap.push_back(make_pair("else",ELSE));
    keyMap.push_back(make_pair("true",TRUE_VALUE));
    keyMap.push_back(make_pair("false",FALSE_VALUE));
    keyMap.push_back(make_pair("float",FLOAT));
    keyMap.push_back(make_pair("for",FOR));
    keyMap.push_back(make_pair("goto",GOTO));
    keyMap.push_back(make_pair("if",IF));
    keyMap.push_back(make_pair("int",INT));
    keyMap.push_back(make_pair("real",REAL));
    keyMap.push_back(make_pair("long",LONG));
    keyMap.push_back(make_pair("register",REGISTER));
    keyMap.push_back(make_pair("return",RETURN));
    keyMap.push_back(make_pair("short",SHORT));
    keyMap.push_back(make_pair("signed",SIGNED));
    keyMap.push_back(make_pair("sizeof",SIZEOF));
    keyMap.push_back(make_pair("static",STATIC));
    keyMap.push_back(make_pair("struct",STRUCT));
    keyMap.push_back(make_pair("switch",SWITCH));
    keyMap.push_back(make_pair("typedef",TYPEDEF));
    keyMap.push_back(make_pair("union",UNION));
    keyMap.push_back(make_pair("unsigned",UNSIGNED));
    keyMap.push_back(make_pair("void",VOID));
    keyMap.push_back(make_pair("volatile",VOLATILE));
    keyMap.push_back(make_pair("while",WHILE));
    keyMap.push_back(make_pair("put",PUT));
    keyMap.push_back(make_pair("get",GET));
    keyMap.push_back(make_pair("function",FUNCTION));
    keyMap.push_back(make_pair("boolean",BOOLEAN));
    keyMap.push_back(make_pair("endif",ENDIF));
    keyMap.push_back(make_pair("describe",DESCRIBE));
    keyMap.push_back(make_pair("type",TYPE));
    keyMap.push_back(make_pair("string",STRING));
    keyMap.push_back(make_pair("digit",DIGIT));
    keyMap.push_back(make_pair("id",IDENTIFER));
}
void initOperMapping()
{
    operMap.clear();
    
    operMap.push_back(make_pair("&",BYTE_AND));
    operMap.push_back(make_pair("~",COMPLEMENT));
    operMap.push_back(make_pair("^",BYTE_XOR));
    operMap.push_back(make_pair("*",MUL));
    operMap.push_back(make_pair("/",DIV));
    operMap.push_back(make_pair("+",ADD));
    operMap.push_back(make_pair("-",SUB));
    operMap.push_back(make_pair("<",LES_THAN));
    operMap.push_back(make_pair(">",GRT_THAN));
    operMap.push_back(make_pair("=",ASG));
    operMap.push_back(make_pair("->",ARROW));
    operMap.push_back(make_pair("++",SELF_ADD));
    operMap.push_back(make_pair("--",SELF_SUB));
    operMap.push_back(make_pair("<<",LEFT_MOVE));
    operMap.push_back(make_pair(">>",RIGHT_MOVE));
    operMap.push_back(make_pair("=<",LES_EQUAL));
    operMap.push_back(make_pair("=>",GRT_EQUAL));
    operMap.push_back(make_pair("==",EQUAL));
    operMap.push_back(make_pair(":",COLON));
    operMap.push_back(make_pair("&&",AND));
    operMap.push_back(make_pair("||",OR));
    operMap.push_back(make_pair("+=",COMPLETE_ADD));
    operMap.push_back(make_pair("-=",COMPLETE_SUB));
    operMap.push_back(make_pair("*=",COMPLETE_MUL));
    operMap.push_back(make_pair("/=",COMPLETE_DIV));
    operMap.push_back(make_pair("^=",COMPLETE_BYTE_XOR));
    operMap.push_back(make_pair("&=",COMPLETE_BYTE_AND));
    operMap.push_back(make_pair("~=",COMPLETE_COMPLEMENT));
    operMap.push_back(make_pair("%%",PERCENTPERCENT));
    operMap.push_back(make_pair("|",BYTE_OR));
}
void initLimitMapping()
{
    limitMap.clear();
    limitMap.push_back(make_pair("(",LEFT_BRA));
    limitMap.push_back(make_pair(")",RIGHT_BRA));
    limitMap.push_back(make_pair("[",LEFT_INDEX));
    limitMap.push_back(make_pair("]",RIGHT_INDEX));
    limitMap.push_back(make_pair("{",L_BOUNDER));
    limitMap.push_back(make_pair("}",R_BOUNDER));
    limitMap.push_back(make_pair(".",POINTER));
    limitMap.push_back(make_pair("#",JING));
    limitMap.push_back(make_pair("_",UNDER_LINE));
    limitMap.push_back(make_pair(",",COMMA));
    limitMap.push_back(make_pair(";",SEMI));
    limitMap.push_back(make_pair("'",SIN_QUE));
    limitMap.push_back(make_pair("\"",DOU_QUE));
}
void initNode()
{
    normalHead = new NormalNode();
    normalTail = new NormalNode();
    strcpy(normalHead->content,"");
    strcpy(normalHead->describe,"");
    normalHead->type = -1;
    normalHead->addr = -1;
    normalHead->line = -1;
    normalHead->next = NULL;
    normalHead->prev = NULL;
    
    normalTail->type = -1;
    normalTail->addr = -1;
    normalTail->line = -1;
    normalTail->next = NULL;
    normalTail->prev = NULL;
    
    errorHead = new ErrorNode();
    strcpy(errorHead->content,"");
    strcpy(errorHead->describe,"");
    errorHead->line = -1;
    errorHead->next = NULL;
    
    idenHead = new IdentiferNode();
    idenTail = new IdentiferNode();
    strcpy(idenHead->content,"");
    strcpy(idenHead->describe,"");
    idenHead->type = -1;
    idenHead->addr = -1;
    idenHead->line = -1;
    idenHead->next = NULL;
    idenHead->prev = NULL;
    
    idenTail->type = -1;
    idenTail->addr = -1;
    idenTail->line = -1;
    idenTail->next = NULL;
    idenTail->prev = NULL;
}

void createNewNode(char * content,char *descirbe,int type,int addr,int line)
{
    NormalNode * p = normalHead;
    NormalNode * temp = new NormalNode();
    
    while(p->next != NULL)
    {
        p = p->next;
    }
    
    strcpy(temp->content,content);
    strcpy(temp->describe,descirbe);
    temp->type = type;
    temp->addr = addr;
    temp->line = line;
    temp->next = NULL;
    p->next = temp;
    temp->prev = p;
    normalTail = temp;
}
void createNewError(char * content,char *descirbe,int type,int line)
{
    ErrorNode * p = errorHead;
    ErrorNode * temp = new ErrorNode();
    
    strcpy(temp->content,content);
    strcpy(temp->describe,descirbe);
    temp->type = type;
    temp->line = line;
    temp->next = NULL;
    while(p->next!=NULL)
    {
        p = p->next;
    }
    p->next = temp;
}

int createNewIden(char * content,char *descirbe,int type,int addr,int line)
{
    IdentiferNode * p = idenHead;
    IdentiferNode * temp = new IdentiferNode();
    int flag = 0;
    int addr_temp = -2;
    while(p->next!=NULL)
    {
        if(strcmp(content,p->next->content) == 0)
        {
            flag = 1;
            addr_temp = p->next->addr;
        }
        p = p->next;
    }
    if(flag == 0)
    {
        addr_temp = ++static_iden_number;
    }
    strcpy(temp->content,content);
    strcpy(temp->describe,descirbe);
    temp->type = type;
    temp->addr = addr_temp;
    temp->line = line;
    temp->next = NULL;
    p->next = temp;
    temp->prev = p;
    idenTail = temp;
    return addr_temp;
}

NormalNode* printNodeLink()
{
    NormalNode * p = normalHead;
    NormalNode * SyntaxPartHead = normalHead;
    p = p->next;
    cout<<"************************************Lexical Analysis Table******************************"<<endl<<endl;
    cout<<setw(30)<<"Lexeme"<<setw(10)<<"\t\tToken"<<"\t\t\t"<<"Line"<<endl;
    while(p!=NULL)
    {
        if(p->type == IDENTIFER)
        {
            cout<<setw(30)<<p->content<<"\t\t"<<setw(10)<<p->describe<<"\t\t\t"<<p->line<<endl;
        }
        else
        {
            cout<<setw(30)<<p->content << "\t\t"<<setw(10) << p->describe<<"\t\t\t"<<p->line<<endl;
        }
        p = p->next;
    }
    cout<<endl<<endl;
    return SyntaxPartHead;
}

void printErrorLink(bool &TokenError)
{
    ErrorNode * p = errorHead;
    if (p->next != NULL) {
        TokenError = true;
    }
    else TokenError = false;
    p = p->next;
    cout<<"************************************Error Table******************************"<<endl<<endl;
    cout<<setw(10)<<"Lexeme"<<setw(30)<<"\t\t\t\t\tToken"<<"\t\t\t"<<"Line"<<endl;
    while(p!=NULL)
    {
        cout<<setw(10)<<p->content<< "\t\t\t\t\t\t" <<setw(30)<< p->describe<<"\t\t\t"<<p->line<<endl;
        p = p->next;
    }
    cout<<endl<<endl;
}

void printIdentLink()
{
    IdentiferNode * p = idenHead;
    p = p->next;
    cout<<"************************************Identifiers Table******************************"<<endl<<endl;
    cout<<setw(30)<<"Lexeme"<<setw(10)<<"\tToken"<<"\t\t"<<"Address"<<"  \t\t"<<"Line"<<endl;
    while(p!=NULL)
    {
        cout<<setw(30)<<p->content << "\t\t" <<setw(10)<<p->describe<<"\t\t"<<p->addr<<"\t\t\t"<<p->line<<endl;
        p = p->next;
    }
    cout<<endl<<endl;
}
int mystrlen(char * word)
{
    if(*word == '\0')
    {
        return 0;
    }
    else
    {
        return 1+mystrlen(word+1);
    }
}

void preProcess(char * word,int line)
{
    const char * include_temp = "include";
    const char * define_temp = "define";
    char * p_include,*p_define;
    int flag = 0;
    p_include = strstr(word,include_temp);
    if(p_include!=NULL)
    {
        flag = 1;
        int i;
        for(i=7;;)
        {
            if(*(p_include+i) == ' ' || *(p_include+i) == '\t')
            {
                i++;
            }
            else
            {
                break;
            }
        }
        createNewNode(p_include+i,HEADER_DESC,HEADER,-1,line);
    }
    else
    {
        p_define = strstr(word,define_temp);
        if(p_define!=NULL)
        {
            flag = 1;
            int i;
            for(i=7;;)
            {
                if(*(p_define+i) == ' ' || *(p_define+i) == '\t')
                {
                    i++;
                }
                else
                {
                    break;
                }
            }
            createNewNode(p_define+i,CONSTANT_DESC,MACRO_VAL,-1,line);
        }
    }
    if(flag == 0)
    {
        
    }
}

void close()
{
    //delete idenHead;
    //delete errorHead;
    //delete normalHead;
}

int seekKey(char * word)
{
    for(int i=0; i<keyMap.size(); i++)
    {
        if(strcmp(word,keyMap[i].first) == 0)
        {
            return i+1;
        }
    }
    return IDENTIFER;
}

void scanner()
{
    char filename[30];
    char ch;
    char array[30];
    char * word;
    int i;
    int line = 1;
    
    
    FILE * infile;
    printf("Please Type in the File name:\n");
    scanf("%s",filename);
    infile = fopen(filename,"r");
    while(!infile)
    {
        printf("Fail to open file！\n");
        return;
    }
    ch = fgetc(infile);
BIGLOOP: while(ch!=EOF)
{
    
    i = 0;
    //Identifiers should begin with letter, can end with '$'
    if((ch>='A' && ch<='Z') || (ch>='a' && ch<='z')) {
        array[i++] = ch;
        ch = fgetc(infile);
        
        while((ch>='A' && ch<='Z')||(ch>='a' && ch<='z')||(ch>='0' && ch<='9') || ch == '$') {
            
            //if the second place of a variable is '$'
            if (ch == '$') {
                array[i++] = ch;
                ch = fgetc(infile);
                //if there's more letters or numbers or '$' after '$', which the variable does not end with '$' but contains '$'
                if ((ch>='A' && ch<='Z')||(ch>='a' && ch<='z')||(ch>='0' && ch<='9') || ch == '$' || ch == '@'
                    || ch == '#' || ch == '?' || ch == '_') {
                    while ((ch>='A' && ch<='Z')||(ch>='a' && ch<='z')||(ch>='0' && ch<='9') || ch == '$' || ch == '@'
                           || ch == '#' || ch == '?' || ch == '_') {
                        array[i++] = ch;
                        ch = fgetc(infile);
                    }
                    fseek(infile,-1L,SEEK_CUR);//go back one place
                    word = new char[i+1];
                    memcpy(word,array,i);
                    word[i] = '\0';
                    createNewError(word,IDENTIFIER_ERROR,IDENTIFIER_ERROR_NUM,line);
                    ch = fgetc(infile);
                    goto BIGLOOP;
                }
                //Variable ends with '$', then create a new lexeme
                else {
                    word = new char[i+1];
                    memcpy(word,array,i);
                    word[i] = '\0';
                    int seekTemp = seekKey(word);
                    if(seekTemp!=IDENTIFER) {
                        createNewNode(word,KEY_DESC,seekTemp,-1,line);
                    }
                    else {
                        int addr_tmp = createNewIden(word,IDENTIFER_DESC,seekTemp,-1,line);
                        createNewNode(word,IDENTIFER_DESC,seekTemp,addr_tmp,line);
                    }
                    fseek(infile,-1L,SEEK_CUR);//go back one place
                    ch = fgetc(infile);
                    goto BIGLOOP;
                }
            }
            array[i++] = ch;
            ch = fgetc(infile);
        }
        word = new char[i+1];
        memcpy(word,array,i);
        word[i] = '\0';
        int seekTemp = seekKey(word);
        if (array[i - 1] >='0' && array[i - 1]<='9') {
            createNewError(word,IDENTIFIER_ERROR,IDENTIFIER_ERROR_NUM,line);
            ch = fgetc(infile);
            goto BIGLOOP;
        }
        else if(seekTemp!=IDENTIFER) {
            createNewNode(word,KEY_DESC,seekTemp,-1,line);
        }
        else {
            int addr_tmp = createNewIden(word,IDENTIFER_DESC,seekTemp,-1,line);
            createNewNode(word,IDENTIFER_DESC,seekTemp,addr_tmp,line);
        }
        fseek(infile,-1L,SEEK_CUR);//go back one place
    }
    
    //if a variable starts with '$', the entire variable is
    else if (ch == '$' || ch == '_') {
        array[i++] = ch;
        ch = fgetc(infile);
        if ((ch>='A' && ch<='Z')||(ch>='a' && ch<='z')||(ch>='0' && ch<='9') || ch == '$' || ch == '@'
            || ch == '#' || ch == '?' || ch == '_') {
            while ((ch>='A' && ch<='Z')||(ch>='a' && ch<='z')||(ch>='0' && ch<='9') || ch == '$' || ch == '@'
                   || ch == '#' || ch == '?' || ch == '_') {
                array[i++] = ch;
                ch = fgetc(infile);
            }
            fseek(infile,-1L,SEEK_CUR);//go back one place
            word = new char[i+1];
            memcpy(word,array,i);
            word[i] = '\0';
            createNewError(word,IDENTIFIER_ERROR,IDENTIFIER_ERROR_NUM,line);
            ch = fgetc(infile);
            goto BIGLOOP;
        }
    }
    //Start with number
    else if(ch >='0' && ch<='9')
    {
        int flag = 0;
        int flag2 = 0;
        //deal with int
        while(ch >='0' && ch<='9')
        {
            array[i++] = ch;
            ch = fgetc(infile);
        }
        //deal with float which is the same as real
        if(ch == '.')
        {
            flag2 = 1;
            array[i++] = ch;
            ch = fgetc(infile);
            if(ch>='0' && ch<='9')
            {
                while(ch>='0' && ch<='9')
                {
                    array[i++] = ch;
                    ch = fgetc(infile);
                }
            }
            else
            {
                flag = 1;
            }
        }
        word = new char[i+1];
        memcpy(word,array,i);
        word[i] = '\0';
        if(flag == 1)
        {
            createNewError(word,REAL_ERROR,REAL_ERROR_NUM,line);
        }
        else if(flag == 2)
        {
            createNewError(word,REAL_ERROR,REAL_ERROR_NUM,line);
        }
        else
        {
            if(flag2 == 0)
            {
                createNewNode(word,INT_DESC,INT_VAL,-1,line);
            }
            else
            {
                createNewNode(word,REAL_DESC,REAL_VAL,-1,line);
            }
        }
        fseek(infile,-1L,SEEK_CUR);
    }
    
//    //start with "/"
//    else if(ch == '/')
//    {
//        ch = fgetc(infile);
//        createNewNode("/",OPE_DESC,DIV,-1,line);
//    }
    
    else if(ch == ' ' || ch == '\t' || ch == '\r' || ch == '\n')
    {
        if(ch == '\n')
        {
            line++;
        }
    }
    else
    {
        if(ch == EOF)
        {
            return;
        }
        else if(ch == '-')
        {
            array[i++] = ch;
            ch = fgetc(infile);
            createNewNode("-",OPE_DESC,SUB,-1,line);
            fseek(infile,-1L,SEEK_CUR);
        }
        else if (ch == '/') {
            ch = fgetc(infile);
            createNewNode("/", OPE_DESC, DIV, -1, line);
            fseek(infile, -1L, SEEK_CUR);
        }
        else if(ch == '+')
        {
            ch = fgetc(infile);
            createNewNode("+",OPE_DESC,ADD,-1,line);
            fseek(infile,-1L,SEEK_CUR);
        }
        
        else if(ch == '*')
        {
            ch = fgetc(infile);
            createNewNode("*",OPE_DESC,MUL,-1,line);
            fseek(infile,-1L,SEEK_CUR);
        }
        
        else if(ch == '^')
        {
            ch = fgetc(infile);
            if(ch == '=')
            {
                createNewNode("^=",OPE_DESC,COMPLETE_BYTE_XOR,-1,line);
            }
        }
        //deal with %%
        else if(ch == '%')
        {
            ch = fgetc(infile);
            if(ch == '%')
            {
                createNewNode("%%",CLE_OPE_DESC,PERCENTPERCENT,-1,line);
            }
        }
        
        //deal with comments
        else if (ch == '!') {
            ch = fgetc(infile);
            while (1) {
                if (ch == '\n') {
                    line++;
                }
                ch = fgetc(infile);
                if (ch == EOF) {
                    createNewError(_NULL, NOTE_ERROR, NOTE_ERROR_NUM, line);
                    return ;
                }
                if (ch == '!') {
                    break;
                }
            }
        }
        //Deal with "<"
        else if(ch == '<')
        {
            ch = fgetc(infile);
            createNewNode("<",OPE_DESC,LES_THAN,-1,line);
            fseek(infile,-1L,SEEK_CUR);
        }
        //Deal with ">"
        else if(ch == '>')
        {
            ch = fgetc(infile);
            createNewNode(">",OPE_DESC,GRT_THAN,-1,line);
            fseek(infile,-1L,SEEK_CUR);
        }
        else if(ch == '=')
        {
            ch = fgetc(infile);
            if(ch == '=')
            {
                createNewNode("==",OPE_DESC,EQUAL,-1,line);
            }
            else if (ch == '>') {
                createNewNode("=>",OPE_DESC,GRT_EQUAL,-1,line);
            }
            else if (ch == '<') {
                createNewNode("=<",OPE_DESC,LES_EQUAL,-1,line);
            }
            else
            {
                createNewNode("=",OPE_DESC,ASG,-1,line);
                fseek(infile,-1L,SEEK_CUR);
            }
        }
        
        else if(ch == '(')
        {
            leftSmall++;
            lineBra[0][leftSmall] = line;
            createNewNode("(",CLE_OPE_DESC,LEFT_BRA,-1,line);
        }
        else if(ch == ')')
        {
            rightSmall++;
            lineBra[1][rightSmall] = line;
            createNewNode(")",CLE_OPE_DESC,RIGHT_BRA,-1,line);
        }
        else if(ch == '[')
        {
            leftMiddle++;
            lineBra[2][leftMiddle] = line;
            createNewNode("[",CLE_OPE_DESC,LEFT_INDEX,-1,line);
        }
        else if(ch == ']')
        {
            rightMiddle++;
            lineBra[3][rightMiddle] = line;
            createNewNode("]",CLE_OPE_DESC,RIGHT_INDEX,-1,line);
        }
        else if(ch == '{')
        {
            leftBig++;
            lineBra[4][leftBig] = line;
            createNewNode("{",CLE_OPE_DESC,L_BOUNDER,-1,line);
        }
        else if(ch == '}')
        {
            rightBig++;
            lineBra[5][rightBig] = line;
            createNewNode("}",CLE_OPE_DESC,R_BOUNDER,-1,line);
        }
        else if(ch == '.')
        {
            createNewNode(".",CLE_OPE_DESC,POINTER,-1,line);
        }
        else if(ch == ',')
        {
            createNewNode(",",CLE_OPE_DESC,COMMA,-1,line);
        }
        else if (ch == ':') {
            createNewNode(":",CLE_OPE_DESC,COLON,-1,line);
        }
        else if(ch == ';')
        {
            createNewNode(";",CLE_OPE_DESC,SEMI,-1,line);
        }
        else
        {
            char temp[2];
            temp[0] = ch;
            temp[1] = '\0';
            createNewError(temp,CHAR_ERROR,CHAR_ERROR_NUM,line);
        }
    }
    ch = fgetc(infile);
}
}
void BraMappingError()
{
    if(leftSmall != rightSmall)
    {
        int i = (leftSmall>rightSmall) ? (leftSmall-rightSmall) : (rightSmall - leftSmall);
        bool  flag = (leftSmall>rightSmall) ? true : false;
        if(flag)
        {
            while(i--)
            {
                createNewError(_NULL,LEFT_BRA_ERROR,LEFT_BRA_ERROR_NUM,lineBra[0][i+1]);
            }
        }
        else
        {
            while(i--)
            {
                createNewError(_NULL,RIGHT_BRA_ERROR,RIGHT_BRA_ERROR_NUM,lineBra[1][i+1]);
            }
        }
    }
    if(leftMiddle != rightMiddle)
    {
        int i = (leftMiddle>rightMiddle) ? (leftMiddle-rightMiddle) : (rightMiddle - leftMiddle);
        bool flag = (leftMiddle>rightMiddle) ? true : false;
        if(flag)
        {
            while(i--)
            {
                createNewError(_NULL,LEFT_INDEX_ERROR,LEFT_INDEX_ERROR_NUM,lineBra[2][i+1]);
            }
        }
        else
        {
            while(i--)
            {
                createNewError(_NULL,RIGHT_INDEX_ERROR,RIGHT_INDEX_ERROR_NUM,lineBra[3][i+1]);
            }
        }
    }
    if(leftBig != rightBig)
    {
        int i = (leftBig>rightBig) ? (leftBig-rightBig) : (rightBig - leftSmall);
        bool flag = (leftBig>rightBig) ? true : false;
        if(flag)
        {
            while(i--)
            {
                createNewError(_NULL,L_BOUNDER_ERROR,L_BOUNDER_ERROR_NUM,lineBra[4][i+1]);
            }
        }
        else
        {
            while(i--)
            {
                createNewError(_NULL,R_BOUNDER_ERROR,R_BOUNDER_ERROR_NUM,lineBra[5][i+1]);
            }
        }
    }
}
