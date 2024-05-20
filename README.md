# 42_3_philosophers

## Threads

### Introduction to Threads, Mutex Locks, and Race Conditions in C, Explained Like I'm Five

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

```c
pthread_create - int pthread_create(pthread_t *thread, const pthread_attr_t *attr, void *(*start_routine) (void *), void *arg);

It takes 4 parameters:

thread: A pointer to a pthread_t variable that will hold the ID of the newly created thread.

attr: An optional pointer to a pthread_attr_t structure containing thread attributes. Thread attributes specify various properties of the new thread, such as a scheduling policy, stack size, or detached status. If NULL is passed, default attributes are passed.

start_routine: A pointer to the function that the new thread will execute. This function must accept a single void* argument and return a void*.

arg: An argument that will be passed to start_routine function when the new thread is created. This argument can be used to pass data or context information to the thread function.
```

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

### Data Races

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

### Process vs Thread

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

## Deadlocks

### Mexican Standoff

A deadlock occurs when a process or thread enters a waiting stage because a requested system resource is held by another waiting process, which in turn is waiting for another resource held by another waiting process.

As an analogy we can think of a deadlock as a mexican standoff. Two or more threads are unable to continue their execution because they're each waiting for the other to release a resource.

Let's create a function to exemplify.

```c
#include <stdio.h>
#include <pthread.h>
#include <string.h>

// GUNS
pthread_mutex_t ugly_gun = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t bad_gun = PTHREAD_MUTEX_INITIALIZER;

typedef struct	s_cowboy
{
	char		*name;
	pthread_t	thread;
}	t_cowboy;

void	*action(void *data)
{
	t_cowboy	cowboy;

	cowboy = *(t_cowboy *)data;
	if (!strcmp(cowboy.name, "ugly"))
		pthread_mutex_lock(&ugly_gun);
	else
		pthread_mutex_lock(&bad_gun);
	printf("%s has taken his own gun\n", cowboy.name);

	// I wanna take the other gun
	// DEADLOCK
	if (!strcmp(cowboy.name, "ugly"))
		pthread_mutex_lock(&bad_gun);
	else
		pthread_mutex_lock(&ugly_gun);

	return NULL;
}

int	main(void)
{
	t_cowboy	ugly = {"ugly"};
	t_cowboy	bad = {"bad"};

	pthread_create(&ugly.thread, NULL, action, &ugly);
	pthread_create(&bad.thread, NULL, action, &bad);

	pthread_join(ugly.thread, NULL);
	pthread_join(bad.thread, NULL);
}
```

Let's breakdown the `cowboy = *(t_cowboy *)data;` line in the action function from above.

Casting:
	`cowboy = *(t_cowboy *)data;`: This part of the expression casts the `data` pointer from a void* type to a t_cowboy type.
		data is declared as a pointer to *void, which means it can point to any type of data. However in this context we know that data actually points to a t_cowboy object.
		By casting data to `*(t_cowboy *)` we inform the compiler we are treating data as a pointer to a t_cowboy object.

Dereferencing:
	`*(t_cowboy *)data;`: This part of the expression dereferences the pointer obtained from the casting.
		*(...) is the dereference operator in C. It is used to access the value stored at the memory location pointed by a pointer.
		(t_cowboy *)data casts data to a t_cowboy* pointer. This means we're treating data as a pointer to a t_cowboy object.
		*(t_cowboy *)data dereferences this pointer, accessing the t_cowboy object it points to.

So, in summary, `(t_cowboy *) data` casts the `void*` pointer `data` to a `t_cowboy*` pointer, and `*(t_cowboy *) data` dereferences this pointer, allowing us to access the `t_cowboy` object it points to. This allow us to retrieve the `t_cowboy` object from the `void*` pointer `data` and assign it to the `cowboy` variable.

If we run this program we either get:

ugly has taken his own gun
bad has taken his own gun

or:

ugly has taken his own gun

After acquiring its own mutex, each thread tries to acquire the other mutex as well. However, the other mutex is already locked by the other thread. Hence, both threads get blocked waiting for the other mutex to be released. This leads to a deadlock situation where neither thread can proceed.
Therefore the second part of our code cannot be executed, hence we only see the first part and it gets stuck in a deadlock "infinite loop".

### Synchronization

```c
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <wait.h>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>

#define ANSI_COLOR_RESET   "\x1b[0m"
#define ANSI_COLOR_RED     "\x1b[31m"
#define ANSI_COLOR_GREEN   "\x1b[32m"
#define ANSI_COLOR_YELLOW  "\x1b[33m"
#define ANSI_COLOR_BLUE    "\x1b[34m"
#define ANSI_COLOR_MAGENTA "\x1b[35m"
#define ANSI_COLOR_CYAN    "\x1b[36m"
#define ANSI_BOLD          "\x1b[1m"
#define ANSI_UNDERLINE     "\x1b[4m"

// GUNS
pthread_mutex_t ugly_gun = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t bad_gun = PTHREAD_MUTEX_INITIALIZER;

typedef struct	s_cowboy
{
	char			*name;
	unsigned long	reaction_time;
	pthread_t		thread;
}	t_cowboy;

void	*action(void *data)
{
	t_cowboy	cowboy;

	cowboy = *(t_cowboy *)data;
	// How fast the cowboy is
	printf("%s reaction time "ANSI_BOLD ANSI_COLOR_CYAN"%lu\n"ANSI_COLOR_RESET,
		cowboy.name,
		cowboy.reaction_time);
	usleep(cowboy.reaction_time);

	if (!strcmp(cowboy.name, "ugly"))
		pthread_mutex_lock(&ugly_gun);
	else
		pthread_mutex_lock(&bad_gun);
	printf("%s has taken his own gun\n", cowboy.name);

	// I wanna take the other gun
	// DEADLOCK
	if (!strcmp(cowboy.name, "ugly"))
		pthread_mutex_lock(&bad_gun);
	else
		pthread_mutex_lock(&ugly_gun);

	// The killer will reach this position
	printf(ANSI_COLOR_RED"%s killed the other\n"ANSI_COLOR_RESET, cowboy.name);
	exit(EXIT_SUCCESS);

	return NULL;
}

int	main(void)
{
	srand((time(NULL)) * getpid());
	t_cowboy	ugly = {"ugly", rand() % 10000};
	t_cowboy	bad = {"bad", rand() % 10000};

	pthread_create(&ugly.thread, NULL, action, &ugly);
	pthread_create(&bad.thread, NULL, action, &bad);

	pthread_join(ugly.thread, NULL);
	pthread_join(bad.thread, NULL);
}
```

ANSI escape sequences allow you to control text formatting, color, and other display options in terminal output.


## Dining Philosophers Problem

### Objectives and Solutions

The Dining Philosophers problem is a classic concurrency problem that illustrates issues with resource sharing and deadlock in concurrent programming.
It presents a scenario where N philosophers are seated around a circular table, with a bowl of spaghetti and one fork between each pair of philosophers.
Each philosopher can be in one of three states: eating, sleeping, or thinking. Philosophers are unware of each other's statuses.
The goal is to simulate the three states of philosophers and log any changes in status, including when a philosopher dies.

The program requires five arguments:

	number_of_philosophers (nop): The number of philosophers seated around the table, which also corresponds to the number of forks available.

	time_to_die (ttd): The time limit in milliseconds. If a philosopher does not start eating within this time after their last meal or the beginning of the simulation, they die.

	time_to_eat (tte): The time it takes in milliseconds for a philosopher to eat. During this time, the philosopher needs to hold onto both forks.

	time_to_sleep (tts): The time in milliseconds that the philosopher spends sleeping.

	number_of_times_each_philosopher_must_eat: (Optional) Specifies how many times each philosopher must eat before the simulation stops. If not specified, the simulation continues until a philosopher dies.

Philosophers can eat if they can pick up the two adjacentforks, but only one fork may be picked up by any one of its adjacent philosophers to avoid potential issues.
This problem highlights challenges related to concurrency control, resource allocation and synchronization in multi-threaded or distributed systems. There are various solutions, such as:

	Resource hierarchy solution (https://eng.libretexts.org/Courses/Delta_College/Operating_System:_The_Basics/06:_Deadlock/6.4:_Dining_Philosopher_Problem#:~:text=Arbitrator%20solution,computers%20competing%20for%20access)

	Delay in eating solution (https://f0rkr.github.io/dining-philosophers-problem/)

	Synchronization solution (https://medium.com/@ridwaneelfilali/philosophers-the-dinning-problem-8ea3c0fc8cc7)

	Waiter/Chandy-Misra solution (https://www.stolaf.edu/people/rab/pdc/text/dpsolns.htm)

	Specific Order of Fork Pickup solution

Here are some advantages and disadvantages:

Specific Order of Fork Pickup:

	Advantages:

		Simple to implement: By enforcing a specific order of picking up forks, you can easily prevent deadlock without complex synchronization mechanisms.
		Deadlock prevention: Ensure that philosophers never reach a circular wait condition, preventing deadlock from occurring.

	Disadvantages

		Potential starvation: Philosophers may have to wait longer to acquire both forks if they must wait for others to release them in a specific order, leading to potential starvation.
		Less flexibility: Philosophers are constrained to a specific order of actions, which might not be optimal for all scenarios.

Delay in Picking Up Forks:

	Advantages:

		Fairness: Philosophers have an equal chance of acquiring the required forks, reducing the likehood of starvation.
		Simple Implementation: Adding a delay before attempting to acquire a fork is straightforward.

	Disadvantages

		Increased latency: Introducing delays can slow down the overall execution of the program, affecting performance.
		Potential inefficiency: Philosophers may waste time waiting even when forks are available, leading to suboptimal resource utilization.

Synchronization Mechanisms:

	Advantages:

		Fine-grained control: Allows precise coordination between threads, ensuring that resorces are acquired and released in a synchronized manner.
		Optimal resource utilization: Can minimize waiting time and maximize concurrency by efficiently managing resource access.

	Disadvantages

		Complexity: Implementing synchronization mechanisms such as mutexes, semaphores or monitors can be complex and error-prone.
		Deadlock risk: Incorrect use of synchronization primitives can lead to deadlock or other concurrency issues.


Monitoring Thread:

	Advantages:

		Real-time monitoring: Allows continuous monitoring of the system's state, enabling early detection and resolution of potential issues.
		Dynamic adjustements: Can dynamically adjust system parameters or interven when necessary to prevent deadlock or starvation.

	Disadvantages

		Overhead: Introducing a monitoring thread adds additional overhead to the system, potentially impacting performance.
		Complexity: Implementing a monitoring thread and associated logic can be complex, requiring careful design and testing.


### The Dining Philosophers by Oceano

Libraries needed:

```c
#include <stdio.h>   // printf
#include <stdlib.h>  // malloc, free
#include <unistd.h>  // write, usleep
#include <stdbool.h> // booleans
#include <pthread.h> // mutex: init destroy lock unlock
					 // threads: create join detach
#include <sys/time.h>// gettimeofday
#include <limits.h>  // INT_MAX
```

Every philo is a struct.

```c
int philo_id
long meals_eaten
bool is_full
long last_meal_time
*left_fork
*right_fork
pthread_t thread_id
```

fork is a mutex.

```c
t_mtx fork
int fork_id
```

table.

```c
int philo_nbr
int time_to_die
int time_to_eat
int time_to_sleep
int nbr_max_meals
t_fork forks
t_philo philos
```

gettimeofday(): int	gettimeofday(struct timeval *tv, struct timezone *tz).

The use of timezone is obsolete, the tz argument should normally be set to NULL.

tv argument is a struct timeval:

```c
struct timeval {
	time_t	tv_sec; // Seconds
	suseconds_t	tv_usec; // Microseconds
};
```

It returns 0 for success, or -1 for failure.

1 second is 1000 miliseconds, 1 milisecond is 1000 microseconds.
