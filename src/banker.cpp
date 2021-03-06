
/**
 * 银行家算法
 */
#define RESOURCE_QUAN 3
#define PROCESS_QUAN 5
#include <iostream>
#include <iomanip>
using namespace std;


// 为了测试方便, 使用 直接设置已分配和剩余, 初始化计算最大
/**
 * 进程所需最大资源
 */
int processMax[PROCESS_QUAN][RESOURCE_QUAN] = {

};

/**
 * 进程已分配资源
 */
int processAllocation[PROCESS_QUAN][RESOURCE_QUAN] = {
		{ 1, 0, 0, },
		{ 3, 2, 0, },
		{ 0, 1, 0, },
		{ 0, 5, 3, },
		{ 0, 0, 1, },
};

/**
 * 进程所需剩余资源
 */
int processNeed[PROCESS_QUAN][RESOURCE_QUAN] = {
		{ 5, 7, 5, },
		{ 4, 2, 0, },
		{ 1, 8, 2, },
		{ 0, 6, 5, },
		{ 3, 4, 2, },
};


/**
 * 系统可用资源
 */
int systemAvaiable[RESOURCE_QUAN] = { 5, 10, 7, };

void printData(bool = false);
void initTable();
void requestResource(int pid, int request[]);
void runTest();

int main()
{
	initTable();
	printData(true);
//	int pid;
//	int request[RESOURCE_QUAN] = {};
//	cout<<"输入进程id: ";
//	cin>>pid;
//
//	for (int i = 0; i < RESOURCE_QUAN; i ++) {
//		cout<<"输入资源"<<i<<":";
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

void print2d(const string title, int data[][RESOURCE_QUAN]) {
	cout<<title<<endl;
	for (int i = 0; i < PROCESS_QUAN; i ++) {
		cout<<"\t{ ";
		for (int j = 0; j < RESOURCE_QUAN; j ++) {
			cout<<setw(2)<<data[i][j]<<",";
		}
		cout<<" }"<<endl;
	}
}

void print1d(const string title, int data[]) {
	cout<<title<<" = { ";
	for (int j = 0; j < RESOURCE_QUAN; j ++) {
		cout<<setw(2)<<data[j]<<",";
	}
	cout<<" }"<<endl;
}
void printData(bool printMax)
{
	if (printMax) {
		print2d("进程所需最大资源", processMax);
	}
	print2d("进程已分配资源", processAllocation);
	print2d("进程所需剩余资源", processNeed);

	print1d("系统可用资源", systemAvaiable);
}

/**
 * 检查请求是否超出进程所需
 */
bool checkProcessNeed(int pid, int request[])
{
	for (int i = 0; i < RESOURCE_QUAN; i ++) {
		if (request[i] > processNeed[pid][i]) {
			cout<<"\t超出进程需求, res="<<i<<" "<<request[i]<<" > "<<processNeed[pid][i]<<endl;
			return false;
		}
	}
	return true;
}

bool checkSystemAvaiable(int request[])
{
	for (int i = 0; i < RESOURCE_QUAN; i ++) {
		if (request[i] > systemAvaiable[i]) {
			cout<<"\t超出系统剩余, res="<<i<<" "<<request[i]<<" > "<<systemAvaiable[i]<<endl;
			return false;
		}
	}
	return true;
}



void preRequest(int pid, int request[])
{
	cout<<"预处理请求"<<endl;

	print1d("\t请求\t", request);
	print1d("\t旧已分配\t", processAllocation[pid]);
	print1d("\t旧还需\t", processNeed[pid]);
	print1d("\t旧系可用\t", systemAvaiable);

	for (int i = 0; i < RESOURCE_QUAN; i ++) {
		// 已分配资源增加
		processAllocation[pid][i] += request[i];
		// 还需资源减少
		processNeed[pid][i] -= request[i];
		// 系统剩余资源减少
		systemAvaiable[i] -= request[i];
	}

	print1d("\tNEW已分配\t", processAllocation[pid]);
	print1d("\tNEW还需\t", processNeed[pid]);
	print1d("\tNEW系可用\t", systemAvaiable);

	cout<<"预处理结束"<<endl;
}
/**
 * 临时回收已分配资源
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
	cout<<"安全检查"<<endl;
	// 可用资源
	int tmpAvailable[RESOURCE_QUAN] = {};
	for (int i = 0; i < RESOURCE_QUAN; i ++) {
		tmpAvailable[i] = systemAvaiable[i];
	}
	// 已进安全序列的进程
	bool safeProcess[PROCESS_QUAN] = {};
	print1d("安全检查可用资源", tmpAvailable);
	start:
	for (int pid = 0; pid < PROCESS_QUAN; pid ++) {
		// 找未进安全序列的进程
		if (! safeProcess[pid]) {
			// 所需小于可用资源, 即为安全
			if (processNeedIsSafe(processNeed[pid], tmpAvailable)) {
				cout<<"pid="<<pid<<", 添加到安全序列"<<endl;
				safeProcess[pid] = true;
				// 回收此进程的资源
				print1d("\t回收前可用\t", tmpAvailable);
				print1d("\t回收前已分\t", processAllocation[pid]);
				revokeSafeAvailable(processAllocation[pid], tmpAvailable);
				print1d("\t回收后可用\t", tmpAvailable);
				goto start;
			}
		}
	}

	for (int i = 0; i < PROCESS_QUAN; i ++) {
		// 此进程未进安全序列
		if (! safeProcess[i]) {
			return false;
		}
	}

	cout<<"安全检查通过"<<endl;
	return true;
}


void revokeRequest(int pid, int request[])
{
	cout<<"回退资源请求"<<endl;
	printData();

	for (int i = 0; i < RESOURCE_QUAN; i ++) {
		// 已分配资源
		processAllocation[pid][i] -= request[i];
		// 还需资源
		processNeed[pid][i] += request[i];
		// 系统剩余资源
		systemAvaiable[i] += request[i];
	}

	cout<<"回退资源结束"<<endl;
	printData();
}

void requestResource(int pid, int request[])
{
	cout<<"处理请求, pid="<<pid<<endl;
	print1d("请求资源", request);
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
