#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#include <sys/syscall.h>


/** Convert hex code to ulong integer */
static ulong hex2ulong(char *code, char len) {
  char c;
  ulong v = 0;
  for(char i=0;i<len;i++) {
    c = code[i];
    v <<= 4;
    if(c>='0' && c<='9') {
      v += (c-'0');
    } else if (c>='A' && c<='F') {
      v += 10 + (c-'A');
    } else if (c>='a' && c<='f') {
      v += 10 + (c-'a');
    } else {
      return 0;
    }
  }
  return v;
}

int main(int argc, char **argv)
{
	FILE *pFile = NULL;
	void *paddr = NULL;

    int BUFFER_SIZE = 1024;
    int BLOCK_SIZE = 10240;


	char buffer[BUFFER_SIZE];
	char address[BUFFER_SIZE];
	char block[BLOCK_SIZE];

	char c;
	int i, ineedreset, iblocks, iSize;
	
	pFile = fopen("/proc/self/maps", "r");
	if (NULL == pFile)
	{
		printf("/proc/self/maps fopen failed, error!\r\n");
		return 1;
	}

	/* 查找vdso动态库映射内存位置  */
	i = 0;
	ineedreset = 1;
	memset(buffer, 0, BUFFER_SIZE);
	while(1) 
	{
		c = fgetc (pFile);
		if (c != EOF) 
		{
			printf("%c", c);
			if (c == '\r' || c == '\n')
			{
				i = 0;
				ineedreset = 1;
			} else
			{
				if (ineedreset)
				{
					if (NULL != strstr(buffer, "vdso"))
					{
						printf("I have got vdso section.\r\n");
						break;
					}
					memset(buffer, 0, BUFFER_SIZE);
					ineedreset = 0;
				}			
				buffer[i++] = c;
			}
		}else
		{
			break;
		}
    }

	printf("vsdo line is:%s\r\n", buffer);
	fclose(pFile);
	pFile = NULL;
	
	/* 获取起始地址 */
	memset(address, 0, BUFFER_SIZE);
	for (i = 0; buffer[i] != '-'; i++)
	{
		address[i] = buffer[i];
		if (buffer[i] == '-')
			break;
	}

	paddr = (void *)hex2ulong(address,4);
	printf("Current VDSO address is 0x%x\r\n", paddr);
	iblocks = (unsigned long)paddr / BLOCK_SIZE;
	printf("We have %d blocks before VDSO library\r\n", iblocks);
	printf("Ready to generate linux-gate.dso from block %d\r\n", iblocks);

	/* 导出vdso动态文件 */
	pFile = fopen("./linux-gate.dso", "w");
	if (NULL == pFile)
	{
		printf("fopen linux-gate.dso failed, exit!\r\n");
		return 1;
	}

	printf("Head:0x%x-%c-%c-%c\r\n", *((char *)paddr + 0),*((char *)paddr + 1),*((char *)paddr + 2),*((char *)paddr + 3));
        memcpy(block, paddr, BLOCK_SIZE);
	iSize = fwrite(block, 1, BLOCK_SIZE, pFile);
	if (BLOCK_SIZE != iSize)
	{
		perror("fwrite error:\r\n");
	}
	printf("copy %d/%d bytes from 0x%x to the file\r\n", iSize, BLOCK_SIZE, paddr);

	fclose(pFile);
	printf("Generate linux-gate.dso Done\r\n");

	return 0;
}