// #include <time.h>
#include "../lib/util.h"


int main(int argc, char **argv) {

	char filename[51], *p;
	unsigned int file_size = 0;
	clock_t t_send;
	double trans_time;
	double trans_rate;

	// TODO: Establish your cache covert channel
	
	map_handle_t *handle;	 // declaring a handle for file mapping
	char *map;

	map = (char *) map_file("map_tmp.txt", &handle);  // mapping a file in memory (virtual address space)
	
	// End TODO

	printf("File to be send: ");
	fgets (filename, 50, stdin);
	if ((p = strchr(filename,'\n')) != NULL) *p = '\0';

	/* TODO:
	 * Send filename to the receiver
	 * Open text file in read mode.
	 * Store the size of file in file_size variable.
	*/
	FILE* file_ptr;
	file_ptr = fopen(filename, "r");
	if (file_ptr == NULL) {
		printf("Could not open %s\n",filename);
		exit(0);
	}
	char *binary;
	binary = string_to_binary(filename);
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
	// Ensure channel is closed
	for (int i = 0; i < 16; i++) {
			send_bit(false, handle);
	}
	// End TODO
	
	t_send = clock();

	// TODO: Transmit file contents to the Receiver over the cache covert channel
	
	for (int i = 0; i < 8; i++) {
		send_bit(sequence[i], handle);
	}
	char ch[2];
	while ((ch[0] = fgetc(file_ptr)) != EOF){
		binary = string_to_binary(ch);
		file_size++;
		for (int ind = 0; ind < 8; ind++) {
				if (binary[ind] == '0') {
					send_bit(false, handle);
				} else {
					send_bit(true, handle);
				}
			}
	}
	file_size--;
		
	unmap_file(handle);
	
	// End TODO
	
	t_send = clock() - t_send;
	trans_time = ((double) t_send) / CLOCKS_PER_SEC;
	trans_rate = (double) (file_size * 8) / trans_time;

	printf("[Sender] File (%s) data Send : %u bytes\n", filename, file_size);
	printf("[Sender] Time to send file data : %lf second\n", trans_time);
	printf("[Sender] Data transmission rate : %lu bps\n", (unsigned long) trans_rate);

	return 0;
}
