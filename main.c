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

int main(int argc, char** argv)
{
	runtime* run = malloc(sizeof(runtime));
	init_msgs(run);
	parse_params(argc, argv, run);
	arp(run);
	free_runtime(run);
}