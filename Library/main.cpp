#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "Windows.h"
#include "graphics.h"
#include <conio.h>

void toptushu(char name[][50], int topnum[]);
void topuser(char name[][50], int topnum[]);

#define MAX_VERTEX 10//最大的顶点个数
#define INT_MAX_ 100000

//结构体
//图书信息结构体
typedef struct Book
{
	char ISBN[20];
	char title[100];
	char author[100];
	int  num;    //图书数量
	int bnum;   //图书被借阅的总次数
	struct Book* next;
}Book;
//用户信息结构体
typedef struct User
{
	char username[50];
	char password[50];
	int isAdmin;  // 0表示普通用户，1表示管理员
	int borrownum = 0;  //用户总借阅量
	struct StackNode* borrowhistory;  //用户借用历史
	struct User* next;
} User;
//借阅历史栈
typedef struct StackNode
{
	User* User;
	Book* Book;
	int days;
	time_t borrowtime;
	struct StackNode* next;
}StackNode;
//借阅请求队列
typedef struct QueueNode
{
	User* user;
	Book* book;
	int days;
	struct QueueNode* next;
}QueueNode;

typedef struct Queue
{
	QueueNode* front;
	QueueNode* rear;
}Queue;

// 定义树节点结构体，代表图书分类
typedef struct TreeNode {
	char category[100];
	struct Book* books;
	struct TreeNode* parent;
	struct TreeNode* children[10];  // 假设最多10个子分类
} TreeNode;


//宏定义
StackNode* allborrowlist = NULL; //总借阅历史

//用户初始化
void UserInit(User*& head)
{
	head = (User*)malloc(sizeof(User));
	head->next = NULL;
}
//图书初始化
void BookInit(Book*& head)
{
	head = (Book*)malloc(sizeof(Book));
	head->next = NULL;
}
 
//删除图书
void deleteBook(Book* head, char isbn[])
{
	int flag = 0;//判断有无查找到；
	if (head == NULL)
	{
		printf("错误未存在图书\n");
		return;
	}
	Book* p;
	Book* pre;
	p = head;
	while (1)
	{
		if (strcmp(p->ISBN, isbn) == 0)
		{
			flag = 1;
			break;
		}
		if (p->next == NULL)
			break;
		pre = p;
		p = p->next;

	}
	if (flag == 0)
	{
		printf("错误 未找到相应图书\n");
		return;
	}
	else
	{
		if (p->next == NULL)
		{
			free(p);
			return;
		}
		else
		{
			pre->next = p->next;
			free(p);
			return;
		}
	}

}
//修改图书信息
void modifyBook(Book* head, char isbn[])
{
	char newauthor[100];
	char newisbn[20];
	char newtitle[100];
	int num;
	int bnum;
	int flag = 0;
	if (head == NULL)
	{
		printf("错误未存在图书\n");
		return;
	}
	Book* p;
	p = head;
	while (1)
	{
		if (strcmp(p->ISBN, isbn) == 0)
		{
			flag = 1;
			break;
		}
		if (p->next == NULL)
			break;
		p = p->next;

	}
	if (flag == 0)
	{
		printf("错误 未找到相应图书\n");
		return;
	}
	else
	{
		printf("----------------------\n");
		printf("1.修改ISBN\n2.修改名称\n3.修改作者\n4.修改数量\n5.修改借阅量\n-1.退出\n");
		printf("----------------------\n");
		int op = 0;
		while (op != -1)
		{
			printf("请输入修改操作码：\n");
			scanf("%d", &op);
			if (op == 1)
			{
				printf("请输入图书的ISBN：\n");
				scanf("%s", newisbn);
				strcpy(p->ISBN, newisbn);
			}
			else if (op == 2)
			{
				printf("请输入图书的名称：\n");
				scanf("%s", newtitle);
				scanf("%s", newtitle);
			}
			else if (op == 3)
			{
				printf("请输入图书的作者：\n");
				scanf("%s", newauthor);
				strcpy(p->author, newauthor);
			}
			else if (op == 4)
			{
				printf("请输入图书的数量：\n");
				scanf("%d", &num);
				p->num = num;
			}
			else if (op == 5)
			{
				printf("请输入图书的借阅量：\n");
				scanf("%d", &bnum);
				p->bnum = bnum;
			}
		}

	}

}
//根据ISBN搜索图书
Book* searchBook(Book* head, char isbn[])
{
	Book* p;
	p = head;
	int flag = 0;
	if (head == NULL)
	{
		printf("错误未存在图书\n");
		return NULL;
	}
	while (1)
	{
		if (strcmp(p->ISBN, isbn) == 0)
		{
			flag = 1;
			break;
		}
		if (p->next == NULL)
			break;
		p = p->next;

	}
	if (flag == 0)
	{
		printf("错误 未找到相应图书\n");
		return NULL;
	}
	else
	{
		printf("该图书的信息为：\n");
		printf("书名：%s\n作者：%s\nISBN：%s\n", p->title, p->author, p->ISBN);
		return p;
	}
}
//打印所有图书信息
void printallBook(Book* head)
{
	Book* p;
	p = head->next;
	while (p != NULL)
	{
		printf("_____________________\n");
		printf("书名：%s\n作者：%s\nISBN：%s\n数量:%d\n借阅量:%d\n", p->title, p->author, p->ISBN, p->num, p->bnum);
		printf("_____________________\n");
		p = p->next;
	}
}
//根据ISBN排序
void sort(Book* head)
{
	Book* p, * tail;
	p = head;
	tail = NULL;
	if (p == NULL || p->next == NULL)
		return;
	while (p != tail)
	{
		while (p->next != tail)
		{

			if (strcmp(p->ISBN, p->next->ISBN) < 0)//按照ISBN
			{
				char tISBN[20], ttitle[100], tauthor[100];
				strcpy(tISBN, p->ISBN);
				strcpy(ttitle, p->title);
				strcpy(tauthor, p->author);
				strcpy(p->ISBN, p->next->ISBN);
				strcpy(p->title, p->next->title);
				strcpy(p->author, p->next->author);
				strcpy(p->next->ISBN, tISBN);
				strcpy(p->next->title, ttitle);
				strcpy(p->next->author, tauthor);

			}
			p = p->next;
		}
		tail = p;
		p = head;
	}
	printf("finish sort\n");
}
//通过文件读入图书信息
void fileread(Book* head, char path[])
{
	FILE* fp = fopen(path, "r");
	if (fp != NULL)
		printf("读取成功\n");
	else
	{
		perror("打开文件错误：");
		return;
	}
	while (!feof(fp))
	{
		char newisbn[20], newtitle[100], newauthor[100];
		int newnum;
		int newbnum;
		Book* newBook = (Book*)malloc(sizeof(Book));
		fscanf(fp, "%s", newisbn);
		fscanf(fp, "%s", newtitle);
		fscanf(fp, "%s", newauthor);
		fscanf(fp, "%d", &newnum);
		fscanf(fp, "%d", &newbnum);
		strcpy(newBook->ISBN, newisbn);
		strcpy(newBook->title, newtitle);
		strcpy(newBook->author, newauthor);
		newBook->num = newnum;
		newBook->bnum = newbnum;
		newBook->next = NULL;
		if (head == NULL)
		{
			head = newBook;
		}
		else
		{
			Book* p = head;
			while (p->next != NULL)
			{
				p = p->next;
			}
			p->next = newBook;
		}

	}


	fclose(fp);
}
//将图书信息导出到文件中
void exportBookfile(Book* head, char path[])
{
	FILE* fp = fopen(path, "w");
	if (fp != NULL)
		printf("导出成功\n");
	else
	{
		perror("导出文件错误：");
		return;
	}
	Book* p = head->next;
	while (p->next != NULL)
	{
		fprintf(fp, "%s\n%s\n%s\n%d\n%d\n", p->ISBN, p->title, p->author, p->num, p->bnum);
		p = p->next;
	}
	fprintf(fp, "%s\n%s\n%s\n%d\n%d", p->ISBN, p->title, p->author, p->num, p->bnum);
	fclose(fp);
	printf("图书信息导出成功\n");
}
//新用户注册
void registerUser(User*& head) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = 18;
	pos.Y = 5;
	SetConsoleCursorPosition(hConsole, pos);
	User* newUser = (User*)malloc(sizeof(User));
	newUser->next = NULL;
	printf("请输入用户名: ");
	scanf("%s", newUser->username);
	pos.X = 18;
	pos.Y = 6;
	SetConsoleCursorPosition(hConsole, pos);
	printf("请输入密码: ");
	scanf("%s", newUser->password);
	pos.X = 10;
	pos.Y = 7;
	SetConsoleCursorPosition(hConsole, pos);
	printf("是否为管理员(0表示普通用户，1表示管理员): ");
	scanf("%d", &newUser->isAdmin);
	newUser->borrowhistory = NULL;
	newUser->borrownum = 0;
	if (head->next == NULL)
	{
		head->next = newUser;
	}
	else
	{
		User* p = head;
		while (p->next != NULL)
		{
			p = p->next;
		}
		p->next = newUser;
	}

}
//用户登录
User* login(User* head) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = 18;
	pos.Y = 5;
	SetConsoleCursorPosition(hConsole, pos);
	char username[50];
	char password[50];
	printf("请输入用户名: ");
	scanf("%s", username);
	pos.X = 18;
	pos.Y = 6;
	SetConsoleCursorPosition(hConsole, pos);
	printf("请输入密码: ");
	scanf("%s", password);
	User* p = head;
	while (p != NULL) {
		if (strcmp(p->username, username) == 0 && strcmp(p->password, password) == 0) {
			return p;
		}
		p = p->next;
	}
	return NULL;
}
//用户信息修改
void modifyUser(User*& user) {
	if (user == NULL) {
		printf("用户不存在，无法修改信息!\n");
		return;
	}
	int choice;
	char newPassword[50];
	int  newisAdmin = 0;
	printf("1. 修改密码\n2. 修改权限\n3. 退出\n");
	while (1)
	{
		scanf("%d", &choice);
		if (choice == 1)
		{
			printf("请输入新密码: ");
			scanf("%s", newPassword);
			strcpy(user->password, newPassword);
			printf("信息修改成功!\n");
		}
		else if (choice == 2)
		{
			printf("请输入新权限(0表示普通用户，1表示管理员): ");
			scanf("%d", &newisAdmin);
			user->isAdmin = newisAdmin;
			printf("信息修改成功!\n");
		}
		else if (choice == 3)
		{
			break;
		}
		else
		{
			printf("无效选择，请重新输入\n");
		}
	}

}
//用户信息导入
void loaduser(User*& head)
{
	FILE* fp = fopen("C://Users//吴品东//Desktop//Library//Userinfomation.txt", "r");
	while (!feof(fp))
	{
		char lusername[50];
		char lpassword[50];
		int lisAdmin = 1;
		int borrownum = 0;
		User* newUser = (User*)malloc(sizeof(User));
		newUser->next = NULL;
		fscanf(fp, "%s", lusername);
		fscanf(fp, "%s", lpassword);
		fscanf(fp, "%d", &lisAdmin);
		fscanf(fp, "%d", &borrownum);
		strcpy(newUser->username, lusername);
		strcpy(newUser->password, lpassword);
		newUser->isAdmin = lisAdmin;
		newUser->borrownum = borrownum;
		//printf("%s %s %d\n", lusername, lpassword, lisAdmin);
		if (head->next == NULL)
		{
			head->next = newUser;
		}
		else
		{
			User* p = head;
			while (p->next != NULL)
			{
				p = p->next;
			}
			p->next = newUser;
		}
	}
	fclose(fp);

}
//用户信息导出
void exportusersfile(User* head)
{
	FILE* fp = fopen("./Userinfomation.txt", "w");
	if (fp != NULL)
		printf("导出成功\n");
	else
	{
		perror("导出文件错误：");
		return;
	}
	User* p = head->next;
	while (p->next != NULL)
	{
		fprintf(fp, "%s\n%s\n%d\n%d\n", p->username, p->password, p->isAdmin, p->borrownum);
		p = p->next;
	}
	fprintf(fp, "%s\n%s\n%d\n%d", p->username, p->password, p->isAdmin, p->borrownum);
	fclose(fp);
	printf("用户信息导出成功\n");
}
//创造借用申请队列
Queue* createQueue()
{
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	if (queue == NULL)
	{
		printf("内存分配失败！\n");
		return NULL;
	}
	queue->front = queue->rear = NULL;
	return queue;
}
//进队操作
void enqueue(Queue* queue, User* user, Book* Book, int day)
{
	QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
	if (newNode == NULL) {
		printf("内存分配失败！\n");
		return;
	}
	newNode->user = user;
	newNode->book = Book;
	newNode->days = day;
	newNode->next = NULL;
	if (queue->rear == NULL) {
		queue->front = queue->rear = newNode;
	}
	else {
		queue->rear->next = newNode;
		queue->rear = newNode;
	}
}
//出队操作
QueueNode* dequeue(Queue* queue)
{
	if (queue->front == NULL)
	{
		return NULL;
	}
	QueueNode* node = queue->front;
	queue->front = queue->front->next;
	if (queue->front == NULL)
	{
		queue->rear = NULL;
	}
	return node;
}
//栈初始化
int stackInit(StackNode* top)
{
	top = (StackNode*)malloc(sizeof(StackNode));
	if (top == NULL)
	{
		printf("申请内存失败\n");
		return -1;
	}
	top->next = NULL;
	return 1;
}
// 向借阅历史栈中压入元素（记录借书情况）
StackNode* pushStack(User* user, Book* Book, int days) {
	StackNode* top = user->borrowhistory;
	StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
	if (newNode == NULL) {
		printf("内存分配失败！\n");
		return top;
	}
	newNode->User = user;
	newNode->Book = Book;
	newNode->days = days;
	newNode->borrowtime = time(NULL);
	newNode->next = top;
	return newNode;
}
//归还时弹栈
StackNode* popStack(StackNode* top)
{
	if (top == NULL)
	{
		return NULL;
	}
	StackNode* temp = top;
	top = top->next;
	free(temp);
	return top;
}
//处理借用（从队列压入栈）
void requestBorrow(User* user, Book* book, int days)
{

	if (book->num > 0)
	{
		book->num--;
		book->bnum++;
		user->borrownum++;
		user->borrowhistory = pushStack(user, book, days);
		/*pushStack(user->borrowhistory, book);*/
		printf("图书《%s》已成功借用\n", book->title);
		return;
	}
	else
	{
		printf("图书《%s》已全部借出，无法借阅\n", book->title);
		return;
	}
}
//还书
int returnbook(StackNode* top, Book* rbook)
{
	char tisbn[20];
	strcpy(tisbn, rbook->ISBN);
	int flag = 0;
	StackNode* p, * ex;
	p = top;
	ex = top;
	StackNode* pre;
	if (p == NULL)
	{
		printf("无借阅图书\n");
		return 0;
	}
	else
	{
		while (1)
		{
			if (strcmp(p->Book->ISBN, tisbn) == 0)
			{
				flag = 1;
				break;
			}
			if (p->next == NULL)
			{
				break;
			}
			pre = p;
			p = p->next;
		}
		if (flag == 0)
		{
			/*printf("错误 未找到相应图书\n");*/
			return 0;
		}
		else
		{
			if (p->next == NULL)
			{
				rbook->num++;
				free(p);
				if (p == ex)
					return 1;
				else
					return 0;
			}
			else
			{
				rbook->num++;
				pre->next = p->next;
				free(p);
				p = NULL;
				return 0;
			}
			printf("已成功归还\n");
		}
	}
}
//打印当前用户借阅历史
void printhistory(User* tuser)
{

	StackNode* p = tuser->borrowhistory;
	if (p == NULL)
	{
		printf("您无借阅历史\n");
	}
	else
	{
		printf("%s的借用历史如下:\n", tuser->username);
		while (p != NULL)
		{
			printf("**********************************\n");
			printf("书名：%s\n作者：%s\nISBN：%s\n申请借阅时长%d天\n", p->Book->title, p->Book->author, p->Book->ISBN, p->days);
			struct tm* borrowTimeInfo = localtime(&p->borrowtime);
			// 格式化输出借阅时间
			printf("借阅时间: %04d-%02d-%02d %02d:%02d:%02d\n", borrowTimeInfo->tm_year + 1900, borrowTimeInfo->tm_mon + 1,
				borrowTimeInfo->tm_mday, borrowTimeInfo->tm_hour, borrowTimeInfo->tm_min, borrowTimeInfo->tm_sec);
			printf("**********************************\n");
			p = p->next;
		}

	}
}
//打印所有用户借阅历史
void printfallhistoury(StackNode* allborrowhistory)
{
	StackNode* p = allborrowhistory;
	if (p == NULL)
		printf("无借阅历史\n");
	else
	{
		printf("总借阅历史：\n");
		while (p != NULL)
		{
			printf("**********************************\n");
			printf("书名：%s\n作者：%s\nISBN：%s\n借阅人：%s\n申请借阅时长：%d天\n", p->Book->title, p->Book->author, p->Book->ISBN, p->User->username, p->days);
			struct tm* borrowTimeInfo = localtime(&p->borrowtime);
			// 格式化输出借阅时间
			printf("借阅时间: %04d-%02d-%02d %02d:%02d:%02d\n", borrowTimeInfo->tm_year + 1900, borrowTimeInfo->tm_mon + 1,
				borrowTimeInfo->tm_mday, borrowTimeInfo->tm_hour, borrowTimeInfo->tm_min, borrowTimeInfo->tm_sec);
			printf("**********************************\n");
			p = p->next;
		}
	}
}
//初始化借书历史（导入前）
void Initborrowhistory(User* head)
{
	User* p = head;
	if (p == NULL)
		return;
	else
	{
		while (p != NULL)
		{
			p->borrowhistory = NULL;
			p = p->next;
		}
	}
}
//检查超时
void checkOverdue(User* user) {
	if (user == NULL || user->borrowhistory == NULL) {
		printf("无借阅记录，无需检查超时情况。\n");
		return;
	}

	int hasOverdue = 0;
	time_t currentTime = time(NULL);
	StackNode* current = user->borrowhistory;
	while (current != NULL) {
		struct tm* borrowTimeInfo = localtime(&current->borrowtime);
		double diffTimeInDays = difftime(currentTime, current->borrowtime) / (60 * 60 * 24);

		if (diffTimeInDays > current->days) {
			hasOverdue = 1;
			double fine = (diffTimeInDays - current->days) * 10;//每超时1天罚款10元
			printf("用户 %s 借阅的图书《%s》已超时 %.0f 天，需缴纳罚款 %.0f 元。\n",
				user->username, current->Book->title, diffTimeInDays - 30, fine);
		}

		current = current->next;
	}

	if (!hasOverdue) {
		printf("用户 %s 的所有借阅均未超时。\n", user->username);
	}
}
//续借
void renewBook(User* user, char* isbn) {
	if (user == NULL || user->borrowhistory == NULL) {
		printf("用户无借阅记录，无法续借。\n");
		return;
	}

	StackNode* current = user->borrowhistory;
	int redays = 0;
	while (current != NULL) {
		if (strcmp(current->Book->ISBN, isbn) == 0) {
			printf("请输入你要借用的天数:\n");
			scanf("%d", &redays);
			current->days += redays;
			printf("图书《%s》续借成功。\n", current->Book->title);
			return;
		}
		current = current->next;
	}

	printf("未找到对应ISBN的借阅记录，无法续借。\n");
}
//更新所有用户借阅历史（借阅增删时更新）
void saveAllBorrowHistory(User* userList)
{
	allborrowlist = NULL;
	User* currentUser = userList->next;
	while (currentUser != NULL) {
		StackNode* currentBorrow = currentUser->borrowhistory;
		while (currentBorrow != NULL) {
			StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
			if (newNode == NULL) {
				printf("内存分配失败，无法保存借阅历史!\n");
				return;
			}

			newNode->User = currentUser;
			newNode->Book = currentBorrow->Book;
			newNode->borrowtime = currentBorrow->borrowtime;
			newNode->days = currentBorrow->days;
			newNode->next = allborrowlist;
			allborrowlist = newNode;

			currentBorrow = currentBorrow->next;
		}
		currentUser = currentUser->next;
	}
}
//导出借阅信息
void exportborrow(StackNode* allborrowlist,char path[])
{
	FILE* fp = fopen(path, "w");
	StackNode* p = allborrowlist;
	while (p != NULL)
	{
		fprintf(fp, "%s\n%s\n%s\n%s\n%d\n", p->User->username, p->Book->title, p->Book->author, p->Book->ISBN, p->days);
		struct tm* borrowtimeinfo = localtime(&p->borrowtime);
		if (borrowtimeinfo != NULL)
		{
			fprintf(fp, "借阅时间: %04d-%02d-%02d %02d:%02d:%02d\n", borrowtimeinfo->tm_year + 1900, borrowtimeinfo->tm_mon + 1,
				borrowtimeinfo->tm_mday, borrowtimeinfo->tm_hour, borrowtimeinfo->tm_min, borrowtimeinfo->tm_sec);
			fprintf(fp, "\n");
			p = p->next;
		}
	}
	printf("所有借用信息已保存\n");
	fclose(fp);
}
//导入借阅历史
void loadAllBorrowHistoryFromFile(User* userList) {
	FILE* file = fopen("./Borrowinformation.txt", "r");
	if (file == NULL) {
		perror("无法打开文件");
		return;
	}

	char username[50];
	char title[100];
	char author[50];
	char ISBN[20];
	int days;
	int year, month, day, hour, minute, second;

	while (!feof(file)) {
		User* targetUser = userList->next;
		StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
		if (newNode == NULL) {
			perror("内存分配失败");
			fclose(file);
			return;
		}

		if (fscanf(file, "%s\n%s\n%s\n%s\n%d\n借阅时间:%d-%d-%d %d:%d:%d\n",
			username, title, author, ISBN, &days, &year, &month, &day, &hour, &minute, &second) == 11) {

			while (targetUser != NULL && strcmp(targetUser->username, username) != 0) {
				targetUser = targetUser->next;
			}

			if (targetUser != NULL) {
				Book* tempBook = (Book*)malloc(sizeof(Book));
				if (tempBook == NULL) {
					perror("内存分配失败");
					free(newNode);
					fclose(file);
					return;
				}

				strcpy(tempBook->title, title);
				strcpy(tempBook->author, author);
				strcpy(tempBook->ISBN, ISBN);

				struct tm borrowTime = { 0 };
				borrowTime.tm_year = year - 1900;
				borrowTime.tm_mon = month - 1;
				borrowTime.tm_mday = day;
				borrowTime.tm_hour = hour;
				borrowTime.tm_min = minute;
				borrowTime.tm_sec = second;
				newNode->borrowtime = mktime(&borrowTime);

				newNode->User = targetUser;
				newNode->Book = tempBook;
				newNode->next = targetUser->borrowhistory;
				newNode->days = days;
				targetUser->borrowhistory = newNode;
			}
		}
	}

	fclose(file);
}

void loadAllBorrowHistoryFromdesignateFile(User* userList,char path[]) {
	FILE* file = fopen(path, "r");
	if (file == NULL) {
		perror("无法打开文件");
		return;
	}

	char username[50];
	char title[100];
	char author[50];
	char ISBN[20];
	int days;
	int year, month, day, hour, minute, second;

	while (!feof(file)) {
		User* targetUser = userList->next;
		StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
		if (newNode == NULL) {
			perror("内存分配失败");
			fclose(file);
			return;
		}

		if (fscanf(file, "%s\n%s\n%s\n%s\n%d\n借阅时间:%d-%d-%d %d:%d:%d\n",
			username, title, author, ISBN, &days, &year, &month, &day, &hour, &minute, &second) == 11) {

			while (targetUser != NULL && strcmp(targetUser->username, username) != 0) {
				targetUser = targetUser->next;
			}

			if (targetUser != NULL) {
				Book* tempBook = (Book*)malloc(sizeof(Book));
				if (tempBook == NULL) {
					perror("内存分配失败");
					free(newNode);
					fclose(file);
					return;
				}

				strcpy(tempBook->title, title);
				strcpy(tempBook->author, author);
				strcpy(tempBook->ISBN, ISBN);

				struct tm borrowTime = { 0 };
				borrowTime.tm_year = year - 1900;
				borrowTime.tm_mon = month - 1;
				borrowTime.tm_mday = day;
				borrowTime.tm_hour = hour;
				borrowTime.tm_min = minute;
				borrowTime.tm_sec = second;
				newNode->borrowtime = mktime(&borrowTime);

				newNode->User = targetUser;
				newNode->Book = tempBook;
				newNode->next = targetUser->borrowhistory;
				newNode->days = days;
				targetUser->borrowhistory = newNode;
			}
		}
	}

	fclose(file);
}
// 创建新的图书节点*
Book* createbookNode(char* ISBN, char* title, char* author) {
	Book* newbook = (Book*)malloc(sizeof(Book));
	newbook->next = NULL;
	strcpy(newbook->title, title);
	strcpy(newbook->ISBN, ISBN);
	strcpy(newbook->author, author);
	newbook->bnum = 0;   //图书被借阅的总次数
	return newbook;
}
// 创建新的树节点（分类节点）
TreeNode* createTreeNode(const char* category) {
	TreeNode* newNode = (TreeNode*)malloc(sizeof(TreeNode));

	strcpy(newNode->category, category);
	for (int i = 0; i < 10; i++) {
		newNode->children[i] = NULL;
	}
	newNode->books = NULL;
	newNode->parent = NULL;
	return newNode;
}
// 在分类节点下添加图书*
void addbookToCategory(TreeNode* categoryNode, char* bookISBN, char* bookTitle, char* bookauthor) {
	Book* newbook = createbookNode(bookISBN, bookTitle, bookauthor);
	if (categoryNode->books == NULL) {
		categoryNode->books = newbook;
	}
	else {
		Book* p = categoryNode->books;
		while (p->next != NULL) {
			p = p->next;
		}
		p->next = newbook;
	}
}
// 在树中添加 分类 节点（作为某个父节点的子节点）
void addCategory(TreeNode* parentNode, char* categoryName) {
	TreeNode* newCategory = createTreeNode(categoryName);
	newCategory->parent = parentNode;

	for (int i = 0; i < 10; i++) {
		if (parentNode->children[i] == NULL) {
			parentNode->children[i] = newCategory;
			break;
		}
	}
}
//遍历输出图书
void visit(Book* bt)
{
	printf("%s\n", bt->title);
}
void display(TreeNode* root)
{
	if (root == NULL)
		return;
	else
	{
		Book* p = root->books;
		while (p != NULL)
		{
			visit(p);
			p = p->next;
		}
		for (int i = 0; i < 10; i++)
		{
			display(root->children[i]);
		}
	}
}
//从文件中读出图书分类情况
void loadbookcategory(TreeNode* root)
{
	FILE* fp = fopen("C://Users//吴品东//Desktop//Library//bookcategory.txt", "r");
	char lcategory[100];
	while (!feof(fp))
	{
		fscanf(fp, "%s", lcategory);
		addCategory(root, lcategory);
	}
	fclose(fp);
}
//从文件中读出图书情况
void loadbook(TreeNode* root)
{
	FILE* fp = fopen("./path.txt", "r");
	int i = 0;
	while (!feof(fp))
	{
		char txt[100];
		//int c;
		//while ((c = getchar()) != '\n' && c!= EOF);
	   // fgets(txt,sizeof(txt),fp);
		fscanf(fp, "%s", txt);
		FILE* FP = fopen(txt, "r");
		while (!feof(FP))
		{
			char pISBN[20];
			char ptitle[100];
			char pauthor[100];
			fscanf(FP, "%s", pISBN);
			fscanf(FP, "%s", ptitle);
			fscanf(FP, "%s", pauthor);;
			addbookToCategory(root->children[i], pISBN, ptitle, pauthor);
		}
		i++;
		fclose(FP);

	}
	fclose(fp);
}
//将图书分类情况写入文件
void exportroot(TreeNode* root)
{
	//bookcategory
	FILE* fp = fopen("./bookcategory.txt", "w");
	int i;
	for (i = 0; i < 10 && root->children[i + 1] != NULL; i++)
	{
		fprintf(fp, "%s\n", root->children[i]->category);
	}
	fprintf(fp, "%s", root->children[i]->category);
	fclose(fp);
	//path
	FILE* Fp = fopen("./path.txt", "w");
	for (i = 0; i < 10 && root->children[i + 1] != NULL; i++)
	{
		fprintf(Fp, "./%s.txt\n", root->children[i]->category);
	}
	fprintf(Fp, "./%s.txt", root->children[i]->category);
	fclose(Fp);
	printf("信息导出成功\n");

}
//将图书情况写入文件
void exportbooks(TreeNode* root)
{
	//categorys
	FILE* f = fopen("./path.txt", "r");
	int i = 0;
	while (!feof(f))
	{
		char txt[100];
		//int c;
		//while ((c = getchar()) != '\n' && c!= EOF);
	   // fgets(txt,sizeof(txt),fp);
		fscanf(f, "%s", txt);
		FILE* FP = fopen(txt, "w");
		// while (!feof(FP))
		{
			TreeNode* p = root->children[i];
			Book* pbook = p->books;
			while (pbook->next != NULL)
			{
				fprintf(FP, "%s\n%s\n%s\n", pbook->ISBN, pbook->title, pbook->author);
				pbook = pbook->next;
			}
			fprintf(FP, "%s\n%s\n%s", pbook->ISBN, pbook->title, pbook->author);
		}
		i++;
		fclose(FP);

	}
	fclose(f);
}
//  vertex顶点 arc边 vertexNum顶点数 arc边数
//  初始化定义书架之间的距离
void InitMGraph(int* vertex, int arc[][MAX_VERTEX], int vertexNum, int arcNum)
{
	int  x;
	for (int i = 0; i < vertexNum; i++) { //顶点数组赋值；
		vertex[i] = i;
	}
	for (int i = 0; i < vertexNum; i++) //初始化邻接矩阵；
		for (int j = 0; j < vertexNum; j++) {
			if (i == j)
				arc[i][j] = 0;
			else
				arc[i][j] = INT_MAX_;//赋值正无穷；
		}
	arc[0][1] = 3;
	arc[1][0] = 3;
	//printf("前台与历史类书籍距离为3\n");
	arc[0][2] = 2;
	arc[2][0] = 2;
	//printf("前台与文学类书籍距离为2\n");
	arc[0][3] = 2;
	arc[3][0] = 2;
	//printf("前台与其他类书籍距离为2\n");
	arc[1][2] = 1;
	arc[2][1] = 1;
	//printf("历史与文学类书籍距离为1\n");
	arc[1][3] = 2;
	arc[3][1] = 2;
	//printf("历史与其他类书籍距离为2\n");
}

//在dist中查找s[i]为0的最小值元素
int findMinDist(int* dist, int* s, int vertexNum) {
	int flag = 0, min, index;
	for (int i = 0; i < vertexNum; i++) {
		if (flag == 0 && s[i] == 0) {
			min = dist[i];
			index = i;
			flag = 1;
		}
		else if (flag == 1 && s[i] == 0 && min > dist[i]) {
			min = dist[i];
			index = i;
		}
	}
	return index;
}
//生成最短路径对应的dist，path
void Dijkstra(int arc[][MAX_VERTEX], int vertexNum, int startV, int* s, int* dist, int* path) {
	for (int i = 0; i < vertexNum; i++) {
		dist[i] = arc[startV][i]; //初始化数组dist,path；
		if (dist[i] != INT_MAX_)
			path[i] = startV; //将原点设为上一条路径；
		else
			path[i] = -1;
	}
	for (int i = 0; i < vertexNum; i++) {
		s[i] = 0;
	}
	//验证数组内容是否正确
	s[startV] = 1; //1值代表该结点已经加入了最短路径；
	int num = 1;
	int min;
	while (num < vertexNum) { //当顶点数num小于图的顶点数，即不是所有顶点加入最小路径；
		min = findMinDist(dist, s, vertexNum); //dist中查找s[i]为0的最小值元素；
		s[min] = 1;
		//将新生成的结点加入集合s中
		for (int i = 0; i < vertexNum; i++) {
			//修改数组dist和path
			if ((s[i] == 0) && (dist[i] > dist[min] + arc[min][i])) {
				dist[i] = dist[min] + arc[min][i]; //用已经找到的最短路径修改对应的dist；
				path[i] = min; //用已经找到的最短路径修改对应的path；
				printf("\n");
			}
		}
		num++;
	}
}
//输出最短路径
void displayPath(int* dist, int* path, int* s, int startV, int reachV, int vertexNum, char di[][20]) {
	int temp;
	int patharr[10];
	int con = 0;
	int t;
	for (int i = 0; i < vertexNum; i++) {
		con = 0;
		if (i == reachV) {
			printf("从%d.%s --> %d.%s:\n", startV, di[startV], i, di[i]);
			if (dist[i] != INT_MAX_)
				printf("最短路径长度为:%d\n", dist[i]);
			else {
				printf("不存在与该点之间的路径！\n");

				printf("\n");
				continue;
			}
			printf("最短路径为:");
			temp = i;
			while (temp != startV)
			{ //得改成逆序的；
				patharr[con++] = path[temp];
				temp = path[temp];
			}
			con--;
			if (con == 0) {
				t = patharr[con];
				printf("%d ->%d\n", patharr[con], i);
			}
			else {
				while (con >= 0) {
					t = patharr[con];
					printf("%d -> ", patharr[con]);
					con--;
				}
				t = patharr[0];
				printf("%d\n", i);
			}
			printf("\n");
		}
	}
}
//生成图书馆布局图
void printftu()
{
	initgraph(600, 700, EX_SHOWCONSOLE);
	setbkcolor(WHITE);
	cleardevice();
	setlinecolor(BLACK);
	setlinestyle(PS_SOLID, 3);
	rectangle(200, 200, 300, 250);
	rectangle(50, 300, 150, 350);
	rectangle(200, 200, 300, 250);
	rectangle(400, 350, 500, 400);
	rectangle(220, 500, 320, 550);
	settextstyle(50, 0, _T("仿宋"));
	settextcolor(BLACK);
	outtextxy(150, 50, _T("图书馆路线"));
	settextstyle(32, 0, _T("仿宋"));
	outtextxy(202, 202, _T("历史类"));
	outtextxy(52, 302, _T("文学类"));
	outtextxy(405, 355, _T("其他"));
	outtextxy(225, 505, _T("前台"));
	line(75, 300, 200, 225);
	outtextxy(125, 225, _T("1"));
	line(250, 250, 270, 500);
	outtextxy(270, 350, _T("3"));
	line(300, 225, 450, 350);
	outtextxy(405, 260, _T("2"));
	line(450, 400, 320, 525);
	outtextxy(418, 455, _T("2"));
	line(100, 350, 220, 525);
	outtextxy(120, 405, _T("2"));
	//输出图片
	getchar();
}
//增加图书
 void addBook(TreeNode* root,Book* head)
{
	Book* newBook = (Book*)malloc(sizeof(Book));
	char newISBN[20];
	char newtitle[100];
	char newauthor[100];
	int num;
	int bnum;
	printf("请输入图书的相关信息：\n");
	printf("请输入新增图书的类别\n");
	for (int i = 0; i < 10 && root->children[i] != NULL; i++)
	{
		printf("%d.%s\n", i, root->children[i]->category);
	}
NEXT:
	int op = -1;
	scanf("%d", &op);
	if (root->children[op] == NULL)
	{
		printf("无效选择，请重新输入\n");
		goto NEXT;
	}
	printf("请输入图书的ISBN：\n");
	scanf("%s", newISBN);
	printf("请输入图书的名称：\n");
	scanf("%s", newtitle);
	printf("请输入图书的作者：\n");
	scanf("%s", newauthor);
	printf("请输入图书的数量：\n");
	scanf("%d", &num);
	printf("请输入图书的总借阅量：\n");
	scanf("%d", &bnum);
	strcpy(newBook->ISBN, newISBN);
	strcpy(newBook->title, newtitle);
	strcpy(newBook->author, newauthor);
	newBook->num = num;
	newBook->bnum = bnum;
	newBook->next = NULL;
	if (head == NULL)
	{
		head = newBook;
	}
	else
	{
		Book* p = head;
		while (p->next != NULL)
		{
			p = p->next;
		}
		p->next = newBook;
	}
	addbookToCategory(root->children[op], newISBN, newtitle, newauthor);
	exportroot(root);
	exportbooks(root);
}
 //推荐图书
 void recommend(User* head, TreeNode* root)
 {
	 struct StackNode* borrow = head->borrowhistory;
	 Book* books = borrow->Book;
	 TreeNode* node = root;
	 int a[10];
	 int i;
	 while (borrow != NULL)
	 {
		 books = borrow->Book;
		 for (i = 0; i < 10 && node->children[i] != NULL; i++)
		 {
			 TreeNode* p = node->children[i];
			 Book* pbook = p->books;
			 while (pbook->next != NULL)
			 {
				 if (strcmp(books->ISBN, pbook->ISBN) == 0)
				 {
					 a[i]++;
				 }
				 pbook = pbook->next;
			 }
		 }
		 borrow = borrow->next;
	 }
	 int max = 0;
	 for (int j = 0; j < 0 && node->children[j] != NULL; j++)
	 {
		 if (a[max] < a[j])
		 {
			 max = j;
		 }
	 }
	 printf("为您智能推荐的书有：\n");
	 display(node->children[max]);
 }
 //热门图书
 void hotbooks(Book* head)
 {
	 Book* p = head;
	 int i = 0;
	 int hotnum[1000];
	 char topname[3][50];
	 while (p != NULL)
	 {
		 hotnum[i++] = p->bnum;
		 p = p->next;
	 }
	 for (int j = 0; j < i; j++)
	 {
		 int max = j;
		 for (int k = j + 1; k < i; k++)
		 {
			 if (hotnum[max] < hotnum[k])
				 max = k;
		 }
		 if (max != j)
		 {
			 int t = hotnum[j];
			 hotnum[j] = hotnum[max];
			 hotnum[max] = t;
		 }
	 }
	 p = head;
	 while (p != NULL)
	 {
		 if (p->bnum == hotnum[0])
		 {
			 strcpy(topname[0], p->title);
		 }
		 else if (p->bnum == hotnum[1])
		 {
			 strcpy(topname[1], p->title);
		 }
		 else if (p->bnum == hotnum[2])
		 {
			 strcpy(topname[2], p->title);
		 }
		 p = p->next;
	 }
	 printf("%s %d\n", topname[0], hotnum[0]);
	 printf("%s %d\n", topname[1], hotnum[1]);
	 printf("%s %d\n", topname[2], hotnum[2]);
	 toptushu(topname, hotnum);
 }
 //top图书
 void toptushu(char name[][50], int topnum[])
 {
	 char str[60];
	 int  i;
	 int num = 5000, w = 45, h = 100;
	 initgraph(800, 800, EX_SHOWCONSOLE);
	 setbkcolor(WHITE);
	 cleardevice();
	 settextcolor(BLACK);
	 settextstyle(50, 0, _T("仿宋"));
	 outtextxy(250, 20, _T("热门图书TOP3"));
	 settextstyle(20, 0, _T("仿宋"));
	 setlinecolor(BLACK);
	 setlinestyle(PS_SOLID, 4);
	 line(100, 100, 100, 700);
	 line(100, 700, 700, 700);
	 outtextxy(75, 100, _T("借"));
	 outtextxy(75, 124, _T("阅"));
	 outtextxy(75, 148, _T("次"));
	 outtextxy(75, 172, _T("数"));
	 outtextxy(15, 194, _T("（单位："));
	 outtextxy(15, 220, _T("次）"));
	 outtextxy(680, 710, _T("书名"));
	 int x1;
	 for (i = 1; i <= 3; i++)
	 {
		 settextstyle(24, 0, _T("仿宋"));
		 char str[50] = "";
		 sprintf(str, "%d", topnum[i - 1]);
		 x1 = 140 + 200 * (i - 1);
		 outtextxy(x1 - 20, 702, name[i - 1]);
		 if (topnum[i - 1] <= 600)
		 {
			 rectangle(x1, 700 - topnum[i - 1], x1 + 120, 700);
			 settextstyle(30, 0, _T("仿宋"));
			 outtextxy(x1 + 20, 700 - topnum[i - 1] - 40, str);
		 }
		 else
		 {
			 rectangle(x1, 100, x1 + 120, 700);
			 settextstyle(30, 0, _T("仿宋"));
			 outtextxy(x1 + 20, 80, _T("600+"));
		 }
	 }
	 FlushBatchDraw();
	 int running = 1;
	 while (running)
	 {
		 if (_kbhit)
		 {
			 char ch = _getch();
			 if (ch == 27)
			 {
				 running = 0;
			 }
		 }
		 else
		 {
			 Sleep(100);
		 }
	 }
	 closegraph();

 }
 //活跃用户
 void hotusers(User* head)
 {
	 User* p = head;
	 int i = 0;
	 int hotnum[1000];
	 char topname[3][50];
	 while (p != NULL)
	 {
		 hotnum[i++] = p->borrownum;
		 p = p->next;
	 }
	 for (int j = 0; j < i; j++)
	 {
		 int max = j;
		 for (int k = j + 1; k < i; k++)
		 {
			 if (hotnum[max] < hotnum[k])
				 max = k;
		 }
		 if (max != j)
		 {
			 int t = hotnum[j];
			 hotnum[j] = hotnum[max];
			 hotnum[max] = t;
		 }
	 }
	 p = head;
	 while (p != NULL)
	 {
		 if (p->borrownum == hotnum[0])
		 {
			 strcpy(topname[0], p->username);
		 }
		 else if (p->borrownum == hotnum[1])
		 {
			 strcpy(topname[1], p->username);
		 }
		 else if (p->borrownum == hotnum[2])
		 {
			 strcpy(topname[2], p->username);
		 }
		 p = p->next;
	 }
	 topuser(topname, hotnum);
 }
 //top用户
 void topuser(char name[][50], int topnum[])
 {
	 char str[60];
	 int  i;
	 int num = 5000, w = 45, h = 100;
	 initgraph(800, 800, EX_SHOWCONSOLE);
	 setbkcolor(WHITE);
	 cleardevice();
	 settextcolor(BLACK);
	 settextstyle(50, 0, _T("仿宋"));
	 outtextxy(180, 30, _T("活跃用户TOP3"));
	 settextstyle(20, 0, _T("仿宋"));
	 setlinecolor(BLACK);
	 setlinestyle(PS_SOLID, 4);
	 line(100, 100, 100, 700);
	 line(100, 700, 700, 700);//绘制坐标轴
	 outtextxy(70, 100, _T("借"));
	 outtextxy(70, 124, _T("阅"));
	 outtextxy(70, 148, _T("次"));
	 outtextxy(70, 172, _T("数"));
	 outtextxy(18, 196, _T("（单位："));
	 outtextxy(20, 220, _T("次）"));
	 int x1;
	 for (i = 1; i <= 3; i++)
	 {
		 settextstyle(24, 0, _T("仿宋"));
		 char str[50] = "";
		 sprintf(str, "%d", topnum[i - 1]);
		 x1 = 140 + 200 * (i - 1);
		 outtextxy(x1 - 20, 702, name[i - 1]);
		 if (topnum[i - 1] <= 600)
		 {
			 rectangle(x1, 700 - topnum[i - 1], x1 + 120, 700);
			 settextstyle(30, 0, _T("仿宋"));
			 outtextxy(x1 + 20, 700 - topnum[i - 1] - 40, str);
		 }
		 else
		 {
			 rectangle(x1, 100, x1 + 120, 700);
			 settextstyle(30, 0, _T("仿宋"));
			 outtextxy(x1 + 20, 80, _T("600+"));
		 }
	 }

	 FlushBatchDraw();
	 int running = 1;
	 while (running)
	 {
		 if (_kbhit)
		 {
			 char ch = _getch();
			 if (ch == 27)
			 {
				 running = 0;
			 }
		 }
		 else
		 {
			 Sleep(100);
		 }
	 }
	 closegraph();

 }
//打印*框
void printframe()
{
	printf("********************************************************\n");
	printf("*                                                      *\n");
	printf("*                                                      *\n");
	printf("*                                                      *\n");
	printf("*                                                      *\n");
	printf("*                                                      *\n");
	printf("*                                                      *\n");
	printf("*                                                      *\n");
	printf("*                                                      *\n");
	printf("*                                                      *\n");
	printf("*                                                      *\n");
	printf("*                                                      *\n");
	printf("*                                                      *\n");
	printf("*                                                      *\n");
	printf("*                                                      *\n");
	printf("*                                                      *\n");
	printf("********************************************************\n");



}
//用户登入注册界面
int userface()
{
	system("cls");
	//用户登入注册界面
	int op1 = 0;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = 21;  // 列
	pos.Y = 3;  // 行
	printframe();
	SetConsoleCursorPosition(hConsole, pos);
	printf("图书馆管理系统\n");

	pos.X = 21;
	pos.Y = 5;
	SetConsoleCursorPosition(hConsole, pos);
	printf("1.登入");
	pos.X = 21;
	pos.Y = 6;
	SetConsoleCursorPosition(hConsole, pos);
	printf("2.注册");
	pos.X = 20;
	pos.Y = 7;
	SetConsoleCursorPosition(hConsole, pos);
	printf("-1.退出系统");
	pos.X = 21;
	pos.Y = 9;
	SetConsoleCursorPosition(hConsole, pos);
	printf("请输入你要进行的操作");
	scanf("%d", &op1);
	return op1;
	getchar();
}
//管理员界面
int administratorface()
{
	system("cls");
	int op1 = 0;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = 21;
	pos.Y = 2;  
	printframe();
	SetConsoleCursorPosition(hConsole, pos);
	printf("总功能列表");
	pos.X = 2;
	pos.Y = 3;
	SetConsoleCursorPosition(hConsole, pos);
	printf("-1.退出                       0.图书导航");
	pos.Y = 4;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 1.增加图书                   2.删除图书");
	pos.Y = 5;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 3.更改图书信息               4.查找图书");
	pos.Y = 6;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 5.列出所有图书信息           6.根据ISBN排序");
	pos.Y = 7;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 7.导入图书信息               8.导出图书信息");
	pos.Y = 8;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 9.借用申请                  10.借出处理");
	pos.Y = 9;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 11.打印当前用户借阅历史     12.归还图书");
	pos.Y = 10;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 13.打印所有用户借阅历史     14.修改用户信息");
	pos.Y = 11;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 15.导出借阅历史             16.导入借阅历史");
	pos.Y = 12;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 17.检查超时                 18.续借图书");
	pos.Y = 13;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 19.新增图书分类             20.推荐图书");
	pos.Y = 14;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 21.查看热门图书             22.展示活跃用户");
	pos.Y = 15;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 请输入操作码：");
	scanf("%d", &op1);
	return op1;

}
//普通用户界面
int regularuserface()
{
	system("cls");
	int op2 = 0;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = 21;
	pos.Y = 2;
	printframe();
	SetConsoleCursorPosition(hConsole, pos);
	printf("总功能列表");
	pos.X = 2;
	pos.Y = 3;
	SetConsoleCursorPosition(hConsole, pos);
	printf("-1.退出                       0.图书导航");
	pos.Y = 4;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 1.查找图书                   2.列出所有图书信息");
	pos.Y = 5;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 3.借用申请                   4.打印当前用户借阅历史");
	pos.Y = 6;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 5.归还图书                   6.修改密码");
	pos.Y = 7;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 7.检查超时                   8.续借图书");
	pos.Y = 8;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 9.推荐图书                   10.查看热门图书");
	pos.Y = 9;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 11.展示活跃用户top3     ");
	pos.Y = 15;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 请输入操作码：");
	scanf("%d", &op2);
	return op2;

}
//登入界面
User* loginface(User* head) 
{
	printframe();
	User * flag = login(head);
	return flag;
	
}
//注册界面
void registerface(User* head)
{
	system("cls");
	printframe();
	registerUser(head);
	system("cls");
	printframe();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = 21;  // 列
	pos.Y = 3;  // 行
	SetConsoleCursorPosition(hConsole, pos);
	printf("注册成功");

}

int main()
{
	Book* bhead;
	BookInit(bhead);
	User* uhead;
	UserInit(uhead);
	TreeNode* root = createTreeNode("根分类");
	User* tuser = NULL;//当前登入的用户；

	Queue* borrowqueue = createQueue();
	int  vertex[10];//储存所有的顶点；
	int arc[10][10];//邻接矩阵，结点间的连通关系；
	int vertexNum = 4, arcNum = 10; //顶点个数，边的个数；
	InitMGraph(vertex, arc, vertexNum, arcNum);
	char di[10][20] = { "前台" ,"历史类","文学类", "其他类" };

	//初始化导入
	Initborrowhistory(uhead);
	char path[1000] = "./Bookinformation.txt";
	fileread(bhead, path);
	loaduser(uhead);
	loadbookcategory(root);
	loadbook(root);
	Initborrowhistory(uhead);
	loadAllBorrowHistoryFromFile(uhead);


	int op1 = -2;
	//goto出口
    gotouserface:
	op1 = userface();
	if (op1 == 1)
	{
		system("cls");
		tuser = loginface(uhead);
		if (tuser != NULL)
		{
			system("cls");
			printframe();
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			COORD pos;
			pos.X = 21;
			pos.Y = 6;
			SetConsoleCursorPosition(hConsole, pos);
			printf("登入成功");
			goto gotoadministratorface;
		}
		else
		{
			system("cls");
			printframe();
			HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
			COORD pos;
			pos.X = 15;
			pos.Y = 6;
			SetConsoleCursorPosition(hConsole, pos);
			printf("登入失败，请重新尝试");
			system("pause");
			goto gotouserface;
		}
	}
	else if (op1 == 2)
	{
		registerface(uhead);
		goto gotouserface;
	}
	else if(op1 == -1)
	{
		system("cls");
		char spath[1000] = "./Bookinformation.txt";
		exportBookfile(bhead, spath);
		exportusersfile(uhead);
		char bpath[1000] = "./Borrowinformation.txt";
		saveAllBorrowHistory(uhead);
		exportborrow(allborrowlist, bpath);
		exportroot(root);
		exportbooks(root);
		return 0;
	}
	else
	{
		goto gotouserface;
	}

	gotoadministratorface:
	if (tuser->isAdmin == 1)
	{
		int op = -2;
		op = administratorface();
		if (op == -1)
		{
			goto gotouserface;
		}
		else if (op == 0)
		{
			system("cls");
			printf("***********************************\n");
			printf("*      0. 图书馆前台              *\n");
			printf("*      1. 历史类书架所在地        *\n");
			printf("*      2. 文学类书架所在地        *\n");
			printf("*      3. 其他类书架所在地        *\n");
			printf("*      4. 输入4查看图书馆布局     *\n");
			printf("*      5. 输入-1退出图书导航      *\n");
			printf("***********************************\n");
			while (1)
			{
				int x, y;
				printf("请输入你将进行的操作或所在位置(序号):");
				scanf("%d", &x);
				if (x == -1)
				{
					printf("退出图书馆导航 \n");
					printf("***********************************\n");
					printf("*      0. 图书馆前台              *\n");
					printf("*      1. 历史类书架所在地        *\n");
					printf("*      2. 文学类书架所在地        *\n");
					printf("*      3. 其他类书架所在地        *\n");
					printf("*      4. 输入4查看图书馆布局     *\n");
					printf("*      5. 输入-1退出图书导航      *\n");
					printf("***********************************\n");
					break;
				}
				else if (x == 4)
				{
					printftu();
					continue;
				}
				printf("请输入你将需要前往图书馆的位置(序号):");
				scanf("%d", &y);
				int s[10], dist[10], path[10];
				//s[i]存放源点和己经生成的终点，其初态为只有一个源点x; 
				//每个分量dist表示当前所找到的从始点x到终点y的最短路径的长度；
				//path[i]是一个字符串，表示当前所找到的从始点x到终点y的最短路径；
				Dijkstra(arc, vertexNum, x, s, dist, path);
				printf("由%d.%s 到%d.%s的最短路径为:\n", x, di[x], y, di[y]);
				printf("\n");
				displayPath(dist, path, s, x, y, vertexNum, di);
				system("pause");
				
			}
			goto gotoadministratorface;
		}
		else if (op == 1)
		{
			system("cls");
			addBook(root, bhead);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 2)
		{
			system("cls");
			printf("请输入要删除的图书的ISBN码:");
			char isbn[20];
			scanf("%s", isbn);
			deleteBook(bhead, isbn);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 3)
		{
			system("cls");
			printf("请输入要删除的图书的ISBN码:");
			char isbn[20];
			scanf("%s", isbn);
			modifyBook(bhead, isbn);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 4)
		{
			system("cls");
			printf("请输入要查找的图书的ISBN码:");
			char isbn[20];
			scanf("%s", isbn);
			searchBook(bhead, isbn);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 5)
		{
			system("cls");
			printallBook(bhead);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 6)
		{
			system("cls");
			sort(bhead);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 7)
		{
			system("cls");
			char path[1000];
			printf("请输入要打开文件的路径:");
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
			fgets(path, sizeof(path), stdin);
			printf("路径为%s\n", path);
			if (path[strlen(path) - 1] == '\n')
			{
				path[strlen(path) - 1] = '\0';
			}
			fileread(bhead, path);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 8)
		{
			system("cls");
			char path[1000];
			printf("请输入要保存文件的路径:");
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
			fgets(path, sizeof(path), stdin);
			printf("路径为%s\n", path);
			if (path[strlen(path) - 1] == '\n')
			{
				path[strlen(path) - 1] = '\0';
			}
			exportBookfile(bhead, path);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 9)
		{
			system("cls");
			printf("请输入你要借用图书的ISBN码:");
			char requestisbn[20];
			int requestday;
			scanf("%s", requestisbn);
			StackNode* repeatbook = tuser->borrowhistory;
			int reflag = 0;
			while (repeatbook != NULL)
			{
				if (strcmp(repeatbook->Book->ISBN, requestisbn) == 0)
				{
					reflag = 1;
					break;
				}
				repeatbook = repeatbook->next;
			}
			if (reflag == 1)
			{
				printf("您已借过本书，若想续借请转到续借功能\n");
				system("pause");
				goto gotoadministratorface;
			}
			Book* requestBook = searchBook(bhead, requestisbn);
			if (requestBook != NULL)
			{
				printf("请输入你要借用的天数:");
				scanf("%d", &requestday);
				enqueue(borrowqueue, tuser, requestBook, requestday);
				printf("申请成功\n");
				system("pause");
				goto gotoadministratorface;
			}
			
		}
		else if (op == 10)
		{
			while (borrowqueue->front != NULL)
			{
				system("cls");
				requestBorrow(borrowqueue->front->user, borrowqueue->front->book, borrowqueue->front->days);
				saveAllBorrowHistory(uhead);
				dequeue(borrowqueue);
				system("pause");
				goto gotoadministratorface;
			}
		}
		else if (op == 11)
		{
			system("cls");
			tuser->borrowhistory = NULL;
			loadAllBorrowHistoryFromFile(uhead);
			printhistory(tuser);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 12)
		{
			system("cls");
			printf("请输入你要归还的图书ISBN\n");
			char tisbn[20];
			scanf("%s", tisbn);
			StackNode* re;//判断是否在借阅历史;
			int reflag = 0;
			re = tuser->borrowhistory;
			while (re != NULL)
			{
				if (strcmp(re->Book->ISBN, tisbn) == 0)
				{
					reflag = 1;
					break;
				}
				re = re->next;
			}
			if (reflag == 0)
			{
				printf("您未借阅过此书\n");
				system("pause");
				goto gotoadministratorface;
			}
			Book* rbook = searchBook(bhead, tisbn);
			int rflag = 0;
			rflag = returnbook(tuser->borrowhistory, rbook);
			if (rflag)
				tuser->borrowhistory = NULL;
			printf("归还成功\n");
			saveAllBorrowHistory(uhead);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 13)
		{
			system("cls");
			saveAllBorrowHistory(uhead);
			printfallhistoury(allborrowlist);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 14)
		{
		system("cls");
		char username[50];
		printf("请输入要更改用户信息的用户名:");
		scanf("%s", username);
		User* p = uhead;
		int flag = 0;
		while (p != NULL)
		{
			if (strcmp(p->username, username) == 0)
			{
				flag = 1;
				break;
			}
			p = p->next;
		}
		if (flag == 0)
		{
			printf("未找到该用户,请重试\n");
			system("pause");
			goto gotoadministratorface;
		}
		else
		{
			modifyUser(p);
			system("pause");
			goto gotoadministratorface;
		}
		}
		else if (op == 15)
		{
			system("cls");
			char path[1000];
			printf("请输入要保存文件的路径:");
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
			fgets(path, sizeof(path), stdin);
			printf("路径为%s\n", path);
			if (path[strlen(path) - 1] == '\n')
			{
				path[strlen(path) - 1] = '\0';
			}
			saveAllBorrowHistory(uhead);
			exportborrow(allborrowlist,path);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 16)
		{
			system("cls");
			char path[1000];
			printf("请输入要保存文件的路径:");
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
			fgets(path, sizeof(path), stdin);
			printf("路径为%s\n", path);
			if (path[strlen(path) - 1] == '\n')
			{
				path[strlen(path) - 1] = '\0';
			}
			loadAllBorrowHistoryFromdesignateFile(uhead,path);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 17)
		{
			system("cls");
			checkOverdue(tuser);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 18)
		{
			system("cls");
			char reisbn[20];
			printf("请输入你要续借图书的ISBN\n");
			scanf("%s", reisbn);
			renewBook(tuser, reisbn);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 19)
		{
			system("cls");
			char category[100];
			printf("请输入新增图书类别名\n");
			scanf("%s", category);
			addCategory(root, category);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 20)
		{
			system("cls");
			recommend(tuser, root);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 21)
		{
			system("cls");
			hotbooks(bhead);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 22)
		{
			system("cls");
			hotusers(uhead);
			system("pause");
			goto gotoadministratorface;
		}

	}

	gotoregularuserface:
	if(tuser->isAdmin == 0)
	{
		int op = -2;
		op = regularuserface();
		if (op == -1)
		{
			goto gotouserface;
		}
		else if (op == 0)
		{
			system("cls");
			printf("***********************************\n");
			printf("*      0. 图书馆前台              *\n");
			printf("*      1. 历史类书架所在地        *\n");
			printf("*      2. 文学类书架所在地        *\n");
			printf("*      3. 其他类书架所在地        *\n");
			printf("*      4. 输入4查看图书馆布局     *\n");
			printf("*      5. 输入-1退出图书导航      *\n");
			printf("***********************************\n");
			while (1)
			{
				int x, y;
				printf("请输入你将进行的操作或所在位置(序号):");
				scanf("%d", &x);
				if (x == -1)
				{
					printf("退出图书馆导航 \n");
					printf("***********************************\n");
					printf("*      0. 图书馆前台              *\n");
					printf("*      1. 历史类书架所在地        *\n");
					printf("*      2. 文学类书架所在地        *\n");
					printf("*      3. 其他类书架所在地        *\n");
					printf("*      4. 输入4查看图书馆布局     *\n");
					printf("*      5. 输入-1退出图书导航      *\n");
					printf("***********************************\n");
					break;
				}
				else if (x == 4)
				{
					printftu();
					continue;
				}
				printf("请输入你将需要前往图书馆的位置(序号):");
				scanf("%d", &y);
				int s[10], dist[10], path[10];
				//s[i]存放源点和己经生成的终点，其初态为只有一个源点x; 
				//每个分量dist表示当前所找到的从始点x到终点y的最短路径的长度；
				//path[i]是一个字符串，表示当前所找到的从始点x到终点y的最短路径；
				Dijkstra(arc, vertexNum, x, s, dist, path);
				printf("由%d.%s 到%d.%s的最短路径为:\n", x, di[x], y, di[y]);
				printf("\n");
				displayPath(dist, path, s, x, y, vertexNum, di);
				system("pause");

			}
			goto gotoregularuserface;
		}
		else if (op == 1)
		{
			system("cls");
			printf("请输入要查找的图书的ISBN码:");
			char isbn[20];
			scanf("%s", isbn);
			searchBook(bhead, isbn);
			system("pause");
			goto gotoregularuserface;
		}
		else if (op == 2)
		{
			system("cls");
			printallBook(bhead);
			system("pause");
			goto gotoregularuserface;
		}
		else if (op == 3)
		{
			system("cls");
			printf("请输入你要借用图书的ISBN码:");
			char requestisbn[20];
			int requestday;
			scanf("%s", requestisbn);
			StackNode* repeatbook = tuser->borrowhistory;
			int reflag = 0;
			while (repeatbook != NULL)
			{
				if (strcmp(repeatbook->Book->ISBN, requestisbn) == 0)
				{
					reflag = 1;
					break;
				}
				repeatbook = repeatbook->next;
			}
			if (reflag == 1)
			{
				printf("您已借过本书，若想续借请转到续借功能\n");
				system("pause");
				goto gotoregularuserface;
			}
			Book* requestBook = searchBook(bhead, requestisbn);
			if (requestBook != NULL)
			{
				printf("请输入你要借用的天数:");
				scanf("%d", &requestday);
				enqueue(borrowqueue, tuser, requestBook, requestday);
				printf("申请成功\n");
				system("pause");
				goto gotoregularuserface;
			}

		}
		else if (op == 4)
		{
			system("cls");
			tuser->borrowhistory = NULL;
			loadAllBorrowHistoryFromFile(uhead);
			printhistory(tuser);
			system("pause");
			goto gotoregularuserface;
		}
		else if (op == 5)
		{
			system("cls");
			printf("请输入你要归还的图书ISBN\n");
			char tisbn[20];
			scanf("%s", tisbn);
			StackNode* re;//判断是否在借阅历史;
			int reflag = 0;
			re = tuser->borrowhistory;
			while (re != NULL)
			{
				if (strcmp(re->Book->ISBN, tisbn) == 0)
				{
					reflag = 1;
					break;
				}
				re = re->next;
			}
			if (reflag == 0)
			{
				printf("您未借阅过此书\n");
				system("pause");
				goto gotoregularuserface;
			}
			Book* rbook = searchBook(bhead, tisbn);
			int rflag = 0;
			rflag = returnbook(tuser->borrowhistory, rbook);
			if (rflag)
				tuser->borrowhistory = NULL;
			printf("归还成功\n");
			saveAllBorrowHistory(uhead);
			system("pause");
			goto gotoregularuserface;
		}
		else if (op == 6)
		{
			system("cls");
			char tpassword[50];
			printf("请输入修改后的密码:");
			scanf("%s", tpassword);
			strcpy(tuser->password, tpassword);
			printf("修改成功\n");
			system("pause");
			goto gotoregularuserface;
		}
		else if (op == 7)
		{
			system("cls");
			checkOverdue(tuser);
			system("pause");
			goto gotoregularuserface;
		}
		else if (op == 8)
		{
			system("cls");
			char reisbn[20];
			printf("请输入你要续借图书的ISBN\n");
			scanf("%s", reisbn);
			renewBook(tuser, reisbn);
			system("pause");
			goto gotoregularuserface;
		}
		else if (op == 9)
		{
			system("cls");
			recommend(tuser, root);
			system("pause");
			goto gotoregularuserface;
		}
		else if (op == 10)
		{
			system("cls");
			hotbooks(bhead);
			system("pause");
			goto gotoregularuserface;
		}
		else if (op == 11)
		{
			system("cls");
			hotusers(uhead);
			system("pause");
			goto gotoregularuserface;
		}

	}

}

	


