char main(){
	char a = 'a';
    char b = 3;
    if(b < a){
        a=9;
        if(a != 0){
            if(a ==0){
                a=100;
            }else{
                a=99;
            }
        }else{
            if(b == 3){
                a=88;
            } else {
                a=77;
            }
        } 
    } else {
            a = 66;
    }
    return a;
}