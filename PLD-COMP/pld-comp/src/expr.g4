grammar expr;

// prog: type 'main' '(' ')' '{' block return_statement '}';
prog: function_declaration*;

block
	: (statement)*
	;

statement
    : declaration
	| definition
	| assignment
	| expr';'
	| if_stat
	| while_stat
	;
	
definition
    : type VAR '=' expr ';'
	;
declaration
	: type VAR ';'
	;

assignment
    : VAR '=' expr ';'
	;

if_stat
	: 'if' '(' expr ')' '{' block '}' ('else' '{' block '}')?
	;

while_stat
	: 'while' '(' expr ')' '{' block '}'
	;

expr: expr op=('*' | '/') expr #multDiv
	| expr op=('+' | '-') expr #addSub
	| expr op=('>' | '<' | '<=' | '>=' | '==' | '!=') expr #comparison
	| expr op=('||' | '&&') expr #logicalOperators
	| INT #int
	| CHAR #char
	| SPECIAL_CHAR #specialChar
	| VAR #var
	| function_call #call
	| '(' expr ')' #par
	;

function_declaration
	: type VAR '(' parameters ')' '{' block return_statement '}'
	| type VAR '(' ')' '{' block return_statement '}'
	;

parameters
	: parameter (','parameter)*
	;

parameter
	: type VAR
	;

function_call
	: VAR '(' ')'
	| VAR '(' arguments ')'
	;

arguments
	: expr (','expr)*
	;
	
condition_block
	: '(' expr ')' '{' block '}' 
	;

type
	: 'int'
	| 'char'
	;
	
return_statement: 'return' expr ';' ;

SPECIAL_CHAR 
	: '\'\\n\''
	| '\'\\r\''
	| '\'\\t\''
	| '\'\\v\''
	| '\'\\b\''
	| '\'\\f\''
	| '\'\\a\''
	;

INT : [0-9]+ ;
CHAR : '\''.'\'';
VAR : [a-zA-Z0-9]+ ;
WS: [ \n\t\r]+ -> skip;
COM: '/*' .*? '*/' -> skip;
LINE_COM: '//' ~[\r\n]* -> skip;
INCLUDE_SKIP: '#' ~[\r\n]* -> skip;
INCLUDE: '#include <stdio.h>' -> skip;
