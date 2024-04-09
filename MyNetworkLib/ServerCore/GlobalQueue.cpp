#include "pch.h"
#include "GlobalQueue.h"

GlobalQueue::GlobalQueue()
{
}

GlobalQueue::~GlobalQueue()
{
}

void GlobalQueue::Push(JobQueueRef jobQueue)
{
    _jobQueues.push(jobQueue);
}

JobQueueRef GlobalQueue::Pop()
{
    return _jobQueues.pop();
}
