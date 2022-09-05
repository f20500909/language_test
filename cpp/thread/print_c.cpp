#include<stdio.h>
#include<pthread.h>
void pri1(){
	for(int i=0;i<10;i++)
		printf("+++\n");
}
void pri2(){
	for(int i=0;i<10;i++)
		printf("---\n");
}
int main()
{
	pthread_t id,id2;
	pthread_create(&id,NULL,(void *)pri1,NULL);
	pthread_create(&id2,NULL,(void *)pri2,NULL);
	pthread_join(id,NULL);
	printf("******\n");
	return 0;
}

