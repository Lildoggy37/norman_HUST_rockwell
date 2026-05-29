#include<iostream>
#include<algorithm>
#include<cstdio>
using namespace std;

int main()
{
    int L,N,M;//L为路程长度，N为石头数量，M为要移除的石头数量
    cin>>L>>N>>M;
    int D[N+2];
    for(int i=1;i<=N;i++)
        cin>>D[i];
    D[0]=0;
    D[N+1]=L;
    //二分可能的答案
    int left=0,right=L,mid,ans=0;
    while(left<=right)
    {
        mid=(left+right)/2;
        int remove=0,last=0;//remove为移除的石头数量，last为上一个石头的位置
        for(int i=1;i<=N+1;i++)
        {
            if(D[i]-D[last]<mid)//如果当前石头与上一个石头的距离小于mid，则移除当前石头
                remove++;
            else
                last=i;//否则更新上一个石头的位置
        }
        if(remove > M)//如果移除的石头数量大于M，说明mid太大了
            right=mid-1;
        else//  否则mid是一个可行解，尝试增大mid
        {
            ans=mid;
            left=mid+1;
        }
    }
    cout<<ans<<endl;
    return 0;
}