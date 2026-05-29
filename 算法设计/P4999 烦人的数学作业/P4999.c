#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define LL unsigned long long
const int MOD=1e9+7;
const LL table[]=
{
    0LL,
    1LL,
    20LL,
    300LL,
    4000LL,
    50000LL,
    600000LL,
    7000000LL,
    80000000LL,
    900000000LL,
    10000000000LL,
    110000000000LL,
    1200000000000LL,
    13000000000000LL,
    140000000000000LL,
    1500000000000000LL,
    16000000000000000LL,
    170000000000000000LL,
    1800000000000000000LL
};
void query(char *s,LL len,LL *ans)
{
    LL i,j,t,k=1,sum=0;
    for(i=len-1;i;i--)
    {
        for(j=1;j<=9;j++)
        {
            ans[j]+=table[len-i]*s[i];
        }
        for(t=1;t<s[i];t++) ans[t]+=k;
        t=s[i];
        ans[t]+=sum+1LL;
        sum+=k*t;
        k*=10LL;
    }
}
int main()
{
    int T;
    scanf("%d", &T);
    while(T--)
    {
        char L[22],R[22];
        LL lenL,lenR,numL[10],numR[10];
        scanf("%s %s", L+1, R+1);
        lenL = strlen(L+1);
        lenR = strlen(R+1);
        for(int i=0;i<10;i++) numL[i]=numR[i]=0;
        for(int i=1;i<=lenL;i++) L[i]-='0', numL[L[i]]++;
        for(int i=1;i<=lenR;i++) R[i]-='0';
        query(L,lenL,numL);
        query(R,lenR,numR);
        LL ans=0;
        for(int i=1;i<=9;i++) ans+=(numR[i]-numL[i])*i%MOD;
        ans=ans%MOD;
        printf("%llu\n", ans);
    }
    return 0;
}