#include <iostream>

using namesapce
std;

template<int ObjectSize, int NumofObjects = 20>
class MemPool {
private:
		//空闲节点结构体
		struct FreeNode {
				FreeNode *pNext;
				char data[ObjectSize];
		};
		//内存块结构体
		struct MemBlock {
				MemBlock *pNext;
				FreeNode data[NumofObjects];
		};
		FreeNode *freeNodeHeader;
		MemBlock *memBlockHeader;
public:
		MemPool() {
			freeNodeHeader = nullptr;
			memBlockHeader = nullptr;
		}

		~MemPool() {
			MemBlock *ptr;
			while (memBlockHeader) {
				ptr = memBlockHeader->pNext;
				delete memBlockHeader;
				memBlockHeader = ptr;
			}
		}

		void *malloc()
		//分配空间的节点
		{
			//无空闲节点,申请新内存块
			if (nullptr == freeNodeHeader) {
				MemBlock *newBlock = new MemBlock;
				newBlock->next = nullptr;
				freeNodeHeader = &newBlock->data[0];
				for (int i = 1; i < NumofObjects; ++i) newBlock->data[i - 1].pNext = &newBlock->data[i];
				newBlock->data[NumofObjects - 1].pNext = nullptr;
				//首次申请内存块
				if (memBlockHeader == nullptr) memBlockHeader = newBlock;
				else {
					newBlock->pNext = memBlockheader;
					memBlockHeader = newBlock;
				}
			}
			//返回空闲节点链表的第一个节点
			void *freeNode = freeNodeHeader;
			freeNodeHeader = freeNodeHeader->pNext;
			return freeNode;
		}

		void free(void *p) {
			FreeNode *pNode = static_cast<FreeNode *>(p);
			pNode->pNext = freeNodeHeader;
			freeNodeHeader = pNode;
		}
};