#include "MCProcess.h"

oninit(MCProcess)
{
    if (init(MCObject)) {
        obj->pid=getpid();
        obj->ppid=getppid();
        obj->uid=getuid();
        obj->euid=geteuid();
        obj->gid=getgid();
        obj->egid=getegid();
        return obj;
    }else{
        return mull;
    }
}

public(MCProcess, void, printIDs, voida)
{
	printf("pid[%d]ppid[%d]uid[%d]euid[%d]gid[%d]egid[%d]\n", 
		obj->pid, obj->ppid, obj->uid, obj->euid, obj->gid, obj->egid);
}

public(MCProcess, int, fork, voida)
{
	//pid_t fork(void);
	//copy-on-write (COW)
	//typically a "page" in a virtual memory system
	return fork();
}

public(MCProcess, int, registerAtExitCallback, void (*func)(void))
{
	if(atexit(func)==0)
		return 0;//success
	else
		return -1;//error
}

public(MCProcess, void, exitWithStatus, int status)
{
	//this is a system call
	_exit(status);
}

public(MCProcess, pid_t, waitAnyChildExit, int* statusAddr)
{
	return wait(statusAddr);
}

public(MCProcess, pid_t, waitPIDChildExit, pid_t pid, int* statusAddr, int options)
{
	return waitpid(pid, statusAddr, options);
}

public(MCProcess, int, isChildExitNormal, int status)
{
	if(WIFEXITED(status))
		return 1;
	else
		return 0;
}

public(MCProcess, int, getChildExitLowOrder8Bit, int status)
{
	return WEXITSTATUS(status);
}

public(MCProcess, int, isChildExitBySignal, int status)
{
	if (WIFSIGNALED(status))
		return 1;
	else
		return 0;
}

public(MCProcess, int, getChildTerminateSignal, int status)
{
	return WTERMSIG(status);
}

public(MCProcess, int, isCoreDumpFileGenerated, int status)
{
	return WCOREDUMP(status);
}

public(MCProcess, int, isChildStopped, int status)
{
	if (WIFSTOPPED(status))
		return 1;
	else
		return 0;
}

public(MCProcess, int, getChildStopSignal, int status)
{
	return WSTOPSIG(status);
}

public(MCProcess, int, isChildContinued, int status)
{
	if(WIFCONTINUED(status))
		return 1;
	else
		return 0;
}

public(MCProcess, pid_t, waitAnyChildExitGetResourceUseage, int* statusAddr, int options, MCProcessRUseage* useage)
{
	return wait3(statusAddr, options, useage->rusage_p);
}

public(MCProcess, pid_t, waitPIDChildExitGetResourceUseage, pid_t pid, int* statusAddr, int options, MCProcessRUseage* useage)
{
	return wait4(pid, statusAddr, options, useage->rusage_p);
}

onload(MCProcess)
{
    if (load(MCObject)) {
        pub(MCProcess, void, printIDs);
        pub(MCProcess, int, fork);
        
        pub(MCProcess, int, registerAtExitCallback, void (*func)(void));
        pub(MCProcess, void, exitWithStatus, int status);
        pub(MCProcess, pid_t, waitAnyChildExit, int* statusAddr);
        
        pub(MCProcess, pid_t, waitPIDChildExit, pid_t pid, int* statusAddr, int options);
        pub(MCProcess, int, isChildExitNormal, int status);
        pub(MCProcess, int, getChildExitLowOrder8Bit, int status);
        pub(MCProcess, int, isChildExitBySignal, int status);
        pub(MCProcess, int, getChildTerminateSignal, int status);
        pub(MCProcess, int, isCoreDumpFileGenerated, int status);
        pub(MCProcess, int, isChildStopped, int status);
        pub(MCProcess, int, getChildStopSignal, int status);
        pub(MCProcess, int, isChildContinued, int status);
        pub(MCProcess, pid_t, waitAnyChildExitGetResourceUseage, int* statusAddr, int options, MCProcessRUseage* useage);
        pub(MCProcess, pid_t, waitPIDChildExitGetResourceUseage, pid_t pid, int* statusAddr, int options, MCProcessRUseage* useage);
        return cla;
    }else{
        return mull;
    }
}