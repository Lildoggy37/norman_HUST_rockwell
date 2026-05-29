#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

int n;
double x[16],y[16];
double dp[1<<16][16];
double mindist;
double distance(int a,int b)
{
    if(a==-1)
        return sqrt(x[b]*x[b]+y[b]*y[b]);
    else
        return sqrt((x[a]-x[b])*(x[a]-x[b])+(y[a]-y[b])*(y[a]-y[b]));
}
void dfs(int state,int curr,double currdist)
{
    if(currdist>=mindist) return;//剪枝
    if(currdist>dp[state][curr]) return;//剪枝

    dp[state][curr]=currdist;
    
    if(state==(1<<n)-1)//所有点都访问过
    {
       if(currdist<mindist)
            mindist=currdist;
        return;
    }
    for(int i=0;i<n;i++)
    {
        if((state&(1<<i))==0)//点i未访问
        {
            int newstate=state|(1<<i);
            double dist=distance(curr,i);
            dfs(newstate,i,currdist+dist);
        }
    }
}
int main()
{
    scanf("%d",&n);
    for(int i=0;i<n;i++)
        scanf("%lf%lf",&x[i],&y[i]);
    for(int i=0;i<(1<<n);i++)
        for(int j=0;j<n;j++)
            dp[i][j]=1e9;
    mindist=1e9;

    for(int i=0;i<n;i++)
    {
        int state=(1<<i);
        double dist=distance(-1,i);
        dfs(state,i,dist);
    }
    printf("%.2lf\n",mindist);
    return 0;
}