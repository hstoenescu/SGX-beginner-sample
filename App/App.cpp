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

// implement write to file - use fopen, fwrite
void ocall_write_file(const char* filename, const char* buf, size_t buf_len) {
    FILE *fp;

    // write to file - filename the buf string with len = buf_len
    fp = fopen(filename, "w+");
    if (fp != NULL) {
	int ret_val = fwrite(buf, sizeof(char), buf_len, fp);
	if (ret_val != buf_len) {
		printf ("Failed to write to file - returned value: %d\n", ret_val);
		// not returning anything as we need to close the stream
	}

	int out_fclose = fclose(fp);
	if (out_fclose != 0) {
		printf ("Failed to close the file - %s\n", filename);
		exit(0);
	}
    } else {
	printf ("Failed to open the file - %s - for writing\n", filename);
	exit(0);
    }
}

// implement read from fil - use fopen, fread
void ocall_read_file(const char* filename, char* buf, size_t buf_len) {

    int result;
    FILE *fp;

    fp = fopen(filename, "r");
    if (fp != NULL) {
	result = fread(buf, sizeof(char), buf_len, fp);
	buf[buf_len] = '\0';
	if (result != buf_len) {
		printf ("Reading error from the file - %s\n", filename);
	} else {
		printf ("The first %d chars from the file %s: %s\n", int(buf_len), filename,buf);
	}
	fclose(fp);

    } else {
	printf ("Failed to open the file - %s - for reading\n", filename);
    	exit(1);
    }

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
    /* for a list of error codes, see: 
     * https://github.com/intel/linux-sgx/blob/master/common/inc/sgx_error.h 
     */
    status = get_sum(global_eid, &sum_result, 3, 4);
    if (status != SGX_SUCCESS) {
        printf("ECall failed.\n");
        return 1;
    }
    printf("Sum from enclave: %d\n", sum_result);

    int diff_result;
    status = get_diff(global_eid, &diff_result, 10, 5);
    if (status != SGX_SUCCESS) {
	printf ("\nEcall failed for diff function - error %x", status);
	return 1;
    }
    printf ("Diff from enclave: %d\n", diff_result);


    /* TODO 1: Using an ECALL that generates a random unsigned int,
    get a random number between 1 and 42. */ 
    unsigned int rand_result;
    status = generate_random_number(global_eid, &rand_result);
    if (status != SGX_SUCCESS) {
	printf ("\nEcall failed for random number generator - error %x\n", status);
	return 1;
    }
    printf ("Random number from enclave: %d\n", rand_result);

    /* Test the ecall to write something to a file - use the ocall implemented above */
    status = write_to_file(global_eid);
    if (status != SGX_SUCCESS) {
	printf ("Ecall failed for writing something to file - error %x\n", status);
	return 1;
    }
    printf ("Successfully wrote to file\n");

    /* Test now the ecall to read the first 5 chars from the file - use the ocall from above */
    status = read_from_file(global_eid);
    if (status != SGX_SUCCESS) {
	printf ("Ecall failed for reading the first 5 chars from the file - error %x\n", status);
	return 1;
    }
    printf ("Successfully read from file\n");

    // note: when printing a failure message, print also the the returned value for it!!
    seal_secret(global_eid);
    unseal_secret(global_eid);

    // seal a secret random value generated in the function also
    //seal_secret_rand(global_eid);

    return 0;
}
