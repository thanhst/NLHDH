#include <Windows.h>
#include <iostream>
#include <vector>
using namespace std;

#define NUM_THREADS 5

HANDLE displayMutex;

DWORD WINAPI ThreadFunc(LPVOID lpParam)
{
    intptr_t threadId = reinterpret_cast<intptr_t>(lpParam);
    WaitForSingleObject(displayMutex, INFINITE);
    cout << "Thread " << threadId << " is running." << endl;
    cout << "Counting numbers from 0 to " << threadId << ":" << endl;
    for (intptr_t i = 0; i <= threadId; i++)
    {
        cout << i << " ";
    }
    cout << endl;
    cout << "Thread " << threadId << " is exiting." << endl;
    ReleaseMutex(displayMutex);
    return 0;
}

int main()
{
    vector<HANDLE> threadHandles;
    vector<intptr_t> threadIds;
    displayMutex = CreateMutex(NULL, FALSE, NULL);
    for (intptr_t i = 1; i <= NUM_THREADS; i++)
    {
        threadIds.push_back(i);
        HANDLE hThread = CreateThread(NULL, 0, ThreadFunc, reinterpret_cast<LPVOID>(i), 0, NULL);
        if (hThread == NULL)
        {
            cout << "Failed to create thread " << i << endl;
            return 1;
        }
        threadHandles.push_back(hThread);
    }

    WaitForMultipleObjects(NUM_THREADS, &threadHandles[0],TRUE, INFINITE);
    for (vector<HANDLE>::iterator it = threadHandles.begin(); it != threadHandles.end(); it++)
    {
        CloseHandle(*it);
    }
	CloseHandle(displayMutex); 
    return 0;
}
