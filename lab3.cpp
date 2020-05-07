#include "lab3.h"
#include <windows.h>
#include <stdio.h>

using namespace std;

#define THREADCOUNT 5

HANDLE Thread[THREADCOUNT];
HANDLE Mutex;
HANDLE Sem1, Sem2, Sem3, Sem4;

unsigned int lab3_thread_graph_id()
{
	return 4;
}

const char* lab3_unsynchronized_threads()
{
	return "bcde";
}

const char* lab3_sequential_threads()
{
	return "efgh";
}

DWORD WINAPI threads_unsynchronized(LPVOID text)
{
	for (int i = 0; i < 4; ++i)
	{
		WaitForSingleObject(Mutex, INFINITE);
		cout <<(char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
	}
	return 0;
}

DWORD WINAPI thread_e(LPVOID text)
{
	for (int i = 0; i < 4; ++i)
	{
		WaitForSingleObject(Sem1, INFINITE);
		WaitForSingleObject(Mutex, INFINITE);
		cout <<(char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
		ReleaseSemaphore(Sem2, 1, NULL);
	}
	return 0;
}

DWORD WINAPI thread_f(LPVOID text)
{
	for (int i = 0; i < 4; ++i)
	{
		WaitForSingleObject(Sem2, INFINITE);
		WaitForSingleObject(Mutex, INFINITE);
		cout <<(char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
		ReleaseSemaphore(Sem3, 1, NULL);
	}
	return 0;
}

DWORD WINAPI thread_g(LPVOID text)
{
	for (int i = 0; i < 4; ++i)
	{
		WaitForSingleObject(Sem3, INFINITE);
		WaitForSingleObject(Mutex, INFINITE);
		cout <<(char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
		ReleaseSemaphore(Sem4, 1, NULL);
	}
	return 0;
}

DWORD WINAPI thread_h(LPVOID text)
{
	for (int i = 0; i < 4; ++i)
	{
		WaitForSingleObject(Sem4, INFINITE);
		WaitForSingleObject(Mutex, INFINITE);
		cout << (char const*)text << flush;
		ReleaseMutex(Mutex);
		computation();
		ReleaseSemaphore(Sem1, 1, NULL);
	}
	return 0;
}

int lab3_init()
{
	DWORD ThreadID;

	Mutex = CreateMutex(NULL, FALSE, NULL);
	if (Mutex == NULL)
	{
		cout << "CreateMutex error " << GetLastError() << endl;
		return 1;
	}

	Sem1 = CreateSemaphore(NULL, 1, 1, NULL);
	if (Sem1 == NULL)
	{
		cout << "CreateSemaphore error: Sem1" << GetLastError() << endl;
		return 1;
	}

	Sem2 = CreateSemaphore(NULL, 0, 1, NULL);
	if (Sem1 == NULL)
	{
		cout << "CreateSemaphore error: Sem2 " << GetLastError() << endl;
		return 1;
	}

	Sem3 = CreateSemaphore(NULL, 0, 1, NULL);
	if (Sem1 == NULL)
	{
		cout << "CreateSemaphore error: Sem3" << GetLastError() << endl;
		return 1;
	}

	Sem4 = CreateSemaphore(NULL, 0, 1, NULL);
	if (Sem1 == NULL)
	{
		cout << "CreateSemaphore error: Sem4" << GetLastError() << endl;
		return 1;
	}


	int count = 0;

	char const* textsCADE[] = { "c", "a", "d", "e" };

	for (int i = 0; i < 4; ++i)
	{
		Thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threads_unsynchronized, (void*)textsCADE[i], 0, &ThreadID);
		if (Thread[i] == NULL)
		{
			cout << "CreateThread error: " << textsCADE[i] << GetLastError() << endl;
			return 1;
		}
		else ++count;
	}

	for (int i = 0; i < count; ++i) {
		WaitForSingleObject(Thread[i], INFINITE);
	}

	count = 0;

	char const* textsCBDE[] = { "c", "b", "d", "e" };

	for (int i = 0; i < 4; ++i)
	{
		Thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threads_unsynchronized, (void*)textsCBDE[i], 0, &ThreadID);
		if (Thread[i] == NULL)
		{
			cout << "CreateThread error: " << textsCBDE[i] << GetLastError() << endl;
			return 1;
		}
		else ++count;
	}

	for (int i = 0; i < count; ++i) {
		WaitForSingleObject(Thread[i], INFINITE);
	}


	count = 0;

	char const* textsHGDE[] = { "h", "g", "d", "e" };

	for (int i = 0; i < 4; ++i)
	{
		Thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threads_unsynchronized, (void*)textsHGDE[i], 0, &ThreadID);
		if (Thread[i] == NULL)
		{
			cout << "CreateThread error: " << textsHGDE[i] << GetLastError() << endl;
			return 1;
		}
		else ++count;
	}

	for (int i = 0; i < count; ++i) {
		WaitForSingleObject(Thread[i], INFINITE);
	}


	count = 0;

	char const* textsHFEG[] = { "e", "f", "g", "h" };

	Thread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_e, (void*)textsHFEG[0], 0, &ThreadID);
	if (Thread[0] == NULL)
	{
		cout << "CreateThread error: " << textsHGDE[0] << GetLastError() << endl;

		return 1;
	}
	Thread[1] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_f, (void*)textsHFEG[1], 0, &ThreadID);
	if (Thread[1] == NULL)
	{
		cout << "CreateThread error: " << textsHGDE[1] << GetLastError() << endl;
		return 1;
	}
	Thread[2] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_g, (void*)textsHFEG[2], 0, &ThreadID);
	if (Thread[2] == NULL)
	{
		cout << "CreateThread error: " << textsHGDE[2] << GetLastError() << endl;
		return 1;
	}

	Thread[3] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)thread_h, (void*)textsHFEG[3], 0, &ThreadID);
	if (Thread[3] == NULL)
	{
		cout << "CreateThread error: " << textsHGDE[3] << GetLastError() << endl;
		return 1;
	}


	for (int i = 0; i < 3; ++i) {
		WaitForSingleObject(Thread[i], INFINITE);
	}

	count = 0;

	char const* textsHIK[] = { "h", "i", "k"};

	for (int i = 0; i < 3; ++i)
	{
		Thread[i] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threads_unsynchronized, (void*)textsHIK[i], 0, &ThreadID);
		if (Thread[i] == NULL)
		{
			cout << "CreateThread error: " << textsHIK[i] << GetLastError() << endl;
			return 1;
		}
		else ++count;
	}

	for (int i = 0; i < count; ++i) {
		WaitForSingleObject(Thread[i], INFINITE);
	}


	char const* textsM = "m";
	Thread[0] = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)threads_unsynchronized, (void*)textsM, 0, &ThreadID);
	if (Thread[0] == NULL)
	{
		cout << "CreateThread error: " << textsM << GetLastError() << endl;
		return 1;
	}

	WaitForSingleObject(Thread[0], INFINITE);

	for (int i = 0; i < THREADCOUNT; ++i) {
		CloseHandle(Thread[i]);
	}

	CloseHandle(Sem1);
	CloseHandle(Sem2);
	CloseHandle(Sem3);
	CloseHandle(Sem4);
	CloseHandle(Mutex);

	cout << endl;

	return 0;
}