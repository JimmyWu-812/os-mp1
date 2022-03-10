#include "kernel/types.h"
#include "user/setjmp.h"
#include "user/threads.h"
#include "user/user.h"
#include "user/setjmp.h"
#define NULL 0

static struct thread* current_thread = NULL;
static struct thread* root_thread = NULL;
static int id = 1;
static jmp_buf env_st;
static jmp_buf env_tmp;
// TODO: necessary declares, if any


struct thread *thread_create(void (*f)(void *), void *arg){
    struct thread *t = (struct thread*) malloc(sizeof(struct thread));
    //unsigned long stack_p = 0;
    unsigned long new_stack_p;
    unsigned long new_stack;
    new_stack = (unsigned long) malloc(sizeof(unsigned long)*0x100);
    new_stack_p = new_stack +0x100*8-0x2*8;
    t->fp = f;
    t->arg = arg;
    t->ID  = id;
    t->buf_set = 0;
    t->stack = (void*) new_stack;
    t->stack_p = (void*) new_stack_p;
    id++;
    return t;
}
void thread_add_runqueue(struct thread *t){
    if(current_thread == NULL){
        // TODO
        current_thread = t;
        root_thread = t;
    }
    else{
        // TODO
        if(current_thread->left == NULL){
            current_thread->left = t;
        }
        else if(current_thread->right == NULL){
            current_thread->right = t;
        }
        else{
            free(t->stack);
            free(t);
        }
    }
}
void thread_yield(void){
    // TODO
    setjmp(current_thread->env);
}
void dispatch(void){
    // TODO
    longjmp(current_thread->env, );
}
void schedule(void){
    // TODO
}
void thread_exit(void){
    if(current_thread == root_thread && current_thread->left == NULL && current_thread->right == NULL){
        // TODO
        // Hint: No more thread to execute
    }
    else{
        // TODO
    }
}
void thread_start_threading(void){
    // TODO
}
