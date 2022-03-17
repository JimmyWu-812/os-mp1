#include "kernel/types.h"
#include "user/setjmp.h"
#include "user/threads.h"
#include "user/user.h"
#define NULL 0

#define debug 0

static struct thread* current_thread = NULL;
static struct thread* root_thread = NULL;
static int id = 1;
static jmp_buf env_st;
// static jmp_buf env_tmp;
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
    if(debug) printf("created: %d\n", id);
    id++;
    return t;
}
void thread_add_runqueue(struct thread *t){
    t->left = NULL;
    t->right = NULL;
    if(current_thread == NULL){
        // TODO
        current_thread = t;
        root_thread = t;
        t->parent = NULL;
        if(debug) printf("added as root: %d\n", t->ID);
    }
    else{
        // TODO
        if(current_thread->left == NULL){
            t->parent = current_thread;
            current_thread->left = t;
            if(debug) printf("added: %d\n", t->ID);
        }
        else if(current_thread->right == NULL){
            t->parent = current_thread;
            current_thread->right = t;
            if(debug) printf("added: %d\n", t->ID);
        }
        else{
            free(t->stack);
            free(t);
        }
    }
}
void thread_yield(void){
    // TODO
    if(setjmp(current_thread->env) == 0){ // 0: going to yield, 1: continue executing
        if(debug) printf("yielded: %d\n", current_thread->ID);
        schedule();
        dispatch();
    }
}
void dispatch(void){
    // TODO
    if(current_thread->buf_set == 0){
        current_thread->buf_set = 1;
        current_thread->env->sp = (unsigned long)current_thread->stack_p;
        (*current_thread->fp)(current_thread->arg);
    }
    else{
        if(debug) printf("dispatched: %d\n", current_thread->ID);
        longjmp(current_thread->env, 1); // first time: hasn't been set??
    }
    // thread_exit()??
}
void schedule(void){
    // TODO
    struct thread *t = current_thread;
    if(t->left != NULL){
        t = t->left;
    }
    else if(t->right != NULL){
        t = t->right;
    }
    else{

    }
    current_thread = t;
    if(debug) printf("scheduled: %d\n", current_thread->ID);
}
void thread_exit(void){
    if(current_thread == root_thread && current_thread->left == NULL && current_thread->right == NULL){
        // TODO
        // Hint: No more thread to execute
        free(current_thread->stack);
        free(current_thread);
        longjmp(env_st, 1);
    }
    else{
        // TODO
        struct thread *t = current_thread;
        while(1){
            if(t->right != NULL){
                t = t->right;
            }
            else if(t->left != NULL){
                t = t->left;
            }
            else{
                break;
            }
        }
        if(t != current_thread){
            if(current_thread == root_thread){
                root_thread = t;
                t->parent = NULL;
            }
            else{
                t->parent = current_thread->parent;
                if(current_thread->parent->left == current_thread){
                    current_thread->parent->left = t;
                }
                else{
                    current_thread->parent->right = t;
                }
            }
            if(current_thread->left == t){
                if(current_thread->right != NULL){
                    t->right = current_thread->right;
                    t->right->parent = t;
                }
            }
            else if(current_thread->right == t){
                if(current_thread->right != NULL){
                    t->left = current_thread->left;
                    t->left->parent = t;
                }
            }
            else{
                if(current_thread->right != NULL){
                    t->right = current_thread->right;
                    t->right->parent = t;
                }
                if(current_thread->right != NULL){
                    t->left = current_thread->left;
                    t->left->parent = t;
                }
            }
        }
        current_thread->parent = NULL;
        current_thread->left = NULL;        
        current_thread->right = NULL;        
        free(current_thread->stack);
        free(current_thread);
        schedule();
        dispatch();
    }
}
void thread_start_threading(void){
    // TODO   
    if(setjmp(env_st) == 0){
        if(debug) printf("started\n");
        dispatch();
    }
    else{
        return;
    }
}