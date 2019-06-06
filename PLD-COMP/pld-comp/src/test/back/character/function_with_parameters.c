char function(int a1, char a2, int a3, char a4, int a5, char a6){
    char x = a1+a2*a3+a4+a5*a6;
	return x;
}

char main(){
    return function(1,2,3,'4',5,'c');
}