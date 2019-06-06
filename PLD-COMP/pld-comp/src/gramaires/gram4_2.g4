grammar expr;
prog: 'int' 'main' '(' ')' '{' (statement ';')* return_statement '}';

statement
    : declaration 
	| assignment
	;
	
declaration 
    : 'int' VAR
	| 'int' VAR '=' expr
	;

assignment
    : VAR '=' expr
	;

expr
    : INT 
	| VAR
	;

	
return_statement: 'return' INT ';' ;

INT : [0-9]+ ;
VAR : [a-zA-Z0-9]+ ;
WS: [ \n\t\r]+ -> skip;