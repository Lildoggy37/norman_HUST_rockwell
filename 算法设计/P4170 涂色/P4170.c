#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define MIN(a,b) ((a)<(b)?(a):(b))

int main()
{
    int n;
    char color[50];
    scanf("%s", color);
    n = strlen(color);
    int dp[n+1][n+1];// dp[i][j]表示区间[i,j]染色所需的最少操作数
    memset(dp, 0x3f, sizeof(dp));

    for(int i = 0; i < n; i++)
        dp[i][i] = 1;

    for(int len = 2; len <= n; len++) // 枚举区间长度
    {
        for(int i=0;i<=n-len;i++)
        {
            int j=i+len-1;
            if(color[i]==color[j])
                dp[i][j]=MIN(dp[i][j-1], dp[i+1][j]);// 两端颜色相同 可以合并
            else// 两端颜色不同 只能分割
            {
                for(int k=i;k<j;k++)//枚举分割点
                    dp[i][j]=MIN(dp[i][j],dp[i][k]+dp[k+1][j]);
            }
        }
    }
    printf("%d\n", dp[0][n-1]);
    return 0;
}