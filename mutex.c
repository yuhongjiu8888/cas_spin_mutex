#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define THREAD_NUM 10

int global_count = 0;

pthread_mutex_t mutex;  //互斥锁
pthread_spinlock_t spin; //自旋锁


int inc(int *value,int add){
    int old;

    __asm__ volatile(
    "lock; xaddl %2,%1;"
    : "=a" (old)
    : "m" (*value), "a" (add)
    : "cc", "memory"
    );
    return old;

}


void *thread_proc(void *arg){
    int *count = (int *)arg;

    int i = 0;
    while (i++<100000)
    {
#if 1
        (*count)++;
        
#elif 0
        pthread_mutex_lock(&mutex);
        (*count)++;
        pthread_mutex_unlock(&mutex);
#elif 0
        pthread_spin_lock(&spin);
        (*count)++;
        pthread_spin_unlock(&spin);
#else
        inc(count,1);
#endif
    
        usleep(100);
    }
    

}

int main(){
    pthread_t thread_num[THREAD_NUM] = {0};

    pthread_mutex_init(&mutex,NULL);
    pthread_spin_init(&spin,PTHREAD_PROCESS_SHARED);

    int i;
    for(i=0;i<THREAD_NUM;i++){
        pthread_create(&thread_num[i],NULL,thread_proc,&global_count);
    }

    int k;
    for ( k= 0; k < 100; k++)
    {
        /* code */
        printf("global_count[%d]\n",global_count);
        sleep(1);
    }
    

    getchar();
    return 0;
}