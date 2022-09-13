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

	map = (char *) map_file("share_mem.txt", &handle);  // mapping a file in memory (virtual address space)

  
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
			// To keep count of all 0s 
			int strike_zeros = 0;

			// Detection algorithm - loop over max_length till a sequence of 8 zeros seen
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

	/* TODO:
	 * Receive file contents from the sender process over covert channel
	 * Wrtie them into the file opened earlier
	 * store the length of the file content in content_length variable
	*/
	
	// To get accuracy, writing to file and using python script
	FILE* file_ptr;
	file_ptr = fopen("accuracy_r.txt", "w");
	if (file_ptr == NULL) {
		printf("Could not open %s\n","accuracy_r.txt");
		exit(0);
	}

	
	char msg[MAX_BUFFER_LEN];
	bitSequence = 0;
	int i = 0, j = 0;
	bool break_val = false;
	t_recv = clock();
	while (1)
	{
		bool bitReceived = detect_bit(handle);
		bitSequence = ((uint32_t) bitSequence<<1) | bitReceived;
		
		// If received the sequence
		if ((bitSequence & sequenceMask) == expSequence) {
		
			// To keep count of all 0s 
			int strike_zeros = 0;
			
			// Detection algorithm - loop over max_length till a sequence of 8 zeros seen
			for (i = 0; i < MAX_BUFFER_LEN/8; i++) {
				if ((i & 7) == 7) content_length++;
				if (detect_bit(handle)) {
					msg[i] = '1';
					strike_zeros = 0;
				} else {
					msg[i] = '0';
					// Set break_val if 0s seen
					if (++strike_zeros >= 16 && i % 8 == 0) {
						break_val = true;
						break;
					}
				}
			}

			// Print out message and write to file
			msg[i+1] = '\0';
			printf("%s", binary_to_string(msg));
			fprintf(file_ptr,"%s",msg);
			fprintf(fptr,"%s",binary_to_string(msg));
			
			// If exit sequence received, then exit. Else, wait for next set of bits after synchronisation
			if (break_val) break;
		}
	}

	unmap_file(handle); 
	fclose(file_ptr);
	fclose(fptr);

	// End TODO


	t_recv = clock() - t_recv;
	recv_time = ((double) t_recv) / CLOCKS_PER_SEC;
	recv_rate = (double) (content_length * 8) / recv_time;

	printf("[Receiver] File (%s) received : %u bytes\n", received_file, content_length);
	printf("[Receiver] Time taken to receive file : %lf second\n", recv_time);
	printf("[Receiver] Data receiving rate : %lu bps\n", (unsigned long) recv_rate);

	return 0;

}
