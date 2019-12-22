#include<stdio.h>
int main(){
char a = 'c';
char b = 0xb9;
short c;
printf("%x..%x\n", a, b);
c = a | b << 8;
printf("%x..%x...%x\n", a,b,c);
return 1;
}
