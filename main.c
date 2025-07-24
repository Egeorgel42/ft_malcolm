#include "malcolm.h"

int main(int argc, char** argv)
{
	runtime* run = malloc(sizeof(runtime));
	init_msgs(run);
	parse_params(argc, argv, run);
	test_arp();
	
}