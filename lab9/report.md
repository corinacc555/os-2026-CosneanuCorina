# Laboratory 9 - Watching System Calls
**Coșneanu Corina, FAF-241**

## Part 1: See the calls of a real command
**Observe:** Which system call does `ls` use to print to the screen? To which descriptor does it write?
- The `ls` command uses the `write` system call to output text to the screen. 
- It writes to file descriptor `1`, which is the standard output (stdout).

## Part 2: A program that calls the kernel directly
**Observe:** The descriptor returned by the first openat is 3, not 0, 1 or 2. Why are those three already taken?
- Descriptors 0, 1, and 2 are automatically opened by the operating system for every new process when it starts. They are reserved for standard streams:
  - `0` is Standard Input (stdin - keyboard)
  - `1` is Standard Output (stdout - screen)
  - `2` is Standard Error (stderr - error messages on screen)
Because these three are already occupied, the first available descriptor that the kernel can assign to our new file is `3`.
