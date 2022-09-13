#include "../lib/util.h"

int main(int argc, char **argv) {

	char msg[51];
	unsigned int msg_len = 0;
	clock_t t_send;
	double trans_time;
	double trans_rate;

	// TODO: Establish your cache covert channel
	
	map_handle_t *handle;	 // declaring a handle for file mapping
	char *map;

	map = (char *) map_file("share_mem.txt", &handle);  // mapping a file in memory (virtual address space)
  
	// End TODO

	printf("Enter message to be send: ");
	fgets (msg, 50, stdin);
	msg_len = strlen(msg);


	// TODO: Transmit message over the cache covert channel
	char *binary;
	binary = string_to_binary(msg);
	
	// To get accuracy, writing to file and using python script
	/*
	FILE* file_ptr;
	file_ptr = fopen("accuracy_s.txt", "w");
	if (file_ptr == NULL) {
		printf("Could not open %s\n","accuracy_s.txt");
		exit(0);
	}
	fprintf(file_ptr,"%s\n",binary);
	fclose(file_ptr);
	*/
	
	// Send test sequence to tell receiver that transmission started
	unsigned int bin_len = strlen(binary);
	bool sequence[8] = {1,0,1,0,1,0,1,1};
	for (int i = 0; i < 8; i++) {
			send_bit(sequence[i], handle);
		}
	// Start timer for measurement
	t_send = clock();
	// Send data across covert channel
	for (int ind = 0; ind < bin_len; ind++) {
			if (binary[ind] == '0') {
				send_bit(false, handle);
			} else {
				send_bit(true, handle);
			}
		}
	// Stop timer
	t_send = clock() - t_send;

	unmap_file(handle); 
	// End TODO


	trans_time = ((double) t_send) / CLOCKS_PER_SEC;
	trans_rate = (double) (msg_len * 8) / trans_time;

	printf("[Sender] Total data Send : %u bytes\n", msg_len);
	printf("[Sender] Time to send data : %lf second\n", trans_time);
	printf("[Sender] Data transmission rate : %lu bps\n", (unsigned long) trans_rate);

	return 0;
}
