final: mysort quicksort bubblesort

mysort: main.o merger.o
	gcc main.o merger.o -o mysort

quicksort: quicksort.o quicksort_implementation.o
	gcc quicksort.o quicksort_implementation.o -o quicksort

bubblesort: bubblesort.o bubblesort_implementation.o
	gcc bubblesort.o bubblesort_implementation.o -o bubblesort

main.o: main.c 
	gcc -c main.c

merger.o: merger.c
	gcc -c merger.c

quicksort.o: quicksort.c
	gcc -c quicksort.c

quicksort_implementation.o: quicksort_implementation.c
	gcc -c quicksort_implementation.c

bubblesort.o: bubblesort.c
	gcc -c bubblesort.c

bubblesort_implementation.o: bubblesort_implementation.c
	gcc -c bubblesort_implementation.c

clean: 
	rm -f main.o merger.o quicksort.o quicksort_implementation.o bubblesort.o bubblesort_implementation.o mysort quicksort bubblesort
