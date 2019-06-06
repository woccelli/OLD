
int func5(int a1, int a2, int a3, int a4, int a5, int a6){
    int z=13;
    return a1+a2+a3+a4+a5+a6-z;
}

int func4(int a){
    int w = 14+124+12;
    return a*3+5+1-123+13-w;
}

int func3(int a){
    int x = 5;
    x = 15*12;
    return x+a+func4(13);

}

int func1(){
	return func3(1)+func5(1,2,3,4,5,6)+func3(2)+func4(12);
}

int func(){
	int c = func1()+func1();
	return c;
}


int main () {
	int a = func();
	return a;
}