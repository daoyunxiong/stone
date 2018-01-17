#include <iostream>
#include <cstdlib>

#include <unistd.h>
#include <pthread.h>

using namespace std;

struct{
    pthread_rwlock_t rwlock;
    int product;
}sharedData = {PTHREAD_RWLOCK_INITIALIZER, 0};

void * produce(void *ptr)
{
    for (int i = 0; i < 5; ++i)
    {
        pthread_rwlock_wrlock(&sharedData.rwlock);
        sharedData.product = i;
        pthread_rwlock_unlock(&sharedData.rwlock);

        sleep(1);
    }
}

void * consume1(void *ptr)
{
    for (int i = 0; i < 5;)
    {
        pthread_rwlock_rdlock(&sharedData.rwlock);
        cout<<"consume1:"<<sharedData.product<<endl;
        pthread_rwlock_unlock(&sharedData.rwlock);
	++i;
        sleep(1);
    }
}

void * consume2(void *ptr)
{
    for (int i = 0; i < 5;)
    {
        pthread_rwlock_rdlock(&sharedData.rwlock);
        cout<<"consume2:"<<sharedData.product<<endl;
        pthread_rwlock_unlock(&sharedData.rwlock);

        ++i;
        sleep(1);
    }
}

int main()
{
    pthread_t tid1, tid2, tid3;

    pthread_create(&tid1, NULL, produce, NULL);
    pthread_create(&tid2, NULL, consume1, NULL);
    pthread_create(&tid3, NULL, consume2, NULL);

    void *retVal;

    pthread_join(tid1, &retVal);
    pthread_join(tid2, &retVal);
    pthread_join(tid3, &retVal);

    return 0;
}
