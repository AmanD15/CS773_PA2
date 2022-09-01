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

	map = (char *) map_file("map_tmp.txt", &handle);  // mapping a file in memory (virtual address space)
  
	// End TODO

	printf("Enter message to be send: ");
	fgets (msg, 50, stdin);
	msg_len = strlen(msg);

	t_send = clock();

	// TODO: Transmit message over the cache covert channel
	char *binary;
	binary = string_to_binary(msg);
	unsigned int bin_len = strlen(binary);
	bool sequence[8] = {1,0,1,0,1,0,1,1};
	for (int i = 0; i < 8; i++) {
			send_bit(sequence[i], handle);
		}
	for (int ind = 0; ind < bin_len; ind++) {
			if (binary[ind] == '0') {
				send_bit(false, handle);
			} else {
				send_bit(true, handle);
			}
		}

	unmap_file(handle); 
	// End TODO


	t_send = clock() - t_send;
	trans_time = ((double) t_send) / CLOCKS_PER_SEC;
	trans_rate = (double) (msg_len * 8) / trans_time;

	printf("[Sender] Total data Send : %u bytes\n", msg_len);
	printf("[Sender] Time to send data : %lf second\n", trans_time);
	printf("[Sender] Data transmission rate : %lu bps\n", (unsigned long) trans_rate);

	return 0;
}
