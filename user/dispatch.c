void dispatch(void){
    // if thread doesn't executed before
    if(current_thread->buf_set == 0) {
        // initialize jump_buf of current thread
        if(setjmp(current_thread->env)) {
            // return from a longjmp means that process's sp is set
            // now we can execute current thread
            current_thread->fp(current_thread->arg);
            // In case the thread's funcion just returns, the thread needs to be 
            // removed from the runqueue and the next one has to be dispatched.
            thread_exit();
        }
        // set sp in jmp_buf, it will be used in longjmp to setup process's sp(in CPU register) later
        current_thread->env->sp = (unsigned long)current_thread->stack_p;
        current_thread->buf_set = 1;
        // use jmp_buf to setup process's sp(in CPU register)
        longjmp(current_thread->env, 1);
    }
    // if thread has executed
    else {
        // Load current thread's context
        longjmp(current_thread->env, 1);        
    }
}