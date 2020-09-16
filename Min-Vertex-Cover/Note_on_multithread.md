## Note on multithread
In main function, 
- initilize a timeout semaphore to_flag = -1. sem_init(&to_flag, 0, -1); to_flag = -1
- Register the handler function to SIGALRM. signal(SIGALRM, TimeOut); Timeout is called, and runs sem_post(&to_flag). to_flag = 0
- Start the timer. retcode = setitimer(ITIMER_REAL, &tick, NULL); &ticl is 15s. after 15s, it generates a SIGALRM signal, which is regiesterd by the timeout function. 
- Start 3 subthreads to run 3 functions. pthread_create
- in one thread (CNF_SAT_VC), call sem_post(&to_flag) when the CNF solver is done (either finished or interrupted by timeout). to_flag = 1
- Wait for timeout (receive a SIGALRM signal) or the complete of t1. sem_wait(&to_flag); to_flag = 0
- cancel trheads. if CNF_SAT_VC times out, call pthread_cancel, pthread_detach to release resources. Otherwise, call pthread_join. Then call pthread_join for the other two threads. 
