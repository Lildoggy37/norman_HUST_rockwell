#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
int n; 
char word[100][21];
int MaxLen=0;
int used[100]={0};
int valid(char *prev,char *next,int k)
{
    int len1=strlen(prev);
    int len2=strlen(next);

    if(k<1 || k>=len1 || k>=len2) return 0;
    for(int j=0;j<k;j++)
    {
        if(prev[len1-k+j]!=next[j]) return 0;
    }
    return 1;
}
void dfs(int idx,int len)
{
    if(len>MaxLen) MaxLen=len;

    for(int i=0;i<n;i++)
    {
        
        if(used[i]>=2) continue;

        char *prev=word[idx];
        char *next=word[i];
        int len1=strlen(word[idx]);
        int len2=strlen(word[i]);
        int possible=(len1<len2)? len1-1:len2-2;
        for(int j=1;j<=possible;j++)
        {
            if(!valid(prev,next,j)) continue;

            used[i]++;
            dfs(i,len+len2-j);
            used[i]--;
        }
    }

}
int main()
{
    scanf("%d",&n);
    for(int i=0;i<n;i++)
    {
        scanf("%s",word[i]);
    }
    char start;
    scanf(" %c",&start);
    for(int i=0;i<n;i++)
    {
        if(word[i][0]==start)//可能作为起点的单词
        {
            used[i]++;
            dfs(i,strlen(word[i]));
            used[i]--;
        }
    }
    printf("%d\n",MaxLen);
    return 0;
}