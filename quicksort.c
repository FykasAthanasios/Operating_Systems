#include "quicksort.h"


int main(int argc , char* argv[]){
    double t1 , t2 , cpu_time ;
    struct tms tb1 , tb2 ;
    double ticspersec ;
    int i, sum = 0;
    ticspersec = ( double ) sysconf ( _SC_CLK_TCK );
    t1 = ( double ) times (& tb1) ;
    if ( argc != 5){
        write( STDERR_FILENO , message6, strlen(message6));
        exit(1);
    }
    char* filename=argv[1];
    int fd = open( filename , O_RDONLY);
    //Check if fd= -1, then open has failed
    if ( fd == -1) { 
        write(STDERR_FILENO, message1, strlen(message1));
        exit(1);
    }
    int j=0;
    int start = atoi(argv[2]);
    int end = atoi(argv[3]);

    Record* Array=(Record*)malloc( (end-start) * sizeof(Record));
    memset(Array,0,(end-start)*sizeof(Record));
    if ( Array == NULL) {
        write(STDERR_FILENO , message9, strlen(message9));
    }
    lseek( fd , sizeof(Record)*(start), SEEK_SET);
    for(int i=0; i < (end-start) ; i++) {
        read(fd, &Array[i] , sizeof(Record));
    }
    Quicksort(Array, 0, end-start);
    int pipefd= atoi(argv[4]);
    for(int i=0; i < (end-start) ; i++) {
         //printf("%s %s\n", Array[i].lastName,Array[i].firstname );
        // fflush(stdout);
        write(1, &Array[i], sizeof(Record));
    }
    close(pipefd);
    free(Array);
    if( (close(fd) == -1)) {
        write( STDERR_FILENO, message2, strlen(message2));
        exit(1);
    }
    // printf("quicky!\n");
    // fflush(stdout);
    
    t2 = ( double ) times (& tb2) ;
    cpu_time = ( double ) (( tb2 . tms_utime + tb2 . tms_stime ) -
    ( tb1 . tms_utime + tb1 . tms_stime ));
    //printf ("Run time was %lf sec ( REAL time ) although we used the CPU for %lf sec ( CPU time ).\n", (t2 - t1) / ticspersec , cpu_time / ticspersec );
    //fflush(stdout);
    return 0;
}
