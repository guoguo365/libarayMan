#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// 1. 写界面
void menu() {
	printf("==================================================\n");
	printf("\t\t图书管理系统\n");
	printf("\t0.退出系统\n");
	printf("\t1.登记书籍\n");
	printf("\t2.浏览书籍\n");
	printf("\t3.借阅书籍\n");
	printf("\t4.归还书籍\n");
	printf("\t5.书籍排序\n");
	printf("\t6.删除书籍\n");
	printf("\t7.查找书籍\n");
	printf("==================================================\n");
	printf("\t请输入（0-7）：");

}

// 3. 数据设计

struct BookInfo {
	char sn[20];	// 书ISBN
	char name[20];	// 书名
	float price;	// 价格
	int num;		// 数量
};

struct Node
{
	struct BookInfo data;
	struct Node* next;
};

// 全局链表
struct Node* list = NULL;

// 创建表头：表头就是一个结构体变量
struct Node* createHead() {

	// 动态内存申请
	struct Node* headNode = (struct Node*)malloc(sizeof(struct Node));
	headNode->next = NULL;
	return headNode;
}

// 创建节点
struct Node* createNode(struct BookInfo data) {
	struct Node* newNode = (struct Node*)malloc(sizeof(struct Node));
	newNode->data = data;
	newNode->next = NULL;
	return newNode;
}

// 从头部插入
void insertNodeByHead(struct Node* headNode, struct BookInfo data) {

	// 动态分配内存，头节点
	struct Node* newNode = createNode(data);

	// 在头节点后插入节点
	newNode->next = headNode->next;
	headNode->next = newNode;
}

// 从尾部插入
void insertNodeByTail(struct Node* headNode, struct BookInfo data) {

	// 定义指针
	struct Node* pMove = headNode;

	// 找到尾部节点
	while (pMove->next != NULL)
	{
		pMove = pMove->next;
	}

	// 在尾部插入
	struct Node* newNode = createNode(data);
	pMove->next = newNode;

}

// 打印链表 
void printList(struct Node* headNode) {
	struct Node* pMove = headNode->next;
	printf("ISBN\t书名\t价格\t数量\n");
	while (pMove != NULL) {

		// 剥洋葱
		printf("%s\t%s\t%.1f\t%d\n", pMove->data.sn,pMove->data.name, pMove->data.price, pMove->data.num);
		pMove = pMove->next;
	}
}

// 删除
void deleteNodeByData(struct Node* headNode, char *isbn) {

	// 定义左节点
	struct Node* posLeftNode = headNode;

	// 定义删除节点
	struct Node* posNode = headNode->next;

	while (posNode != NULL && strcmp(posNode->data.sn, isbn)) {
		posLeftNode = posNode;
		posNode = posNode->next;
	}

	// 没有找到
	if (posNode == NULL) {
		return;
	}

	// 找到
	posLeftNode->next = posNode->next;
	free(posNode);
	posNode = NULL;

}

/*
文件写操作
@fileName:文件写入路径
@headNode:写入的链表
*/
void saveToFile(const char* fileName, struct Node* headNode) {

	// 获取文件指针
	FILE* fp = fopen(fileName, "w");

	struct Node* pMove = headNode->next;

	// 写入文件
	while (pMove != NULL)
	{
		fprintf(fp, "%s\t%s\t%.1f\t%d\n", pMove->data.sn, pMove->data.name, pMove->data.price, pMove->data.num);
		pMove = pMove->next;
	}


	// 关闭文件
	fclose(fp);

}

/*
文件读操作
@fileName:文件读路径
@headNode:文件读到哪个链表
*/
void readFromFile(const char* fileName, struct Node* headNode) {

	// 获取文件指针，第一次打开文件不存在
	FILE* fp = fopen(fileName, "r");

	// 文件不存在， 创建文件
	if (fp == NULL) {
		fp = fopen(fileName, "w+");
	}

	// 读取文件
	struct BookInfo bookInfoTemp;
	while (fscanf(fp, "%s\t%s\t%f\t%d\n", bookInfoTemp.sn, bookInfoTemp.name, &bookInfoTemp.price, &bookInfoTemp.num) != EOF)
	{
		insertNodeByHead(list, bookInfoTemp);
	}

	// 关闭文件
	fclose(fp);
}

/*
冒泡排序
*/
void bubbleSort(struct Node* headNode) {
	for (struct Node* p = headNode->next; p != NULL; p = p->next) {
		for (struct Node* q = headNode->next; q->next != NULL; q = q->next) {
			if (q->data.price > q->next->data.price) {

				// 交换
				struct BookInfo temp = q->data;
				q->data = q->next->data;
				q->next->data = temp;
			}
		}
	}
}

/*
查找
*/
struct Node* searchByName(struct Node* headNode, char* bookName) {

	// 定义查找节点
	struct Node* posNode = headNode;
	while (posNode != NULL && strcmp(posNode->data.name, bookName))
	{
		posNode = posNode->next;
	}
	return posNode;
}

// 2. 交互
void keyDown() {
	int userKey = 0;

	// 临时变量
	struct BookInfo bookTemp;
	struct Node* result;

	scanf("%d", &userKey);

	switch (userKey)
	{
	case 0:
		printf("【退出系统】\n");
		printf("退出成功\n");
		system("pause");
		exit(0);
		break;
	case 1:
		printf("【登记书籍】\n");
		printf("输入书籍信息（isbn name price num）：");
		scanf("%s%s%f%d",bookTemp.sn, bookTemp.name, &bookTemp.price, &bookTemp.num);
		insertNodeByHead(list, bookTemp);
		saveToFile("bookInfo.txt", list);
		break;
	case 2:
		printf("【浏览书籍】\n");
		printList(list);
		break;
	case 3:
		printf("【借阅书籍】\n");
		printf("请输入要借阅的书名：");
		scanf("%s", bookTemp.name);
		result = searchByName(list, bookTemp.name);
		if (result == NULL) {
			printf("您要借阅的书不存在！！！\n");
			break;
		}
		if (result->data.num < 1) {
			printf("您要借阅的书数量小于1不能借阅！！！\n");
			break;
		}
		result->data.num--;
		printf("借阅成功!\n");
		
		break;
	case 4:
		printf("【归还书籍】\n");
		break;
	case 5:
		printf("【排序】\n");
		bubbleSort(list);
		break;
	case 6:
		printf("【删除书籍】\n");
		printf("请输入图书的ISBN号：");
		scanf("%s", bookTemp.sn);
		deleteNodeByData(list, bookTemp.sn);
		saveToFile("bookInfo.txt", list);
		break;
	case 7:
		printf("【查找书籍】\n");
		printf("请输入要查找的书名：");
		scanf("%s", bookTemp.name);
		result = searchByName(list, bookTemp.name);
		if (result == NULL) {
			printf("未找到您要查找的书籍!");
		}
		else {
			printf("ISBN\t书名\t价格\t数量\n");
			printf("%s\t%s\t%f\t%d\n", result->data.sn, result->data.name, result->data.price, result->data.num);
		}
		break;
	default:
		printf("【输入错误】\n");
		break;
	}
}

int main() {

	list = createHead();

	// 读取文件内容
	readFromFile("bookInfo.txt", list);

	//struct Node* headNode = createHead();
	//for (int i = 0; i < 5; i++) {
	//	//insertNodeByHead(headNode, i);
	//	insertNodeByTail(headNode, i);
	//}
	//printf("打印节点：\n");
	//printList(headNode);
	//int delpos = 9;
	//printf("\n删除节点%d\n", delpos);
	//deleteNodeByData(headNode, delpos);
	//printf("打印节点\n");
	//printList(headNode);

	while (true)
	{
		menu();
		keyDown();
	}

	system("pause");
	return 0;
}