#include "../lib/util.h"

int main(int argc, char **argv) {

	char msg[51*8]; //buffer to receive data
	unsigned int msg_len = 0;
	clock_t t_recv;
	double recv_time;
	double recv_rate;
	msg[0] = '\0';
	
	// TODO: Establish your cache covert channel
	
	map_handle_t *handle;	 // declaring a handle for file mapping
	char *map;

	map = (char *) map_file("share_mem.txt", &handle);  // mapping a file in memory (virtual address space)

  
	// End TODO


	// TODO: synchronize with sender and receive data in msg buffer.
	uint32_t sequenceMask = ((uint32_t) 1<<6) - 1;
	uint32_t expSequence = 0b101011;
	uint32_t bitSequence = 0;
	while (1)
	{
		bool bitReceived = detect_bit(handle);
		bitSequence = ((uint32_t) bitSequence<<1) | bitReceived;
		if ((bitSequence & sequenceMask) == expSequence) {
			int strike_zeros = 0;
			t_recv = clock();
			for (int i = 0; i < MAX_BUFFER_LEN; i++) {
				if (detect_bit(handle)) {
					msg[i] = '1';
					strike_zeros = 0;
				} else {
					msg[i] = '0';
					if (++strike_zeros >= 8 && i % 8 == 0) {
						break;
					}
				}
			}

			// Print out message
			printf("%s\n", binary_to_string(msg));
			break;
		}
	}	
	unmap_file(handle);
	// End TODO

	// To get accuracy, writing to file and using python script
	/*
	FILE* file_ptr;
	file_ptr = fopen("accuracy.txt", "a");
	if (file_ptr == NULL) {
		printf("Could not open %s\n","accuracy.txt");
		exit(0);
	}
	fprintf(file_ptr,"%s\n",msg);
	fclose(file_ptr);
	*/


	t_recv = clock() - t_recv;
	msg_len = strlen(msg)/8;
	recv_time = ((double) t_recv) / CLOCKS_PER_SEC;
	recv_rate = (double) (msg_len * 8) / recv_time;

	printf("[Receiver] Received data : %s\n", msg);
	printf("[Receiver] Total data received : %u bytes\n", msg_len);
	printf("[Receiver] Time taken to receive data : %lf second\n", recv_time);
	printf("[Receiver] Data receiving rate : %lu bps\n", (unsigned long) recv_rate);

	return 0;

}
