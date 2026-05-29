#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
int N,A,B;
int main() 
{
    scanf("%d%d%d",&N,&A,&B);
    int K[N+5];
    for(int i=1;i<=N;i++)
        scanf("%d",&K[i]);

    int visited[200+5]={0};
    int queue[200+5][2];//queue[i][0]存放楼层编号，queue[i][1]存放到达楼层的步数
    int front=0,rear=0;

    visited[A]=1;
    queue[rear][0]=A;
    queue[rear][1]=0;
    rear++;
    int ans=-1;
    while(front<rear)
    {
        int cur_floor=queue[front][0];
        int cur_step=queue[front][1];
        front++;
        if(cur_floor==B)
        {
            ans=cur_step;
            break;
        }

        int up_floor=cur_floor+K[cur_floor];
        int down_floor=cur_floor-K[cur_floor];

        if(up_floor>=1 && up_floor<=N && visited[up_floor]==0)
        {
            visited[up_floor]=1;
            queue[rear][0]=up_floor;
            queue[rear][1]=cur_step+1;
            rear++;
        }

        if(down_floor>=1 && down_floor<=N && visited[down_floor]==0)
        {
            visited[down_floor]=1;
            queue[rear][0]=down_floor;
            queue[rear][1]=cur_step+1;
            rear++;
        }
    }
    printf("%d\n",ans);
    return 0;
}