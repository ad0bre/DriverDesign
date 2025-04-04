#include <Windows.h>
#include <stdio.h>
#include <stdlib.h>

//allocate N pages of memory with VirtualAlloc

int main()
{
	SIZE_T pageSize;
	SYSTEM_INFO sysInfo;

	GetSystemInfo(&sysInfo);
	pageSize = sysInfo.dwPageSize;

	printf("Page size: %zu bytes\n", pageSize);

	int N;
	printf("Enter the number of pages to allocate: ");
	if (scanf_s("%d", &N) != 1 || N <= 0)
	{
		fprintf(stderr, "Invalid input. Please enter a positive integer.\n");
		return 1;
	}

	SIZE_T totalSize = N * pageSize;

	void* allocatedMemory = VirtualAlloc(NULL, totalSize, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE);

	if (allocatedMemory == NULL)
	{
		fprintf(stderr, "Memory allocation failed. Error: %lu\n", GetLastError());
		return 1;
	}

	printf("Allocated %d pages (%zu bytes) at address: %p\n", N, totalSize, allocatedMemory);

	if (VirtualFree(allocatedMemory, 0, MEM_RELEASE) == 0)
	{
		printf("Memory deallocation failed.\n");
		return 1;
	}

	printf("Memory deallocated successfully.\n");

	return 0;
}