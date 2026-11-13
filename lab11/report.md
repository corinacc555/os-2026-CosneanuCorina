# Lab 11 - The Race and the Semaphore

Cosneanu Corina, FAF-241

## Part 1: See the race

1. `counter++` is three steps for the CPU:
   1. load: read the value of `counter` from memory into a register;
   2. add: add 1 to the register;
   3. store: write the register back into `counter` in memory.

2. Both threads can do the load step before either of them does the store step.
   Example with `counter = 5`: thread A loads 5, thread B loads 5, A adds 1 and stores 6,
   B adds 1 and also stores 6. Two additions were made, but the counter grew only by 1,
   so one addition is lost.

3. The result depends on the exact moments when the two threads are interleaved, and the
   scheduler decides this differently at every run (it depends on timing, on the CPU cores
   and on the other processes of the system). The number of lost additions is therefore
   different every time.

## Part 2: Fix it: mutex, then semaphore

1. Yes. I ran `./mutex` and `./sem` three times each and the answer was always
   `counter = 2000000 (expected 2000000)`.

2. With `sem_init(&s, 0, 2)` the semaphore has two units, so both threads can pass
   `sem_wait` at the same time and both are inside the critical section together. The
   semaphore does not protect anything anymore and the race is back: the result is again
   smaller than 2 000 000 and different at every run.

3. Every single addition now also pays for a lock and an unlock operation (2 000 000 pairs
   in total), which cost much more than `counter++` itself. In addition, the two threads
   cannot work at the same time anymore: when the lock is taken, the other thread has to
   wait (and may be put to sleep and woken up by the kernel).

## Part 3: A semaphore as a signal

1. With `sem_init(&ready, 0, 1)` B prints `B: I read ""`, an empty message, and it prints it
   at once, before `A: message written`. The value 1 means the door is already open:
   `sem_wait` takes the unit and returns immediately, so B does not wait for A and reads
   `message` before A has written it (the array is still empty).

2. A semaphore that starts at 0 means "nothing is ready yet": it is a closed door, and
   whoever calls `sem_wait` must wait until another thread calls `sem_post`.

## Part 4: A semaphore as a counter: the parking lot

1. The largest value of `inside` is 3. It is guaranteed by the line `sem_wait(&places);`
   at the beginning of `car()`, together with the start value from
   `sem_init(&places, 0, PLACES);`: the semaphore has only 3 units, so the fourth car
   blocks in `sem_wait` until a car leaves and calls `sem_post`.

2. With `PLACES` set to 1 the semaphore becomes a binary semaphore, which behaves like a
   mutex (a lock): only one car can be inside at a time, so the cars enter and leave one
   by one and `inside` is never larger than 1.
