#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
#define MAXSHIP 30005

//并查集
int father[MAXSHIP];//亲代/父/母节点
int dist[MAXSHIP];//dist[u]表示u到father[u]的距离
int size[MAXSHIP];//根结点对应队列大小
int find(int x)//
{
    if(father[x]!=x)
    {
        int root=find(father[x]);
        dist[x]+=dist[father[x]];
        return father[x]=root;
    }
    return x;
}
int main()
{
    int T; scanf("%d",&T);
    for(int i=1;i<MAXSHIP;i++)
    {
        father[i]=i;
        dist[i]=0;
        size[i]=1;
    }

    while(T--)
    {
        char op;
        int i,j;
        scanf(" %c %d %d",&op,&i,&j);
        if(op=='M')
        {
            int root_i=find(i);
            int root_j=find(j);

            father[root_i]=root_j;//父节点合并
            dist[root_i]=size[root_j];
            size[root_j]+=size[root_i];
        }
        else if(op=='C')
        {
            int root_i=find(i);
            int root_j=find(j);

            if(root_i!=root_j) printf("-1\n");
            else
            {
                int ans=abs(dist[i]-dist[j])-1;
                printf("%d\n",ans);
            }
        }
    }
    return 0;
}