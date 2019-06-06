int fib(int n) {
    int ret;
    if (n == 0) {
        ret = 0;
    }
    if (n == 1) {
        ret = 1;
    }
    if(n > 1){
        ret = fib(n - 1) + fib(n - 2);
    }
    return ret;
}

int main() {
     return fib(13);
}