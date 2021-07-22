# Threads
An algorithm made to compare the execution time between using threads and don't.

# The function
The function used to compare the time of execution defines the total of [Perfect, Abundant and Defective](https://www.encyclopedia.com/education/news-wires-white-papers-and-books/numbers-abundant-deficient-perfect-and-amicable) numbers in an especified range (worksize).
It runs 3 times.
1º Sequential - runs without any thread, only using for loops and function calls.
2º Chunk distribution - divides the worksize in equal parts (workize/number of threads) and each part is designed to an unique thread.

3º Sparce distribution - divides the worksize "1 by 1". Each number is designed to one thread



# How to use
1. Compile `threads.c` using:
```
gcc -pthread threads.c -o threads
```
2. Now, execute the `threads` file. You'll need to pass the number of threads wanted and the worksize. Ex.:
```
./threads 4 100
          ^  ^  
```
`4` being the number of threads and `100` the worksize.
3. The output for the above command would be:

