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

volatile sig_atomic_t keepRunning = 1;

void	check_signal_exit(runtime *run)
{
	if (!keepRunning)
	{
		print_step(STEP_EXIT, run);
		free_runtime(run);
		exit(0);
	}
}

void stop_signal(int signal) {
	(void)signal;
    keepRunning = 0;
}

int main(int argc, char** argv)
{
    struct sigaction act;
    act.sa_handler = stop_signal;
	sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGINT, &act, NULL);

	runtime* run = malloc(sizeof(runtime));
	init_msgs(run);
	parse_params(argc, argv, run);
	check_signal_exit(run);
	arp(run);
	free_runtime(run);
}