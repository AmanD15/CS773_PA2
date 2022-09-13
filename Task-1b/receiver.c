#include "../lib/util.h"

int main(int argc, char **argv) {

	char filename[51*8]; //buffer to receive filename
	char received_file[61] = "received_";
	unsigned int content_length = 0;
	clock_t t_recv;
	double recv_time;
	double recv_rate;

	// TODO: Establish your cache covert channel

	map_handle_t *handle;	 // declaring a handle for file mapping
	char *map;

	map = (char *) map_file("map_tmp.txt", &handle);  // mapping a file in memory (virtual address space)

  
	// End TODO
	
	filename[0] = '\0';

	/* TODO:
	 * Receive the filename from the sender process over covert channel
	 * Use filename buffer to store received file name
	*/
	uint32_t sequenceMask = ((uint32_t) 1<<6) - 1;
	uint32_t expSequence = 0b101011;
	uint32_t bitSequence = 0;
	char* file_n;
	while (1)
	{
		bool bitReceived = detect_bit(handle);
		bitSequence = ((uint32_t) bitSequence<<1) | bitReceived;
		if ((bitSequence & sequenceMask) == expSequence) {
			int strike_zeros = 0;
			for (int i = 0; i < MAX_BUFFER_LEN; i++) {
				bool det = detect_bit(handle);
				if (det) {
					filename[i] = '1';
					strike_zeros = 0;
				} else {
					filename[i] = '0';
					if (++strike_zeros >= 8 && i % 8 == 0) {
						break;
					}
				}
			}

			// Print out message
			file_n = binary_to_string(filename);
			break;
		}
	}	
	// End TODO

	strcat(received_file, file_n);

	// TODO: Create a text file with the filename stored in received_file buffer in write mode.
	printf("Opening %s\n",received_file);
	FILE *fptr;
	fptr = fopen(received_file, "w");
	if (fptr == NULL) {
		printf("Could not open %s\n",received_file);
		exit(0);
	}
	// End TODO
	t_recv = clock();

	/* TODO:
	 * Receive file contents from the sender process over covert channel
	 * Wrtie them into the file opened earlier
	 * store the length of the file content in content_length variable
	*/
	char* msg;
	bool set_bit = false;
	bitSequence = 0;
	msg[8] = '\0';
	while (1)
	{
		if (set_bit == false){
			bool bitReceived = detect_bit(handle);
			bitSequence = ((uint32_t) bitSequence<<1) | bitReceived;
			if ((bitSequence & sequenceMask) == expSequence) {
				set_bit = true;
			}
		}
		else
		{
			bool current_char = 0;
			for (int i = 0; i < 8; i++) {
				bool rec = detect_bit(handle);
				current_char = (current_char | rec);
				if (rec) {
					msg[i] = '1';
				}
				else {
					msg[i] = '0';
				}
			}
			if (current_char == false) break;
			content_length++;
			fprintf(fptr,"%s",binary_to_string(msg));
		}
	}

	unmap_file(handle); 
	fclose(file_ptr);

	// End TODO


	t_recv = clock() - t_recv;
	recv_time = ((double) t_recv) / CLOCKS_PER_SEC;
	recv_rate = (double) (content_length * 8) / recv_time;

	printf("[Receiver] File (%s) received : %u bytes\n", received_file, content_length);
	printf("[Receiver] Time taken to receive file : %lf second\n", recv_time);
	printf("[Receiver] Data receiving rate : %lu bps\n", (unsigned long) recv_rate);

	return 0;

}
