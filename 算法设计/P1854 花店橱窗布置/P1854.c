#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define MAX(a,b) ((a)>(b)?(a):(b))

int main()
{
    int F,V;
    scanf("%d %d",&F,&V);
    int gnarly[F+1][V+1];
    int last_location=0;
    int dp[F+1][V+1];//将前 i 束花放入前 j 个花瓶，且第 i 束花放在第 j 个花瓶时的最大美学值
    int memory[F+1][V+1];//memory[i][j]表示第i朵花在j处时，，i-1朵花所在的花瓶位置
    memset(memory,0,sizeof(memory));
    
    for(int i=1;i<=F;i++)
        for(int j=1;j<=V;j++)
            scanf("%d",&gnarly[i][j]);
    for(int j=1;j<=V;j++)
        dp[1][j]=gnarly[1][j];
    int ans=0x80000000;
    for(int i=2;i<=F;i++)
    {
        for(int j=i;j<=V;j++)
        {
            dp[i][j]=0x80000000;
            for(int k=i-1;k<j;k++)
            {
                if(dp[i][j]<dp[i-1][k]+gnarly[i][j])
                {
                    dp[i][j]=dp[i-1][k]+gnarly[i][j];
                    memory[i][j]=k;
                }
                else
                    dp[i][j]=dp[i][j];
            }
            if(i==F)
            {
                if(ans<=dp[i][j])
                {
                    ans=dp[i][j];
                    last_location=j;//记录最后一朵花的位置
                }
            }
        }
    }
    printf("%d\n",ans);

    int location[F+1];
    location[F]=last_location;
    for(int i=F;i>=1;i--)
    {
        location[i-1]=memory[i][location[i]];
    }
    for(int i=1;i<=F;i++)
        printf("%d ",location[i]);
    return 0;
}