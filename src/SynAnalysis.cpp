//
//  SynAnalysis.cpp
//  CPSC323_HW3
//
//  Created by Liren on 4/21/18.
//  Copyright © 2018 Liren. All rights reserved.
//
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fstream>
#include <vector>
#include <string>
#include <stack>
#include <iomanip>
#include "LexAnalysis.h"
#include "SynAnalysis.h"
using namespace std;

extern NormalNode * normalHead;//First node
extern NormalNode * normalTail;//Last node
extern vector<pair<const char *,int> > keyMap;
extern vector<pair<const char *,int> > operMap;
extern vector<pair<const char *,int> > limitMap;
vector<pair<const char *,int> > specialMap;//special symbol in grammar (-> | EPSILON $)
fstream resultfile;

string SYMBOL_TYPE = "";

stack<int> jumpStack;

struct Instruction_Table {
    int inst_address_;
    string op_;
    string oprnd_;
};

struct Symbol_Table {
    string symbol_;
    string type_;
    int address_;
};

int instr_address = 1;
int symbol_table_num = 1;
int curr_Symbol_addr = 2000;
Instruction_Table* Inst_table = new Instruction_Table[500]();
int Inst_table_size = 0;
Symbol_Table* Sym_table = new Symbol_Table[100]();
int Sym_table_size = 1;

void gen_instr(string op, string oprnd) {
    Inst_table[instr_address].inst_address_ = instr_address;
    Inst_table[instr_address].op_ = op;
    Inst_table[instr_address].oprnd_ = oprnd;
    instr_address++;
}

int get_address(NormalNode node) {
    return Sym_table[node.addr].address_;
}


void back_patch(int instr_address) {
    int addr = 0;
    addr = jumpStack.top();
    jumpStack.pop();
    Inst_table[addr].oprnd_ = to_string(instr_address);
}

bool Rat18S(NormalNode& p) {
    NormalNode iterator = p;
    if (!OptFunctionDefinitions(iterator)) return false;
    p = iterator;
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    string str(iterator.content);
    if (str.compare("%%") != 0) {
        cout << "Syntax Error: " << "Line " << p.line << " , %% not matched." << endl;
        return false;
    }
    
    if (iterator.next == NULL) {
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    if (!OptDeclarationList(iterator)) {
        return false;
    }
    p = iterator;
    if (iterator.next == NULL) {
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    if (!StatementList(iterator)) {
        return false;
    }
    p = iterator;
    cout << "Line " << p.line << " , <Rat18S> matched." << endl;
    return true;
}

bool OptFunctionDefinitions(NormalNode& p) {
    NormalNode iterator = p;
    if (FunctionDefinitions(iterator)) {
        cout << "Line " << p.line << " , <OptFunctionDefinitions> -> <OptFunctionDefinitions> is match." << endl;
        p = iterator;
        return true;
    }
    else {
        cout << "Line " << p.line << " , <OptFunctionDefinitions> -> <Empty>" << endl;
        p = *p.prev;
        return true;
    }
}

bool FunctionDefinitions(NormalNode& p) {
    NormalNode iterator = p;
    if (!Function(iterator)) {
        return false;
    }
    p = iterator;
    if (iterator.next == NULL) {
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    if (!FunctionDefinitionsP(iterator)) {
        return false;
    }
    cout << "Line " << p.line << " , <FunctionDefinitions> -> <Function><FunctionDefinitionsP> is match." << endl;
    p = iterator;
    return true;
}

bool Function(NormalNode& p) {
    NormalNode iterator = p;
    string str(p.content);
    if (str.compare("function") != 0) {
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    if (!Identifier(iterator)) {
        return false;
    }
    p = iterator;
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;

    string str2(p.content);
    if (str2.compare("[") != 0) {
        return false;
    }

    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    if (!OptParameterList(iterator)) {
        return false;
    }
    p = iterator;
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;

    string str3(p.content);
    if (str3.compare("]") != 0) {
        cout << "Syntax Error: " << "Line " << p.line << " , seperator \"]\" not matched." << endl;
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    if (!OptDeclarationList(iterator)) {
        return false;
    }
    p = iterator;
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;

    if (!Body(iterator)) {
        return false;
    }
    cout << "Line " << p.line << " , <Function> -> function <Identifier> [ <OptParameterList> ] <OptDeclarationList> <Body> matched "<< endl;
    p = iterator;
    return true;
}


bool OptParameterList(NormalNode& p) {
    NormalNode iterator = p;
    if (ParameterList(iterator)) {
        cout << "Line " << p.line << " , <OptParameterList> -> <ParameterList> matched." << endl;
        p = iterator;
        return true;
    }
    else {
        cout << "Line " << p.line << " , <OptParameterList> -> <Empty>" << endl;
        p = *p.prev;
        return true;
    }
}


bool ParameterList(NormalNode& p) {
    NormalNode iterator = p;
    if (!Parameter(iterator)) {
        cout << "Syntax Error: " << "Line " << p.line << " , <Parameter> not matched." << endl;
        return false;
    }
    
    p = iterator;
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;

    if (!ParameterListP(iterator)) {
        cout << "Syntax Error: " << "Line " << p.line << " , <ParameterListP> not matched." << endl;
        return false;
    }
    
    cout << "Line " << p.line << " , <ParameterList> -> <Parameter> <ParameterListP> matched." << endl;
    p = iterator;
    return true;
}

bool Parameter(NormalNode& p) {
    NormalNode iterator = p;
    if (!IDs(iterator)) {
        return false;
    }
    p = iterator;
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;

    string str(p.content);
    if (str.compare(":") != 0) {
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    if (!Qualifier(iterator)) {
        return false;
    }
    
    cout << "Line " << p.line << " , <Parameter> -> <IDs> : <Qualifier> matched." << endl;
    p = iterator;
    return true;
}


bool Qualifier(NormalNode& p) {
    string str(p.content);
    if (str.compare("int") == 0) {
        cout << "Line " << p.line << " , <Qualifier> -> int  matched." << endl;
        return true;
    }
    else if (str.compare("boolean") == 0) {
        cout << "Line " << p.line << " , <Qualifier> -> boolean  matched." << endl;
        return true;
    }
    else if (str.compare("real") == 0) {
        cout << "Line " << p.line << " , <Qualifier> -> real  matched." << endl;
        return true;
    }
    else {
        return false;
    }
}


bool Body(NormalNode& p) {
    NormalNode iterator = p;
    string str(p.content);
    if (str.compare("{") != 0) {
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    if (!StatementList(iterator)) {
        return false;
    }
    
    p = iterator;
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;

    
    string str2(p.content);
    if (str2.compare("}") != 0) {
        cout << "Syntax Error: " << "Line " << p.line << " , seperator \"}\" not matched." << endl;
        return false;
    }
    
    cout << "Line " << p.line << " , <Body> -> { <StatementList> } matched." << endl;
    p = iterator;
    return true;
}

bool OptDeclarationList(NormalNode& p) {
    NormalNode iterator = p;
    if (DeclarationList(iterator)) {
        cout << "Line " << p.line << " , <OptDeclarationList> -> <DeclarationList> matched." << endl;
        p = iterator;
        return true;
    }
    else {
        cout << "Line " << p.line << " , <OptDeclarationList> -> <Empty>." << endl;
        p = *p.prev;
        return true;
    }
}

bool DeclarationList(NormalNode& p) {
    NormalNode iterator = p;
    if (!Declaration(iterator)) {
        return false;
    }
    
    p = iterator;
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;

    string str(p.content);
    if (str.compare(";") != 0) {
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    if (!DeclarationListP(iterator)) {
        return false;
    }
    
    cout << "Line " << p.line << " , <DeclarationList> -> <Declaration> ; <DeclarationListP> matched." << endl;
    p = iterator;
    return true;
}


bool Declaration(NormalNode& p) {
    NormalNode iterator = p;
    string str(iterator.content);    
    if (str.compare("int") == 0) {
        SYMBOL_TYPE = "int";
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        
        
        NormalNode declarationStart = p;
        if (!IDs(iterator)) {
            return false;
        }
        else {
            NormalNode declarationEnd = iterator;
            while (declarationStart.next != declarationEnd.next) {
                string declarationStartString(declarationStart.describe);
                if (declarationStartString.compare("Identifier") == 0) {
                    //check duplicate declaration
                    if (Sym_table[declarationStart.addr].address_ != 0) {
                        cout << "Syntax Error: \"" << declarationStart.content << "\" is already defined." << endl;
                        return false;
                    }
                    //create declared variables in symbol table
                    string declarationStartContent(declarationStart.content);
                    Sym_table[declarationStart.addr].type_ = SYMBOL_TYPE;
                    Sym_table[declarationStart.addr].address_ = declarationStart.addr - 1 + 2000;
                    Sym_table[declarationStart.addr].symbol_ = declarationStartContent;
                }
                declarationStart = *declarationStart.next;
            }
            string declarationStartString(declarationStart.describe);
            if (declarationStartString.compare("Identifier") == 0) {
                //check duplicate declaration
                if (Sym_table[declarationStart.addr].address_ != 0) {
                    cout << "Syntax Error: \"" << declarationStart.content << "\" is already defined." << endl;
                    return false;
                }
                string declarationStartContent(declarationStart.content);
                Sym_table[declarationStart.addr].type_ = SYMBOL_TYPE;
                Sym_table[declarationStart.addr].address_ = declarationStart.addr - 1 + 2000;
                Sym_table[declarationStart.addr].symbol_ = declarationStartContent;
            }
            cout << "Line " << p.line << " , <Declaration> -> int <IDs> matched." << endl;
            p = iterator;
            return true;
        }
    }
    else if (str.compare("boolean") == 0) {
        SYMBOL_TYPE = "boolean";
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        
        if (!IDs(iterator)) {
            return false;
        }
        else {
            cout << "Line " << p.line << " , <Declaration> -> boolean <IDs> matched." << endl;
            p = iterator;
            return true;
        }
    }
    else if (str.compare("real") == 0) {
        cout << "No \"real\" type variable is allowed in simplified Rat18S compiler." << endl;
        return false;
        SYMBOL_TYPE = "real";
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        
        if (!IDs(iterator)) {
            return false;
        }
        else {
            cout << "Line " << p.line << " , <Declaration> -> real <IDs> matched." << endl;
            p = iterator;
            return true;
        }
    }
    else {
        //cout << "Syntax Error: " << "Line " << p.line << " , <Declaration> not matched." << endl;
        return false;
    }
    
}

bool IDs(NormalNode& p) {
    NormalNode iterator = p;
    if (!Identifier(iterator)) {
        return false;
    }
    
    p = iterator;
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;

    if (!IDsP(iterator)) {
        return false;
    }
    
    cout << "Line " << p.line << " , <IDs> -> <Identifier> <IDsP> matched." << endl;
    p = iterator;
    return true;
}

bool StatementList(NormalNode& p) {
    NormalNode iterator = p;
    if (!Statement(iterator)) {
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    if (!StatementListP(iterator)) {
        return false;
    }
    
    cout << "Line " << p.line << " , <StatementList> -> <Statement> <StatementListP> matched." << endl;
    p = iterator;
    return true;
}

bool Statement(NormalNode& p) {
    NormalNode iterator = p;
    if (Compound(iterator)) {
        cout << "Line " << p.line << " , <Statement> -> <Compound> matched." << endl;
        p = iterator;
        return true;
    }
    else if (Assign(iterator)) {
        cout << "Line " << p.line << " , <Statement> -> <Assign> matched." << endl;
        p = iterator;
        return true;
    }
    else if (If(iterator)) {
        cout << "Line " << p.line << " , <Statement> -> <If> matched." << endl;
        p = iterator;
        return true;
    }
    else if (Return(iterator)) {
        cout << "Line " << p.line << " , <Statement> -> <Return> matched." << endl;
        p = iterator;
        return true;
    }
    else if (Print(iterator)) {
        cout << "Line " << p.line << " , <Statement> -> <Print> matched." << endl;
        p = iterator;
        return true;
    }
    else if (Scan(iterator)) {
        cout << "Line " << p.line << " , <Statement> -> <Scan> matched." << endl;
        p = iterator;
        return true;
    }
    else if (While(iterator)) {
        cout << "Line " << p.line << " , <Statement> -> <While> matched." << endl;
        p = iterator;
        return true;
    }
    else {
        //cout << "Syntax Error: " << "Line " << p.line << " , <Statement> not matched." << endl;
        return false;
    }
}

bool Compound(NormalNode& p) {
    NormalNode iterator = p;
    string str(p.content);
    if (str.compare("{") != 0) {
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    if (!StatementList(iterator)) {
        return false;
    }
    
    p = iterator;
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;

    
    string str2(p.content);
    if (str2.compare("}") != 0) {
        return false;
    }
    
    cout << "Line " << p.line << " , <Compound> -> { <StatementList> } matched." << endl;
    p = iterator;
    return true;
}


bool Assign(NormalNode& p) {
    NormalNode iterator = p;
    if (!Identifier(iterator)) {
        return false;
    }

    NormalNode save = iterator;
    string pContent(save.content);
    string pDescribe(save.describe);
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    string str(p.content);
    if (str.compare("=") != 0) {
        cout << "Line: " << p.line << "Syntax Error: " << "= expected" << endl;
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    
    
    if (!Expression(iterator)) {
        return false;
    }
    
    string saveContent(save.content);
    Sym_table[save.addr].type_ = SYMBOL_TYPE;
    Sym_table[save.addr].address_ = save.addr - 1 + 2000;
    Sym_table[save.addr].symbol_ = saveContent;
    gen_instr("POPM", to_string(get_address(save)));
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    string str2(p.content);
    if (str2.compare(";") != 0) {
        return false;
    }
    
    cout << "Line " << p.line << " , <Assign> -> <Identifier> = <Expression> ; matched." << endl;
    p = iterator;
    return true;
}


bool If(NormalNode& p) {
    NormalNode iterator = p;
    string str(p.content);
    if (str.compare("if") != 0) {
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    string str2(p.content);
    if (str2.compare("(") != 0) {
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    if (!Condition(iterator)) {
        return false;
    }

    p = iterator;
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;

    
    string str3(p.content);
    if (str3.compare(")") != 0) {
        cout << "Line: " << p.line << " Syntax Error: " << ") expected." << endl;
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    if (!Statement(iterator)) {
        return false;
    }
    
    back_patch(instr_address);

    p = iterator;
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;

    if (!IfP(iterator)) {
        return false;
    }
    
    cout << "Line " << p.line << " , <If> -> if ( <Condition> ) <Statement> <IfP> matched." << endl;
    p = iterator;
    return true;
}

bool Return(NormalNode& p) {
    NormalNode iterator = p;
    string str(p.content);
    if (str.compare("return") != 0) {
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    if (!ReturnP(iterator)) {
        return false;
    }
    
    cout << "Line " << p.line << " , <Return> -> return <ReturnP> matched." << endl;
    p = iterator;
    return true;
}

bool Print(NormalNode& p) {
    NormalNode iterator = p;
    string str(p.content);
    if (str.compare("put") != 0) {
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    string str2(p.content);
    if (str2.compare("(") != 0) {
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
//
//    NormalNode printIDstart = iterator;
    
    if (!Expression(iterator)) {
        return false;
    }
//
//    NormalNode printIDend = iterator;
//    while (printIDstart.next != printIDend.next) {
//        string printIDstartString(printIDstart.describe);
//        if (printIDstartString.compare("Identifier") == 0) {
//            gen_instr("STDOUT", to_string(printIDstart.addr - 1 + 2000));
//        }
//        else if (printIDstartString.compare("int") == 0) {
//            gen_instr("STDOUT", to_string(atoi(printIDstart.content)));
//        }
//        printIDstart = *printIDstart.next;
//    }
//    string printIDstartString(printIDstart.describe);
//    if (printIDstartString.compare("Identifier") == 0) {
//        gen_instr("STDOUT", to_string(printIDstart.addr - 1 + 2000));
//    }
//    else if (printIDstartString.compare("int") == 0) {
//        gen_instr("STDOUT", to_string(atoi(printIDstart.content)));
//    }
//
    gen_instr("STDOUT", " ");
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    string str3(p.content);
    if (str3.compare(")") != 0) {
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    string str4(p.content);
    if (str4.compare(";") != 0) {
        return false;
    }
    cout << "Line " << p.line << " , <Print> -> put ( <Expression> ) ; matched." << endl;
    p = iterator;
    return true;
}

bool Scan(NormalNode& p) {
    NormalNode iterator = p;
    string str(p.content);
    if (str.compare("get") != 0) {
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    string str2(p.content);
    if (str2.compare("(") != 0) {
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    NormalNode scanIDstart = p;
    
    if (!IDs(iterator)) {
        return false;
    }
    
    NormalNode scanIDend = iterator;
    while (scanIDstart.next != scanIDend.next) {
        string scanIDstartString(scanIDstart.describe);
        if (scanIDstartString.compare("Identifier") == 0) {
            gen_instr("STDIN", " ");
            gen_instr("POPM", to_string(get_address(scanIDstart)));
        }
        else if (scanIDstartString.compare("int") == 0) {
            gen_instr("STDIN", " ");
            gen_instr("POPM", to_string(get_address(scanIDstart)));
        }
        scanIDstart = *scanIDstart.next;
    }
    string scanIDstartString(scanIDstart.describe);
    if (scanIDstartString.compare("Identifier") == 0) {
        gen_instr("STDIN", " ");
        gen_instr("POPM", to_string(get_address(scanIDstart)));
    }
    else if (scanIDstartString.compare("int") == 0) {
        gen_instr("STDIN", " ");
        gen_instr("POPM", to_string(get_address(scanIDstart)));
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    string str3(p.content);
    if (str3.compare(")") != 0) {
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    string str4(p.content);
    if (str4.compare(";") != 0) {
        return false;
    }
    cout << "Line " << p.line << " , <Scan> -> get ( <IDs> ) ; matched." << endl;
    p = iterator;
    return true;
}

bool While(NormalNode& p) {
    NormalNode iterator = p;
    string str(p.content);
    if (str.compare("while") != 0) {
        cout << "Line: " << p.line << " Syntax Error: " << "while expected" << endl;
        return false;
    }
    int addr = instr_address;
    gen_instr("LABEL", " ");
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    string str2(p.content);
    if (str2.compare("(") != 0) {
        cout << "Line: " << p.line << " Syntax Error: " << "( expected" << endl;
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    if (!Condition(iterator)) {
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    string str3(p.content);
    if (str3.compare(")") != 0) {
        cout << "Line: " << p.line << " Syntax Error: " << ") expected" << endl;
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    if (!Statement(iterator)) {
        return false;
    }
    
    gen_instr("JUMP", to_string(addr));
    back_patch(instr_address);
    
    cout << "Line " << p.line << " , <While> -> while ( <Condition> ) <Statement> matched." << endl;
    p = iterator;
    return true;
}

bool Condition(NormalNode& p) {
    NormalNode iterator = p;
    if (!Expression(iterator)) {
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    
    if (!Relop(iterator)) {
        return false;
    }
    NormalNode op = iterator;
    string opstring(op.content);
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    if (!Expression(iterator)) {
        return false;
    }
    
    if (opstring.compare("<") == 0) {
        gen_instr("LES", " ");
        jumpStack.push(instr_address);
        gen_instr("JUMPZ", " ");
    }
    else if (opstring.compare(">") == 0) {
        gen_instr("GRT", " ");
        jumpStack.push(instr_address);
        gen_instr("JUMPZ", " ");
    }
    else if (opstring.compare("==") == 0) {
        gen_instr("EQU", " ");
        jumpStack.push(instr_address);
        gen_instr("JMPZ", " ");
    }
    else if (opstring.compare("^=") == 0) {
        gen_instr("NEQ", " ");
        jumpStack.push(instr_address);
        gen_instr("JUMPZ", " ");
    }
    else if (opstring.compare("=>") == 0) {
        gen_instr("GEQ", " ");
        jumpStack.push(instr_address);
        gen_instr("JUMPZ", " ");
    }
    else if (opstring.compare("=<") == 0) {
        gen_instr("LEQ", " ");
        jumpStack.push(instr_address);
        gen_instr("JUMPZ", " ");
    }
    cout << "Line " << p.line << " , <Condition> -> <Expression> <Relop> <Expression> matched." << endl;
    p = iterator;
    return true;
}

bool Relop(NormalNode& p) {
    string str(p.content);
    if (str.compare("==") == 0) {
        cout << "Line " << p.line << " , <Relop> -> == matched." << endl;
        return true;
    }
    else if (str.compare("^=") == 0) {
        cout << "Line " << p.line << " , <Relop> -> ^= matched." << endl;
        return true;
    }
    else if (str.compare("<") == 0) {
        cout << "Line " << p.line << " , <Relop> -> < matched." << endl;
        return true;
    }
    else if (str.compare(">") == 0) {
        cout << "Line " << p.line << " , <Relop> -> > matched." << endl;
        return true;
    }
    else if (str.compare("=>") == 0) {
        cout << "Line " << p.line << " , <Relop> -> => matched." << endl;
        return true;
    }
    else if (str.compare("=<") == 0) {
        cout << "Line " << p.line << " , <Relop> -> =< matched." << endl;
        return true;
    }
    else {
        cout << "Line: " << p.line << " Syntax Error: " << "==, < , >, =>, =< or ^= token is expcted. " << endl;
        return false;
    }
}

bool Expression(NormalNode& p) {
    NormalNode iterator = p;
    if (!Term(iterator)) {
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    if (!ExpressionP(iterator)) {
        return false;
    }
    cout << "Line " << p.line << " , <Expression> -> <Term> <ExpressionP> matched." << endl;
    p = iterator;
    return true;
}

bool Term(NormalNode& p) {
    NormalNode iterator = p;
    if (!Factor(iterator)) {
        return false;
    }
    
    if (iterator.next == NULL) {
        p = iterator;
        cout << "EOF: " << "Line " << p.line << "." << endl;
        return true;
    }
    p = *iterator.next;
    iterator = p;
    
    
    if (!TermP(iterator)) {
        return false;
    }
    cout << "Line " << p.line << " , <Term> -> <Factor> <TermP> matched." << endl;
    p = iterator;
    return true;
}

bool Factor(NormalNode& p) {
    NormalNode iterator = p;
    string str(p.content);
    if (str.compare("-") == 0) {
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        if (!Primary(iterator)) {
            return false;
        }
        else {
            cout << "Line " << p.line << " , <Factor> -> - <Primary> matched." << endl;
            p = iterator;
            return true;
        }
    }
    if (Primary(iterator)) {
        cout << "Line " << p.line << " , <Factor> -> <Primary> matched." << endl;
        p = iterator;
        return true;
    }
    cout << "Line: " << p.line << " Syntax Error: id expected" << endl;
    return false;
}

bool Primary(NormalNode& p) {
    NormalNode iterator = p;
    string str(iterator.content);
    if (Identifier(iterator)) {
        if (Sym_table[iterator.addr].address_ == 0) {
            cout << "Error: " << iterator.content << " is not declared." << endl;
            return false;
        }
        else {
            gen_instr("PUSHM", to_string(get_address(p)));
        }
        
        //gen_instr("PUSHM", to_string(get_address(p)));
        
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        
        if (PrimaryP(iterator)) {
            cout << "Line " << p.line << " , <Primary> -> <Identifier> <PrimaryP> matched." << endl;
            p = iterator;
            return true;
        }
        else {
            return false;
        }
    }
    else if (Integer(iterator)) {
        cout << "Line " << p.line << " , <Primary> -> <Integer> matched." << endl;
        p = iterator;
        return true;
    }
    else if (str.compare("(") == 0) {
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        
        if (!Expression(iterator)) {
            return false;
        }
        
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        
        string str2(iterator.content);
        if (str2.compare(")") != 0) {
            return false;
        }
        cout << "Line " << p.line << " , <Primary> -> ( <Expression> ) matched." << endl;
        p = iterator;
        return true;
    }
    else if (Real(iterator)) {
        cout << "Line " << p.line << " , <Primary> -> <Real> matched." << endl;
        p = iterator;
        return true;
    }
    else if (str.compare("true") == 0) {
        cout << "Line " << p.line << " , <Primary> -> true matched." << endl;
        p = iterator;
        return true;
    }
    else if (str.compare("false") == 0) {
        cout << "Line " << p.line << " , <Primary> -> false matched." << endl;
        p = iterator;
        return true;
    }
    else {
        return false;
    }
}

bool Identifier(NormalNode& p) {
    string str(p.describe);
    if (str.compare("Identifier") == 0 ) {
        cout << "Line " << p.line << ", id = " << p.content << " , <Identifier> -> id matched." << endl;
        return true;
    }
    return false;
}

bool Real(NormalNode& p) {
    string str(p.describe);
    if (str.compare("real") == 0) {
        //cout << "Line " << p.line << " , <Real> -> real matched." << endl;
        cout << "No real type is allowed in Rat18S compiler." << endl;
        return false;
    }
    return false;
}

bool Integer(NormalNode& p) {
    string str(p.describe);
    if (str.compare("int") == 0) {
        string pContent(p.content);
        Sym_table[p.addr].type_ = SYMBOL_TYPE;
        Sym_table[p.addr].address_ = atoi(p.content);
        Sym_table[p.addr].symbol_ = pContent;
        gen_instr("PUSHI", to_string(get_address(p)));
        cout << "Line " << p.line << " , <Integer> -> int matched." << endl;
        return true;
    }
    return false;
}

bool Empty(NormalNode& p) {
    cout << "Line " << p.line << " , <Empty> -> EPSILON matched." << endl;
    return true;
}

bool ExpressionP(NormalNode& p) {
    NormalNode iterator = p;
    string str(p.content);
    if (str.compare("+") == 0) {
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        
        if (!Term(iterator)) {
            return false;
        }
        
        gen_instr("ADD", " ");
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        
        if (!ExpressionP(iterator)) {
            return false;
        }
        cout << "Line " << p.line << " , <ExpressionP> -> + <Term> <ExpressionP> matched." << endl;
        p = iterator;
        return true;
    }
    else if (str.compare("-") == 0) {
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        
        if (!Term(iterator)) {
            return false;
        }
        
        gen_instr("SUB", " ");
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        
        if (!ExpressionP(iterator)) {
            return false;
        }
        cout << "Line " << p.line << " , <ExpressionP> -> - <Term> <ExpressionP> matched." << endl;
        p = iterator;
        return true;
    }
    else {
        cout << "Line " << p.line << " , <ExpressionP> -> EPSILON matched." << endl;
        p = *p.prev;
        return true;
    }
}

bool TermP(NormalNode& p) {
    NormalNode iterator = p;
    string str(p.content);
    if (str.compare("*") == 0) {
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        
        if (!Factor(iterator)) {
            return false;
        }
        
        gen_instr("MUL", " ");
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;

        if (!TermP(iterator)) {
            return false;
        }
        else {
            p = iterator;
            cout << "Line " << p.line << " , <TermP> -> * <Factor> <TermP>  matched." << endl;
            return true;
        }
    }
    else if (str.compare("/") == 0) {
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        
        if (!Factor(iterator)) {
            return false;
        }
        
        gen_instr("DIV", " ");
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        
        
        if (!TermP(iterator)) {
            return false;
        }
        else {
            p = iterator;
            cout << "Line " << p.line << " , <TermP> -> / <Factor> <TermP>  matched." << endl;
            return true;
        }
    }
    else {
        cout << "Line " << p.line << " , <TermP> -> EPSILON matched." << endl;
        p = *p.prev;
        return true;
    }
}


bool FunctionDefinitionsP(NormalNode& p) {
    NormalNode iterator = p;
    if (FunctionDefinitions(iterator)) {
        p = iterator;
        cout << "Line " << p.line << " , <FunctionDefinitionsP> -> <FunctionDefinitions>  matched." << endl;
        return true;
    }
    else {
        p = *p.prev;
        cout << "Line " << p.line << " , <FunctionDefinitionsP> -> EPSILON matched." << endl;
        return true;
    }
}

bool ParameterListP(NormalNode& p) {
    NormalNode iterator = p;
    string str(p.content);
    if (str.compare(",") == 0) {
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        
        if (!ParameterList(iterator)) {
            return false;
        }
        else {
            p = iterator;
            cout << "Line " << p.line << " , <ParameterListP> -> <ParameterList> matched." << endl;
            return true;
        }
    }
    else {
        p = *p.prev;
        cout << "Line " << p.line << " , <ParameterListP> -> EPSILON matched." << endl;
        return true;
    }
}

bool DeclarationListP(NormalNode& p) {
    NormalNode iterator = p;
    if (DeclarationList(iterator)) {
        p = iterator;
        cout << "Line " << p.line << " , <DeclarationListP> -> <DeclarationList> matched." << endl;
        return true;
    }
    else {
        p = *p.prev;
        cout << "Line " << p.line << " , <DeclarationListP> -> EPSILON matched." << endl;
        return true;
    }
}

bool IDsP(NormalNode& p) {
    NormalNode iterator = p;
    string str(p.content);
    if (str.compare(",") == 0) {
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        
        if (!IDs(iterator)) {
            return false;
        }
        else {
            p = iterator;
            cout << "Line " << p.line << " , <IDsP> -> , <IDs> matched." << endl;
            return true;
        }
    }
    else {
        p = *p.prev;
        cout << "Line " << p.line << " , <IDsP> -> EPSILON matched." << endl;
        return true;
    }
}

bool StatementListP(NormalNode& p) {
    NormalNode iterator = p;
    
    if (StatementList(iterator)) {
        p = iterator;
        cout << "Line " << p.line << " , <StatementListP> -> <StatementList> matched." << endl;
        return true;
    }
    else {
        p = *p.prev;
        cout << "Line " << p.line << " , <StatementListP> -> EPSILON matched." << endl;
        return true;
    }
}


bool IfP(NormalNode& p) {
    NormalNode iterator = p;
    string str(p.content);
    if (str.compare("endif") == 0) {
        cout << "Line " << p.line << " , <IfP> -> endif matched." << endl;
        return true;
    }
    else if (str.compare("else") == 0) {
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        
        if (!Statement(iterator)) {
            return false;
        }
        
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        
        string str2(p.content);
        if (str2.compare("endif") != 0) {
            cout << "Syntax Error: " << "Line " << p.line << " , \"endif\" is missing." << endl;
            return false;
        }
        
        p = iterator;
        cout << "Line " << p.line << " , <IfP> -> else <Statement> endif matched." << endl;
        return true;
    }
    else {
        cout << "Syntax Error: " << "Line " << p.line << " , \"endif or else\" is missing." << endl;
        return false;
    }
}

bool ReturnP(NormalNode& p) {
    NormalNode iterator = p;
    string str(p.content);
    if (str.compare(";") == 0) {
        cout << "Line " << p.line << " , <ReturnP> -> ; matched." << endl;
        return true;
    }
    else if (Expression(iterator)) {
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        
        string str2(p.content);
        if (str2.compare(";") != 0) {
            cout << "Syntax Error: " << "Line " << p.line << " , \";\" is missing." << endl;
            return false;
        }
        else {
            cout << "Line " << p.line << " , <ReturnP> -> <Expression> matched." << endl;
            p = iterator;
            return true;
        }
    }
    cout << "Syntax Error: " << "Line " << p.line << " , \";\" is missing." << endl;
    return false;
}

bool PrimaryP(NormalNode& p) {
    NormalNode iterator = p;
    string str(p.content);
    if (str.compare("(") == 0) {
        cout << "Line " << p.line << " , seperator \"(\" matched." << endl;
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        
        if (!IDs(iterator)) {
            return false;
        }
        
        if (iterator.next == NULL) {
            p = iterator;
            cout << "EOF: " << "Line " << p.line << "." << endl;
            return true;
        }
        p = *iterator.next;
        iterator = p;
        
        string str2(p.content);
        if (str2.compare(")") != 0) {
            cout << "Syntax Error: " << "Line " << p.line << " , seperator \")\" not matched." << endl;
            return false;
        }
        else {
            cout << "Line " << p.line << " , <PrimaryP> -> ( <IDs> ) matched." << endl;
            p = iterator;
            return true;
        }
    }
    else {
        cout << "Line " << p.line << " , <PrimaryP> -> EPSILON is matched." << endl;
        p = *p.prev;
        return true;
    }
}


void analysis(NormalNode& head) {
    NormalNode p = head;
    p = *p.next;
    if (Rat18S(p) && p.next == NULL) {
        cout << "success" << endl;
        gen_instr("", "");
        //Symbol_Table* Sym_table = new Symbol_Table[100]();
        Symbol_Table* Sym_table_iterator = Sym_table;
        //Instruction_Table* Inst_table = new Instruction_Table[500]();
        Instruction_Table* Inst_table_iterator = Inst_table;
        cout << endl << endl;
        //Analysis Result output file
        resultfile.open("Symbol_Table.txt",std::ios::out);
        cout << "************************ Symbol Table ****************************" << endl << endl;
        cout << "\t" << "Identifier" << "  \t\t" << "Type" << "\t\t" << "Memory Addr." << endl;
        resultfile << "************************ Symbol Table ****************************" << endl << endl;
        resultfile << "\t" << "Identifier" << "\t\t" << "Type" << "\t\t" << "Memory Addr." << endl;
        for (int i = 1 ; i < 10 ; i++) {
            if (Sym_table_iterator[i].address_ == 0) break;
            cout << setw(15) << Sym_table_iterator[i].symbol_ << setw(20) << Sym_table_iterator[i].type_ << setw(20) << Sym_table_iterator[i].address_ << endl;
            resultfile << setw(15) << Sym_table_iterator[i].symbol_ << setw(20) << Sym_table_iterator[i].type_ << setw(20) << Sym_table_iterator[i].address_ << endl;
        }
        cout << endl;
        resultfile.close();
        
        resultfile.open("Instruction_Table.txt",std::ios::out);
        cout << "************************* Instruction Table ****************************" << endl << endl;
        cout << "\t\t" << "Instruction Addr." << "  \t\t" << "Operation" << "\t\t" << "Operand" << endl;
        resultfile << "******************************* Instruction Table **********************************" << endl << endl;
        resultfile << "\t\t" << "Instruction Addr." << "  \t\t" << "Operation" << "\t\t" << "Operand" << endl;
        for (int i = 1 ; i < instr_address; i++) {
            cout  << setw(18) << Inst_table_iterator[i].inst_address_ << setw(35) << Inst_table_iterator[i].op_ << setw(24) << Inst_table_iterator[i].oprnd_ << endl;
            resultfile << setw(18) << Inst_table_iterator[i].inst_address_ << setw(35) << Inst_table_iterator[i].op_ << setw(24) << Inst_table_iterator[i].oprnd_ << endl;
        }
        cout << endl;
        resultfile.close();
    }
    else cout << "Syntax Error" << endl;
}
