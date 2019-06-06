int func7(){
    int a=1+2+3+4+5+6+6;
    int b = a+7-1;
    return b-a;
}


int func5(int a1, int a2, int a3, int a4, int a5, int a6){
    int z=13;
    return a1+a2+a3+a4+a5+a6-z;
}

int func4(int a){
    int w = 14+124+12;
    return a*3+5+1-123+13-w+func5(6,5,4,3,2,1);
}

int func3(int a){
    int x = 5;
    x = 15*12;
    return x+a+func4(13);

}

int func1(){
    int z=13-2;
	return func3(1)+func5(1,2,3,4,5,6)+func3(2)+func4(12)-z;
}

int func(){
	int c = func1()+func1();
	return c+12+func3(3);
}


int main () {
	int a = func()+func7();
	return a;
}