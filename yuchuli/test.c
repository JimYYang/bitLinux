#include <stdio.h>

#define MAX 100
int g_val = 2023;

int Add(int x, int y)
{
    return x + y;
}
#define PRINT(N, FORMAT) printf("the value of "#N" is "FORMAT"\n", N)
// 这是一个编译
struct B{
    char a;
    int b;
    char c;
};
#define OFFSET(type, name) (int)&(((type*)0)->name)

#define AVG(ARRAY, COUNT, TYPE) ({ \
    TYPE sum = 0; \
    for (int i = 0; i < (COUNT); ++i) { \
        sum += (ARRAY)[i]; \
    } \
    (double)sum / (COUNT); \
})

#define fun(arr, n, type)\
[=](){\
	type sum = 0;\
	for (int i = 0; i < n; ++i)\
	{\
		sum += arr[i];\
	}\
	return sum / n;\
}()

int main()
{
    int arr[] = { 2, 4, 6 };
    int c = 3;
    double a = fun(arr, c, int);
    printf("haha%lf\n", a);
    return 0;
}