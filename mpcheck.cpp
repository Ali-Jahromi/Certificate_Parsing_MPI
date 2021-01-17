#include "mpi.h"
#include <bits/stdc++.h>
#include <openssl/asn1.h>
#include <openssl/bio.h>
#include <openssl/bn.h>
#include <openssl/pem.h>
#include <openssl/x509.h>
#include <openssl/x509_vfy.h>
#include <openssl/x509v3.h>

using namespace std;

/***
//Function to extract subject and issuer from certificates
string SI_Extractor(X509 cert){

	char res[100];
	strcat (res, subj);
	strcat (res, issuer);
	return res;
}
***/
int main(int argc, char* argv[]){
	//help switch
	if(argc == 2 && strcmp(argv[1], "--help") == 0 || strcmp(argv[1] ,"-h") ==0){
	cout << "certificate files type: " << endl <<
		"--Single or -s: pem files containing single certificates" << endl <<
		"--Rapid7 or -r7: Rapid7 certificate datasets containing fingerprints, certificates" << endl << endl;
	cout << "e.g.,  mpirun -np 4 <Compiled Binary> <files name> -r7" << endl <<
	       "files name starts with .0 (e.g., file.0) and finishes with file.n; n= number of processors" << endl;		
	}
	else{
	// Initialize
	MPI::Init(argc, argv);
	auto mpisize = MPI::COMM_WORLD.Get_size();
	auto mpirank = MPI::COMM_WORLD.Get_rank();
	
	// Get file name as an input
	string fname(argv[1]);	
	fname += "." + to_string(mpirank);
	cout << "Rank #" << mpirank << " reading " << fname << endl;
	cout << "Argv[2] is: " << argv[2] << " and argv[1]: " << argv[1] << endl;
	string arg2(argv[2]);
	if (arg2 == "--Single" || arg2 == "-s"){
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
	}
	// To parse Rapid7 certificate datasets
	if (arg2 == "--Rapid7" || arg2 == "-r7"){
	// Vector to Hold certificates
	vector<string> certificates;
	string line, crt;
	//Reading Rapid7 datasets
	ifstream inFile(fname.c_str(), ios::in);
  	if (inFile.is_open()){
    		while ( getline (inFile,line) )    		
		{
		bool crtflag = false;
		crt = "-----BEGIN CERTIFICATE-----\n";
		int newline_ctr = 0;
		for(std::string::size_type i = 0; i < line.size(); ++i) {
			if (crtflag == true){
			        crt += line[i];	
				newline_ctr += 1;
				if (newline_ctr == 64){
					crt += '\n';
					newline_ctr = 0;
				}
				//cout << "crtflag is true" << endl;
			}
			if (line[i] == ','){
				crtflag = true;	
				//cout << "Line[i] is: " << line[i] << endl;
			}
		}
		crt += '\n';
		crt += "-----END CERTIFICATE-----\n";
		//cout << crt << endl;
		size_t certLen = strlen(crt.c_str());
		BIO* certBio = BIO_new(BIO_s_mem());
		BIO_write(certBio, crt.c_str(), certLen);
		X509 *cert = PEM_read_bio_X509(certBio, NULL, NULL, NULL);
		if (!cert) {
			fprintf(stderr, "unable to parse certificate in: %s\n", fname.c_str());
			//fclose(fp);
			continue;
	}
	// Extracting certificate subject
        char *subj = X509_NAME_oneline(X509_get_subject_name(cert), NULL, 0);	
	// Extracging certificate issuer
	char *issuer = X509_NAME_oneline(X509_get_issuer_name(cert), NULL, 0);

	cout << "File " << fname << " ==> Subject: " << subj << endl << 
		"Issuer: " << issuer << endl;
	X509_free(cert);
	//fclose(fp);
    		}
    	inFile.close();
  	}
	}	
	//Finalize
	MPI::Finalize();
	}
}
