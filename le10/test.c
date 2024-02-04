#include <stdio.h>
#include <time.h>

void Print(int sum)
{
    long long timestamp = time(NULL);
    printf("result = %d, timestamp: %lld\n", sum, timestamp);
}

int AddToVal(int a, int b)
{
    int sum = 0;
    for (int i = a; i <= b; ++i)
    {
        sum += i;
    }
    return sum;
}
int main()
{
    int sum = AddToVal(0, 100);
    printf("helllo a\n");
    printf("helllo b\n");
    printf("helllo c\n");
    printf("helllo d\n");
    printf("helllo e\n");
    printf("helllo f\n");
    Print(sum);
    return 0;
}
