# 42_3_philosophers

## Introduction to Threads, Mutex Locks, and Race Conditions in C, Explained Like I'm Five

In computer science, threads are like separate paths within a program. Just like a group of friends can split up to do different tasks and then come back together, threads allow a program to do multiple things at once.

Imagine you have a powerful multi-core computer chip. Threads are like workers that can take advantage of each core, helping your program run faster.

To use threads in C, we use a library called `<pthread.h>`. Here's a quick summary:

**Main Features:**
- **Thread Creation**: Make new threads.
- **Thread Synchronization**: Make sure threads share resources safely using mutexes and coordinate their actions with condition variables.
- **Thread Termination**: End threads when they're done.
- **Thread Joining**: Wait for threads to finish.
- **Thread Attributes**: Set special features for threads.
- **Thread Detachment**: Automatically clean up resources when threads are finished.

**Common Functions:**
- Thread Creation: `pthread_create`
- Thread Joining: `pthread_join`
- Thread Termination: `pthread_exit`
- Mutex Handling: `pthread_mutex_init`, `pthread_mutex_destroy`, `pthread_mutex_lock`, `pthread_mutex_unlock`
- Condition Variable Handling: `pthread_cond_init`, `pthread_cond_destroy`, `pthread_cond_wait`, `pthread_cond_signal`, `pthread_cond_broadcast`

Let's create a function to better illustrate threads.
An analogy we can use is starbucks, where every worker is equivalent to a different thread. For example, a simple starbucks program:

```c
#include <stdio.h>
#include <unistd.h>

void	make_coffee(char *name)
{
	printf("Barista %s is making coffee...\n", name);
	sleep(2);
	printf("coffee ready\n");
}

int	main()
{
	make_coffee("Luca");
	make_coffee("Paolo");
}
```

And now multi-threaded:
