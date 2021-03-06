#include "cstringext.h"
#include "sys/semaphore.h"
#include "sys/thread.h"
#include "sys/system.h"
#include <stdio.h>

static semaphore_t sem1;
static semaphore_t sem2;

static int STDCALL thread0(void* param)
{
	param = param;
	printf("semaphore thread0 - init\n");
#if defined(OS_LINUX) || defined(OS_WINDOWS)
	assert(WAIT_TIMEOUT == semaphore_timewait(&sem1, 1000));
	printf("thread0 - timewait ok\n");
#endif

	assert(0 == semaphore_post(&sem1));
	printf("thread0 - post 1 ok\n");
	assert(0 == semaphore_wait(&sem2));
	printf("thread0 - wait 1 ok\n");
	assert(0 == semaphore_post(&sem1));
	printf("thread0 - post 2 ok\n");
	assert(0 == semaphore_wait(&sem2));
	printf("thread0 - wait 2 ok\n");
	assert(0 == semaphore_post(&sem1));
	printf("thread0 - post 3 ok\n");
	assert(0 == semaphore_wait(&sem2));
	printf("thread0 - wait 3 ok\n");
	assert(0 == semaphore_post(&sem1));
	printf("thread0 - post 4 ok\n");
	assert(0 == semaphore_wait(&sem2));
	printf("thread0 - wait 4 ok\n");
	assert(0 == semaphore_post(&sem1));
	printf("thread0 - post 5 ok\n");
	assert(0 == semaphore_wait(&sem2));
	printf("thread0 - wait 5 ok\n");
	printf("thread0 - exit\n");
	return 0;
}

static int STDCALL thread1(void* param)
{
	param = param;
	printf("semaphore thread1 - init\n");

	assert(0 == semaphore_wait(&sem1));
	printf("thread1 - wait 1 ok\n");
	assert(0 == semaphore_post(&sem2));
	printf("thread1 - post 1 ok\n");
	assert(0 == semaphore_wait(&sem1));
	printf("thread1 - wait 2 ok\n");
	assert(0 == semaphore_post(&sem2));
	printf("thread1 - post 2 ok\n");
	assert(0 == semaphore_wait(&sem1));
	printf("thread1 - wait 3 ok\n");
	assert(0 == semaphore_post(&sem2));
	printf("thread1 - post 3 ok\n");
	assert(0 == semaphore_wait(&sem1));
	printf("thread1 - wait 4 ok\n");
	assert(0 == semaphore_post(&sem2));
	printf("thread1 - post 4 ok\n");
	assert(0 == semaphore_wait(&sem1));
	printf("thread1 - wait 5 ok\n");
	assert(0 == semaphore_post(&sem2));
	printf("thread1 - post 5 ok\n");
	printf("thread1 - exit\n");
	return 0;
}

void semaphore_test(void)
{
	pthread_t threads[2];
	assert(0 == semaphore_create(&sem1, NULL, 3));
	assert(0 == semaphore_create(&sem2, NULL, 0));
	assert(0 == semaphore_wait(&sem1));
	assert(0 == semaphore_wait(&sem1));
	assert(0 == semaphore_trywait(&sem1));
	assert(0 != semaphore_trywait(&sem1));

	thread_create(&threads[0], thread0, NULL);
	thread_create(&threads[1], thread1, NULL);

	thread_destroy(threads[0]);
	thread_destroy(threads[1]);

	printf("all thread exit\n");
	assert(0 == semaphore_destroy(&sem1));
	assert(0 == semaphore_destroy(&sem2));
}
