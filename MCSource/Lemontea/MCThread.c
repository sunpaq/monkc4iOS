#include "MCThread.h"

/* MCRunnable */

oninit(MCRunnable)
{
    if (init(MCObject)) {
        obj->init_routine = 0;
        return obj;
    }else{
        return mull;
    }
}

public(MCRunnable, MCRunnable*, initWithFunctionPointer, void (*init_routine)(void))
{
	obj->init_routine = init_routine;
	return obj;
}

public(MCRunnable, void, run, voida)
{
	//do nothing
}

/* MCThread */

onload(MCThread)
{
	pub(MCThread, MCThread*, initWithRunnable, MCRunnable* runnable);
	pub(MCThread, int, start, void* result);
	pub(MCThread, int, equal, MCThread* thread);
	pub(MCThread, void, bye);
	return cla;
}

oninit(MCThread)
{
	//init the vars
	pthread_once_t ponce = PTHREAD_ONCE_INIT;
	obj->once_control = ponce;
	obj->isRunOnce = 0;//default is NO
	//if you need, you can set the attribute use the raw pthread APIs
	//example: pthread_attr_getstacksize(m_thread->attribute);
	pthread_attr_init(&obj->attribute);
	return obj;
}

public(MCThread, MCThread*, initWithRunnable, MCRunnable* runnable)
{
	if (runnable==mull)
	{
		error_log("%s\n","runnable can not be nil, do nothing");
		return mull;
	}
    retain(runnable);
	obj->runnable = runnable;
	return obj;
}

int MCThread_join(MCThread* thread, void** result)
{
	return pthread_join(thread->self, result);
}

int MCThread_detach(MCThread* thread)
{
	return pthread_detach(thread->self);
}

int MCThread_cancel(MCThread* thread)
{
	return pthread_cancel(thread->self);
}

//must call this inside the runnable run_routine
void MCThread_stop(void* result)
{
	pthread_exit(result);
}

pthread_t MCThread_self()
{
	return pthread_self();
}

//this is a bridge between static pthread callback
//and dynamic Mocha inherit tree method calling
static void* fireRun(void* obj)
{
	return ff(cast(MCThread*, obj)->runnable, run, 0);//no result
}

//pthread_once:     void (*)(void)
//pthread_create:   void*(*)(void*)
public(MCThread, int, start, void* result)
{
	int res;
	if (obj->isRunOnce==1)
	{
		if (obj->runnable->init_routine!=mull)
			res = pthread_once(&(obj->once_control), obj->runnable->init_routine);
		else
			res = pthread_once(&(obj->once_control), cast(void(*)(void), fireRun));

	}else{
		if (obj->runnable->init_routine!=mull)
			res = pthread_create(&obj->self,//tid, pthread_t type
				   &obj->attribute, 
				   cast(void*(*)(void*), obj->runnable->init_routine),
			       obj);//no argument
		else
			res = pthread_create(&obj->self,//tid, pthread_t type
				   &obj->attribute, 
				   fireRun,
			       obj);//no argument
	}
	return res;
}

public(MCThread, int, equal, MCThread* thread)
{
	return pthread_equal(obj->self, thread->self);
}

public(MCThread, void, bye, voida)
{
	pthread_attr_destroy(&obj->attribute);
	release(&(obj->runnable));
}

onload(MCRunnable)
{
    if (load(MCObject)) {
        pub(MCRunnable, void, run);
        pub(MCRunnable, MCRunnable*, initWithFunctionPointer, void (*init_routine)(void));
        return cla;
    }else{
        return mull;
    }
}

