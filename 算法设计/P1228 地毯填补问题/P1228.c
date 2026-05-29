#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>
int k,x,y;
void fill(int sx,int sy,int gx,int gy,int size)//sx,sy区块起始坐标 gx,gy填充物坐标 size区块边长
{
    if(size==1) return;
    //确定填充物位置
    //在这个区块的对应角放地毯
    //carpet分为四块调用
    if(gx<=sx+size/2-1 && gy<=sy+size/2-1)//左上角
    {
        printf("%d %d 1\n",sx+size/2,sy+size/2);
        fill(sx,sy,gx,gy,size>>1);//左上角区块递归
        fill(sx,sy+size/2,sx+size/2-1,sy+size/2,size>>1);//右上角区块递归
        fill(sx+size/2,sy,sx+size/2,sy+size/2-1,size>>1);//左下角区块递归
        fill(sx+size/2,sy+size/2,sx+size/2,sy+size/2,size>>1);//右下角区块递归
    }
    else if(gx<=sx+size/2-1 && gy>=sy+size/2)//右上角
    {
        printf("%d %d 2\n",sx+size/2,sy+size/2-1);
        fill(sx,sy,sx+size/2-1,sy+size/2-1,size>>1);//左上角区块递归
        fill(sx,sy+size/2,gx,gy,size>>1);//右上角区块递归
        fill(sx+size/2,sy,sx+size/2,sy+size/2-1,size>>1);//左下角区块递归
        fill(sx+size/2,sy+size/2,sx+size/2,sy+size/2,size>>1);//右下角区块递归
    }
    else if(gx>=sx+size/2 && gy<=sy+size/2-1)//左下角
    {
        printf("%d %d 3\n",sx+size/2-1,sy+size/2);
       fill(sx,sy,sx+size/2-1,sy+size/2-1,size>>1);//左上角区块递归
        fill(sx,sy+size/2,gx,gy,size>>1);//右上角区块递归
        fill(sx+size/2,sy,gx,gy,size>>1);//左下角区块递归
        fill(sx+size/2,sy+size/2,sx+size/2,sy+size/2,size>>1);//右下角区块递归
    }
    else if(gx>=sx+size/2 && gy>=sy+size/2)//右下角
    {
        printf("%d %d 4\n",sx+size/2-1,sy+size/2-1);
        fill(sx,sy,sx+size/2-1,sy+size/2-1,size>>1);//左上角区块递归
        fill(sx,sy+size/2,sx+size/2-1,sy+size/2,size>>1);//右上角区块递归
        fill(sx+size/2,sy,sx+size/2,sy+size/2-1,size>>1);//左下角区块递归
        fill(sx+size/2,sy+size/2,gx,gy,size>>1);//右下角区块递归
    }
    return;
}
int main()
{
    scanf("%d%d%d",&k,&x,&y);
    int size=1<<(k);
    if(x>size || y>size) return 0;
    fill(1,1,x,y,size);
    return 0;
}