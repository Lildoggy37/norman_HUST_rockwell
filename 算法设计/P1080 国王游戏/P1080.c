#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
int n;
int king[2];
int hand[1005][3];
int cmp(const void*a,const void*b)//升序
{
    int*pa=(int*)a;
    int*pb=(int*)b;
    if(pa[2]!=pb[2]) return pa[2]-pb[2];
    else return (pa[0]-pb[0]);
}
int mul(int *num,int len,int x)//高精度乘法
{
    int carry=0;
    for(int i=0;i<len;i++)
    {
        int temp=num[i]*x+carry;
        num[i]=temp%10;
        carry=temp/10;
    }
    while(carry>0)
    {
        num[len++]=carry%10;
        carry/=10;
    }
    return len;
}
int divv(int *num,int len,int x,int *res)//高精度除法
{
    int remainder = 0;
    int res_len = 0;
    // 从高位到低位计算
    for (int i = len - 1; i >= 0; i--) 
    {
        remainder = remainder * 10 + num[i];
        res[res_len++] = remainder / x;
        remainder = remainder % x;
    }
    // 去掉前导零
    while (res_len > 0 && res[0] == 0) 
    {
        memmove(res, res + 1, --res_len * sizeof(int));
    }
    // 若结果为0，保留一位0
    if (res_len == 0) 
    {
        res[0] = 0;
        res_len = 1;
    }
    return res_len;
}
int copy(int *dest,int *src,int len)
{
    for(int i=0;i<len;i++)
        dest[i]=src[i];
    return len;
}
int compare(int *a,int len_a,int *b,int len_b)//比较高精度数大小 若a>b 返回1
{
    if(len_a!=len_b)
        return len_a-len_b;
    for(int i=0;i<len_a;i++)
    {
        if(a[i]!=b[i])
            return a[i]-b[i];
    }
    return 0;
}
int main() 
{
    scanf("%d", &n);
    scanf("%d%d", &king[0], &king[1]);//0为左手 1为右手
    for(int i = 0; i < n; i++) 
    {
        scanf("%d%d", &hand[i][0], &hand[i][1]);//0为左手 1为右手 2为乘积
        hand[i][2] = hand[i][0] * hand[i][1];
    }

    qsort(hand, n, sizeof(hand[0]),cmp);

    int product[4000]={0};//记录乘积
    int len_p=0;//记录乘积长度

    int temp=king[0];//计算国王左手入乘积
    while(temp)
    {
        product[len_p++]=temp%10;
        temp/=10;
    }

    int maxreward[4000]={0};//记录最大奖励
    int len_max=1;//记录最大奖励长度

    for(int i=0;i<n;i++)//遍历大臣
    {
        int reward[4000]={0};//记录奖励
        int len_rew=divv(product,len_p,hand[i][1],reward);//计算奖励和长度

        if(compare(reward,len_rew,maxreward,len_max)>0)//比较奖励大小
        {
            len_max=copy(maxreward,reward,len_rew);
        }

        len_p=mul(product,len_p,hand[i][0]);//更新乘积
    }

    for(int i=0;i<len_max;i++)
        printf("%d",maxreward[i]);

    return 0;
}