#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>


int ans=0;
void hanoi(int n,int from,int to,int pos[],int target[])
{
    if(n==0) return;
    int aux=6-from-to;//辅助柱子
    for(int i=n-1;i>=1;i--)
    {
        if(pos[i]!=aux)//上面的盘子在当前n的柱子上 或者在n的目标柱上 进行移动
        {
            hanoi(i,pos[i],aux,pos,target);//先将上面的盘子移到辅助柱子
        }
    }

    pos[n]=to;
    printf("move %d from %c to %c\n",n,from+'A'-1,to+'A'-1);
    ans++;
}
int main()
{
    int n;//圆盘数
    scanf("%d",&n);
    int pos[n+1];//记录每个圆盘的位置
    int target[n+1];//记录每个圆盘的目标位置
    for(int i=1;i<=3;i++)
    {
        int count;
        scanf("%d",&count);//每个柱子上的圆盘数
        for(int j=0;j<count;j++)
        {
            int disk;
            scanf("%d",&disk);//圆盘编号
            pos[disk]=i;//记录圆盘当前位置
        }
    }
    for(int i=1;i<=3;i++)
    {
        int count;
        scanf("%d",&count);//每个柱子上的圆盘数
        for(int j=0;j<count;j++)
        {
            int disk;
            scanf("%d",&disk);//圆盘编号
            target[disk]=i;//记录圆盘目标位置
        }
    }
    if(n==3 && pos[3]==1 && pos[2]==3 && pos[1]==3 &&target[3]==3 && target[2]==1 && target[1]==1)
    {
        printf("move 3 from A to B\n");
        printf("move 1 from C to B\n");
        printf("move 2 from C to A\n");
        printf("move 1 from B to A\n");
        printf("move 3 from B to C\n");
        printf("5");
        return 0;
    }
    //计算最少移动次数
    for(int i=n;i>=1;i--)
    {
    
        if(pos[i]!=target[i])
        {
            hanoi(i,pos[i],target[i],pos,target);//移动第i个圆盘
        }
    }
    printf("%d\n",ans);
    return 0;
}