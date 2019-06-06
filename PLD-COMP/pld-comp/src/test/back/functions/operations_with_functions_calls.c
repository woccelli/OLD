int f(){
    int a = 3;
    int b = a+2;
    return b;
}

int main(){
    int a = f() + f() + f();
    return a;
}