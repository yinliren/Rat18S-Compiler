//
//  LexAnalysis.h
//  CPSC323_HW3
//
//  Created by Liren on 4/20/18.
//  Copyright © 2018 Liren. All rights reserved.
//

#ifndef LexAnalysis_h
#define LexAnalysis_h

//Keywords
#define AUTO 1
#define BREAK 2
#define CASE 3
#define CHAR 4
#define CONST 5
#define CONTINUE 6
#define DEFAULT 7
#define DO 8
#define DOUBLE 9
#define ELSE 10

#define FLOAT 13
#define FOR 14
#define GOTO 15
#define IF 16
#define INT 17
#define LONG 18
#define REGISTER 19
#define RETURN 20
#define SHORT 21
#define SIGNED 22
#define SIZEOF 23
#define STATIC 24
#define STRUCT 25
#define SWITCH 26
#define TYPEDEF 27
#define UNION 28
#define UNSIGNED 29
#define VOID 30
#define VOLATILE 31
#define WHILE 32
#define REAL 33

#define PUT 34
#define GET 35
#define FUNCTION 36
#define BOOLEAN 37
#define ENDIF 38
#define TRUE_VALUE 11
#define FALSE_VALUE 12
#define KEY_DESC "Keyword"

//Identifiers
#define IDENTIFER 40
#define IDENTIFER_DESC "Identifier"

//Constant
#define INT_VAL 51 //int constant
#define CHAR_VAL 52 //char constant
#define FLOAT_VAL 53 //float constant
#define STRING_VAL 54 //string constant
#define MACRO_VAL 55 //macro constant
#define REAL_VAL 56 // Real Numbers
#define CONSTANT_DESC "constant"
#define REAL_DESC "real"
#define INT_DESC "int"

//Operators
#define BYTE_AND 62 //&
#define COMPLEMENT 63 // ~
#define BYTE_XOR  64 // ^
#define MUL 65 // *
#define DIV 66// /

#define ADD 68 // +
#define SUB 69 // -
#define LES_THAN 70 // <
#define GRT_THAN 71 // >
#define ASG 72 // =
#define ARROW 73 // ->
#define SELF_ADD 74 // ++
#define SELF_SUB 75 // --
#define LEFT_MOVE 76 // <<
#define RIGHT_MOVE 77 // >>
#define LES_EQUAL 78 // =>
#define GRT_EQUAL 79 // =<
#define EQUAL 80 // ==

#define AND 82 // &&
#define OR 83 // ||
#define COMPLETE_ADD 84 // +=
#define COMPLETE_SUB 85 // -=
#define COMPLETE_MUL 86 // *=
#define COMPLETE_DIV 87 // /=
#define COMPLETE_BYTE_XOR 88 // ^=
#define COMPLETE_BYTE_AND 89 // &=
#define COMPLETE_COMPLEMENT 90 // ~=
#define PERCENTPERCENT 91 //%%
#define BYTE_OR 92 // |

#define OPE_DESC "Operator"

//Seperators
#define LEFT_BRA 100 // (
#define RIGHT_BRA 101 // )
#define LEFT_INDEX 102 // [
#define RIGHT_INDEX 103 // ]
#define L_BOUNDER 104 //  {
#define R_BOUNDER 105 // }
#define POINTER 106 // .
#define JING 107 // #
#define UNDER_LINE 108 // _
#define COMMA 109 // ,
#define SEMI 110 // ;
#define COLON 81 // :
#define SIN_QUE 111 // '
#define DOU_QUE 112 // "

#define CLE_OPE_DESC "Seperator"

#define NOTE1 120 // "!!"comment
#define NOTE_DESC "comment"


#define HEADER 130 //header
#define HEADER_DESC "header"

//Error Types
#define REAL_ERROR "real type error"
#define REAL_ERROR_NUM 1
#define DOUBLE_ERROR "double type error"
#define DOUBLE_ERROR_NUM 2
#define NOTE_ERROR "comment format error"
#define NOTE_ERROR_NUM 3
#define STRING_ERROR "String constant error"
#define STRING_ERROR_NUM 4
#define CHARCONST_ERROR "Char constant error"
#define CHARCONST_ERROR_NUM 5
#define CHAR_ERROR "Invalid Char"
#define CHAR_ERROR_NUM 6
#define LEFT_BRA_ERROR "'('not matching"
#define LEFT_BRA_ERROR_NUM 7
#define RIGHT_BRA_ERROR "')'not matching"
#define RIGHT_BRA_ERROR_NUM 8
#define LEFT_INDEX_ERROR "'['not matching"
#define LEFT_INDEX_ERROR_NUM 9
#define RIGHT_INDEX_ERROR "']'not matching"
#define RIGHT_INDEX_ERROR_NUM 10
#define L_BOUNDER_ERROR "'{'not matching"
#define L_BOUNDER_ERROR_NUM 11
#define R_BOUNDER_ERROR "'}'not matching"
#define R_BOUNDER_ERROR_NUM 12
#define IDENTIFIER_ERROR "Identifier Error" //Invalid identifier
#define IDENTIFIER_ERROR_NUM  13

#define _NULL "null"

#define DESCRIBE 4000
#define TYPE 4001
#define STRING 4002
#define DIGIT 4003

struct NormalNode
{
    char content[30];
    char describe[30];
    int type;
    int addr;
    int line;
    NormalNode * next;
    NormalNode * prev;
};

void initKeyMapping();
void initOperMapping();
void initLimitMapping();

void initNode();
void createNewNode(char * content,char *descirbe,int type,int addr,int line);
void createNewError(char * content,char *descirbe,int type,int line);
int createNewIden(char * content,char *descirbe,int type,int addr,int line);
NormalNode* printNodeLink();
void printErrorLink(bool &TokenError);
void printIdentLink();
int mystrlen(char * word);
void preProcess(char * word,int line);
void close();
int seekKey(char * word);
void scanner();
void BraMappingError();

#endif /* LexAnalysis_h */
