                                                        # Rat18S-Compiler
                                               CSUF CPSC323 Spring 2018 Final Project

The programming assignments are based on a language called "Rat18S" which is described as
follows. The Rat18S language is designed to be an easy to understand.  It has a short grammar and  relatively clean semantics.  

1) Lexical Conventions:
The lexical units of a program are identifiers, keywords, integers, reals, operators and other 
separators.  Blanks, tabs and newlines (collectively, "white space") as described below
are ignored except as they serve to separate tokens.
Some white space is required to separate otherwise adjacent identifiers, keywords, reals and integers.
<Identifier> is a sequence of letters or digits, however, the first character must be a letter and last char must be either $ or letter. Upper and lower cases are same.
<Integer>  is an unsigned decimal integer i.e., a sequence of decimal digits.
<Real> is integer followed by “.” and Integer, e.g., 123.00
Some identifiers are reserved for use as keywords, and may not be used otherwise:
       e.g.,  int, if, else, endif,  while, return, get, put   etc.

 Comments are enclosed in    !        !

2) Syntax rules : The following BNF  describes the Rat18S.  <br />

R1. <Rat18S>  ::=   <Opt Function Definitions>   %%  <Opt Declaration List>  <Statement List>  <br />
R2. <Opt Function Definitions> ::= <Function Definitions>     |  <Empty><br />
R3. <Function Definitions>  ::= <Function> | <Function> <Function Definitions>   <br />
R4. <Function> ::= function  <Identifier>  [ <Opt Parameter List> ]  <Opt Declaration List>  <Body><br />
R5. <Opt Parameter List> ::=  <Parameter List>    |     <Empty><br />
R6. <Parameter List>  ::=  <Parameter>    |     <Parameter> , <Parameter List><br />
R7. <Parameter> ::=  <IDs > : <Qualifier> <br />
R8. <Qualifier> ::= int     |    boolean    |  real <br />
R9. <Body>  ::=  {  < Statement List>  }<br />
R10. <Opt Declaration List> ::= <Declaration List>   |    <Empty><br />
R11. <Declaration List>  := <Declaration> ;     |      <Declaration> ; <Declaration List><br />
R12. <Declaration> ::=   <Qualifier > <IDs>     <br />
R13. <IDs> ::=     <Identifier>    | <Identifier>, <IDs><br />
R14. <Statement List> ::=   <Statement>   | <Statement> <Statement List><br />
R15. <Statement> ::=   <Compound>  |  <Assign>  |   <If>  |  <Return>   | <Print>   |   <Scan>   |  <While> <br />
R16. <Compound> ::=   {  <Statement List>  } <br />
R17. <Assign> ::=     <Identifier> = <Expression> ;<br />
R18. <If> ::=     if  ( <Condition>  ) <Statement>   endif    |
                          if  ( <Condition>  ) <Statement>   else  <Statement>  endif   <br />
R19. <Return> ::=  return ; |  return <Expression> ;<br />
R20. <Print> ::=    put ( <Expression>);<br />
R21. <Scan> ::=    get ( <IDs> );<br />
R22. <While> ::=  while ( <Condition>  )  <Statement> <br />
R23. <Condition> ::=     <Expression>  <Relop>   <Expression><br />
R24. <Relop> ::=        ==   |   ^=    |   >     |   <    |   =>    |   =<      <br />
R25. <Expression>  ::=    <Expression> + <Term>    | <Expression>  - <Term>    |    <Term><br />
R26. <Term>    ::=      <Term>  *  <Factor>     |   <Term>  /  <Factor>     |     <Factor><br />
R27. <Factor> ::=      -  <Primary>    |    <Primary><br />
R28. <Primary> ::=     <Identifier>  |  <Integer>  |   <Identifier>  ( <IDs> )   |   ( <Expression> )   |
                                     <Real>  |   true   |  false        <br />
R29. <Empty>   ::= ε<br />


3) Some Semantics

¬	Rat18S is a conventional imperative programming language. A Rat18S program consists of a sequence of functions followed by the "main body" where the program executes. <br />
¬	All variables and functions must be declared before use.<br />
¬	Function arguments are passed by value. <br />
¬	There is an implied expressionless return at the end of all functions; the value returned by  expressionless return statement is undefined. <br />
¬	Arithmetic expressions have their conventional meanings. <br />
¬	Integer division ignores any remainder. <br />
¬	Type casting is not allowed (e.g., assigning an integer to a real variable)<br />
¬	No arithmetic operations are allowed with booleans (e.g., true + false)<br />
¬	Others as we will define during the semester <br />

