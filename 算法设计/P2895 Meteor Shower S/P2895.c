#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define MAXSIZE 302
int M;
int burntime[MAXSIZE][MAXSIZE];//表示每个格子最早交交时间
int dx[4]={1,-1,0,0};
int dy[4]={0,0,1,-1};
typedef struct 
{
    int x;
    int y;
    int t;
}Node;//表示坐标与到达坐标的时间
int main() 
{
    scanf("%d", &M);
    for(int i=0;i<MAXSIZE;i++)
        for(int j=0;j<MAXSIZE;j++)
            burntime[i][j]=1e9;//初始化为无穷大
    for(int i=0;i<M;i++)
    {
        int x,y,t;
        scanf("%d%d%d",&x,&y,&t);
        if(t<burntime[x][y])
            burntime[x][y]=t;
        for(int d=0;d<4;d++)
        {
            int nx=x+dx[d];
            int ny=y+dy[d];
            if(nx>=0 && nx<MAXSIZE && ny>=0 && ny<MAXSIZE)
            {
                if(t<burntime[nx][ny])
                    burntime[nx][ny]=t;
            }
        }
    }

    //BFS
    Node queue[MAXSIZE*MAXSIZE];
    int front=0,rear=0;
    int visited[MAXSIZE][MAXSIZE]={0};

    if(burntime[0][0]>0)
    {
        queue[rear].x=0;
        queue[rear].y=0;
        queue[rear].t=0;
        rear++;
        visited[0][0]=1;
    }

    int ans=-1;
    while(front<rear)
    {
        Node curr=queue[front++];
        int curr_x=curr.x;
        int curr_y=curr.y;
        int curr_t=curr.t;

        if(burntime[curr_x][curr_y]==1e9)
        {
            ans=curr_t;
            break;
        }

        for(int d=0;d<4;d++)
        {
            int nx=curr_x+dx[d];
            int ny=curr_y+dy[d];
            int nt=curr_t+1;
            if(nx>=0 && nx<MAXSIZE && ny>=0 && ny<MAXSIZE && !visited[nx][ny] && nt<burntime[nx][ny])
            {
                queue[rear].x=nx;
                queue[rear].y=ny;
                queue[rear].t=nt;
                rear++;
                visited[nx][ny]=1;
            }
        }
    }
    printf("%d\n",ans);
    return 0;
}