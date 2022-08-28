#include "../lib/util.h"

int main(int argc, char **argv) {

	char msg[51]; //buffer to receive data
	unsigned int msg_len = 0;
	clock_t t_recv;
	double recv_time;
	double recv_rate;
	msg[0] = '\0';
	
	// TODO: Establish your cache covert channel
	
	map_handle_t *handle;	 // declaring a handle for file mapping
	char *map;

	map = (char *) map_file("map_tmp.txt", &handle);  // mapping a file in memory (virtual address space)

  
	// End TODO

	t_recv = clock();

	// TODO: synchronize with sender and receive data in msg buffer.
	
	printf("%p\n",map);
	for (int i = 0; i<10; i++){
	clflush(&map);
	printf("%ld\n",measure_one_block_access_time(&map));}
	unmap_file(handle); 
	// End TODO


	t_recv = clock() - t_recv;
	msg_len = strlen(msg);
	recv_time = ((double) t_recv) / CLOCKS_PER_SEC;
	recv_rate = (double) (msg_len * 8) / recv_time;

	printf("[Receiver] Received data : %s\n", msg);
	printf("[Receiver] Total data received : %u bytes\n", msg_len);
	printf("[Receiver] Time taken to receive data : %lf second\n", recv_time);
	printf("[Receiver] Data receiving rate : %lu bps\n", (unsigned long) recv_rate);

	return 0;

}
