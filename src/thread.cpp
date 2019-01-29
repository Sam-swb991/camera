#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "thread.h"

int ThreadCreateSched( pthread_t *threadId,
                void *(*threadFunc)(void*), void *arg,
                int schedPolicy, int schedPriority )
{
    int nRet = -1;

    pthread_attr_t attr;
    nRet = pthread_attr_init( &attr );
    if ( nRet != 0 ) printf( "pthread_attr_init error .\r\n" );

    nRet = pthread_attr_setschedpolicy( &attr, schedPolicy );
    if ( nRet != 0 ) printf( "pthread_attr_setschedpolicy error .\r\n" );

    if ( schedPolicy == SCHED_FIFO || schedPolicy == SCHED_RR )
    {
        sched_param param;
        param.sched_priority = schedPriority;
        nRet = pthread_attr_setschedparam( &attr, &param );
        if ( nRet != 0 ) printf( "pthread_attr_setschedparam error .\r\n" );
    }

    nRet = pthread_create( threadId, &attr, threadFunc, arg );
    if ( nRet != 0 ) printf( "pthread_create error .\r\n" );

    return nRet;
}

/***************************************************************************
* fn	: ?????????????????????????????? THREAD_PRIORITY_COMMON ????????????????????????????
* threadId : out, ????????ID??????
* threadFunc : ????????????????
* arg : ??????????????????????????????
*****************************************************************************/
int ThreadCreateCommonPriority( pthread_t *threadId, void *(*threadFunc)(void*), void *arg )
{
    return ThreadCreateSched( threadId, threadFunc, arg, THREAD_SCHED_POLICY, THREAD_PRIORITY_COMMON );
}

int ThreadCreate( pthread_t *threadId, void *(*threadFunc)(void*), void *arg )
{
    return pthread_create( threadId, NULL, threadFunc, arg );
}

unsigned long ThreadSelf()
{
    return (unsigned long)pthread_self();
}

int ThreadJoin( pthread_t thread, void **valuePtr )
{
    int errNo = pthread_join( thread, valuePtr );
    return errNo;
}

int ThreadDetach( pthread_t thread )
{
    int errNo = pthread_detach( thread );
    return errNo;
}

void ThreadExit( void *valuePtr )
{
    pthread_exit( valuePtr );
}

