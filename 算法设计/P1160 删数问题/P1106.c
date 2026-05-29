#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
char n[255];
int k;
int main() 
{
    scanf("%s", n);
    int len = strlen(n);
    scanf("%d", &k);
    for(int i=0;i<k;i++)
    {
        int pos=len-1;
        for(int j=0;j<len-1;j++)
        {
            if(n[j]>n[j+1] && n[j]!='-1' && n[j+1]!='-1')
            {
                pos=j;
                break;
            }
        }
        for(int j=pos;j<len-1;j++)
        {
            n[j]=n[j+1];
        }
        len--;
    }
    int s=0;
    while(s<len && n[s]=='0')
    {
        s++;
    }
    if(s==len) printf("0\n");
    else
    {
        for(int i=s;i<len;i++)
        {
            printf("%c",n[i]);
        }
        printf("\n");
    }
    return 0;
}