#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
int n,m,x,y;
//马的移动方式
int dx[8]={2,1,-1,-2,-2,-1,1,2};
int dy[8]={1,2,2,1,-1,-2,-2,-1};

int main() 
{
    scanf("%d%d%d%d",&n,&m,&x,&y);
    int dist[405][405];
    for(int i=0;i<=n;i++)
        for(int j=0;j<=m;j++)
            dist[i][j]=-1;//初始化距离数组

    int queue[405*405][2];
    int front=0,rear=0;

    int sx=x-1,sy=y-1;
    dist[sx][sy]=0;
    queue[rear][0]=sx;
    queue[rear][1]=sy;
    rear++;

    //广度优先搜索
    while(front<rear)
    {
        int ux=queue[front][0];
        int uy=queue[front][1];
        front++;
        for(int i=0;i<8;i++)
        {
            int vx=ux+dx[i];
            int vy=uy+dy[i];
            if(vx>=0&&vx<n&&vy>=0&&vy<m&&dist[vx][vy]==-1)
            {
                dist[vx][vy]=dist[ux][uy]+1;
                queue[rear][0]=vx;
                queue[rear][1]=vy;
                rear++;
            }
        }
    }

    //输出结果
    for(int i=0;i<n;i++)
    {
        for(int j=0;j<m;j++)
        {
            printf("%d",dist[i][j]);
            if(j!=m-1) printf(" ");
        }
        printf("\n");
    }
    return 0;
}