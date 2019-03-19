#include <stdio.h>

#include "Enclave_u.h"
#include "sgx_urts.h"
#include "sgx_utils/sgx_utils.h"

/* Global Enclave ID */
sgx_enclave_id_t global_eid;

/* OCall implementations */
void ocall_print(const char* str) {
    printf("%s\n", str);
}

void ocall_write_file(const char* filename, const char* buf, size_t buf_len) {
    /* TODO 2: Implement write */
}

void ocall_read_file(const char* filename, char* buf, size_t buf_len) {
    /* TODO 2: Implement write */
}

int main(int argc, char const *argv[]) {
    int sum_result;
    sgx_status_t status;
  
    /* Enclave Initialization */ 
    if (initialize_enclave(&global_eid, "enclave.token", "enclave.signed.so") < 0) {
        printf("Fail to initialize enclave.\n");
        return 1;
    }

    /* Call a simple method inside enclave */ 
    status = get_sum(global_eid, &sum_result, 3, 4);
    if (status != SGX_SUCCESS) {
        printf("ECall failed.\n");
        return 1;
    }
    printf("Sum from enclave: %d\n", sum_result);
    /* TODO 1: Using an ECALL that generates a random unsigned int,
    get a random number between 3 and 42. */ 
   
    // note: when printing a failure message, print also the the returned value for it!!!
    unsigned int rand_number; 
    status = generate_random_number(global_eid, &rand_number);
    if (status != SGX_SUCCESS) {
	printf ("Ecall failed\n -> %x", status);
	return 1;
    }
    printf ("The random value returned from the enclave is: %d\n", rand_number);

    /* TODO 3, TODO 4: Uncomment sealing/unsealing calls */ 
    /* seal_secret(global_eid); */
    /* unseal_secret(global_eid); */

    return 0;
}
