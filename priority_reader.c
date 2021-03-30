#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#define N 5
int x = 0;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t r = PTHREAD_COND_INITIALIZER, w = PTHREAD_COND_INITIALIZER;
int reader_count = 0; //-1 if being writen
int total_readers = N;
void * reader(void *args){
	for(int i = 0; i < 10; i++){
		pthread_mutex_lock(&lock);
		while(reader_count == -1){
			pthread_cond_wait(&r, &lock);
		}
		reader_count++;
		printf("%d readers currently working:x : %d\n", reader_count, x);
		reader_count--;
		pthread_cond_signal(&r);
		pthread_mutex_unlock(&lock);
	}
	pthread_mutex_lock(&lock);
	total_readers--;
	if(total_readers == 0) pthread_cond_signal(&w);
	pthread_mutex_unlock(&lock);
	return NULL;
}
void * writer(void * args){
	for(int i = 0; i < 10; i++){
		pthread_mutex_lock(&lock);
		while(reader_count !=  0 ||  total_readers != 0){
			pthread_cond_wait(&w, &lock);
		}
		reader_count = -1;
		printf("Writing... x is %d and total readers is %d\n", ++x, total_readers);
		reader_count = 0;
		pthread_cond_signal(&w);
		pthread_mutex_unlock(&lock);
	}
	return NULL;
}
int main(){
	pthread_t readers[N], writers[N];
	for(int i = 0; i < N; i++){
		if(pthread_create(&readers[i], NULL, reader, NULL) != 0)
			fprintf(stderr,"failed to create thread");
	}
	for(int i = 0; i < N; i++){
		if(pthread_create(&writers[i], NULL, writer, NULL) != 0)
			fprintf(stderr,"failed to create thread");
	}
	for(int i = 0; i < N; i++){
		pthread_join(readers[i], NULL);
		pthread_join(writers[i], NULL);
	}
	printf("x: %d\n", x);
	
}
