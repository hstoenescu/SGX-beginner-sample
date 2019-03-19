#include "string.h"
#include "Enclave_t.h"
#include <stdio.h>
/* TODO: Add SGX trusted libraries headers if needed */ 

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

int get_sum(int a, int b) {
	ocall_print("Adding numbers inside enclave...");
	return a + b;
}

// TODO 1: Generate a random unsigned int using a trusted library 
unsigned int generate_random_number() {
	return 10;	
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
