grammar exprComplete;
includes: INCLUDE*;
prog: includes 'int' 'main' '(' ')' 
	'{' (statement';')* return_statement '}';

statement: declaration 
	| assignment;
	
declaration : 'int' VAR
	| 'int' assignment;

assignment: VAR '=' expresion;

expresion: expresion '*' expresion
	| expresion '/' expresion
	| expresion '+'expresion
	| expresion '-' expresion
	| INT
	| VAR
	| '(' expresion ')' 
	;

	
return_statement: 'return' INT ';' ;

INT : [0-9]+ ;
VAR : [a-zA-Z0-9]+ ;
FILE : [a-zA-Z/]+ '.' [a-zA-Z] ;
INCLUDE : '#include' [ ]+ '<' FILE '>'
	| '#include' [ ]+ '"' FILE '"';
WS: [ \n\t\r]+ -> skip;