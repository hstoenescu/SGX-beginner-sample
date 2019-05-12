# Intel SGX - beginner's project

This is a fairly simple Intel SGX project that consists in only just 1 enclave (see .../Enclave folder).
The enclave implements general operations, like: add, diff, generate random number, read from a file/write to file using ocalls, seal and unseal data.

Used for inspiration the sample code found on this lab: https://ocw.cs.pub.ro/courses/isc/labs/03

**Usage:**  
(1) git clone https://github.com/hstoenescu/SGX-beginner-sample  
(2) cd SGX-beginner-sample  
(3) make SGX\_MODE=SIM (for simulation mode using SDK) or SGX\_MODE=HW (for using SGX in hardware mode)  
(4) run from the root folder the executable app: ./app  
(5) optional - fell free to change code, inspect it, add new one or test it  
