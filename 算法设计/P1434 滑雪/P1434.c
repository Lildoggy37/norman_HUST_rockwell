#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define MAX(a,b) ((a)>(b)?(a):(b))

int martix[105][105];
int dp[105][105];//dp[i][j]表示从(i,j)出发的最长路径的长度
int dfs(int i,int j,int R,int C)
{
    if(dp[i][j]!=0)
        return dp[i][j];
    dp[i][j]=1;
    //接下来四个方向随机（bu）遍历 递归计算其最长路径并且更新
    if(i-1>=0&&martix[i-1][j]>martix[i][j])//上
        dp[i][j]=MAX(dp[i][j],dfs(i-1,j,R,C)+1);
    if(i+1<R&&martix[i+1][j]>martix[i][j])//下
        dp[i][j]=MAX(dp[i][j],dfs(i+1,j,R,C)+1);
    if(j-1>=0&&martix[i][j-1]>martix[i][j])//左
        dp[i][j]=MAX(dp[i][j],dfs(i,j-1,R,C)+1);
    if(j+1<C&&martix[i][j+1]>martix[i][j])//右
        dp[i][j]=MAX(dp[i][j],dfs(i,j+1,R,C)+1);
    return dp[i][j];
}
int main()
{
    int R,C;
    scanf("%d %d",&R,&C);
    for(int i=0;i<R;i++)
        for(int j=0;j<C;j++)
            scanf("%d",&martix[i][j]);//R行C列的矩阵输入
    memset(dp,0,sizeof(dp));//初始化dp数组
    int ans=0;
    for(int i=0;i<R;i++)
        for(int j=0;j<C;j++)
            ans=MAX(ans,dfs(i,j,R,C));
    printf("%d\n",ans);
    return 0;
}