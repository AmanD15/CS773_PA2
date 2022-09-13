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

	map = (char *) map_file("share_mem.txt", &handle);  // mapping a file in memory (virtual address space)
	
	// End TODO

	// Get filename as input
	printf("File to be send: ");
	fgets (filename, 50, stdin);
	if ((p = strchr(filename,'\n')) != NULL) *p = '\0';

	/* TODO:
	 * Send filename to the receiver
	 * Open text file in read mode.
	 * Store the size of file in file_size variable.
	*/
	
	// Open file
	FILE* fptr;
	fptr = fopen(filename, "r");
	if (fptr == NULL) {
		printf("Could not open %s\n",filename);
		exit(0);
	}
	
	// Send the filename using same method as Task 1a
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
	// Ensure channel is closed by sending extra zeros
	for (int i = 0; i < 16; i++) {
			send_bit(false, handle);
	}
	// End TODO
	

	// TODO: Transmit file contents to the Receiver over the cache covert channel
	
	// Read file contents - done to prevent lot of file IO during transmit
	int i = 0;
	
	// Get length of file to create buffer
	fseek( fptr , 0L , SEEK_END);
    int lSize = ftell( fptr );
    rewind( fptr );
	char ch[lSize+1], read_char;
    
	while ((read_char = fgetc(fptr)) != EOF)
	{
		ch[i] = read_char;
		i++;file_size++;
	}
	ch[i] = '\0';

	// Wait for receiver to be ready
	io_sync();

	// Send the file contents
	binary = string_to_binary(ch);
	bin_len = strlen(binary);
	int ind = 0;
	bool done = false;
	
	// Start timer
	t_send = clock();
	while (!done){
		for (int i = 0; i < 8; i++) {
			send_bit(sequence[i], handle);
		}
		for (int j = 0; j < MAX_BUFFER_LEN/8; j++) {

			// Send the test sequence
			// Send data
			if (binary[ind] == '0') {
				send_bit(false, handle);
			} else {
				send_bit(true, handle);
			}
			ind++;
			if (ind == bin_len) {done = true;break;}
		}
		
		// Send zeros when done
		if (done)
		{for (int i = 0; i < 32; i++) {
			send_bit(false, handle);
		}}
		// Wait for receiver to be ready
		io_sync();
	}
	// Ensure channel is closed by sending extra zeros
	// Wait for receiver to be ready
	io_sync();
	for (int i = 0; i < 8; i++) {
			send_bit(sequence[i], handle);
	}
	for (int i = 0; i < 32; i++) {
			send_bit(false, handle);
	}
	
	// Print file data to stdout
	printf("%s\n",ch);
	
	// To get accuracy, writing to file and using python script
	FILE* file_ptr;
	file_ptr = fopen("accuracy_s.txt", "w");
	if (file_ptr == NULL) {
		printf("Could not open %s\n","accuracy_s.txt");
		exit(0);
	}
	fprintf(file_ptr,"%s\n",binary);
	fclose(file_ptr);
	
	// Unmap handle and close file pointer
	unmap_file(handle);
	fclose(fptr);
	
	// End TODO
	
	t_send = clock() - t_send;
	trans_time = ((double) t_send) / CLOCKS_PER_SEC;
	trans_rate = (double) (file_size * 8) / trans_time;

	printf("[Sender] File (%s) data Send : %u bytes\n", filename, file_size);
	printf("[Sender] Time to send file data : %lf second\n", trans_time);
	printf("[Sender] Data transmission rate : %lu bps\n", (unsigned long) trans_rate);

	return 0;
}
