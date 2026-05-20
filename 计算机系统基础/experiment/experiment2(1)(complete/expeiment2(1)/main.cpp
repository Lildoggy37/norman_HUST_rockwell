#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define STUDENTS_NUM  10


typedef struct
{
	char  sname[8];
	char  sid[11];    //  如U202315123
	short  scores[8]; //  8门课的分数
	short  average;   //  平均分
}student;

void computeAverageScore(student* s, int num) 
{

}

void display(student* s, int num)//展示学生信息吧
{
	for (int i = 0;i < num;i++)
	{
		printf("第 %d 个学生信息:\n", i + 1);
		printf("name:%s\n", s[i].sname);
		printf("sid:%s\n", s[i].sid);
		printf("八门课的分数:");
		for (int j = 0;j < 8;j++)
		{
			printf("%d ", s[i].scores[j]);
		}
		printf("\n");
		printf("average:%d\n", s[i].average);
	}
}
void initStudents(student* s, int num)
{
	strcpy(s[0].sname, "whq");
	strcpy(s[0].sid, "U202411160");
	s[0].scores[0] = 95;
	s[0].scores[1] = 85;
	s[0].scores[2] = 90;
	for (int i = 3;i < 8;i++)
		s[0].scores[i] = 80 + i;
	s[0].average = 0;

	srand((unsigned int)time(NULL));
	char names[][8] = { "thy", "hwm", "wyq", "gby", "yxh", "lsh", "mzc", "xsb", "ljh" };

	for (int i = 1;i < num;i++)
	{
		// 随机姓名
		strcpy(s[i].sname, names[rand() % 9]);

		// 生成学号 
		sprintf(s[i].sid, "U2024116%03d", i);

		for (int j = 0;j < 8;j++)
		{
			s[i].scores[j] = 60 + rand() % 41;
		}
		s[i].average = 0;
	}
}

void computeAverageScore_origin(student* s, int num)
{
	for (int i = 0; i < num;i++)
	{
		int sum = 0;
		for (int j = 0;j < 8;j++)
		{
			sum += s[i].scores[j];
		}
		s[i].average = sum / 8;
	}
}
int partition(student* s, int low, int high)
{
	int pivot = s[high].average;  // 选择最后一个元素作为基准
	int i = low - 1;

	for (int j = low; j < high; j++)
	{
		// 按平均分降序排列
		if (s[j].average >= pivot)
		{
			i++;
			// 交换学生信息
			student temp = s[i];
			s[i] = s[j];
			s[j] = temp;
		}
	}

	// 将基准元素放到正确位置
	student temp = s[i + 1];
	s[i + 1] = s[high];
	s[high] = temp;

	return i + 1;
}

// 快速排序递归函数
void quickSort(student* s, int low, int high)
{
	if (low < high)
	{
		int pi = partition(s, low, high);
		quickSort(s, low, pi - 1);
		quickSort(s, pi + 1, high);
	}
}

void SortByAverage(student* s, int num)
{
	quickSort(s, 0, num - 1);
}

int main()
{
	student s[STUDENTS_NUM];
	LARGE_INTEGER start_1, finish_1, start_2, finish_2, frequency;
	double duration1, duration2;
	QueryPerformanceFrequency(&frequency);

	initStudents(s, STUDENTS_NUM);         // 初始化学生信息

	QueryPerformanceCounter(&start_1);
	computeAverageScore(s, STUDENTS_NUM);//计算平均分
	QueryPerformanceCounter(&finish_1);
	duration1 = (double)(finish_1.QuadPart - start_1.QuadPart) / frequency.QuadPart;

	display(s, STUDENTS_NUM);//展示学生信息

	printf("计算平均成绩用时： %lf  毫秒\n", duration1 * 1000);

	QueryPerformanceCounter(&start_2);
	SortByAverage(s, STUDENTS_NUM);
	QueryPerformanceCounter(&finish_2);
	duration2 = (double)(finish_2.QuadPart - start_2.QuadPart) / frequency.QuadPart;

	printf("排序用时： %lf  毫秒\n", duration2 * 1000);
	return 0;
}

