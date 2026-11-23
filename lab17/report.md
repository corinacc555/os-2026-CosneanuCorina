# Lab 17 - Threads Share Memory, and Race

Cosneanu Corina, FAF-241

## Part 1: Shared and racing

### 1. Thread 2 prints a non-zero counter at the start. What does that prove?

It proves that the two threads share the same memory. `counter` is one global variable,
and there is only one copy of it in the process. The thread that starts second already
sees the additions made by the first thread, without any message or copy between them:
the change is visible instantly. (If each thread had its own memory, the second thread
would also see 0.) The order of the two threads can change from run to run, so sometimes
it is thread 1 that prints the non-zero value, but always the one that starts later.

### 2. Why is the final counter less than 2000000, and different each run?

`counter++` is not one step for the CPU, it is three:

1. read: load the value of `counter` from memory into a register;
2. add: add 1 to the register;
3. write: store the register back into `counter` in memory.

The two threads run at the same time, so their steps can be interleaved. Example with
`counter = 5`: thread 1 reads 5, thread 2 reads 5, thread 1 adds 1 and writes 6, thread 2
adds 1 and also writes 6. Two additions were made, but the counter grew only by 1: one
update is lost. This happens many times during the two million additions, so the final
counter is less than 2000000.

The result is different every run because the moments when the threads are interleaved
are decided by the scheduler and by the timing of the CPU cores, and they are never the
same twice. The number of lost updates is therefore different every time.

## Part 2: Make it worse, then think

With four threads (`fire4.c`) the expected value is 4000000, and the loss gets bigger:
millions of updates are lost instead of hundreds of thousands, and the result is even more
different from run to run. More threads means more collisions between their read and write
steps, so more updates overwrite each other. Sometimes a thread even writes back an old
value from its register, so the counter can go down after another thread has already seen
a bigger value.

### 3. In one sentence: what single idea would stop the lost updates?

Make `counter++` a critical section that only one thread at a time may execute, by
protecting it with a lock (a mutex): a thread locks before the read and unlocks after the
write, so the three steps can no longer be interleaved.
