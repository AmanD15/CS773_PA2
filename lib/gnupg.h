#ifndef GNUPG_H_
#define GNUPG_H_

#define gnupg_path "path_to_gnupg_binary"

#define base_add 0x08048000  		

#define square_add 0x08058023		//mpih-mul.c:270 (First cache line in mpih_sqr_n())

#define remainder_add 0x08058172	//mpih-div.c:329 (Loop in default case in mpihelp_divrem())

#define multiply_add 0x08058243	//mpih-mul.c:121 (First cache line of mul_n())

#endif

