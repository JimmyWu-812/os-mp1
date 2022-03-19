#include "kernel/types.h"
#include "user/setjmp.h"
#include "user/threads.h"
#include "user/user.h"
#define NULL 0

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
    // printf("  created: %d\n", id);
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
        // printf("  added as root: %d\n", t->ID);
    }
    else{
        // TODO
        if(current_thread->left == NULL){
            t->parent = current_thread;
            current_thread->left = t;
            // printf("  added: %d\n", t->ID);
        }
        else if(current_thread->right == NULL){
            t->parent = current_thread;
            current_thread->right = t;
            // printf("  added: %d\n", t->ID);
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
        // printf("  yielded: %d\n", current_thread->ID);
        // printf("  ID/sp: %d/%d\n", current_thread->ID, current_thread->env->sp);
        schedule();
        dispatch();
    }
    else{
        // printf("  continued: %d\n", current_thread->ID);
        // printf("  ID/sp: %d/%d\n", current_thread->ID, current_thread->env->sp);
    }
}
void dispatch(void){
    // TODO
    if(current_thread->buf_set == 0){
        if(setjmp(current_thread->env) == 1){
            // printf("  first dispatched: %d\n", current_thread->ID);
            // printf("  thread function executed: %d\n", current_thread->ID);
            current_thread->fp(current_thread->arg);
            // printf("  returned: %d\n", current_thread->ID);
            thread_exit();
        }
        current_thread->buf_set = 1;
        current_thread->env->sp = (unsigned long)current_thread->stack_p;
        // printf("  dispatched(current->buf_set == 0): %d\n", current_thread->ID);
        longjmp(current_thread->env, 1);
    }
    else{
        // printf("                dispatched: %d\n", current_thread->ID);
        // printf("  ID/sp: %d/%d\n", current_thread->ID, current_thread->env->sp);
        // printf("  lonjmp(current->buf_set == 1): %d\n", current_thread->ID);
        longjmp(current_thread->env, 1);
    }
}
void printPreTrav(struct thread *t){
    if(t == NULL){
        printf("N ");
        return;
    }
    printf("(%d) ", t->parent->ID);
    printf("%d ", t->ID);
    printPreTrav(t->left);
    printPreTrav(t->right);
}
void schedule(void){
    // TODO
    struct thread *t = current_thread;
    // printf("  ");
    // printPreTrav(root_thread);
    // printf("\n");
    // printf("t == root_thread? %d\n", (t == root_thread));
    if(t->left != NULL){
        t = t->left;
    }
    else if(t->right != NULL){
        t = t->right;
    }
    else if(t != root_thread){
        while(1){
            while(t->parent->right == t){
                t = t->parent;
                // printf("  t: %d\n", t->ID);
                if(t == root_thread){
                    current_thread = t;
                    // printf("  scheduled: %d\n", current_thread->ID);
                    return;
                }
            }
            while(t->parent->left == t){
                t = t->parent;
                if(t == root_thread){
                    if(t->right == NULL){
                        current_thread = t;
                    }
                    else{
                        current_thread = t->right;
                    }
                    // printf("  scheduled: %d\n", current_thread->ID);
                    return;
                }
                if(t->right != NULL){
                    current_thread = t->right;
                    // printf("  scheduled: %d\n", current_thread->ID);
                    return;
                }
            }
        }
    }
    current_thread = t;
    // printf("  scheduled: %d\n", current_thread->ID);
}
void thread_exit(void){
    if(current_thread == root_thread && current_thread->left == NULL && current_thread->right == NULL){
        // TODO
        // Hint: No more thread to execute
        // printf("  exited: %d\n", current_thread->ID);
        free(current_thread->stack);
        free(current_thread);
        current_thread = NULL;
        longjmp(env_st, 1);
    }
    else{
        // TODO
        struct thread *t = current_thread;
        while(1){
            if(t->right != NULL){
                // printf("  right, t->ID: %d\n", t->ID);
                t = t->right;
            }
            else if(t->left != NULL){
                // printf("  left, t->ID: %d\n", t->ID);
                t = t->left;
            }
            else{
                // printf("  break, t->ID: %d\n", t->ID);
                break;
            }
        }
        if(t != current_thread){
            if(current_thread->left == t){
                if(current_thread->right != NULL){
                    t->right = current_thread->right;
                    t->right->parent = t;
                }
            }
            else if(current_thread->right == t){
                if(current_thread->left != NULL){
                    t->left = current_thread->left;
                    t->left->parent = t;
                }
            }
            else{
                if(current_thread->right != NULL){
                    t->right = current_thread->right;
                    t->right->parent = t;
                }
                if(current_thread->left != NULL){
                    t->left = current_thread->left;
                    t->left->parent = t;
                }
                if(t->parent->left == t){
                    t->parent->left = NULL;
                }
                else{
                    t->parent->right = NULL;
                }
            }
            if(current_thread == root_thread){
                t->parent = NULL;
                root_thread = t;
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
            current_thread->parent = NULL;
            current_thread->left = NULL;        
            current_thread->right = NULL;        
            free(current_thread->stack);
            free(current_thread);
            current_thread = t;
            schedule();
            dispatch();
        }
        else{
            schedule();
            // printf("  after exited, next thread: %d\n", current_thread->ID);
            if(t->parent->left == t){
                t->parent->left = NULL;
            }
            else if(t->parent->right == t){
                t->parent->right = NULL;
            }
            t->parent = NULL;
            t->left = NULL;        
            t->right = NULL;        
            free(t->stack);
            free(t);
            dispatch();
        }
    }
}
void thread_start_threading(void){
    // TODO   
    if(setjmp(env_st) == 0){
        // printf("  started\n");
        dispatch();
    }
    else{
        // printf("  ended\n");
        return;
    }
}