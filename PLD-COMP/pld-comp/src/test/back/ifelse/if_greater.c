int main () {
    int a = 5;
    int b = 5;
    int c = 5;
    int d = 5;
    if (5){
        a=0;
    }
    if (3>2){
        b=0;
    }
    if (1>a){
        c =0;
    }
    if (d>1){
        d=0;
    }
    if (a >= 0){
        a = 1+a;
    }
    if (0 >= b) {
        b = 1+b;
    }
    if ((a + b) >=2){
        c= 1+c;
    }
    if (3>= (a + b + c)) {
        d = 1 +d;
    }
    a=a+b+c+d;
    return a;
}