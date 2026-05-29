#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
int N;
long long a[100005];
typedef struct
{
    long long value;
    int index;
} node;
int cmp(const void *a, const void *b) 
{
    node *nodeA = (node *)a;
    node *nodeB = (node *)b;
    if (nodeA->value < nodeB->value) return -1;
    else if (nodeA->value > nodeB->value) return 1;
    else return 0;
}
int main() 
{
    scanf("%d", &N);
    node *NodeArray = (node *)malloc(N * sizeof(node));
    for(int i = 0; i < N; i++) 
    {
        scanf("%lld", &a[i]);
        NodeArray[i].value = a[i];
        NodeArray[i].index = i;
    }
    qsort(NodeArray,N,sizeof(node),cmp);

    long long minValue = NodeArray[0].value;
    int *value_to_index=(int *)malloc((N+1)*sizeof(int));
    for(int i = 0; i < N; i++) 
    {
        value_to_index[NodeArray[i].index] = i;
    }

    int visited[100005] = {0};
    long long total_cost=0;
    for(int i=0;i<N;i++)
    {
        if(!visited[i] && a[i]!=NodeArray[i].value)
        {
            long long cycle_sum=0;
            long long cycle_min=LLONG_MAX;
            int cycle_size=0;
            int j=i;

            while(!visited[j])
            {
                visited[j]=1;
                long long val=a[j];
                cycle_sum+=val;
                if(val<cycle_min)
                    cycle_min=val;
                    
                cycle_size++;

                j=value_to_index[j];
            }

            long long c1=cycle_sum+(cycle_size-2)*cycle_min;
            long long c2=cycle_sum+cycle_min+(cycle_size+1)*minValue;
            total_cost+=c1<c2?c1:c2;
        }

        
    }
    printf("%lld\n",total_cost);
    return 0;
}