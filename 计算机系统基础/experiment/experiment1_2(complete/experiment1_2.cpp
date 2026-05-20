#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <climits>
using namespace std;

//1
int absVal(int x)//返回x的绝对值
{
	int sign = x >> 31;//符号位0 or 0xffff ffff                
	return (x ^ sign) - sign;
}
int absVal_standard(int x)
{
	return (x < 0) ? -x : x;
}
//2
int negate1(int x)//返回相反数
{
	return ~x + 1;
}
int negate_standard(int x)
{
	return -x;
}
//3
int bitAnd(int x, int y)//x&y
{
	return ~(~x | ~y);
}
int bitAnd_standard(int x, int y)
{
	return x & y;
}
//4
int bitOr(int x, int y)//x|y
{
	return ~(~x & ~y);
}
int bitOr_standard(int x, int y)
{
	return x | y;
}
//5
int bitXor(int x, int y)//x^y=(x & ~y) | (~x & y)
{
	int a = x & ~y;
	int b = ~x & y;
	return ~(~a & ~b);
}
int bitXor_standard(int x, int y)
{
	return x ^ y;
}
//6
int isTmax(int x)//判断x是否为最大整数0x7fff ffff
{
	int Tmin = x + 1;
	int flag1 = ~(x ^ Tmin);
	return (flag1 == 0) && (Tmin != 0);
}
int isTmax_standard(int x)
{
	return (x == 0x7FFFFFFF);
}
//7
int bitCount(int x)//统计x二进制中1的个数（不超过40次
{
	x = (x & 0x55555555) + ((x >> 1) & 0x55555555);
	x = (x & 0x33333333) + ((x >> 2) & 0x33333333);
	x = (x & 0x0F0F0F0F) + ((x >> 4) & 0x0F0F0F0F);
	x = (x & 0x00FF00FF) + ((x >> 8) & 0x00FF00FF);
	x = (x & 0x0000FFFF) + ((x >> 16) & 0x0000FFFF);//没懂..
	return x;
}
int bitCount_standard(int x)
{
	int count = 0;
	while (x)
	{
		count += x & 1;
		x = x >> 1;
	}
	return count;
}
//8
int bitMask(int highbit, int lowbit)//产生从lowbit 到 highbit 全为1，其他位为0的数
{
	int allone = (1 << (highbit + 1)) - 1;//highbit位全1
	int lowone = (1 << lowbit) - 1;
	return allone & ~lowone;
}
int bitMask_standard(int highbit, int lowbit)
{
	return ((1 << (highbit + 1)) - 1) ^ ((1 << lowbit) - 1);
}
//9
int addOK(int x, int y)//判断x+y是否溢出
{
	int sum = x + y;
	int x_sign = x >> 31;
	int y_sign = y >> 31;
	int sum_sign = sum >> 31;//3个符号位
	int flag1 = !x_sign & !y_sign & sum_sign;//负负得正和正正得负
	int flag2 = x_sign & y_sign & !sum_sign;
	return flag1 | flag2;
}
int addOK_standard(int x, int y)
{
	long long sum = (long long)x + y;
	return sum < INT_MIN || sum > INT_MAX;
}
//10
int byteSwap(int x, int n, int m)//交换x的第n和m字节
{
	int bn = (x >> (n << 3)) & 0xFF;//获得字节 8位二进制数
	int bm = (x >> (m << 3)) & 0xFF;

	x = x & ~(0xFF << (n << 3));//滞空
	x = x & ~(0xFF << (m << 3));

	x = x | (bm << (n << 3));//回填
	x = x | (bn << (m << 3));
	return x;
}
int byteSwap_standard(int x, int n, int m)
{
	unsigned char* p = (unsigned char*)&x;//转化为char
	unsigned char temp = p[n];
	p[n] = p[m];
	p[m] = temp;
	return x;
}
//11
int bang(int x)//实现逻辑非
{
	int sign = (x | (~x + 1)) >> 31;//?
	return ~sign & 1;
}
int bang_standard(int x)
{
	return !x;
}
//12
int bitParity(int x)//判断1的个数是否为奇数
{
	x = x ^ (x >> 16);
	x = x ^ (x >> 8);
	x = x ^ (x >> 4);
	x = x ^ (x >> 2);
	x = x ^ (x >> 1);
	return x & 1;
}
int bitParity_standard(int x)
{
	int count = 0;
	while (x)
	{
		count += x & 1;
		x = x >> 1;
	}
	return count % 2;
}
int random_range(int min, int max)
{
	double a = (double)rand() / RAND_MAX;
	return min + (int)(a * (max - min + 1));
}

int safe_random_int()
{
	int high = rand() % (1 << 16);
	int low = rand() % (1 << 16);
	return (high << 16) | low;
}

int main()
{
	srand(time(NULL));
	int flag = 1;

	//1
	for (int i = 0;i < 10;i++)
	{
		int x = safe_random_int();
		if (absVal(x) != absVal_standard(x))
		{
			printf("[wrong in (1)absVal] x=%d\n", x);
			flag = 0;
			break;
		}
	}
	if (flag) printf("(1) absVal 测试通过\n");

	//2
	flag = 1;
	for (int i = 0; i < 10; i++)
	{
		int x = safe_random_int();
		if (negate1(x) != negate_standard(x))
		{
			printf("[wrong in (2)negate] x=%d\n", x);
			flag = 0;
		}
	}
	if (flag) printf("(2) negate 测试通过\n");

	//3
	flag = 1;
	for (int i = 0; i < 10; i++)
	{
		int x = safe_random_int();
		int y = safe_random_int();
		if (bitAnd(x, y) != bitAnd_standard(x, y))
		{
			printf("[wrong in (3)bitAnd] x=%d, y=%d\n", x, y);
			flag = 0;
		}
	}
	if (flag) printf("(3) bitAnd 测试通过\n");

	//4
	flag = 1;
	for (int i = 0; i < 10; i++)
	{
		int x = safe_random_int();
		int y = safe_random_int();
		if (bitOr(x, y) != bitOr_standard(x, y))
		{
			printf("[wrong in (4)bitOr] x=%d, y=%d\n", x, y);
			flag = 0;
		}
	}
	if (flag) printf("(4) bitOr 测试通过\n");

	//5
	flag = 1;
	for (int i = 0; i < 10; i++)
	{
		int x = safe_random_int();
		int y = safe_random_int();
		if (bitXor(x, y) != bitXor_standard(x, y))
		{
			printf("[wrong in (5)bitXor] x=%d, y=%d\n", x, y);
			flag = 0;
		}
	}
	if (flag) printf("(5) bitXor 测试通过\n");

	//6
	flag = 1;
	int test_values_6[] = { 0, 1, -1, INT_MAX, INT_MIN, 10086, -10086 };
	for (int i = 0; i < 7; i++)
	{
		int x = test_values_6[i];
		if (isTmax(x) != isTmax_standard(x))
		{
			printf("[wrong in (6)isTmax] x=%d\n", x);
			flag = 0;
		}
	}
	if (flag) printf("(6) isTmax 测试通过\n");

	//7
	flag = 1;
	for (int i = 0; i < 10; i++)
	{
		int x = safe_random_int();
		if (bitCount(x) != bitCount_standard(x))
		{
			printf("[wrong in (7)bitCount] x=%d\n", x);
			flag = 0;
		}
	}
	if (flag) printf("(7) bitCount 测试通过\n");

	//8
	flag = 1;
	for (int i = 0; i < 10; i++)
	{
		int lowbit = rand() % 16;  //避免溢出
		int highbit = lowbit + rand() % (16 - lowbit);
		if (bitMask(highbit, lowbit) != bitMask_standard(highbit, lowbit))
		{
			printf("[wrong in (8)bitMask] lowbit=%d, highbit=%d\n", lowbit, highbit);
			flag = 0;
		}
	}
	if (flag) printf("(8) bitMask 测试通过\n");

	//9
	flag = 1;
	int test_values_9[][2] = 
	{
		{1000000000, 1000000000},
		{-1000000000, -1000000000},
		{100, 200},
		{-100, -200},
		{INT_MAX / 2, INT_MAX / 2}, 
		{INT_MIN / 2, INT_MIN / 2}
	};
	for (int i = 0; i < 6; i++)
	{
		int x = test_values_9[i][0];
		int y = test_values_9[i][1];
		if (addOK(x, y) != addOK_standard(x, y))
		{
			printf("[wrong in (9)addOK] x=%d, y=%d\n", x, y);
			flag = 0;
		}
	}
	if (flag) printf("(9) addOK 测试通过\n");

	//10
	flag = 1;
	for (int i = 0; i < 10; i++)
	{
		int x = safe_random_int();
		int n = rand() % 4;  // 0-3字节
		int m = rand() % 4;
		if (byteSwap(x, n, m) != byteSwap_standard(x, n, m))
		{
			printf("[wrong in (10)byteSwap] x=%d, n=%d, m=%d\n", x, n, m);
			flag = 0;
		}
	}
	if (flag) printf("(10) byteSwap 测试通过\n");

	//11
	flag = 1;
	int test_values_11[] = { 0, 1, -1, 11037, -11037, INT_MAX, INT_MIN };
	for (int i = 0; i < 7; i++)
	{
		int x = test_values_11[i];
		if (bang(x) != bang_standard(x))
		{
			printf("[wrong in (11)bang] x=%d\n", x);
			flag = 0;
		}
	}
	if (flag) printf("(11) bang 测试通过\n");

	//12
	flag = 1;
	for (int i = 0; i < 10; i++)
	{
		int x = safe_random_int();
		if (bitParity(x) != bitParity_standard(x))
		{
			printf("[wrong in (12)bitParity] x=%d\n", x);
			flag = 0;
		}
	}
	if (flag) printf("(12) bitParity 测试通过\n");

	return 0;
}