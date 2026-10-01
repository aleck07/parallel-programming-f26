For reduction
```c++
#pragma omp parallel for reduction(operator: variable)
```

ex: `reduction(+: totalSum)`

Talk about the processor in the write up, such as how many cores, is it hyper threaded, and cache sizes. Possibly information about memory speed and clock speed can be helpful.