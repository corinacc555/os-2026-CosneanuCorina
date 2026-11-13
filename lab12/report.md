# Lab 12 - Producer, Consumer and the Other Tools

Cosneanu Corina, FAF-241

## Part 1: Producer and consumer

1. The call `sem_wait(&empty)` at the beginning of the producer loop. The semaphore `empty`
   starts at 4 and counts the free slots: every `put` takes one unit, so after four items
   its value is 0 and the producer blocks there until the consumer takes an item and calls
   `sem_post(&empty)`.

2. The consumer waits at the start. The buffer is empty, so the semaphore `full` is 0 and
   the consumer blocks in `sem_wait(&full)` until the producer puts the first item and
   calls `sem_post(&full)`. The producer does not wait, because `empty` starts at 4.

3. The program hangs (deadlock) as soon as the buffer is full. The producer takes the mutex
   first and then blocks in `sem_wait(&empty)` because there is no free slot, and it sleeps
   while still holding the mutex. The consumer is the only one that can free a slot, but
   it needs the same mutex to take an item, so it blocks in `pthread_mutex_lock`. Each
   thread waits for the other one and nothing moves anymore. Rule: wait on the semaphore
   first, lock the mutex after.

## Part 2: The condition variable

1. The normal variable `ready` (protected by the mutex `lock`) remembers it. The condition
   variable `changed` has no memory: it is only the place where B sleeps.

2. Yes, with `if (!ready)` the program still works here and prints the same three lines,
   because there is only one waiting thread and it is woken up exactly once, after `ready`
   was set to 1.

3. Because waking up does not guarantee that the condition is true. One reason:
   `pthread_cond_wait` is allowed to return without any signal (a spurious wakeup), so the
   thread wakes up and `ready` is still 0. Another reason: with several waiting threads,
   another thread can run first and change the state back before this one gets the mutex
   again. The `while` checks the condition again and goes back to sleep if needed.

## Part 3: Readers and writers

1. The two lines

       if (++readers == 1)
           sem_wait(&room);

   Only the first reader takes the semaphore `room`; the next readers find `readers > 1`,
   skip `sem_wait(&room)` and go in together. While at least one reader is inside, `room`
   stays at 0, so a writer blocks in its own `sem_wait(&room)`; a writer that is inside
   holds `room`, so the first reader blocks. That is why a writer is always alone. (The
   pair `if (--readers == 0) sem_post(&room);` gives the room back when the last reader
   leaves.)

2. Because `++readers` and `--readers` are not one step (load, add, store, like `counter++`
   in Lab 11) and the test must be done together with the change. Without `rlock` two
   readers could change the counter at the same time and lose an update, or both could
   believe they are the first one. Then the counter is wrong: the second "first" reader
   blocks forever in `sem_wait(&room)`, or the room is given back while a reader is still
   inside (a writer writes during a read), or it is never given back.

3. If readers keep arriving without a pause, `readers` never goes back to 0, so the last
   reader never calls `sem_post(&room)`. Writer 2 waits in `sem_wait(&room)` forever, even
   though nothing is blocked: this is starvation.

## Part 4: Build a deadlock, then remove it

1. Thread `one` holds mutex A and waits for mutex B. Thread `two` holds mutex B and waits
   for mutex A. The circle:

       thread one --waits for--> mutex B
           ^                        |
        held by                  held by
           |                        v
        mutex A <--waits for-- thread two

2. No. Without the `sleep(1)` lines it did not hang at all in my ten runs: every run
   finished with exit code 0. A run blocks only if both threads take their first mutex
   before either of them takes the second one, and without the sleep the first thread
   usually takes both mutexes and finishes before the second thread even starts. That is
   worse, not better: the bug is still in the program, but now it is hidden. It can pass
   all the tests and then appear very rarely, when the timing is unlucky, and it is very
   hard to reproduce and to debug.

3. If every thread takes the locks in the same order, a thread that waits for a lock can
   only hold locks that come before it in that order, so a circle of threads waiting for
   each other can never be formed.
