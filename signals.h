#include "lib.h"

int signal1=0;
int signal2=0;

void signal_handler(int signum) {
    if (signum == SIGUSR1) {
        signal1++;
    }
	if (signum == SIGUSR2) {
		signal2++;
	}
}
