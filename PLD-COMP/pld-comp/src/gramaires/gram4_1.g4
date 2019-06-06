grammar expr;
prog: 'int' 'main' '(' ')' '{' return_statement '}';
return_statement: 'return' INT ';' ;
INT : [0-9]+ ;
WS: [ \n\t\r]+ -> skip;