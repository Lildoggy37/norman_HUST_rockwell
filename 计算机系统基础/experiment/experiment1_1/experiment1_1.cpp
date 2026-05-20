#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
using namespace std;
#define N 5
#define N1 2
#define N2 3
struct student
{
    char  name[8];
    short  age;
    float  score;
    char  remark[200];  // 备注信息
};
student old_s[N], new_s[N];
char message[2000];
int length = 0;
void input(student* s)//所有信息进行输入
{
    for (int i = 0;i < N;i++)
    {
        cout << "请输入第" << i << "个学生的相关信息:" << endl;
        cout << "name:";
        cin >> s[i].name;
        cout << "age:";
        cin >> s[i].age;
        cout << "score:";
        cin >> s[i].score;
        cout << "remark:";
        cin >> s[i].remark;
    }
    return;
}
void output(student* s, int num)//输出num个信息
{
    for (int i = 0;i < num;i++)
    {
        cout << "第" << i << "个学生的相关信息:" << endl;
        cout << "name:" << s[i].name << endl;
        cout << "age:" << s[i].age << endl;
        cout << "score:" << s[i].score << endl;
        cout << "remark:" << s[i].remark << endl;
    }
    return;
}
int pack_student_bytebybyte(student* s, int sno, char* buf)//返回压缩后的字节数
{
    int len = 0;
    for (int i = 0;i < sno;i++)
    {
        //压缩name
        int name_len = strlen(s[i].name) + 1;
        buf[len++] = (unsigned char)name_len;
        for (int j = 0;j < name_len;j++)
            buf[len++] = s[i].name[j];

        //压缩age
        buf[len++] = (char)(s[i].age & 0xFF);//低八位
        buf[len++] = (char)((s[i].age >> 8) & 0xFF);//高八位

        //压缩score
        unsigned char* temp = (unsigned char*)&(s[i].score);//用一个指针指向起始地址
        for (int j = 0;j < (sizeof(float));j++)//float占4字节
            buf[len++] = temp[j];

        //压缩remark
        int remark_len = strlen(s[i].remark) + 1;
        buf[len++] = (unsigned char)remark_len;
        for (int j = 0;j < remark_len;j++)
            buf[len++] = s[i].remark[j];//往里进
    }

    return len;
}
int pack_student_whole(student* s, int sno, char* buf)//返回压缩后的字节数
{
    int len = 0;

    for (int i = 0;i < sno;i++)
    {
        //name
        int name_len = strlen(s[i].name) + 1;
        *(unsigned char*)(buf + len) = (unsigned char)name_len; 
        len += 1;
        strcpy(buf + len, s[i].name);  
        len += name_len;
        //age
        *(short*)(buf + len) = s[i].age;
        len += sizeof(short);
        //score
        *(float*)(buf + len) = s[i].score;
        len += sizeof(float);
        //remark
        int remark_len = strlen(s[i].remark) + 1;
        *(unsigned char*)(buf + len) = (unsigned char)remark_len; 
        len += 1;
        strcpy(buf + len, s[i].remark);  
        len += remark_len;
    }

    return len;
}
int restore_student(char* buf, int len, student* s)//返回解压的人数
{
    int num = 0;
    int current = 0;
    while (current < len)
    {
        int name_len = (unsigned char)buf[current++];
        memcpy(s[num].name, buf + current, name_len);
        current += name_len;

        unsigned char age_low = (unsigned char)buf[current++];
        unsigned char age_high = (unsigned char)buf[current++];
        s[num].age = (short)((age_high << 8) | age_low);

        memcpy(&s[num].score, buf + current, sizeof(float));
        current += sizeof(float);

        int remark_len = (unsigned char)buf[current++];
        memset(s[num].remark, 0, 200);  
        memcpy(s[num].remark, buf + current, remark_len);
        current += remark_len;

        num++;
    }
    return num;
}
void print_message(char* buf, int len)
{
    cout << "_________________________________" << endl;
    cout << "message前40个字节为:" << endl;
    for (int i = 0;i < 40 && i < len;i++)
    {
        printf("%02X ", (unsigned char)buf[i]);
    }
    cout << endl;
}
int main()
{
    input(old_s);

    length += pack_student_bytebybyte(old_s, N1, message);
    length += pack_student_whole(old_s + N1, N2, message + length);

    int newnum = restore_student(message, length, new_s);

    print_message(message, length);
    cout << "_________________________________" << endl;
    cout << "压缩前信息：" << endl;
    output(old_s, N);
    cout << "_________________________________" << endl;
    cout << "压缩后信息：" << endl;
    output(new_s, newnum);
    cout << "_________________________________" << endl;

    cout << "压缩前长度:" << N * (8 + sizeof(short) + sizeof(float) + 200) << endl;
    cout << "压缩后长度:" << length << endl;
    return 0;
}

