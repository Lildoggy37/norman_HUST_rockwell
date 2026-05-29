#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
int n;
long long a[1000005];
int cmp(const void* a, const void* b) 
{
    long long* pa = (long long*)a;
    long long* pb = (long long*)b;
    return (*pa > *pb) ? 1 : ((*pa < *pb) ? -1 : 0);
}
int main() 
{
    scanf("%d", &n);
    long long sum = 0;
    for(int i = 0; i < n; i++) 
    {
        scanf("%lld", &a[i]);
        sum += a[i];
    }
    long long avg=sum/n;

    long long s[1000005];//前缀和
    s[0] = 0;
    for(int i = 0; i < n; i++) 
    {
        s[i] = s[i - 1] + a[i] - avg;
    }
    qsort(s,n,sizeof(long long),cmp);  
    long long mid = s[(n) / 2];//中位数
    long long ans = 0;
    for(int i = 0; i < n; i++) 
    {
        ans += llabs(s[i] - mid);//统计代价
    }
    printf("%lld\n", ans);
    return 0;
}