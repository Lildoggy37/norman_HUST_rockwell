#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
int X,N;
int cmp(const void *a, const void *b) 
{
    int intA = *(int *)a;
    int intB = *(int *)b;
    return intA - intB;
}
int main() 
{
    scanf("%d %d", &X,&N);
    int coins[N+5];
    for(int i = 0; i < N; i++) 
    {
        scanf("%d", &coins[i]);
    }
    qsort(coins,N,sizeof(int),cmp);
    if(coins[0]!=1)
    {
        printf("-1\n");
        return 0;
    }
    int count=0;//携带的硬币数
    int currentmax=0;//当前能支付的最大金额
    int i=0;
    while(currentmax<X)
    {
        int max_coin=0;
        for(int j=0;j<N;j++)
        {
            if(coins[j]<=currentmax+1 && coins[j]>max_coin)
            {
                max_coin=coins[j];
            }
        }
        if(max_coin==0)
        {
            printf("-1\n");
            return 0;
        }
        count++;
        currentmax+=max_coin;
        if(max_coin>currentmax+1 && currentmax<X)
        {
            printf("-1\n");
            return 0;
        }
    }
    printf("%d\n", count);
    return 0;
}