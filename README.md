# *This project has been created as part of the 42 curriculum by <fbendnan>.*

# *Codexion* #

# Description

**codexion** is a multi‑threaded simulation of coders sharing rare USB dongles in a circular office.
Each coder (thread) must plug two dongles simultaneously to use the quantum compiler.
The simulation models:

- **Resource contention** – more coders than simultaneous compile slots.
- **Deadlock avoidance** – consistent lock ordering prevents circular wait.
- **Fair scheduling** – FIFO (First In, First Out) or EDF (Earliest Deadline First) arbitration.
- **Dongle cooldown** – released dongles stay unavailable for a configurable time.
- **Burnout deadlines** – a coder must start a new compile within `time_to_burnout` ms, or the simulation stops.

Coders cycle through **compiling → debugging → refactoring**. The simulation ends when all coders have compiled a required number of times, or when any coder burns out.

# Instructions
Compilation → make command and compiler flags

Execution → full command‑line with an argument table (N, burnout, compile, debug, refactor, compilations, cooldown, scheduler)

Cleanup → make clean, make fclean

## Compilation

```bash
make
```


## Execution

```bash
./codexion number_of_coders time_to_burnout time_to_compile time_to_debug time_to_refactor number_of_compiles_required cooldown_time fifo|edf
```

## Example

```bash
./codexion 5 800 200 200 200 7 100 fifo
```

## Clean

```bash
make clean
make fclean
```

## Rebuild

```bash
make re
```

## Blocking cases handled

- **Deadlock prevention** – coders lock dongles in a fixed order (odd: left→right, even: right→left), breaking circular wait.
- **Starvation prevention** – each dongle uses a FIFO/EDF priority queue; only the highest‑priority waiter is woken. The immediate‑take path is allowed only when the queue is empty.
- **Cooldown handling** – `dongle_release` sets a `cooldown_until` timestamp; waiters spin or sleep until cooldown expires, always unlocking the mutex while idle.
- **Precise burnout detection** – a separate monitor thread checks deadlines every 500 µs under `sim->mutex`; burnout is logged within 10 ms tolerance.
- **Log serialisation** – all output is protected by `print_mutex`; no interleaved messages.

## Thread synchronization mechanisms

- **Mutexes (`pthread_mutex_t`)** – protect shared state:
  - `dongle->mutex` – dongle availability, cooldown timer, wait queue.
  - `sim->mutex` – global running flag, `compiles_done`, `last_time_compilation`.
  - `print_mutex` – serialises console output.
  - `coder->personal_mutex` – prevents lost wake‑ups on the personal condition variable.
- **Condition variables (`pthread_cond_t`)** – each coder has a `personal_cond`; when a dongle is released, the scheduler signals **only** the highest‑priority waiter, enforcing FIFO/EDF order without thundering herds.
- **Race condition prevention** – all shared reads/writes are performed under the appropriate mutex. The immediate‑take path checks `wait_queue == NULL` under `dongle->mutex` to avoid queue‑jumping.
- **Thread‑safe communication** – the monitor stops the simulation by setting `sim->running = 0` under `sim->mutex`, then signals every coder’s `personal_cond` so that any blocked coder wakes up, sees the stop flag, and exits cleanly.

# Resources
1) Threads basics
https://medium.com/@akshatarhabib/understanding-threads-in-c-c9feb5e9372a
2) Foundation: threads, shared memory, race conditions, mutex
https://www.codequoi.com/en/threads-mutexes-and-concurrent-programming-in-c/
3) Mutex deep understanding
https://www.geeksforgeeks.org/linux-unix/mutex-lock-for-linux-thread-synchronization/
4) Deadlock concept + 4 conditions
https://www.geeksforgeeks.org/operating-systems/introduction-of-deadlock-in-operating-system/
5) Deadlock (real explanation)
https://stackoverflow.com/questions/34512/what-is-a-deadlock
6) Deadlock detection & debugging
https://undo.io/resources/detecting-deadlocks-c-cplusplus/
7) Deadlock handling strategies
https://www.geeksforgeeks.org/operating-systems/handling-deadlocks/
8) Videos
https://www.youtube.com/watch?v=LjWug2tvSBU
 https://www.youtube.com/watch?v=rFYYifVj5gs

