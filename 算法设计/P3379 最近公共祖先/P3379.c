#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAXN 500005
#define LOG_MAX 20  // 2^20 > 5e5，满足数据规模要求

// 邻接表存储多叉树
typedef struct AdjNode 
{
    int to;
    struct AdjNode *next;
} AdjNode;

typedef struct AdjList 
{
    AdjNode *head;
} AdjList;

AdjList tree[MAXN];  // 整棵树的邻接表
int depth[MAXN];     // 每个节点的深度
int fa[MAXN][LOG_MAX];// 倍增数组：fa[u][k] 表示u的2^k级祖先

// 初始化邻接表
void initAdjList(int n) 
{
    for (int i = 1; i <= n; i++) 
    {
        tree[i].head = NULL;
        depth[i] = 0;
        memset(fa[i], 0, sizeof(fa[i]));
    }
}

// 向邻接表中添加边（无向边，建树时转为有向边）
void addEdge(int u, int v) 
{
    AdjNode *newNode = (AdjNode *)malloc(sizeof(AdjNode));
    newNode->to = v;
    newNode->next = tree[u].head;
    tree[u].head = newNode;

    // 无向边，双向添加
    newNode = (AdjNode *)malloc(sizeof(AdjNode));
    newNode->to = u;
    newNode->next = tree[v].head;
    tree[v].head = newNode;
}

// 预处理节点深度和1级祖先（fa[u][0]）
void dfs(int u, int father) 
{
    fa[u][0] = father;  // u的直接祖先（2^0级）
    depth[u] = depth[father] + 1;  // u的深度=父节点深度+1

    // 预处理倍增数组（2^k级祖先）
    for (int k = 1; k < LOG_MAX; k++) 
    {
        fa[u][k] = fa[fa[u][k-1]][k-1];
    }

    // 遍历所有子节点，递归dfs
    AdjNode *p = tree[u].head;
    while (p != NULL) 
    {
        int v = p->to;
        if (v != father) 
        {  // 避免回溯到父节点
            dfs(v, u);
        }
        p = p->next;
    }
}

// LCA
int LCA(int u, int v) {

    if (depth[u] < depth[v]) 
    {
        int temp = u;
        u = v;
        v = temp;
    }


    for (int k = LOG_MAX - 1; k >= 0; k--) 
    {
        if (depth[fa[u][k]] >= depth[v]) 
        {
            u = fa[u][k];
        }
    }


    if (u == v) return u;


    for (int k = LOG_MAX - 1; k >= 0; k--) 
    {
        if (fa[u][k] != fa[v][k]) 
        {
            u = fa[u][k];
            v = fa[v][k];
        }
    }

    return fa[u][0];
}


int main() {
    int N, M, S;
    scanf("%d%d%d", &N, &M, &S);


    initAdjList(N);

    //读取N-1条边，构建树
    for (int i = 0; i < N-1; i++) 
    {
        int x, y;
        scanf("%d%d", &x, &y);
        addEdge(x, y);
    }


    dfs(S, 0);  // 根节点的父节点为0（虚拟节点，深度为0）

    //处理M次询问，输出每次的LCA结果
    for (int i = 0; i < M; i++) 
    {
        int a, b;
        scanf("%d%d", &a, &b);
        printf("%d\n", LCA(a, b));
    }

    return 0;
}