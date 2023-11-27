#  K22 Operating Systems - Assignment 2
# Fykas Athanasios- sdi2000282

## Compiling/For clean
```
make
make clean
```

## Running
```
For the  main program
./mysort -i DataFile -k NumofChildren -e1 ./quicksort -e2 ./bubblesort

For the algorithm programs (if you just need to use them seperatly on their own)
./quicksort DataFile StartLine EndLine FileDescriptor
./bubblesort DataFile StartLine EndLine FileDescriptor

```

## Notes

### The asssignment is fully implemented except the print of the time required for each sorter


Firstly about the two sorting algorithms, they start reading from StartLine ++ , for exp. if we give StartLine=0 and EndLine 50
it will read and sort records from the first line to the 50th line in the file given. Secondly for the merger funtion , the idea is 
that we merge two subarrays, and save the sorted records to the original array. It merges in a loop the subarray from 0 to the end of the previous pipe, and the right subarray from that end to the end of the current pipe. It creates two temporary arrays, allocates them and fills them up wiht the correct records, and then starting from 0 to lenght(subarray) it compares the two records( one from the left and one from the right subarray) and finally puts one of the two in the original array. Lastly the whole logic behind the nested forks and the loops for merging, reading and writing are based on values such as num_ records, num_children, num_grandchildren that change accordingly to where i am in the nested fork tree currently.  

