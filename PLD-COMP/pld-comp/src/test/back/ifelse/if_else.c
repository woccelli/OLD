int main () {
    int a =5;
    int b = 5;
    int c = 5;
    if(1<3){
        a=0;
    }
    else {
        a=1;
    }
    if(3<1){
        b=0;
    }
    else {
        b=1;
    }
    if(a==c){
        b=10;
    } 
    else {
        b=11;
    }
    if (a+b == b+c){
        a=2;
    } else {
        a=3;
    }
    a = a + b+ c;
    return a;
}