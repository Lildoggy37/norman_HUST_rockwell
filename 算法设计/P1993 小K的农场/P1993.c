#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include<math.h>
//判断是否存在负环
#define MAXN 5005
#define MAXM 15005
typedef struct AdjNode
{
    int to;
    int w;
    struct AdjNode *next;
}AdjNode;
typedef struct AdjList
{
    AdjNode *head;
}AdjList;
AdjList graph[MAXN];
int dist[MAXN];
int cnt[MAXN];
bool inqueue[MAXN];
int queue[1000*MAXN];//把这个改大就过了。
int front,rear;

//邻接表初始化
void initGrapg(int n)
{
    for(int i=0;i<=n;i++)
    {
        graph[i].head=NULL;
        dist[i]=0x3f3f3f3f;
        cnt[i]=0;
        inqueue[i]=false;
    }
    front=0; rear=0;
}
//邻接表添加变边
void addEdge(int v,int u,int w)//v->u 权w
{
    AdjNode *temp=(AdjNode*)malloc(sizeof(AdjNode));
    temp->to=u;
    temp->w=w;
    temp->next=graph[v].head;
    graph[v].head=temp;
}

bool bfs(int n)
{
    //入队虚拟源点
    dist[0]=0;
    inqueue[0]=true;
    queue[rear++]=0;
    cnt[0]=1;

    while(front<rear)
    {

        int u=queue[front++];//出队
        inqueue[u]=false;

        //遍历当前结点的边
        AdjNode *p=graph[u].head;
        while(p!=NULL)
        {
            int v=p->to;
            int w=p->w;

            //松弛
            if(dist[v]>dist[u]+w)
            {
                dist[v]=dist[u]+w;
                cnt[v]=cnt[u]+1;

                if(cnt[v]>=n+1) return false;//存在负环

                if(!inqueue[v])//不在队列中，进行入队
                {
                    inqueue[v]=true;
                    queue[rear++]=v;
                }
            }
            p=p->next;
        }

    }
    return true;
}


int main()
{
    int n,m;
    scanf("%d%d",&n,&m);
    initGrapg(n);
    for(int i=0;i<m;i++)
    {
        int op,a,b,c;
        scanf("%d",&op);
        switch(op)
        {
            case 1:
                scanf("%d%d%d",&a,&b,&c);//边 a->b，权值-c
                addEdge(a,b,-c);
                break;
            case 2:
                scanf("%d%d%d",&a,&b,&c);//边 b->a，权值c
                addEdge(b,a,c);
                break;
            case 3:
                scanf("%d%d",&a,&b);//b->a,a->b权都为0;
                addEdge(a,b,0);
                addEdge(b,a,0);
                break;
        }
    }

    for(int i=1;i<=n;i++) addEdge(0,i,0);//添加一个虚拟原点到边的0->i的权0边

    if(bfs(n))  printf("Yes\n");
    else        printf("No\n");

    return 0;
}