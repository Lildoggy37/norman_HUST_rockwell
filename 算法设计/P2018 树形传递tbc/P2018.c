#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXN 1005

// 邻接表存储树
typedef struct Edge 
{
    int to;
    struct Edge* next;
} Edge;

Edge* graph[MAXN];
// 记忆化缓存：dp[u][p] 表示u的父节点是p时，u子树的传递时间
int memo[MAXN][MAXN]; // 因为节点编号最大为1000，父节点最多1000种可能，用二维数组缓存

// 添加边到邻接表
void addEdge(int u, int v) 
{
    Edge* edge = (Edge*)malloc(sizeof(Edge));
    edge->to = v;
    edge->next = graph[u];
    graph[u] = edge;
}

// 比较函数，用于qsort降序排序
int cmp(const void* a, const void* b) 
{
    return *(int*)b - *(int*)a;
}

// 树形DP：计算u的父节点为parent时，u子树的传递时间
int dp(int u, int parent) 
{
    // 检查缓存，避免重复计算
    if (memo[u][parent] != -1) 
    {
        return memo[u][parent];
    }

    int child_times[MAXN], cnt = 0;
    // 遍历u的所有邻接节点，排除父节点（子节点）
    Edge* p = graph[u];
    while (p != NULL) 
    {
        int v = p->to;
        if (v != parent) 
        {
            child_times[cnt++] = dp(v, u);
        }
        p = p->next;
    }

    // 对孩子的传递时间降序排序
    qsort(child_times, cnt, sizeof(int), cmp);

    int max_time = 0;
    for (int i = 0; i < cnt; i++) 
    {
        int current = child_times[i] + (i + 1);
        if (current > max_time) 
        {
            max_time = current;
        }
    }

    // 存入缓存
    memo[u][parent] = max_time;
    return max_time;
}

int main() 
{
    int n;
    scanf("%d", &n);
    if (n == 1) 
    {
        printf("0\n1\n");
        return 0;
    }

    // 初始化邻接表和缓存
    memset(graph, 0, sizeof(graph));
    memset(memo, -1, sizeof(memo));

    // 构建树：输入2~n号节点的父节点
    for (int i = 2; i <= n; i++) 
    {
        int p;
        scanf("%d", &p);
        addEdge(i, p);
        addEdge(p, i); // 无向边
    }

    // 计算每个节点作为起始点的传递时间（父节点为0，表示无父节点）
    int min_time = 0x3f3f3f3f;
    int times[MAXN]; // 存储每个节点的传递时间
    for (int start = 1; start <= n; start++) 
    {
        int t = dp(start, 0);
        times[start] = t;
        if (t < min_time) 
        {
            min_time = t;
        }
    }

    // 收集所有最小时间的节点并排序
    int candidates[MAXN], cnt = 0;
    for (int i = 1; i <= n; i++) 
    {
        if (times[i] == min_time) 
        {
            candidates[cnt++] = i;
        }
    }

    // 输出结果
    printf("%d\n", min_time);
    for (int i = 0; i < cnt; i++) 
    {
        if (i > 0) printf(" ");
        printf("%d", candidates[i]);
    }
    printf("\n");


    for (int i = 1; i <= n; i++) 
    {
        Edge* p = graph[i];
        while (p != NULL) 
        {
            Edge* tmp = p;
            p = p->next;
            free(tmp);
        }
    }

    return 0;
}