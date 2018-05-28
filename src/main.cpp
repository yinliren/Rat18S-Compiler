//
//  main.cpp
//  CPSC323_HW3
//
//  Created by Liren on 4/20/18.
//  Copyright © 2018 Liren. All rights reserved.
//

//main.cpp
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iomanip>
#include "LexAnalysis.h"
#include "SynAnalysis.h"


int main()
{
    bool tokenError = false;
    system("pwd");
    NormalNode* SyntaxPartHead;
    NormalNode* SemanticPartHead;
    //Lexical Analysis Part
    initKeyMapping();
    initOperMapping();
    initLimitMapping();
    initNode();
    scanner();
    BraMappingError();
    SyntaxPartHead = printNodeLink();
    SemanticPartHead = SyntaxPartHead;
//    printErrorLink(tokenError);
//    if (tokenError) {
//        std::cout << "Since Token Error exists, Syntax Analysis will not proceed.\n";
//        return 0;
//    }
    printIdentLink();

    //Syntax Analysis Part
    analysis(*SyntaxPartHead);
}

