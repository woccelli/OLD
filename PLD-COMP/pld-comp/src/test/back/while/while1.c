int main(){
	int i = 0;
    int j = 25;
    int l = 0;

    while(i<=15){
        i = i+2;
        int k=20;
        while(i != k){
            j = j -1;
            k = k -1;
            if( k == 10){
                l = l+1;
            }
        }
    }

    while(i>=2){
        i = i-1;
    }
    
    return l;
}