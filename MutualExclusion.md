# Mutual Exclusion

## Goal
The goal is to verify whether the following algorithm guarantees mutual exclusion, so that only one program can reach its critical section at a time.

## Algorithm
The algorithm to verify is the following
```c
while TRUE {
 flag [ i ] = TRUE ;
 while ( turn != i ) {
  while ( flag [ j ]) { 
   noop ;
  }
  turn = i ;
 }
 criticalSection ( Pi ) ;
 flag [ i ] = FALSE ;
 remainderSection ( Pi ) ;
}
```
With standard values flags = [false, false]
And two competing processes P0 and P1

## Solution
Since the initialization of the program is left for us, and the starting process is not specified, we are using the following values:

turn = 0
Assuming P1 begins execution, the following code gets executed:
```c
flag[1] = true
while(1 != 1){ // true
  while(flag[0]){} // false
  --- T0
  turn = 1
}
```
If we now assume that in a case the execution is interrupted at T0 and moved to P0, P0 will now run with
flags = [false, true]
turn = 0

```c
flag[0] = true
while(0 != 0){} // false
criticalSection()
```

P0 has now entered its critical section.
Assuming now that execution yet again is moved from P0 to P1, P1 will execute with
flags = [true, true]
turn = 0

```c
while(turn != 1)
  --- T0
  turn = 1 // breaks the while loop
}

criticalSection()
```

Both processes are now in their critical sections and therefore violate mutual exclusion. 
