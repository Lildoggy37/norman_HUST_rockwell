#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define STUDENTS_NUM  10

typedef struct temp {
	char  sname[8];
	char  sid[11];    //  如U202315123
	short  scores[8]; //  8门课的分数
	short  average;   //  平均分
}student;

void display(student* s, int num);
void initStudents(student* s,int num)
{
	strcpy(s[0].sname, "xuxy");
	strcpy(s[0].sid, "U202315001");
	s[0].scores[0] = 95;
	s[0].scores[1] = 85;
	s[0].scores[2] = 90;
	for (int i=1;i<8;i++)
	   s[0].scores[i] = 80+i;
	s[0].average = 0;

	strcpy(s[1].sname, "wang");
	strcpy(s[1].sid, "U202315002");
	s[1].scores[0] = 100;
}

void computeAverageScore(student* s,int num)
{

}

int main()
{
	student s[STUDENTS_NUM];
	int start, finish, duration;

	printf("%d\n", sizeof(student));
	initStudents(s, STUDENTS_NUM);         // 初始化学生信息
	display(s, STUDENTS_NUM);
	start = GetTickCount();  // 头文件windows.h。得到的是系统运行的时间 精确到毫秒，
	computeAverageScore(s, STUDENTS_NUM);
	finish = GetTickCount();
	duration = finish - start;
	printf("计算平均成绩用时： %d  毫秒\n", duration);

	return 0;
}

