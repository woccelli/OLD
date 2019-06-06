char main () {
	int a = 5;
	int b = 2;
	char c = 8;
	char d = 8;
	char e = 'r';
	char f = '2';
	char g = ';';

	d = 1+2;
	b = b + 3;
	c = 2 + c;
	a = b + c;
	d = (a + b) * (c + d);

	return d;
}