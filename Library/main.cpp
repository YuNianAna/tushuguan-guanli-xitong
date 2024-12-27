#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "time.h"
#include "Windows.h"
#include "graphics.h"
#include <conio.h>

void toptushu(char name[][50], int topnum[]);
void topuser(char name[][50], int topnum[]);

#define MAX_VERTEX 10//���Ķ������
#define INT_MAX_ 100000

//�ṹ��
//ͼ����Ϣ�ṹ��
typedef struct Book
{
	char ISBN[20];
	char title[100];
	char author[100];
	int  num;    //ͼ������
	int bnum;   //ͼ�鱻���ĵ��ܴ���
	struct Book* next;
}Book;
//�û���Ϣ�ṹ��
typedef struct User
{
	char username[50];
	char password[50];
	int isAdmin;  // 0��ʾ��ͨ�û���1��ʾ����Ա
	int borrownum = 0;  //�û��ܽ�����
	struct StackNode* borrowhistory;  //�û�������ʷ
	struct User* next;
} User;
//������ʷջ
typedef struct StackNode
{
	User* User;
	Book* Book;
	int days;
	time_t borrowtime;
	struct StackNode* next;
}StackNode;
//�����������
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

// �������ڵ�ṹ�壬����ͼ�����
typedef struct TreeNode {
	char category[100];
	struct Book* books;
	struct TreeNode* parent;
	struct TreeNode* children[10];  // �������10���ӷ���
} TreeNode;


//�궨��
StackNode* allborrowlist = NULL; //�ܽ�����ʷ

//�û���ʼ��
void UserInit(User*& head)
{
	head = (User*)malloc(sizeof(User));
	head->next = NULL;
}
//ͼ���ʼ��
void BookInit(Book*& head)
{
	head = (Book*)malloc(sizeof(Book));
	head->next = NULL;
}
 
//ɾ��ͼ��
void deleteBook(Book* head, char isbn[])
{
	int flag = 0;//�ж����޲��ҵ���
	if (head == NULL)
	{
		printf("����δ����ͼ��\n");
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
		printf("���� δ�ҵ���Ӧͼ��\n");
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
//�޸�ͼ����Ϣ
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
		printf("����δ����ͼ��\n");
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
		printf("���� δ�ҵ���Ӧͼ��\n");
		return;
	}
	else
	{
		printf("----------------------\n");
		printf("1.�޸�ISBN\n2.�޸�����\n3.�޸�����\n4.�޸�����\n5.�޸Ľ�����\n-1.�˳�\n");
		printf("----------------------\n");
		int op = 0;
		while (op != -1)
		{
			printf("�������޸Ĳ����룺\n");
			scanf("%d", &op);
			if (op == 1)
			{
				printf("������ͼ���ISBN��\n");
				scanf("%s", newisbn);
				strcpy(p->ISBN, newisbn);
			}
			else if (op == 2)
			{
				printf("������ͼ������ƣ�\n");
				scanf("%s", newtitle);
				scanf("%s", newtitle);
			}
			else if (op == 3)
			{
				printf("������ͼ������ߣ�\n");
				scanf("%s", newauthor);
				strcpy(p->author, newauthor);
			}
			else if (op == 4)
			{
				printf("������ͼ���������\n");
				scanf("%d", &num);
				p->num = num;
			}
			else if (op == 5)
			{
				printf("������ͼ��Ľ�������\n");
				scanf("%d", &bnum);
				p->bnum = bnum;
			}
		}

	}

}
//����ISBN����ͼ��
Book* searchBook(Book* head, char isbn[])
{
	Book* p;
	p = head;
	int flag = 0;
	if (head == NULL)
	{
		printf("����δ����ͼ��\n");
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
		printf("���� δ�ҵ���Ӧͼ��\n");
		return NULL;
	}
	else
	{
		printf("��ͼ�����ϢΪ��\n");
		printf("������%s\n���ߣ�%s\nISBN��%s\n", p->title, p->author, p->ISBN);
		return p;
	}
}
//��ӡ����ͼ����Ϣ
void printallBook(Book* head)
{
	Book* p;
	p = head->next;
	while (p != NULL)
	{
		printf("_____________________\n");
		printf("������%s\n���ߣ�%s\nISBN��%s\n����:%d\n������:%d\n", p->title, p->author, p->ISBN, p->num, p->bnum);
		printf("_____________________\n");
		p = p->next;
	}
}
//����ISBN����
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

			if (strcmp(p->ISBN, p->next->ISBN) < 0)//����ISBN
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
//ͨ���ļ�����ͼ����Ϣ
void fileread(Book* head, char path[])
{
	FILE* fp = fopen(path, "r");
	if (fp != NULL)
		printf("��ȡ�ɹ�\n");
	else
	{
		perror("���ļ�����");
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
//��ͼ����Ϣ�������ļ���
void exportBookfile(Book* head, char path[])
{
	FILE* fp = fopen(path, "w");
	if (fp != NULL)
		printf("�����ɹ�\n");
	else
	{
		perror("�����ļ�����");
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
	printf("ͼ����Ϣ�����ɹ�\n");
}
//���û�ע��
void registerUser(User*& head) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = 18;
	pos.Y = 5;
	SetConsoleCursorPosition(hConsole, pos);
	User* newUser = (User*)malloc(sizeof(User));
	newUser->next = NULL;
	printf("�������û���: ");
	scanf("%s", newUser->username);
	pos.X = 18;
	pos.Y = 6;
	SetConsoleCursorPosition(hConsole, pos);
	printf("����������: ");
	scanf("%s", newUser->password);
	pos.X = 10;
	pos.Y = 7;
	SetConsoleCursorPosition(hConsole, pos);
	printf("�Ƿ�Ϊ����Ա(0��ʾ��ͨ�û���1��ʾ����Ա): ");
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
//�û���¼
User* login(User* head) {
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = 18;
	pos.Y = 5;
	SetConsoleCursorPosition(hConsole, pos);
	char username[50];
	char password[50];
	printf("�������û���: ");
	scanf("%s", username);
	pos.X = 18;
	pos.Y = 6;
	SetConsoleCursorPosition(hConsole, pos);
	printf("����������: ");
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
//�û���Ϣ�޸�
void modifyUser(User*& user) {
	if (user == NULL) {
		printf("�û������ڣ��޷��޸���Ϣ!\n");
		return;
	}
	int choice;
	char newPassword[50];
	int  newisAdmin = 0;
	printf("1. �޸�����\n2. �޸�Ȩ��\n3. �˳�\n");
	while (1)
	{
		scanf("%d", &choice);
		if (choice == 1)
		{
			printf("������������: ");
			scanf("%s", newPassword);
			strcpy(user->password, newPassword);
			printf("��Ϣ�޸ĳɹ�!\n");
		}
		else if (choice == 2)
		{
			printf("��������Ȩ��(0��ʾ��ͨ�û���1��ʾ����Ա): ");
			scanf("%d", &newisAdmin);
			user->isAdmin = newisAdmin;
			printf("��Ϣ�޸ĳɹ�!\n");
		}
		else if (choice == 3)
		{
			break;
		}
		else
		{
			printf("��Чѡ������������\n");
		}
	}

}
//�û���Ϣ����
void loaduser(User*& head)
{
	FILE* fp = fopen("C://Users//��Ʒ��//Desktop//Library//Userinfomation.txt", "r");
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
//�û���Ϣ����
void exportusersfile(User* head)
{
	FILE* fp = fopen("./Userinfomation.txt", "w");
	if (fp != NULL)
		printf("�����ɹ�\n");
	else
	{
		perror("�����ļ�����");
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
	printf("�û���Ϣ�����ɹ�\n");
}
//��������������
Queue* createQueue()
{
	Queue* queue = (Queue*)malloc(sizeof(Queue));
	if (queue == NULL)
	{
		printf("�ڴ����ʧ�ܣ�\n");
		return NULL;
	}
	queue->front = queue->rear = NULL;
	return queue;
}
//���Ӳ���
void enqueue(Queue* queue, User* user, Book* Book, int day)
{
	QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
	if (newNode == NULL) {
		printf("�ڴ����ʧ�ܣ�\n");
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
//���Ӳ���
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
//ջ��ʼ��
int stackInit(StackNode* top)
{
	top = (StackNode*)malloc(sizeof(StackNode));
	if (top == NULL)
	{
		printf("�����ڴ�ʧ��\n");
		return -1;
	}
	top->next = NULL;
	return 1;
}
// �������ʷջ��ѹ��Ԫ�أ���¼���������
StackNode* pushStack(User* user, Book* Book, int days) {
	StackNode* top = user->borrowhistory;
	StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
	if (newNode == NULL) {
		printf("�ڴ����ʧ�ܣ�\n");
		return top;
	}
	newNode->User = user;
	newNode->Book = Book;
	newNode->days = days;
	newNode->borrowtime = time(NULL);
	newNode->next = top;
	return newNode;
}
//�黹ʱ��ջ
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
//������ã��Ӷ���ѹ��ջ��
void requestBorrow(User* user, Book* book, int days)
{

	if (book->num > 0)
	{
		book->num--;
		book->bnum++;
		user->borrownum++;
		user->borrowhistory = pushStack(user, book, days);
		/*pushStack(user->borrowhistory, book);*/
		printf("ͼ�顶%s���ѳɹ�����\n", book->title);
		return;
	}
	else
	{
		printf("ͼ�顶%s����ȫ��������޷�����\n", book->title);
		return;
	}
}
//����
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
		printf("�޽���ͼ��\n");
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
			/*printf("���� δ�ҵ���Ӧͼ��\n");*/
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
			printf("�ѳɹ��黹\n");
		}
	}
}
//��ӡ��ǰ�û�������ʷ
void printhistory(User* tuser)
{

	StackNode* p = tuser->borrowhistory;
	if (p == NULL)
	{
		printf("���޽�����ʷ\n");
	}
	else
	{
		printf("%s�Ľ�����ʷ����:\n", tuser->username);
		while (p != NULL)
		{
			printf("**********************************\n");
			printf("������%s\n���ߣ�%s\nISBN��%s\n�������ʱ��%d��\n", p->Book->title, p->Book->author, p->Book->ISBN, p->days);
			struct tm* borrowTimeInfo = localtime(&p->borrowtime);
			// ��ʽ���������ʱ��
			printf("����ʱ��: %04d-%02d-%02d %02d:%02d:%02d\n", borrowTimeInfo->tm_year + 1900, borrowTimeInfo->tm_mon + 1,
				borrowTimeInfo->tm_mday, borrowTimeInfo->tm_hour, borrowTimeInfo->tm_min, borrowTimeInfo->tm_sec);
			printf("**********************************\n");
			p = p->next;
		}

	}
}
//��ӡ�����û�������ʷ
void printfallhistoury(StackNode* allborrowhistory)
{
	StackNode* p = allborrowhistory;
	if (p == NULL)
		printf("�޽�����ʷ\n");
	else
	{
		printf("�ܽ�����ʷ��\n");
		while (p != NULL)
		{
			printf("**********************************\n");
			printf("������%s\n���ߣ�%s\nISBN��%s\n�����ˣ�%s\n�������ʱ����%d��\n", p->Book->title, p->Book->author, p->Book->ISBN, p->User->username, p->days);
			struct tm* borrowTimeInfo = localtime(&p->borrowtime);
			// ��ʽ���������ʱ��
			printf("����ʱ��: %04d-%02d-%02d %02d:%02d:%02d\n", borrowTimeInfo->tm_year + 1900, borrowTimeInfo->tm_mon + 1,
				borrowTimeInfo->tm_mday, borrowTimeInfo->tm_hour, borrowTimeInfo->tm_min, borrowTimeInfo->tm_sec);
			printf("**********************************\n");
			p = p->next;
		}
	}
}
//��ʼ��������ʷ������ǰ��
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
//��鳬ʱ
void checkOverdue(User* user) {
	if (user == NULL || user->borrowhistory == NULL) {
		printf("�޽��ļ�¼�������鳬ʱ�����\n");
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
			double fine = (diffTimeInDays - current->days) * 10;//ÿ��ʱ1�췣��10Ԫ
			printf("�û� %s ���ĵ�ͼ�顶%s���ѳ�ʱ %.0f �죬����ɷ��� %.0f Ԫ��\n",
				user->username, current->Book->title, diffTimeInDays - 30, fine);
		}

		current = current->next;
	}

	if (!hasOverdue) {
		printf("�û� %s �����н��ľ�δ��ʱ��\n", user->username);
	}
}
//����
void renewBook(User* user, char* isbn) {
	if (user == NULL || user->borrowhistory == NULL) {
		printf("�û��޽��ļ�¼���޷����衣\n");
		return;
	}

	StackNode* current = user->borrowhistory;
	int redays = 0;
	while (current != NULL) {
		if (strcmp(current->Book->ISBN, isbn) == 0) {
			printf("��������Ҫ���õ�����:\n");
			scanf("%d", &redays);
			current->days += redays;
			printf("ͼ�顶%s������ɹ���\n", current->Book->title);
			return;
		}
		current = current->next;
	}

	printf("δ�ҵ���ӦISBN�Ľ��ļ�¼���޷����衣\n");
}
//���������û�������ʷ��������ɾʱ���£�
void saveAllBorrowHistory(User* userList)
{
	allborrowlist = NULL;
	User* currentUser = userList->next;
	while (currentUser != NULL) {
		StackNode* currentBorrow = currentUser->borrowhistory;
		while (currentBorrow != NULL) {
			StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
			if (newNode == NULL) {
				printf("�ڴ����ʧ�ܣ��޷����������ʷ!\n");
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
//����������Ϣ
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
			fprintf(fp, "����ʱ��: %04d-%02d-%02d %02d:%02d:%02d\n", borrowtimeinfo->tm_year + 1900, borrowtimeinfo->tm_mon + 1,
				borrowtimeinfo->tm_mday, borrowtimeinfo->tm_hour, borrowtimeinfo->tm_min, borrowtimeinfo->tm_sec);
			fprintf(fp, "\n");
			p = p->next;
		}
	}
	printf("���н�����Ϣ�ѱ���\n");
	fclose(fp);
}
//���������ʷ
void loadAllBorrowHistoryFromFile(User* userList) {
	FILE* file = fopen("./Borrowinformation.txt", "r");
	if (file == NULL) {
		perror("�޷����ļ�");
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
			perror("�ڴ����ʧ��");
			fclose(file);
			return;
		}

		if (fscanf(file, "%s\n%s\n%s\n%s\n%d\n����ʱ��:%d-%d-%d %d:%d:%d\n",
			username, title, author, ISBN, &days, &year, &month, &day, &hour, &minute, &second) == 11) {

			while (targetUser != NULL && strcmp(targetUser->username, username) != 0) {
				targetUser = targetUser->next;
			}

			if (targetUser != NULL) {
				Book* tempBook = (Book*)malloc(sizeof(Book));
				if (tempBook == NULL) {
					perror("�ڴ����ʧ��");
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
		perror("�޷����ļ�");
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
			perror("�ڴ����ʧ��");
			fclose(file);
			return;
		}

		if (fscanf(file, "%s\n%s\n%s\n%s\n%d\n����ʱ��:%d-%d-%d %d:%d:%d\n",
			username, title, author, ISBN, &days, &year, &month, &day, &hour, &minute, &second) == 11) {

			while (targetUser != NULL && strcmp(targetUser->username, username) != 0) {
				targetUser = targetUser->next;
			}

			if (targetUser != NULL) {
				Book* tempBook = (Book*)malloc(sizeof(Book));
				if (tempBook == NULL) {
					perror("�ڴ����ʧ��");
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
// �����µ�ͼ��ڵ�*
Book* createbookNode(char* ISBN, char* title, char* author) {
	Book* newbook = (Book*)malloc(sizeof(Book));
	newbook->next = NULL;
	strcpy(newbook->title, title);
	strcpy(newbook->ISBN, ISBN);
	strcpy(newbook->author, author);
	newbook->bnum = 0;   //ͼ�鱻���ĵ��ܴ���
	return newbook;
}
// �����µ����ڵ㣨����ڵ㣩
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
// �ڷ���ڵ������ͼ��*
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
// ��������� ���� �ڵ㣨��Ϊĳ�����ڵ���ӽڵ㣩
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
//�������ͼ��
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
//���ļ��ж���ͼ��������
void loadbookcategory(TreeNode* root)
{
	FILE* fp = fopen("C://Users//��Ʒ��//Desktop//Library//bookcategory.txt", "r");
	char lcategory[100];
	while (!feof(fp))
	{
		fscanf(fp, "%s", lcategory);
		addCategory(root, lcategory);
	}
	fclose(fp);
}
//���ļ��ж���ͼ�����
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
//��ͼ��������д���ļ�
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
	printf("��Ϣ�����ɹ�\n");

}
//��ͼ�����д���ļ�
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
//  vertex���� arc�� vertexNum������ arc����
//  ��ʼ���������֮��ľ���
void InitMGraph(int* vertex, int arc[][MAX_VERTEX], int vertexNum, int arcNum)
{
	int  x;
	for (int i = 0; i < vertexNum; i++) { //�������鸳ֵ��
		vertex[i] = i;
	}
	for (int i = 0; i < vertexNum; i++) //��ʼ���ڽӾ���
		for (int j = 0; j < vertexNum; j++) {
			if (i == j)
				arc[i][j] = 0;
			else
				arc[i][j] = INT_MAX_;//��ֵ�����
		}
	arc[0][1] = 3;
	arc[1][0] = 3;
	//printf("ǰ̨����ʷ���鼮����Ϊ3\n");
	arc[0][2] = 2;
	arc[2][0] = 2;
	//printf("ǰ̨����ѧ���鼮����Ϊ2\n");
	arc[0][3] = 2;
	arc[3][0] = 2;
	//printf("ǰ̨���������鼮����Ϊ2\n");
	arc[1][2] = 1;
	arc[2][1] = 1;
	//printf("��ʷ����ѧ���鼮����Ϊ1\n");
	arc[1][3] = 2;
	arc[3][1] = 2;
	//printf("��ʷ���������鼮����Ϊ2\n");
}

//��dist�в���s[i]Ϊ0����СֵԪ��
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
//�������·����Ӧ��dist��path
void Dijkstra(int arc[][MAX_VERTEX], int vertexNum, int startV, int* s, int* dist, int* path) {
	for (int i = 0; i < vertexNum; i++) {
		dist[i] = arc[startV][i]; //��ʼ������dist,path��
		if (dist[i] != INT_MAX_)
			path[i] = startV; //��ԭ����Ϊ��һ��·����
		else
			path[i] = -1;
	}
	for (int i = 0; i < vertexNum; i++) {
		s[i] = 0;
	}
	//��֤���������Ƿ���ȷ
	s[startV] = 1; //1ֵ����ý���Ѿ����������·����
	int num = 1;
	int min;
	while (num < vertexNum) { //��������numС��ͼ�Ķ����������������ж��������С·����
		min = findMinDist(dist, s, vertexNum); //dist�в���s[i]Ϊ0����СֵԪ�أ�
		s[min] = 1;
		//�������ɵĽ����뼯��s��
		for (int i = 0; i < vertexNum; i++) {
			//�޸�����dist��path
			if ((s[i] == 0) && (dist[i] > dist[min] + arc[min][i])) {
				dist[i] = dist[min] + arc[min][i]; //���Ѿ��ҵ������·���޸Ķ�Ӧ��dist��
				path[i] = min; //���Ѿ��ҵ������·���޸Ķ�Ӧ��path��
				printf("\n");
			}
		}
		num++;
	}
}
//������·��
void displayPath(int* dist, int* path, int* s, int startV, int reachV, int vertexNum, char di[][20]) {
	int temp;
	int patharr[10];
	int con = 0;
	int t;
	for (int i = 0; i < vertexNum; i++) {
		con = 0;
		if (i == reachV) {
			printf("��%d.%s --> %d.%s:\n", startV, di[startV], i, di[i]);
			if (dist[i] != INT_MAX_)
				printf("���·������Ϊ:%d\n", dist[i]);
			else {
				printf("��������õ�֮���·����\n");

				printf("\n");
				continue;
			}
			printf("���·��Ϊ:");
			temp = i;
			while (temp != startV)
			{ //�øĳ�����ģ�
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
//����ͼ��ݲ���ͼ
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
	settextstyle(50, 0, _T("����"));
	settextcolor(BLACK);
	outtextxy(150, 50, _T("ͼ���·��"));
	settextstyle(32, 0, _T("����"));
	outtextxy(202, 202, _T("��ʷ��"));
	outtextxy(52, 302, _T("��ѧ��"));
	outtextxy(405, 355, _T("����"));
	outtextxy(225, 505, _T("ǰ̨"));
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
	//���ͼƬ
	getchar();
}
//����ͼ��
 void addBook(TreeNode* root,Book* head)
{
	Book* newBook = (Book*)malloc(sizeof(Book));
	char newISBN[20];
	char newtitle[100];
	char newauthor[100];
	int num;
	int bnum;
	printf("������ͼ��������Ϣ��\n");
	printf("����������ͼ������\n");
	for (int i = 0; i < 10 && root->children[i] != NULL; i++)
	{
		printf("%d.%s\n", i, root->children[i]->category);
	}
NEXT:
	int op = -1;
	scanf("%d", &op);
	if (root->children[op] == NULL)
	{
		printf("��Чѡ������������\n");
		goto NEXT;
	}
	printf("������ͼ���ISBN��\n");
	scanf("%s", newISBN);
	printf("������ͼ������ƣ�\n");
	scanf("%s", newtitle);
	printf("������ͼ������ߣ�\n");
	scanf("%s", newauthor);
	printf("������ͼ���������\n");
	scanf("%d", &num);
	printf("������ͼ����ܽ�������\n");
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
 //�Ƽ�ͼ��
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
	 printf("Ϊ�������Ƽ������У�\n");
	 display(node->children[max]);
 }
 //����ͼ��
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
 //topͼ��
 void toptushu(char name[][50], int topnum[])
 {
	 char str[60];
	 int  i;
	 int num = 5000, w = 45, h = 100;
	 initgraph(800, 800, EX_SHOWCONSOLE);
	 setbkcolor(WHITE);
	 cleardevice();
	 settextcolor(BLACK);
	 settextstyle(50, 0, _T("����"));
	 outtextxy(250, 20, _T("����ͼ��TOP3"));
	 settextstyle(20, 0, _T("����"));
	 setlinecolor(BLACK);
	 setlinestyle(PS_SOLID, 4);
	 line(100, 100, 100, 700);
	 line(100, 700, 700, 700);
	 outtextxy(75, 100, _T("��"));
	 outtextxy(75, 124, _T("��"));
	 outtextxy(75, 148, _T("��"));
	 outtextxy(75, 172, _T("��"));
	 outtextxy(15, 194, _T("����λ��"));
	 outtextxy(15, 220, _T("�Σ�"));
	 outtextxy(680, 710, _T("����"));
	 int x1;
	 for (i = 1; i <= 3; i++)
	 {
		 settextstyle(24, 0, _T("����"));
		 char str[50] = "";
		 sprintf(str, "%d", topnum[i - 1]);
		 x1 = 140 + 200 * (i - 1);
		 outtextxy(x1 - 20, 702, name[i - 1]);
		 if (topnum[i - 1] <= 600)
		 {
			 rectangle(x1, 700 - topnum[i - 1], x1 + 120, 700);
			 settextstyle(30, 0, _T("����"));
			 outtextxy(x1 + 20, 700 - topnum[i - 1] - 40, str);
		 }
		 else
		 {
			 rectangle(x1, 100, x1 + 120, 700);
			 settextstyle(30, 0, _T("����"));
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
 //��Ծ�û�
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
 //top�û�
 void topuser(char name[][50], int topnum[])
 {
	 char str[60];
	 int  i;
	 int num = 5000, w = 45, h = 100;
	 initgraph(800, 800, EX_SHOWCONSOLE);
	 setbkcolor(WHITE);
	 cleardevice();
	 settextcolor(BLACK);
	 settextstyle(50, 0, _T("����"));
	 outtextxy(180, 30, _T("��Ծ�û�TOP3"));
	 settextstyle(20, 0, _T("����"));
	 setlinecolor(BLACK);
	 setlinestyle(PS_SOLID, 4);
	 line(100, 100, 100, 700);
	 line(100, 700, 700, 700);//����������
	 outtextxy(70, 100, _T("��"));
	 outtextxy(70, 124, _T("��"));
	 outtextxy(70, 148, _T("��"));
	 outtextxy(70, 172, _T("��"));
	 outtextxy(18, 196, _T("����λ��"));
	 outtextxy(20, 220, _T("�Σ�"));
	 int x1;
	 for (i = 1; i <= 3; i++)
	 {
		 settextstyle(24, 0, _T("����"));
		 char str[50] = "";
		 sprintf(str, "%d", topnum[i - 1]);
		 x1 = 140 + 200 * (i - 1);
		 outtextxy(x1 - 20, 702, name[i - 1]);
		 if (topnum[i - 1] <= 600)
		 {
			 rectangle(x1, 700 - topnum[i - 1], x1 + 120, 700);
			 settextstyle(30, 0, _T("����"));
			 outtextxy(x1 + 20, 700 - topnum[i - 1] - 40, str);
		 }
		 else
		 {
			 rectangle(x1, 100, x1 + 120, 700);
			 settextstyle(30, 0, _T("����"));
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
//��ӡ*��
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
//�û�����ע�����
int userface()
{
	system("cls");
	//�û�����ע�����
	int op1 = 0;
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = 21;  // ��
	pos.Y = 3;  // ��
	printframe();
	SetConsoleCursorPosition(hConsole, pos);
	printf("ͼ��ݹ���ϵͳ\n");

	pos.X = 21;
	pos.Y = 5;
	SetConsoleCursorPosition(hConsole, pos);
	printf("1.����");
	pos.X = 21;
	pos.Y = 6;
	SetConsoleCursorPosition(hConsole, pos);
	printf("2.ע��");
	pos.X = 20;
	pos.Y = 7;
	SetConsoleCursorPosition(hConsole, pos);
	printf("-1.�˳�ϵͳ");
	pos.X = 21;
	pos.Y = 9;
	SetConsoleCursorPosition(hConsole, pos);
	printf("��������Ҫ���еĲ���");
	scanf("%d", &op1);
	return op1;
	getchar();
}
//����Ա����
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
	printf("�ܹ����б�");
	pos.X = 2;
	pos.Y = 3;
	SetConsoleCursorPosition(hConsole, pos);
	printf("-1.�˳�                       0.ͼ�鵼��");
	pos.Y = 4;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 1.����ͼ��                   2.ɾ��ͼ��");
	pos.Y = 5;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 3.����ͼ����Ϣ               4.����ͼ��");
	pos.Y = 6;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 5.�г�����ͼ����Ϣ           6.����ISBN����");
	pos.Y = 7;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 7.����ͼ����Ϣ               8.����ͼ����Ϣ");
	pos.Y = 8;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 9.��������                  10.�������");
	pos.Y = 9;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 11.��ӡ��ǰ�û�������ʷ     12.�黹ͼ��");
	pos.Y = 10;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 13.��ӡ�����û�������ʷ     14.�޸��û���Ϣ");
	pos.Y = 11;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 15.����������ʷ             16.���������ʷ");
	pos.Y = 12;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 17.��鳬ʱ                 18.����ͼ��");
	pos.Y = 13;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 19.����ͼ�����             20.�Ƽ�ͼ��");
	pos.Y = 14;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 21.�鿴����ͼ��             22.չʾ��Ծ�û�");
	pos.Y = 15;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" ����������룺");
	scanf("%d", &op1);
	return op1;

}
//��ͨ�û�����
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
	printf("�ܹ����б�");
	pos.X = 2;
	pos.Y = 3;
	SetConsoleCursorPosition(hConsole, pos);
	printf("-1.�˳�                       0.ͼ�鵼��");
	pos.Y = 4;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 1.����ͼ��                   2.�г�����ͼ����Ϣ");
	pos.Y = 5;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 3.��������                   4.��ӡ��ǰ�û�������ʷ");
	pos.Y = 6;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 5.�黹ͼ��                   6.�޸�����");
	pos.Y = 7;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 7.��鳬ʱ                   8.����ͼ��");
	pos.Y = 8;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 9.�Ƽ�ͼ��                   10.�鿴����ͼ��");
	pos.Y = 9;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" 11.չʾ��Ծ�û�top3     ");
	pos.Y = 15;
	SetConsoleCursorPosition(hConsole, pos);
	printf(" ����������룺");
	scanf("%d", &op2);
	return op2;

}
//�������
User* loginface(User* head) 
{
	printframe();
	User * flag = login(head);
	return flag;
	
}
//ע�����
void registerface(User* head)
{
	system("cls");
	printframe();
	registerUser(head);
	system("cls");
	printframe();
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	COORD pos;
	pos.X = 21;  // ��
	pos.Y = 3;  // ��
	SetConsoleCursorPosition(hConsole, pos);
	printf("ע��ɹ�");

}

int main()
{
	Book* bhead;
	BookInit(bhead);
	User* uhead;
	UserInit(uhead);
	TreeNode* root = createTreeNode("������");
	User* tuser = NULL;//��ǰ������û���

	Queue* borrowqueue = createQueue();
	int  vertex[10];//�������еĶ��㣻
	int arc[10][10];//�ڽӾ��󣬽������ͨ��ϵ��
	int vertexNum = 4, arcNum = 10; //����������ߵĸ�����
	InitMGraph(vertex, arc, vertexNum, arcNum);
	char di[10][20] = { "ǰ̨" ,"��ʷ��","��ѧ��", "������" };

	//��ʼ������
	Initborrowhistory(uhead);
	char path[1000] = "./Bookinformation.txt";
	fileread(bhead, path);
	loaduser(uhead);
	loadbookcategory(root);
	loadbook(root);
	Initborrowhistory(uhead);
	loadAllBorrowHistoryFromFile(uhead);


	int op1 = -2;
	//goto����
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
			printf("����ɹ�");
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
			printf("����ʧ�ܣ������³���");
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
			printf("*      0. ͼ���ǰ̨              *\n");
			printf("*      1. ��ʷ��������ڵ�        *\n");
			printf("*      2. ��ѧ��������ڵ�        *\n");
			printf("*      3. ������������ڵ�        *\n");
			printf("*      4. ����4�鿴ͼ��ݲ���     *\n");
			printf("*      5. ����-1�˳�ͼ�鵼��      *\n");
			printf("***********************************\n");
			while (1)
			{
				int x, y;
				printf("�������㽫���еĲ���������λ��(���):");
				scanf("%d", &x);
				if (x == -1)
				{
					printf("�˳�ͼ��ݵ��� \n");
					printf("***********************************\n");
					printf("*      0. ͼ���ǰ̨              *\n");
					printf("*      1. ��ʷ��������ڵ�        *\n");
					printf("*      2. ��ѧ��������ڵ�        *\n");
					printf("*      3. ������������ڵ�        *\n");
					printf("*      4. ����4�鿴ͼ��ݲ���     *\n");
					printf("*      5. ����-1�˳�ͼ�鵼��      *\n");
					printf("***********************************\n");
					break;
				}
				else if (x == 4)
				{
					printftu();
					continue;
				}
				printf("�������㽫��Ҫǰ��ͼ��ݵ�λ��(���):");
				scanf("%d", &y);
				int s[10], dist[10], path[10];
				//s[i]���Դ��ͼ������ɵ��յ㣬���̬Ϊֻ��һ��Դ��x; 
				//ÿ������dist��ʾ��ǰ���ҵ��Ĵ�ʼ��x���յ�y�����·���ĳ��ȣ�
				//path[i]��һ���ַ�������ʾ��ǰ���ҵ��Ĵ�ʼ��x���յ�y�����·����
				Dijkstra(arc, vertexNum, x, s, dist, path);
				printf("��%d.%s ��%d.%s�����·��Ϊ:\n", x, di[x], y, di[y]);
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
			printf("������Ҫɾ����ͼ���ISBN��:");
			char isbn[20];
			scanf("%s", isbn);
			deleteBook(bhead, isbn);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 3)
		{
			system("cls");
			printf("������Ҫɾ����ͼ���ISBN��:");
			char isbn[20];
			scanf("%s", isbn);
			modifyBook(bhead, isbn);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 4)
		{
			system("cls");
			printf("������Ҫ���ҵ�ͼ���ISBN��:");
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
			printf("������Ҫ���ļ���·��:");
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
			fgets(path, sizeof(path), stdin);
			printf("·��Ϊ%s\n", path);
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
			printf("������Ҫ�����ļ���·��:");
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
			fgets(path, sizeof(path), stdin);
			printf("·��Ϊ%s\n", path);
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
			printf("��������Ҫ����ͼ���ISBN��:");
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
				printf("���ѽ�����飬����������ת�����蹦��\n");
				system("pause");
				goto gotoadministratorface;
			}
			Book* requestBook = searchBook(bhead, requestisbn);
			if (requestBook != NULL)
			{
				printf("��������Ҫ���õ�����:");
				scanf("%d", &requestday);
				enqueue(borrowqueue, tuser, requestBook, requestday);
				printf("����ɹ�\n");
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
			printf("��������Ҫ�黹��ͼ��ISBN\n");
			char tisbn[20];
			scanf("%s", tisbn);
			StackNode* re;//�ж��Ƿ��ڽ�����ʷ;
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
				printf("��δ���Ĺ�����\n");
				system("pause");
				goto gotoadministratorface;
			}
			Book* rbook = searchBook(bhead, tisbn);
			int rflag = 0;
			rflag = returnbook(tuser->borrowhistory, rbook);
			if (rflag)
				tuser->borrowhistory = NULL;
			printf("�黹�ɹ�\n");
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
		printf("������Ҫ�����û���Ϣ���û���:");
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
			printf("δ�ҵ����û�,������\n");
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
			printf("������Ҫ�����ļ���·��:");
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
			fgets(path, sizeof(path), stdin);
			printf("·��Ϊ%s\n", path);
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
			printf("������Ҫ�����ļ���·��:");
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
			fgets(path, sizeof(path), stdin);
			printf("·��Ϊ%s\n", path);
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
			printf("��������Ҫ����ͼ���ISBN\n");
			scanf("%s", reisbn);
			renewBook(tuser, reisbn);
			system("pause");
			goto gotoadministratorface;
		}
		else if (op == 19)
		{
			system("cls");
			char category[100];
			printf("����������ͼ�������\n");
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
			printf("*      0. ͼ���ǰ̨              *\n");
			printf("*      1. ��ʷ��������ڵ�        *\n");
			printf("*      2. ��ѧ��������ڵ�        *\n");
			printf("*      3. ������������ڵ�        *\n");
			printf("*      4. ����4�鿴ͼ��ݲ���     *\n");
			printf("*      5. ����-1�˳�ͼ�鵼��      *\n");
			printf("***********************************\n");
			while (1)
			{
				int x, y;
				printf("�������㽫���еĲ���������λ��(���):");
				scanf("%d", &x);
				if (x == -1)
				{
					printf("�˳�ͼ��ݵ��� \n");
					printf("***********************************\n");
					printf("*      0. ͼ���ǰ̨              *\n");
					printf("*      1. ��ʷ��������ڵ�        *\n");
					printf("*      2. ��ѧ��������ڵ�        *\n");
					printf("*      3. ������������ڵ�        *\n");
					printf("*      4. ����4�鿴ͼ��ݲ���     *\n");
					printf("*      5. ����-1�˳�ͼ�鵼��      *\n");
					printf("***********************************\n");
					break;
				}
				else if (x == 4)
				{
					printftu();
					continue;
				}
				printf("�������㽫��Ҫǰ��ͼ��ݵ�λ��(���):");
				scanf("%d", &y);
				int s[10], dist[10], path[10];
				//s[i]���Դ��ͼ������ɵ��յ㣬���̬Ϊֻ��һ��Դ��x; 
				//ÿ������dist��ʾ��ǰ���ҵ��Ĵ�ʼ��x���յ�y�����·���ĳ��ȣ�
				//path[i]��һ���ַ�������ʾ��ǰ���ҵ��Ĵ�ʼ��x���յ�y�����·����
				Dijkstra(arc, vertexNum, x, s, dist, path);
				printf("��%d.%s ��%d.%s�����·��Ϊ:\n", x, di[x], y, di[y]);
				printf("\n");
				displayPath(dist, path, s, x, y, vertexNum, di);
				system("pause");

			}
			goto gotoregularuserface;
		}
		else if (op == 1)
		{
			system("cls");
			printf("������Ҫ���ҵ�ͼ���ISBN��:");
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
			printf("��������Ҫ����ͼ���ISBN��:");
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
				printf("���ѽ�����飬����������ת�����蹦��\n");
				system("pause");
				goto gotoregularuserface;
			}
			Book* requestBook = searchBook(bhead, requestisbn);
			if (requestBook != NULL)
			{
				printf("��������Ҫ���õ�����:");
				scanf("%d", &requestday);
				enqueue(borrowqueue, tuser, requestBook, requestday);
				printf("����ɹ�\n");
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
			printf("��������Ҫ�黹��ͼ��ISBN\n");
			char tisbn[20];
			scanf("%s", tisbn);
			StackNode* re;//�ж��Ƿ��ڽ�����ʷ;
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
				printf("��δ���Ĺ�����\n");
				system("pause");
				goto gotoregularuserface;
			}
			Book* rbook = searchBook(bhead, tisbn);
			int rflag = 0;
			rflag = returnbook(tuser->borrowhistory, rbook);
			if (rflag)
				tuser->borrowhistory = NULL;
			printf("�黹�ɹ�\n");
			saveAllBorrowHistory(uhead);
			system("pause");
			goto gotoregularuserface;
		}
		else if (op == 6)
		{
			system("cls");
			char tpassword[50];
			printf("�������޸ĺ������:");
			scanf("%s", tpassword);
			strcpy(tuser->password, tpassword);
			printf("�޸ĳɹ�\n");
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
			printf("��������Ҫ����ͼ���ISBN\n");
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

	


