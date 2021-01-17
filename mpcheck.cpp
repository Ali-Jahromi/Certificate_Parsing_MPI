#include <bits/stdc++.h>
#include "mpi.h"
#include <openssl/x509.h>
#include <openssl/x509v3.h>

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







	//Finalize
	MPI::Finalize();
}
