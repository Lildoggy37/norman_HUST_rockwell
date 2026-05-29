#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define MAXSIZE 1005
int dx[4]={1,0,-1,0};
int dy[4]={0,1,0,-1};
int n,m;
char martix[MAXSIZE][MAXSIZE];
int result[MAXSIZE][MAXSIZE];
int visit[MAXSIZE][MAXSIZE];
void bfs(int sx,int sy)
{
    int queue[MAXSIZE*MAXSIZE][2];
    int front=0,rear=0;
    queue[rear][0]=sx;
    queue[rear][1]=sy;
    rear++;
    visit[sx][sy]=1;
    int count=0;
    char curr_char=martix[sx][sy];

    while(front<rear)
    {
        int cx=queue[front][0];
        int cy=queue[front][1];
        front++;
        count++;

        for(int d=0;d<4;d++)
        {
            int nx=cx+dx[d];
            int ny=cy+dy[d];
            if(nx>=0 && nx<n && ny>=0 && ny<n && !visit[nx][ny] && martix[nx][ny]!=curr_char )
            {
                visit[nx][ny]=1;
                queue[rear][0]=nx;
                queue[rear][1]=ny;
                rear++;
            }
        }
    }

    front=0;
    while(front<rear)
    {
        int cx=queue[front][0];
        int cy=queue[front][1];
        front++;
        result[cx][cy]=count;
    }
}
int main() 
{
    scanf("%d%d",&n,&m);
    for(int i=0;i<n;i++)
        scanf("%s",martix[i]);
    
        memset(visit,0,sizeof(visit));
        memset(result,0,sizeof(result));
        for(int i=0;i<n;i++)
        {
            for(int j=0;j<n;j++)
            {
                if(!visit[i][j])
                    dfs(i,j);
            }
        }
    
    for(int i=0;i<m;i++)
    {
        int x,y;
        scanf("%d%d",&x,&y);
        printf("%d\n",result[x-1][y-1]);
    }
    return 0;
}