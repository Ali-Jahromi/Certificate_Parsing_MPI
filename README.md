# Parsing Certificates using Openssl and MPI
Parsing X.509 Certificates using Openssl library and message passing interface (MPI).

### Required Packages:
* libopenmpi-dev
* libssl-dev

### Compile and Run the Program:
```
mpic++ -std=c++14 mpcheck.cpp -o <output-executable-file-name> -lssl -lcrypto  //Compile 
mpirun -np <#processors(4)> execfile file  //Run
```


## TODO
* Reading multiple certificates from each file
* Extracting other certificate parameters from certificates
* Write the extracted parameters in output files
