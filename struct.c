
typedef struct mystruct mystruct_t;
struct mystruct
{
    int x;
    char s[10];
};

int
main(int argc, char const *argv[])
{
    mystruct_t a;
    mystruct_t *pa = &a;
    
    mystruct_t b;
    
    pa->x = 4;
    b = *pa;
    
    /*printf("b.x = %d\n", b.x);*/

    (void)argc;
    (void)argv;
    return 0;
}
