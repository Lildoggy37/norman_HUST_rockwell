#include <stdio.h>
#include <string.h>
#include <sys/time.h>   // gettimeofday

#define STUDENTS_NUM 10

typedef struct temp {
	char  sname[8];
	char  sid[11];    //  如U202315123
	short  scores[8]; //  8门课的分数
	short  average;   //  平均分
}student;

extern void display(student* s, int num);

void initStudents(student* s, int num)
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

// 获取当前时间（毫秒）
long long current_millis()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return (long long)tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int main()
{
	student s[STUDENTS_NUM];
	long long start, finish, duration;

	printf("%d\n", sizeof(student));
	initStudents(s, STUDENTS_NUM); 
	display(s, STUDENTS_NUM);
	start = current_millis();
	computeAverageScore(s, STUDENTS_NUM);
	finish = current_millis();
	duration = finish - start;
	printf("计算平均成绩用时： %lld  毫秒\n", duration);
    return 0;
}
