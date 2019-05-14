# Intel SGX - beginner's project

This is a fairly simple Intel SGX project that consists in only just 1 enclave (see .../Enclave folder).
The enclave implements general operations, like: add, diff, generate random number, read from a file/write to file using ocalls, seal and unseal data.

Used for inspiration the sample code and exercises found on this lab: https://ocw.cs.pub.ro/courses/isc/labs/03

The skeleton used for this project is found in this repo: 02-sgx-initial.tar.gz

**Usage**  
Clone and change to this directory:  
```console
user@hostname $ git clone https://github.com/hstoenescu/SGX-beginner-sample
Cloning into 'SGX-beginner-sample'...
[...]
user@hostname $ cd SGX-beginner-sample 
```
**Usage SIM mode**  
(1.1) Compile the code for SIM mode
```console
user@hostname $ make SGX_MODE=SIM # for simulation mode using SDK
GEN  =>  App/Enclave_u.c
CC   <=  App/Enclave_u.c
CXX  <=  App/App.cpp
CXX  <=  App/sgx_utils/sgx_utils.cpp
LINK =>  app
[...]
# Note that can be compiled with adding SGX_MODE as default value is 1
````
(2.1) Run from the root folder the executable app (sample output):  
```console
user@hostname $ ./app  
Adding numbers inside enclave...
Sum from enclave: 7
Diff between 2 numbers inside the enclave...
Diff from enclave: 5
Random number from enclave: 6
Successfully wrote to file
[...]
```

**Usage HW mode**  
(1.2) Compile the code for HW mode with SGX\_DEBUG or SGX\_PRERELEASE set. Only one of it needs to be set - see [here](https://github.com/hstoenescu/SGX-beginner-sample/blob/master/Makefile#L56).  
By using one of these 2 flags, the code will run in enclave-debug mode. For the moment, it doesn't work in [release mode](https://github.com/hstoenescu/SGX-beginner-sample/blob/master/Makefile#L140s).  
Moreover, the signature for non-release mode is generated automatically by the makefile - see [compilation_signing](https://github.com/hstoenescu/SGX-beginner-sample/blob/master/Makefile#L160) and [signing](https://github.com/hstoenescu/SGX-beginner-sample/blob/master/Makefile#L206).  

```console
user@hostname $ make SGX_MODE=HW SGX_DEBUG=1 # or make SGX_MODE=HW SGX_PRERELEASE=1
GEN  =>  App/Enclave_u.c
CC   <=  App/Enclave_u.c
CXX  <=  App/App.cpp
CXX  <=  App/sgx_utils/sgx_utils.cpp
LINK =>  app
[...]
```
(2.2) Run and the same output will appear:
```console
user@hostname $ ./app
Adding numbers inside enclave...
Sum from enclave: 7
Diff between 2 numbers inside the enclave...
Diff from enclave: 5
Random number from enclave: 1
Successfully wrote to file
[...]
```
(3) To clean the files, simply use:
```console
user@hostname $ make clean # it removes the application, objs, wrappers, signed enclave and created files
```
(4) optional - fell free to change code, inspect it, add new one or test it  
