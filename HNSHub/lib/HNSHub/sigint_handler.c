#ifdef __linux__

#include "sigint_handler.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>

struct sigaction act = {0};

void sigint_handler_init(){
	act.sa_handler = sigint_handler;
	if(sigaction(SIGINT, &act, NULL) ==-1){
		perror("sigaction error");
		exit(EXIT_FAILURE);
	}
}

void sigint_handler(int sig){
	exit(EXIT_SUCCESS);
}
#endif
