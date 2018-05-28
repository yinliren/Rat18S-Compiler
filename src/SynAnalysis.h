//
//  SynAnalysis.h
//  CPSC323_HW3
//
//  Created by Liren on 4/21/18.
//  Copyright © 2018 Liren. All rights reserved.
//

#ifndef SynAnalysis_h
#define SynAnalysis_h

bool Rat18S(NormalNode&);
bool OptFunctionDefinitions(NormalNode&);
bool FunctionDefinitions(NormalNode&);
bool Function(NormalNode&);
bool OptParameterList(NormalNode&);
bool ParameterList(NormalNode&);
bool Parameter(NormalNode&);
bool Qualifier(NormalNode&);
bool Body(NormalNode&);
bool OptDeclarationList(NormalNode&);
bool DeclarationList(NormalNode&);
bool Declaration(NormalNode&);
bool IDs(NormalNode&);
bool StatementList(NormalNode&);
bool Statement(NormalNode&);
bool Compound(NormalNode&);
bool Assign(NormalNode&);
bool If(NormalNode&);
bool Return(NormalNode&);
bool Print(NormalNode&);
bool Scan(NormalNode&);
bool While(NormalNode&);
bool Condition(NormalNode&);
bool Relop(NormalNode&);
bool Expression(NormalNode&);
bool Term(NormalNode&);
bool Factor(NormalNode&);
bool Primary(NormalNode&);
bool Identifier(NormalNode&);
bool Real(NormalNode&);
bool Integer(NormalNode&);
bool Empty(NormalNode&);
bool ExpressionP(NormalNode&);
bool TermP(NormalNode&);
bool FunctionDefinitionsP(NormalNode&);
bool ParameterListP(NormalNode&);
bool DeclarationListP(NormalNode&);
bool IDsP(NormalNode&);
bool StatementListP(NormalNode&);
bool IfP(NormalNode&);
bool ReturnP(NormalNode&);
bool PrimaryP(NormalNode&);


void analysis(NormalNode&);


#endif /* SynAnalysis_h */
