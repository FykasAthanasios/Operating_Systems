#include "Errors.h"
#include "signals.h"
#include "merge.h"

int main(int argc, char* argv[]) {
    
    signal(SIGUSR1, signal_handler);
    signal(SIGUSR2, signal_handler);
    
    if (argc!=9) {//Check if input syntax is correct
      	write(STDERR_FILENO, message3, strlen(message3));
      	exit(1);
   	}
    char* filename=argv[2];
    int fd = open( filename , O_RDONLY);
    
    if ( fd == -1) { //Check if fd= -1, then open has failed
        write(STDERR_FILENO, message1, strlen(message1));
        exit(1);
    }
    off_t file_size;
    file_size= lseek( fd, 0 , SEEK_END);
    
    if ( file_size == -1) {//Check if lseek fails
        write(STDERR_FILENO, message4, strlen(message4));
        exit(1);
    }
    
    int num_records= file_size / (sizeof(Record));
    // printf("file size:%d\n",num_records);
    
    if ( (close(fd) == -1)) {//Check if close fails
        write(STDERR_FILENO, message2, strlen(message2));
        exit(1);
    }

    int num_children = atoi( argv[4]);
    int pid_root, pid_mergers;
    int records_per_child = num_records / num_children;

    int pipe_parent_child[num_children][2];//Create pipes
    for (int i = 0; i < num_children; i++) {
        if (pipe(pipe_parent_child[i]) == -1) {
            write(STDERR_FILENO, message8, strlen(message8));
            exit(1);
        }
    }

    int*** pipes_child_grandchild = malloc(num_children * sizeof(int**));
    if (pipes_child_grandchild == NULL) {
        write(STDERR_FILENO, message9, strlen(message9));
        exit(1);
    }

    for (int i = 0; i < num_children; i++) {
        int num_grandchildren = num_children - i;  // Number of grandchildren for this child
        pipes_child_grandchild[i] = malloc(num_grandchildren * sizeof(int*));
        if (pipes_child_grandchild[i] == NULL) {
            write(STDERR_FILENO, message9, strlen(message9));
            exit(1);
        }

        for (int j = 0; j < num_grandchildren; j++) {
            pipes_child_grandchild[i][j] = malloc(2 * sizeof(int));
            if (pipes_child_grandchild[i][j] == NULL) {
                write(STDERR_FILENO, message9, strlen(message9));
                exit(1);
            }
            if (pipe(pipes_child_grandchild[i][j]) == -1) {
                write(STDERR_FILENO, message8, strlen(message8));
                exit(1);
            }
        }
    }                                        
    for( int i=0; i < num_children ; i++ ) {
        pid_root = fork();
        //Check if fork failed
        if ( pid_root == -1) {
            write(STDERR_FILENO, message5, strlen(message5));
            exit(1);
        }
        //Check if in child process
        if ( pid_root == 0) {
            signal(SIGUSR1, SIG_DFL);
            signal(SIGUSR2, SIG_DFL);
            close(pipe_parent_child[i][READ]);//Close read end in pipe with parent process
            
            int parent_pid=getppid();

            int child_start = i * records_per_child;
            int num_grandchildren = num_children - i;
            int records_per_grandchild = records_per_child / num_grandchildren;

            for(int j=1; j<= num_grandchildren; j++) {
                pid_mergers = fork();
                
                if ( pid_mergers == -1) {//Check if fork fails
                    write(STDERR_FILENO, message5, strlen(message5));
                    exit(1);
                }

                if ( pid_mergers == 0) { //Im in grandchild process
                    close(pipes_child_grandchild[i][j-1][READ]);
                    int start = child_start + (j - 1) * records_per_grandchild;
                    int end_line = start + records_per_grandchild;

                    if (j == num_grandchildren) {
                        end_line = child_start + records_per_child; // Handle any remainder
                    }
                    if (i == num_children-1) { //We are at the last merger, so were are going to add any remaining records left
                        end_line += num_records%num_children;
                    }
                    
                    char startStr[20];
                    char endStr[20];
                    char pipefdStr[20];
                    sprintf(startStr, "%d", start);
                    sprintf(endStr, "%d", end_line);
                    sprintf(pipefdStr, "%d", pipes_child_grandchild[i][j-1][WRITE]);
                    kill(parent_pid, SIGUSR1);
                    dup2(pipes_child_grandchild[i][j-1][WRITE],1);
                    
                    if ( (j%2) == 0) { // This means we execute with quicksort
                        
                        execl( argv[6], argv[6],argv[2],startStr, endStr ,pipefdStr, (char *) NULL);
                    }
                    else { // This means we execute with bubblesort
                        
                        execl( argv[8], argv[8],  argv[2],startStr, endStr , pipefdStr, (char *) NULL);
                    }
                    //if exec fails, then free allocated memory and exit
                    for (int i = 0; i < num_children; i++) {
                        for (int j = 0; j < num_children - i; j++) {
                            free(pipes_child_grandchild[i][j]);
                        }
                        free(pipes_child_grandchild[i]);
                    }
                    free(pipes_child_grandchild);
                    free(pipe_parent_child);
                    exit(1);
                }
                else { // im in child process
                    // while(wait(NULL)>0); //waiting for EACH child 
                    close(pipes_child_grandchild[i][j-1][WRITE]);
                }
            }
            //waiting for all my granchildren processes to end writing
            // while(wait(NULL)>0);
            records_per_child= num_records/num_children;
            if (i == num_children-1) {
                records_per_child+= num_records%num_children;
            }
            //Reading from all the grandchildren of a signle child and storing them in Array, then merging them , and finally writing them to parent_child_ pipe
            Record* Array = (Record*)malloc(records_per_child* sizeof(Record));
            int Merger_Records=0;
            int k ;
            for( k=0; k < num_grandchildren; k++){ 
                //Because records_per_grandchild has special cases, update when needed, which is when were at the last merger, or the last sorter
                if ( i == num_children-1) {
                    records_per_grandchild += num_records%num_children;
                }
                if( k == num_grandchildren -1) {
                    records_per_grandchild += records_per_child%num_grandchildren;
                }
                int count=0;
                while( Merger_Records< records_per_child &&(count++ < records_per_grandchild) && (read(pipes_child_grandchild[i][k][READ], &Array[Merger_Records++], sizeof(Record))==sizeof(Record)));
                close(pipes_child_grandchild[i][k][READ]);
            }
            
            
            records_per_grandchild = records_per_child / num_grandchildren;

            int currentSize = records_per_grandchild; // Initial size of the merged array

            for (int k = 0; k < num_grandchildren - 1; k++) {
                int left = 0; // Always start from the beginning of the array
                int mid = left + currentSize - 1; // Mid is the end of the merged array so far
                int right = mid + records_per_grandchild; // Right is the end of the next subarray

                if (k == num_grandchildren - 2) { // If this is the last merge
                    right += records_per_child % num_grandchildren; // Add any leftover records
                }

                merge(Array, left, mid, right);
                currentSize += records_per_grandchild; // Increase the size of the merged array
            }
            for( k=0 ; k < records_per_child; k++) {
                write( pipe_parent_child[i][WRITE] , &Array[k] , sizeof(Record));
            }
            kill(parent_pid, SIGUSR2);
            close(pipe_parent_child[i][WRITE]);
            //Before exiting, free the allocated space for the pipes
            free(Array);
            for (int i = 0; i < num_children; i++) {
                for (int j = 0; j < num_children - i; j++) {
                    free(pipes_child_grandchild[i][j]);
                }
                free(pipes_child_grandchild[i]);
            }
            free(pipes_child_grandchild);
            exit(0);
        }
        else {
            // while(wait(NULL)>0);
            close(pipe_parent_child[i][WRITE]);
        }
    }
    //reading all results
    Record* Array=(Record*)malloc(num_records * sizeof(Record));
    int Root_Records=0;
    for ( int i=0 ; i < num_children ; i++) {
        records_per_child = num_records / num_children;
        if ( i == num_children-1) {
            records_per_child += num_records % num_children;
        }
        int count=0, b;
        while( Root_Records<num_records && count<records_per_child && (b=(read( pipe_parent_child[i][READ], &Array[Root_Records] , sizeof(Record)))==sizeof(Record))) {
            Root_Records++;
            count++;
        }
    }
    int records_per_segment = num_records / num_children;
    int currentMergedSize = records_per_segment; // Initial size of the merged array

    for (int i = 0; i < num_children - 1; i++) {
        int left = 0; // Always start from the beginning of the array
        int mid = currentMergedSize - 1; // Mid is the end of the merged array so far
        int right = mid + records_per_segment; // Right is the end of the next segment

        // Adjust the right index in the last iteration to include leftover records
        if (i == num_children - 2) {
            right += num_records % num_children; 
        }

        merge(Array, left, mid, right);
        currentMergedSize += records_per_segment; // Update the size of the merged array
    }


    for ( int k =0 ; k<num_records ; k++ ) {
        printf("%d %s %s %s\n",Array[k].id,Array[k].lastName,Array[k].firstname,Array[k].TK);
    }
    printf("SIGUSR1 count: %d\n", signal1);
    printf("SIGUSR2 count: %d\n", signal2);
    //Before exiting, free the allocated space for the Array and the pipes
    for (int i = 0; i < num_children; i++) {
        for (int j = 0; j < num_children - i; j++) {
            free(pipes_child_grandchild[i][j]);
        }
        free(pipes_child_grandchild[i]);
    }
    free(pipes_child_grandchild);
    free(Array);
    return 0;
}
