# Threads
An algorithm made to compare the execution time between using threads and don't.

# The function
The function used to compare the time of execution defines the total of [Perfect, Abundant and Defective](https://www.encyclopedia.com/education/news-wires-white-papers-and-books/numbers-abundant-deficient-perfect-and-amicable) numbers in an especified range (worksize).

It uses 3 methods:

**1º Sequential** - runs without any thread, only using for loops and function calls.

**2º Chunk distribution** - divides the worksize in equal parts (workize/number of threads) and each part is designed to an unique thread.

![image](https://user-images.githubusercontent.com/55641441/126576365-ea88a83d-9678-4e4c-a3b4-c40f13371736.png)


**3º Sparce distribution** - divides the worksize "1 by 1". Each number is designed to one thread

![image](https://user-images.githubusercontent.com/55641441/126576371-076bf04c-2538-4018-a483-dbaa54714f51.png)


# How to use
1. Compile `threads.c` using:
```
gcc -pthread threads.c -o threads
```
2. Now, execute the `threads` file. You'll need to pass the number of threads wanted and the worksize. Ex.:
```
./threads 4 100 
```
`4` being the number of threads and `100` the worksize.

3. The output for the above command would be:
```
----------------------------------------------
* Sequential	[P]	[A]	[D]	[WTot]	
		2	22	76	100	
----------------------------------------------
* Chunk 	[P]	[A]	[D]	[WTh]
- Thread [1]: 	1	4	20	25	
- Thread [2]: 	1	5	19	25	
- Thread [3]: 	0	6	19	25	
- Thread [4]: 	0	7	18	25	
  [TOTAL] 	2	22	76	100
----------------------------------------------
* Sparse 	[P]	[A]	[D]	[WTh]
- Thread [1]: 	0	0	25	25	
- Thread [2]: 	1	8	16	25	
- Thread [3]: 	0	0	25	25	
- Thread [4]: 	1	14	10	25	
  [TOTAL] 	2	22	76	100
----------------------------------------------
Sequential time:  0.000024s
Using threads time: 0.000150s (chunk distribution)
Using threads time: 0.000084s (sparse distribution)
```


