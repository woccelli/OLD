int func1(){
	return 8;
}

int func(){
	int c = func1();
	return c;
}


int main () {
	int a = func();
	return a;
}