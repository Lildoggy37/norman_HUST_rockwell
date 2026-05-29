#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

#define MAX(a,b) ((a)>(b)?(a):(b))
#define MOD (80112002)
int n,m;
typedef struct Node
{
    int to;
    struct Node* next;
}Node;//邻接表节点
Node *head[5005];//邻接表头节点数组
int indeg[5005];//入度数组
int outdeg[5005];//出度数组
long long dp[5005];//dp数组 dp[i]表示以i为终点的最长路径长度
void add_edge(int u,int v)//添加一条从u到v的有向边 u-->v 表示u被v吃了
{
    Node* newNode=(Node*)malloc(sizeof(Node));
    newNode->to=v;
    newNode->next=head[u];
    head[u]=newNode;
    outdeg[u]++;
    indeg[v]++;
}
void topological_sort()//拓补排序
{
    int queue[5005];//队列
    int front=0,rear=0;//队头队尾指针

    //将所有入度为0的节点入队 其dp初始化为1
    for(int i=1;i<=n;i++)
    {
        if(indeg[i]==0)
        {
            queue[rear++]=i;
            dp[i]=1;
        }
    }
    while(front<rear)
    {
        int u=queue[front++];//出队
        //遍历u的所有邻接节点v
        Node* p=head[u];
        while(p)
        {
            int v=p->to;
            //更新dp[v]
            dp[v] = (dp[v] + dp[u]) % MOD;
            //入度减1
            indeg[v]--;
            //如果入度为0则入队
            if(indeg[v]==0)
            {
                queue[rear++]=v;
            }
            p=p->next;
        }
    }
}
int main()
{
    scanf("%d%d",&n,&m);
    memset(head,NULL,sizeof(head));
    memset(indeg,0,sizeof(indeg));
    memset(outdeg,0,sizeof(outdeg));
    memset(dp,0,sizeof(dp));
    for(int i=0;i<m;i++)
    {
        int u,v;
        scanf("%d%d",&u,&v);
        add_edge(u,v);
    }

    topological_sort();
    long long ans=0;
    for(int i=1;i<=n;i++)
    {
        if(outdeg[i]==0)//终点节点
        {
            ans=(ans+dp[i])%MOD;
        }
    }
    printf("%lld\n",ans);
    return 0;
}
//1--3--2
//2--5--3
//3--4--5
//4--5
//5