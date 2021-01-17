#include <bits/stdc++.h>
#include "mpi.h"
#include <openssl/x509.h>
#include <openssl/x509v3.h>
#include <openssl/pem.h>

using namespace std;

int main(int argc, char* argv[]){
	// Initialize
	MPI::Init(argc, argv);
	auto mpisize = MPI::COMM_WORLD.Get_size();
	auto mpirank = MPI::COMM_WORLD.Get_rank();
	
	// Get file name as an input
	string fname(argv[1]);	
	fname += "." + to_string(mpirank);
	cout << "Rank #" << mpirank << " reading " << fname << endl;
	
	// Openning and Reading the Input files
	FILE *fp = fopen(fname.c_str(), "r");
	if (!fp) {
			fprintf(stderr, "unable to open: %s\n", fname.c_str());
				return EXIT_FAILURE;
	}
	
	X509 *cert = PEM_read_X509(fp, NULL, NULL, NULL);
	if (!cert) {
			fprintf(stderr, "unable to parse certificate in: %s\n", fname.c_str());
				fclose(fp);
					return EXIT_FAILURE;
	}

	// Extracting certificate subject
        char *subj = X509_NAME_oneline(X509_get_subject_name(cert), NULL, 0);	
	// Extracging certificate issuer
	char *issuer = X509_NAME_oneline(X509_get_issuer_name(cert), NULL, 0);

	cout << "File " << fname << " ==> Subject: " << subj << endl << 
		"Issuer: " << issuer << endl;
	X509_free(cert);
	fclose(fp);






	//Finalize
	MPI::Finalize();
}
