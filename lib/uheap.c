#include <inc/lib.h>

// malloc()
//	This function use FIRST FIT strategy to allocate space in heap
//  with the given size and return void pointer to the start of the allocated space

//	To do this, we need to switch to the kernel, allocate the required space
//	in Page File then switch back to the user again.
//
//	We can use sys_allocateMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls allocateMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the allocateMem function is empty, make sure to implement it.

//==================================================================================//
//============================ REQUIRED FUNCTIONS ==================================//
//==================================================================================//
void* bestFitStrategy(uint32 size) {
	min = num_pages + 1;
	size = ROUNDUP(size, PAGE_SIZE) / PAGE_SIZE;
	int numFreePages = 0;
	uint32 freeAddress, BestFreeAddress, isFristTime = 1, isEntr = 0;

	for (uint32 i = 0; i < num_pages; i++) {
		if (isUsed[i] == 0) {
			if (isFristTime == 1) {
				freeAddress = USER_HEAP_START + i * PAGE_SIZE;
				isFristTime = 0;
				numFreePages = 1;
			} else {
				numFreePages++;
			}
		} else {
			isFristTime = 1;
			if (numFreePages >= size) {
				isEntr = 1;
				if (numFreePages < min) {
					min = numFreePages;
					BestFreeAddress = freeAddress;
				}
			}
			numFreePages = 0;
		}

	}
	if (numFreePages >= size) {
		isEntr = 1;
		if (numFreePages < min) {
			min = numFreePages;
			BestFreeAddress = freeAddress;
		}
	}
	if (isEntr == 1)
		return (void*) BestFreeAddress;

	return NULL;
}


void* malloc(uint32 size)
{
	uint32* BestFreeAddress;
	BestFreeAddress = bestFitStrategy(size);
	if(BestFreeAddress == NULL)
		return NULL;

	sys_allocateMem((uint32)BestFreeAddress, size);

	pages[count].virtualAddress = (uint32)BestFreeAddress;
	pages[count].usedSpace = size;
    count++;

	uint32 begin = ((uint32) BestFreeAddress - USER_HEAP_START)/ PAGE_SIZE;
	uint32 sz = ROUNDUP(size, PAGE_SIZE)/PAGE_SIZE;

	for(int i=begin; i<sz+begin; i++) {
		isUsed[i]=1;
	}

	return (void*) BestFreeAddress;
}

// free():
//	This function frees the allocation of the given virtual_address
//	To do this, we need to switch to the kernel, free the pages AND "EMPTY" PAGE TABLES
//	from page file and main memory then switch back to the user again.
//
//	We can use sys_freeMem(uint32 virtual_address, uint32 size); which
//		switches to the kernel mode, calls freeMem(struct Env* e, uint32 virtual_address, uint32 size) in
//		"memory_manager.c", then switch back to the user mode here
//	the freeMem function is empty, make sure to implement it.

void free(void* virtual_address)
{
	//TODO: [PROJECT 2021 - [2] User Heap] free() [User Side]
	// Write your code here, remove the panic and write your code
	//you should get the size of the given allocation using its address
	uint32 size ;
	for (uint32 i = 0; i < num_pages; i++)
	{
		if (pages[i].virtualAddress == (uint32) virtual_address)
		{
			size = pages[i].usedSpace;
			sys_freeMem((uint32)virtual_address,pages[i].usedSpace);
			pages[i].virtualAddress= pages[count - 1].virtualAddress;
			pages[i].usedSpace= pages[count - 1].usedSpace;
			count--;


		}
	}

	        uint32 begin = ((uint32) virtual_address - USER_HEAP_START)/ PAGE_SIZE;
			uint32 sz = ROUNDUP(size, PAGE_SIZE)/PAGE_SIZE;

			for(int i=begin; i<sz+begin; i++) {
				isUsed[i]=0;
			}

	//refer to the project presentation and documentation for details
}


//==================================================================================//
//================================ OTHER FUNCTIONS =================================//
//==================================================================================//

void* smalloc(char *sharedVarName, uint32 size, uint8 isWritable)
{
	panic("this function is not required...!!");
	return 0;
}

void* sget(int32 ownerEnvID, char *sharedVarName)
{
	panic("this function is not required...!!");
	return 0;
}

void sfree(void* virtual_address)
{
	panic("this function is not required...!!");
}

void *realloc(void *virtual_address, uint32 new_size)
{
	panic("this function is not required...!!");
	return 0;
}

void expand(uint32 newSize)
{
	panic("this function is not required...!!");
}
void shrink(uint32 newSize)
{
	panic("this function is not required...!!");
}

void freeHeap(void* virtual_address)
{
	panic("this function is not required...!!");
}
