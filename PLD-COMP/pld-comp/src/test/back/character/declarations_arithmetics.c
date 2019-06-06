char main () {
	int a = 5;
	int b = 2;
	char c = 8;
	char d = 8;
	char e = 'r';
	char f = '2';
	int g = ';';
	int h = 5;
	int i = 2;
	char j = 8;
	char k = 8;
	char l = 'r';
	char m = '2';
	int n = ';';

	d = 1+2;
	b = b + 3;
	c = 2 + c;
	a = b + c;
	d = (a + b) * (c + d);
	g = l;
	l = 3+k;
	a = j;
	j = g;
	n = 3;
	a = 8 + 3 * g + a / 2;
	d = b * l + i;

	return d;
}