

/**
 * ���м��㷨
 */

#include <stdio.h>
#include <stdlib.h>

#define RESOURCE_QUAN 3
#define PROCESS_QUAN 5
#define true 1
#define false 0
#define bool int


// Ϊ�˲��Է���, ʹ�� ֱ�������ѷ����ʣ��, ��ʼ���������
/**
 * �������������Դ
 */
int processMax[PROCESS_QUAN][RESOURCE_QUAN] = {

};

/**
 * �����ѷ�����Դ
 */
int processAllocation[PROCESS_QUAN][RESOURCE_QUAN] = {
		{ 1, 0, 0, },
		{ 3, 2, 0, },
		{ 0, 1, 0, },
		{ 0, 5, 3, },
		{ 0, 0, 1, },
};

/**
 * ��������ʣ����Դ
 */
int processNeed[PROCESS_QUAN][RESOURCE_QUAN] = {
		{ 5, 7, 5, },
		{ 4, 2, 0, },
		{ 1, 8, 2, },
		{ 0, 6, 5, },
		{ 3, 4, 2, },
};


/**
 * ϵͳ������Դ
 */
int systemAvaiable[RESOURCE_QUAN] = { 5, 10, 7, };

void printTable(int);
void printData()
{
	printTable(0);
}
void printDataAll()
{
	printTable(1);
}

void initTable();
void requestResource(int pid, int request[]);
void runTest();

int main()
{
	initTable();
	printDataAll();
//	int pid;
//	int request[RESOURCE_QUAN] = {};
//	cout<<"�������id: ";
//	cin>>pid;
//
//	for (int i = 0; i < RESOURCE_QUAN; i ++) {
//		cout<<"������Դ"<<i<<":";
//		cin>>request[i];
//	}
//	requestResource(pid, request);

	runTest();

	return 0;
}


void runTest()
{
	int pid = 0;
	int request[] = { 1, 2, 5, };
//	int request[] = { 0, 0, 1, };
	requestResource(pid, request);
}
void initTable()
{
	for (int i = 0; i < PROCESS_QUAN; i ++) {
		for (int j = 0; j < RESOURCE_QUAN; j ++) {
			processMax[i][j] = processNeed[i][j] + processAllocation[i][j];
		}
	}
}

void print2d(const char* title, int data[][RESOURCE_QUAN]) {
	printf("%s\n", title);

	for (int i = 0; i < PROCESS_QUAN; i ++) {
		printf("\t{ ");
		for (int j = 0; j < RESOURCE_QUAN; j ++) {
			printf("%2d,", data[i][j]);
		}
		printf(" }\n");
	}
}

void print1d(const char* title, int data[]) {
	printf("%s = { ", title);

	for (int j = 0; j < RESOURCE_QUAN; j ++) {
		printf("%2d,", data[j]);
	}
	printf(" }\n");
}


void printTable(int printMax)
{
	if (printMax) {
		print2d("�������������Դ", processMax);
	}
	print2d("�����ѷ�����Դ", processAllocation);
	print2d("��������ʣ����Դ", processNeed);

	print1d("ϵͳ������Դ", systemAvaiable);
}

/**
 * ��������Ƿ񳬳���������
 */
int checkProcessNeed(int pid, int request[])
{
	for (int i = 0; i < RESOURCE_QUAN; i ++) {
		if (request[i] > processNeed[pid][i]) {
			printf("\t������������, res=%d %d > %d\n", i, request[i], processNeed[pid][i]);
			return false;
		}
	}
	return true;
}

bool checkSystemAvaiable(int request[])
{
	for (int i = 0; i < RESOURCE_QUAN; i ++) {
		if (request[i] > systemAvaiable[i]) {
			printf("\t����ϵͳʣ��, res=%d %d > %d\n", i, request[i], systemAvaiable[i]);
			return false;
		}
	}
	return true;
}



void preRequest(int pid, int request[])
{
	printf("Ԥ��������\n");

	print1d("\t����\t", request);
	print1d("\t���ѷ���\t", processAllocation[pid]);
	print1d("\t�ɻ���\t", processNeed[pid]);
	print1d("\t��ϵ����\t", systemAvaiable);

	for (int i = 0; i < RESOURCE_QUAN; i ++) {
		// �ѷ�����Դ����
		processAllocation[pid][i] += request[i];
		// ������Դ����
		processNeed[pid][i] -= request[i];
		// ϵͳʣ����Դ����
		systemAvaiable[i] -= request[i];
	}

	print1d("\tNEW�ѷ���\t", processAllocation[pid]);
	print1d("\tNEW����\t", processNeed[pid]);
	print1d("\tNEWϵ����\t", systemAvaiable);

	printf("Ԥ�������\n");
}
/**
 * ��ʱ�����ѷ�����Դ
 */
void revokeSafeAvailable(int processAllocation[], int available[])
{
	for (int i = 0; i < PROCESS_QUAN; i ++) {
		available[i] += processAllocation[i];
	}
}
bool processNeedIsSafe(int processNeed[], int available[])
{
	for (int i = 0; i < RESOURCE_QUAN; i ++) {
		if (processNeed[i] > available[i]) {
			return false;
		}
	}
	return true;
}

bool isSafe()
{
	printf("��ȫ���\n");
	// ������Դ
	int tmpAvailable[RESOURCE_QUAN] = {};
	for (int i = 0; i < RESOURCE_QUAN; i ++) {
		tmpAvailable[i] = systemAvaiable[i];
	}
	// �ѽ���ȫ���еĽ���
	bool safeProcess[PROCESS_QUAN] = {};
	print1d("��ȫ��������Դ", tmpAvailable);
	start:
	for (int pid = 0; pid < PROCESS_QUAN; pid ++) {
		// ��δ����ȫ���еĽ���
		if (! safeProcess[pid]) {
			// ����С�ڿ�����Դ, ��Ϊ��ȫ
			if (processNeedIsSafe(processNeed[pid], tmpAvailable)) {
				printf("pid=%d ��ӵ���ȫ����\n", pid);
				safeProcess[pid] = true;
				// ���մ˽��̵���Դ
				print1d("\t����ǰ����\t", tmpAvailable);
				print1d("\t����ǰ�ѷ�\t", processAllocation[pid]);
				revokeSafeAvailable(processAllocation[pid], tmpAvailable);
				print1d("\t���պ����\t", tmpAvailable);
				goto start;
			}
		}
	}

	for (int i = 0; i < PROCESS_QUAN; i ++) {
		// �˽���δ����ȫ����
		if (! safeProcess[i]) {
			return false;
		}
	}

	printf("��ȫ���ͨ��\n");
	return true;
}


void revokeRequest(int pid, int request[])
{
	printf("������Դ����\n");
	printData();

	for (int i = 0; i < RESOURCE_QUAN; i ++) {
		// �ѷ�����Դ
		processAllocation[pid][i] -= request[i];
		// ������Դ
		processNeed[pid][i] += request[i];
		// ϵͳʣ����Դ
		systemAvaiable[i] += request[i];
	}

	printf("������Դ����\n");
	printData();
}

void requestResource(int pid, int request[])
{
	printf("��������, pid=%d\n", pid);
	print1d("������Դ", request);
	if (! checkProcessNeed(pid, request)) {
		return;
	}
	if (! checkSystemAvaiable(request)) {
		return;
	}

	preRequest(pid, request);

	if (! isSafe()) {
		revokeRequest(pid, request);
	}

}
