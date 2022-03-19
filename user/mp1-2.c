#include "kernel/types.h"
#include "user/user.h"
#include "user/threads.h"

#define NULL 0

void ft(void *arg)
{
    printf("thread t: forbidden\n");
    return;
}

void f23(void *arg)
{
    int i = *(int *)arg;

    while(1) {
        printf("thread 23: %d\n", i++);
        if (i == 4) {
            return;
        }
        thread_yield();
    }
}

void f22(void *arg)
{
    int i = *(int *)arg;

    while(1) {
        printf("thread 22: %d\n", i++);
        if (i == 4) {
            thread_exit();
        }
        thread_yield();
    }
}

void f21(void *arg)
{
    int i = *(int *)arg;

    while(1) {
        printf("thread 21: %d\n", i++);
        if (i == 2) {
            return;
        }
        thread_yield();
    }
}

void f20(void *arg)
{
    int i = *(int *)arg;

    while(1) {
        printf("thread 20: %d\n", i++);
        if (i == 2) {
            thread_exit();
        }
        thread_yield();
    }
}

void f12(void *arg)
{
    int i = *(int *)arg;

    while(1) {
        printf("thread 12: %d\n", i++);
        if (i == 2) {
            thread_exit();
        }
        thread_yield();
    }
}

void f11(void *arg)
{
    int i = *(int *)arg;
    int j = *(int *)arg;

    struct thread *t22 = thread_create(f22, (void *)&j);
    thread_add_runqueue(t22);
    struct thread *t23 = thread_create(f23, (void *)&j);
    thread_add_runqueue(t23);

    while(1) {
        printf("thread 11: %d\n", i++);
        if (i == 5) {
            return;
        }
        thread_yield();
    }
}

void f10(void *arg)
{
    int i = *(int *)arg;
    int j = *(int *)arg;

    while(1) {
        printf("thread 10: %d\n", i++);
        if (i == 7) {
            thread_exit();
        }
        if (i == 4){
            struct thread *t20 = thread_create(f20, (void *)&j);
            thread_add_runqueue(t20);
            struct thread *t21 = thread_create(f21, (void *)&j);
            thread_add_runqueue(t21);
        }
        thread_yield();
    }
}

void f7(void *arg)
{
    int i = *(int *)arg;

    while(1) {
        printf("thread 7: %d\n", i++);
        if (i == 2) {
            return;
        }
        thread_yield();
    }
}

void f6(void *arg)
{
    int i = *(int *)arg;
    int j = *(int *)arg;

    struct thread *t12 = thread_create(f12, (void *)&j);
    thread_add_runqueue(t12);
    
    while(1) {
        printf("thread 6: %d\n", i++);
        if (i == 3) {
            thread_exit();
        }
        thread_yield();
    }
}

void f5(void *arg)
{
    int i = *(int *)arg;
    int j = *(int *)arg;


    while(1) {
        printf("thread 5: %d\n", i++);
        if (i == 5) {
            return;
        }
        if (i == 2){
            struct thread *t10 = thread_create(f10, (void *)&j);
            thread_add_runqueue(t10);
        }
        if (i == 3){
            struct thread *t11 = thread_create(f11, (void *)&j);
            thread_add_runqueue(t11);
        }
        if (i == 4){
            struct thread *tt = thread_create(ft, (void *)&j);
            thread_add_runqueue(tt);
        }
        thread_yield();
    }
}

void f4(void *arg)
{
    int i = *(int *)arg;

    while(1) {
        printf("thread 4: %d\n", i++);
        if (i == 2) {
            thread_exit();
        }
        thread_yield();
    }
}

void f3(void *arg)
{
    int i = *(int *)arg;
    int j = *(int *)arg;
    
    struct thread *t6 = thread_create(f6, (void *)&j);
    thread_add_runqueue(t6);

    while(1) {
        printf("thread 3: %d\n", i++);
        if (i == 3) {
            return;
        }
        if (i == 2){
            struct thread *t7 = thread_create(f7, (void *)&j);
            thread_add_runqueue(t7);
        }
        thread_yield();
    }
}

void f2(void *arg)
{
    int i = *(int *)arg;
    int j = *(int *)arg;

    struct thread *t4 = thread_create(f4, (void *)&j);
    thread_add_runqueue(t4);
    struct thread *t5 = thread_create(f5, (void *)&j);
    thread_add_runqueue(t5);

    while(1) {
        printf("thread 2: %d\n", i++);
        if (i == 4) {
            thread_exit();
        }
        thread_yield();
    }
}

void f1(void *arg)
{
    int i = *(int *)arg;
    int j = *(int *)arg;

    struct thread *t2 = thread_create(f2, (void *)&j);
    thread_add_runqueue(t2);
    struct thread *t3 = thread_create(f3, (void *)&j);
    thread_add_runqueue(t3);
    
    while(1) {
        printf("thread 1: %d\n", i++);
        if (i == 2) {
            return;
        }
        thread_yield();
    }
}

int main(int argc, char **argv)
{
    printf("mp1-2\n");
    int arg = 0;
    struct thread *t1 = thread_create(f1, (void *)&arg);
    thread_add_runqueue(t1);
    thread_start_threading();
    printf("\nexited\n");
    exit(0);
}
