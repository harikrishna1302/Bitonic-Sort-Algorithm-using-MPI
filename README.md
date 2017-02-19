# Bitonic-Sort-Algorithm-using-MPI
Bitonic  sort  algorithm  minimizes  the  number  of communication steps and increases the speedup when executed on multiple cores. The resulting algorithm is faster than the Quicksort when implemented in Parallel Computing process.
##  Bitonic Sequence:
A bitonic sequence is a sequence of values a_0, a_1,..a_(n−1) with the property that, there exists an index i, where o≤i≤n-1, such that a_0 through a_i is monotonically increasing and a_i through a_(n−1) is monotonically decreasing.
![untitled](https://cloud.githubusercontent.com/assets/18632383/23100685/2b350110-f655-11e6-8dde-9507d29e56d8.png)
## Bitonic Sort Algorithm
![untitled](https://cloud.githubusercontent.com/assets/18632383/23100703/a558adb6-f655-11e6-894a-3265e28e998d.png)
## Implementation
* Input, Number of Processors, data length.  
* Find the ranks of each processor.  
* Generate data in each processor using randomize function.  
* Sort the lists generated in the processor.  
* Compare and exchange data with a neighbor whose (d-bit binary) processor number differs only at the jth bit to merge the local subsequences.  
## Pseudo Code
```c 
d= 〖𝑙𝑜𝑔〗_2 P;
            sort (local -〖𝑑𝑎𝑡𝑎〗_k);
             for i=1 to d, do
              window –id = Most significant (d-i) bits of 𝑃_k.
                for j= i-1 down to 0 do
               if ((window-id is even AND jth bit of 𝑃_k = 0 )
                OR(window-id is odd AND jth bit of 𝑃_k = 1 ))
               then call CompareLow(j)
               else call CompareHigh(j)
               endif
                end for
                end for
```
