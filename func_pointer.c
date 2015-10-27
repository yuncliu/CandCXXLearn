#include<stdio.h>
typedef int (*FUNC)(int);

int print_int(int a) {
    printf("%d\n", a);
    return 0;
}
int main(int argc, char* argv[]) {
    FUNC x = &print_int;  //here the '&' is optional
    FUNC y = print_int;
    x(1);
    y(10);
    (*x)(100);      //here the '*' is optional
    (*y)(1000);
    int (*abc)(int);
    abc = print_int;
    abc(123);
    return 0;
}
