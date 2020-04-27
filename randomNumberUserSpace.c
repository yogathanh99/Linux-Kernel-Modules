#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>  // Required for read function
#include <fcntl.h>   // Required for O_RDONLY -> Open for Reading Only

int main()
{
	int fd, randNum;

	printf("Starting Random Number Device\n");

	// Open the device with READ ONLY access
	fd = open("/dev/RandomNumberDevice", O_RDONLY); 
	if (fd < 0){
		perror("Failed to open the device");
		return errno;
	}

	// Read from RNG device
	printf("Reading from Random Number Device\n");
	read(fd, &randNum, sizeof(randNum));
	
	printf("Random number is generated = %d\n", randNum);
	printf("End of the program\n");
	return 0;       	
}