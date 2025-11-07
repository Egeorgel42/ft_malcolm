#include "malcolm.h"

void	free_runtime(runtime *run)
{
	for (int i = 0; i < ERR_MAX; i++)
		free(run->err[i]);
	for (int i = 0; i < STEPS_MAX; i++)
		free(run->steps[i]);
	free(run->err);
	free(run->steps);
	free(run);
}

void stop_signal(int signal) {
	(void)signal;
    keepRunning = false;
}

int main(int argc, char** argv)
{
    struct sigaction act;
    act.sa_handler = stop_signal;
    sigaction(SIGINT, &act, NULL);

	runtime* run = malloc(sizeof(runtime));
	init_msgs(run);
	parse_params(argc, argv, run);
	check_signal_exit(run);
	arp(run);
	free_runtime(run);
}