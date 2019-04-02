#include "string.h"
#include "Enclave_t.h"
#include <stdio.h>

// trusted libraries for SGX
#include "sgx_trts.h"
#include "sgx_tseal.h"

#define SECRET_FILE "enclave_secret"
#define RANDOM_FILE "enclave_secret_rand"

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

// write to file/read from file using ocalls
void write_to_file() {
	ocall_write_file ("test_file.txt", "horiahoria", 10);
}

void read_from_file() {
	char *buf = (char *) malloc(5);
	if (buf == NULL) {
		ocall_print("Failed to allocate space for buf\n");
	}
	ocall_read_file("test_file.txt", buf, 5);
	free(buf);
}

// Sealing function
void seal_secret() {
	/* PLAINTEXT part */
	// seal the string "SGX_RULLZ" using sgx_seal_data
	uint32_t PLAINTEXT_LEN = 9;
	char plaintext_str[PLAINTEXT_LEN] = "SGX_RULLZ";

	/* SEALED part */
	// get the cipher size using sgx_calc_sealed_data_size
	// this function determines how much memory we need to allocate
	// for sgx_sealed_data_t structure
	// [param1]: add_mac_txt_size   = len for opt additional data stream in B
	// [param2]: txt_encrypted_size = len for data stream to be encrypted in B
	uint32_t CIPH_SIZE = sgx_calc_sealed_data_size(0,PLAINTEXT_LEN);
	uint8_t* sealed = (uint8_t*) malloc(CIPH_SIZE);
	sgx_status_t res;

	res = sgx_seal_data(0,NULL,PLAINTEXT_LEN, (uint8_t *)plaintext_str,CIPH_SIZE,(sgx_sealed_data_t *) sealed);
	if (res != SGX_SUCCESS) {
		ocall_print("Error while sealing data on [ENCLAVE]...\n");
	} else {
		ocall_print("Successfully sealed data in [ENCLAVE]...\n");
		ocall_print("Writing the encrypted string to file..\n");

		// just for testing only
		//ocall_write_file("enclave_not_so_secret", plaintext_str, PLAINTEXT_LEN);
		ocall_write_file(SECRET_FILE, (char *) sealed, CIPH_SIZE);

		// just to see if the sealing was ok
		uint8_t* plaintext_decr = (uint8_t *) malloc(PLAINTEXT_LEN);
		res = sgx_unseal_data((sgx_sealed_data_t *) sealed, NULL,NULL,plaintext_decr, &PLAINTEXT_LEN);

		if (res != SGX_SUCCESS) {
			ocall_print("Failed to unseal the secret from file ...\n");
		} else {
			ocall_write_file("not_so_secret_string",(char *) plaintext_decr,PLAINTEXT_LEN);
		}
		free(plaintext_decr);
	}

	free(sealed);
}

// generate a random number and seal it to file
// void seal_secret_rand() {
// 	int RAND_LEN = 10;
// 	unsigned char *random_str = (unsigned char *) malloc(RAND_LEN);
// 	if (random_str == NULL) {
// 		ocall_print("Failed to allocate space for random string\n");
// 	}
// 	sgx_read_rand(random_str,RAND_LEN);
// 	printf ("len=%d\n", strlen((const char*)random_str));
// 	free(random_str);
// }

// Unsealing function
void unseal_secret(){

	// read the encrypted file
	uint32_t PLAINTEXT_LEN = 9;
	uint32_t CIPH_SIZE = sgx_calc_sealed_data_size(0,PLAINTEXT_LEN);

	char *encrypted_str = (char *) malloc(sizeof(char)*CIPH_SIZE);
	if (encrypted_str == NULL) { ocall_print ("Failed to allocate memory for buffer\n");}

	ocall_read_file(SECRET_FILE, encrypted_str,CIPH_SIZE);

  	// unseal the data and write to other file - 'not_so_secret_value'
	sgx_status_t res;
	uint8_t* plaintext_decr = (uint8_t *) malloc(PLAINTEXT_LEN);
	res = sgx_unseal_data((sgx_sealed_data_t *)encrypted_str, NULL, NULL, plaintext_decr, &PLAINTEXT_LEN );
	ocall_write_file("not_so_secret", (char*) plaintext_decr, PLAINTEXT_LEN);

	// de-allocate space for the encrypted string
	free(encrypted_str);
}

