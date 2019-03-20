#include "string.h"
#include "Enclave_t.h"
#include <stdio.h>
/* TODO: Add SGX trusted libraries headers if needed */ 
#include "sgx_trts.h"

#define SECRET_FILE "enclave_secret"

void printf(const char *fmt, ...)
{
    char buf[BUFSIZ] = {'\0'};
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, BUFSIZ, fmt, ap);
    va_end(ap);
    ocall_print(buf);
}

/* Simple functions - just to simply test intel sgx */
int get_sum(int a, int b) {
	ocall_print("Adding numbers inside enclave...");
	return a + b;
}


int get_diff (int a, int b) {
	ocall_print ("Diff between 2 numbers inside the enclave...");
	return a-b;
}

// Generate a random unsigned int using a trusted library 
unsigned int generate_random_number() {
	int max_value = 42, min_value = 1;
	unsigned int rand_nr;
	sgx_read_rand((unsigned char *) &rand_nr, sizeof(unsigned int));
	return rand_nr % (max_value + 1 - min_value) + min_value;
}

void write_to_file() {
	ocall_write_file ("test_file.txt", "horiahoria", 10);
}

void read_from_file() {
	char *buf;
	ocall_read_file("test_file.txt", buf, 5);
}

/* TODO 3: Sealing function 
void seal_secret() {
	// Add code here to seal "SGX_RULLZ".
	// TODO 4: Generate random string to seal.
}
*/

/* TODO 5: Unsealing function
void unseal_secret(){
	// Add code here
}
*/
