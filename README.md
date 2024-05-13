# 42_3_philosophers

## Introduction to Threads, Mutex Locks, and Race Conditions in C, Explained Like I'm Five

In computer science, threads are like separate paths within a program. Just like a group of friends can split up to do different tasks and then come back together, threads allow a program to do multiple things at once.

Imagine you have a powerful multi-core computer chip. Threads are like workers that can take advantage of each core, helping your program run faster.

To use threads in C, we use a library called `<pthread.h>`. Here's a quick summary:

**Main Features:**
- **Thread Creation**: Make new threads.
- **Thread Synchronization**: Make sure threads share resources safely using mutexes and coordinate their actions with condition variables.
- **Thread Termination**: End threads when they're done.
- **Thread Joining**: Wait for threads to finish, like waitpid function with processes.
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

```c

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

void	*make_coffee(void *data)
{
	printf("Barista is making coffee...\n");
	sleep(2);
	printf("coffee ready\n");
	return (NULL);
}

int	main()
{
	pthread_t	barista_1;
	pthread_t	barista_2;

	pthread_create(&barista_1, NULL, make_coffee, NULL);
	pthread_create(&barista_2, NULL, make_coffee, NULL);

	pthread_join(barista_1, NULL);
	pthread_join(barista_2, NULL);
}
```

`pthread_join` here works like waitpid function when working with child processes.
Let's make it work with 500 baristas and show us our thread id:

```c

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include <stdlib.h>

void	*make_coffee(void *data)
{
	printf("Barista is making coffee...\n");
	sleep(2);
	printf("coffee ready\n");
	return (NULL);
}

int	main()
{
	pthread_t	baristas[500];
	int			i;

	i = -1;
	while (++i < 500)
	{
		if (pthread_create(baristas + i, NULL, make_coffee, NULL))
			exit(EXIT_FAILURE);
	}
	i = -1;
	while (++i < 500)
		printf("Thread id -> %lu\n", baristas[i]);
	i = -1;
	while (++i < 500)
		pthread_join(baristas[i], NULL);
}
```

While it appears that all baristas make coffee at the same time when executing our program, therefore that threads are running parallelly it is important to know that they are actually running concurrently.
`Concurrent execution` involves tasks running independently and potentially interleaved on a single processing unit (e.g., CPU core).
`Parallel execution` involves tasks running simultaneously on separate processing units (e.g., multiple CPU cores), achieving true simultaneous execution.

## Data Races

A data race happens in concurrent programming, where two or more threads access a shared resource, like a variable or memory location, and at least one of those accesses is a write operation.
If the accesses are not properly synchronized, the outcome of the program becomes unpredictable, as it depends on the timing of the thread's execution.
Data races can lead to bugs, crashes or incorrect behavior.

In our starbucks analogy:

Imagine you have a Starbucks café where multiple baristas (workers) are making coffee orders simultaneously.
Each barista has access to a shared coffee machine and a register to process payments. Here's how a data race could occur:

	Two Baristas, One Machine: Let's say Barista A and Barista B both receive orders to make a cappuccino. They both go to the coffee machine to prepare the drink simultaneously.

	Race for Resources: Barista A starts frothing milk while Barista B starts grinding coffee beans. However, they both reach for the same container of espresso at the same time to make the drink.

	Conflicting Access: Since the espresso container is a shared resource, both baristas attempt to pour espresso into their respective cups simultaneously. This creates a conflict, as only one barista should access the espresso container at a time.

	Unpredictable Outcome: Depending on the timing of their actions, one barista might pour espresso into their cup first, while the other might find the container empty or partially filled. This leads to inconsistent or incorrect coffee orders being served to customers.

We therefore need a way to lock that shared resource, so that while one thread is using it the other ones have to wait until it finished and unlocks the resource again.

We can make a program to illustrate the data race condition:

```c
#include <stdio.h>
#include <unistd.h>
#include <pthread.h> // thread programming
#include <stdlib.h>
#include <stdint.h> // standard integer

#define TH 100

// Shared resource
uint64_t	ledger = 0;

void	*update_ledger(void *data)
{
	ledger += 1;
	return NULL;
}

int	main(void)
{
	pthread_t	waiters[TH];
	int			i;

	i = -1;
	while (++i < TH)
	{
		if (pthread_create(waiters + i, NULL, update_ledger, NULL))
			exit(EXIT_FAILURE);
	}
	i = -1;
	while (++i < TH)
	{
		pthread_join(waiters[i], NULL);
	}
	printf("\t%lu\n", ledger);
	/* In terminal do:
	'while true; do
    ./a.out
done'
	*/
}
```

To lock our ledger in order to get a consistent value everytime (with program above sometimes ledger is updated to TH, sometimes less) and avoid data races we use `mutex_locks`.
Mutexes can be initiliazed either statically or dynamically using `pthread_mutex_init`. Here are some differences:

	Static Initialization:
		Static initialization means that the mutex is initialized at compile time. This is tipically done by definig the mutex as a global or tatic variable and initializing it with the `PTHREAD_MUTEX_INITIALIZER` macro. (Examples in the next block of code)
		The mutex is initialized before the program starts executingand is available throughout the program's lifetime.
		Advantages: Simple and straightforward. No need for additional initialization code.
		Disadvantages: Limited flexibility. Once initialized statically, the mutex cannot be reinitialized or destroyed.

	Dynamic Initialization:
		Dynamic initialization involves initializing the mutex at runtime using the `pthread_mutex_init` function. (Examples in the next block of code)
		This allows more flexibility in configuring the mutex, such as stting attributes like type, protocol, and robustness.
		Advantages: More flexibility in configuring mutex attributes. Can be reinitialized or destroyed dynamically.
		Disadvantages: Requires additional initialization code. Potential overhead compared to static initialization.

```c
#define TH 100

// Shared resource
uint64_t	ledger = 0;

// Lock
pthread_mutex_t	ledger_lock = PTHREAD_MUTEX_INITIALIZER;

void	*update_ledger(void *data)
{
	pthread_mutex_lock(&ledger_lock);
	ledger += 1;
	pthread_mutex_unlock(&ledger_lock);
	return NULL;
}

int	main(void)
{
	pthread_t	waiters[TH];
	int			i;

	// pthread_mutex_init(&ledger_lock, NULL);
	i = -1;
	while (++i < TH)
	{
		if (pthread_create(waiters + i, NULL, update_ledger, NULL))
			exit(EXIT_FAILURE);
	}
	i = -1;
	while (++i < TH)
	{
		pthread_join(waiters[i], NULL);
	}
	printf("\t%lu\n", ledger);
}
```

## Process vs Thread

	Definition:
		Thread: A thread is the smallest unit of execution within a process. Multiple threads can exist within the same process and share the same memory space and resources.
		Process: A process is an instance of a program that is running on a computer. It includes the program code, data, and resources (such as memory and CPU time) allocated to it.

	Resource Allocation:
		Thread: Threads within the same process share the same memory space, file descriptors, and other resources. Each thread has its own stack but shares the heap and global variables with other threads in the process.
		Process: Processes have separate memory spaces, file descriptors, and other resources. Each process runs independently and cannot directly access the memory or resources of other processes.

	Communication:
		Thread: Threads within the same process can communicate with each other directly using shared memory or synchronization primitives like mutexes, semaphores, or condition variables.
		Process: Processes communicate with each other using inter-process communication (IPC) mechanisms such as pipes, sockets, shared memory, or message queues.

	Creation Overhead:
		Thread: Creating a thread within a process typically has lower overhead than creating a new process. This is because threads share resources and memory within the same process context.
		Process: Creating a new process involves more overhead, as it requires duplicating the entire process state, including memory space and resources.

	Concurrency vs. Parallelism:
		Thread: Threads enable concurrency within a single process, allowing multiple tasks to be executed concurrently. Threads may run in parallel on multi-core processors if the operating system supports it.
		Process: Processes run independently of each other, and multiple processes can run in parallel on multi-core processors.

In summary, threads are lightweight units of execution within a process that share resources and memory, while processes are independent instances of programs that run in separate memory spaces and communicate via IPC. Threads enable concurrent execution within a process, while processes provide isolation and independent execution of programs.

Race conditions are intra-process things, not inter-process.
