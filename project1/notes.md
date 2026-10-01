For reduction
```c++
#pragma omp parallel for reduction(operator: variable)
```

ex: `reduction(+: totalSum)`