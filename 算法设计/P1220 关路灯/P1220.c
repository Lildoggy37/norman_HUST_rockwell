#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
int fmin(int a,int b)
{
    return a<b?a:b;
}
int main()
{
    int n,c;
    scanf("%d%d",&n,&c);
    int pos[55],w[55];
    int total=0;
    int sum[55][55] = {0};

    for(int i=1;i<=n;i++)
    {
        scanf("%d%d",&pos[i],&w[i]);
        total+=w[i];//记录总功率
    }

    for(int i=1;i<=n;i++)
    {
        int tempsum=0;
        for(int j=i;j<=n;j++)
        {
            tempsum+=w[j];
            sum[i][j]=tempsum;//预处理区间和
        }
    }
    int dp[55][55][2];
    memset(dp,0x7f,sizeof(dp));
    dp[c][c][0]=0;
    dp[c][c][1]=0;
    for(int len=1;len<=n;len++)//枚举区间长度
    {
        for(int i=1;i+len-1<=n;i++)
        {
            int j=i+len-1;
            if(i>1)//向左扩展
            {
                dp[i-1][j][0]=fmin(dp[i-1][j][0],dp[i][j][0]+(pos[i]-pos[i-1])*(total-sum[i][j]));
                dp[i-1][j][0]=fmin(dp[i-1][j][0],dp[i][j][1]+(pos[j]-pos[i-1])*(total-sum[i][j]));
            }
            if(j<n)//向右扩展
            {
                dp[i][j+1][1]=fmin(dp[i][j+1][1],dp[i][j][1]+(pos[j+1]-pos[j])*(total-sum[i][j]));
                dp[i][j+1][1]=fmin(dp[i][j+1][1],dp[i][j][0]+(pos[j+1]-pos[i])*(total-sum[i][j]));
            }
        }
    }
    int ans = fmin(dp[1][n][0], dp[1][n][1]);
    printf("%d\n", ans);
}