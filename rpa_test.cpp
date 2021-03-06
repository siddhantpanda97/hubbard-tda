#include "rpa.hpp"
#include <cstring>
#include <chrono>
#include <cstdlib>

double t=1;
double U_prime=2;
int L=4;
MatrixXd sigma;
MatrixXcd U;

using namespace std::chrono;

void greens_sigma_generate(MatrixXd& suggested_sigma, int lattice_index, long & idum)
{
  if(ran0(&idum)<=0.5) suggested_sigma(lattice_index,2) *= -1;
}

int main(int argc, char* argv[])
{
  if(argc!=4) {cerr << "Enter (1) lattice size, (2) U and (3) temperature.\n"; exit(1);}
  L = atoi(argv[1]);
  U_prime = atof(argv[2]);
  double temperature = atof(argv[3]);

  sigma = MatrixXd::Zero(L,3);
  sigma.col(2) = VectorXd::Constant(L,1);
  for(int i=0; i<L; i++) sigma(i,2) = pow(-1,i);//  greens_sigma_generate(sigma, i, idum);
  
  MatrixXcd H0 = construct_h0();
  MatrixXcd H_spa = H0 - U_prime/2*matrixelement_sigmaz(sigma);

  pair<MatrixXcd,VectorXd> spa_spectrum = Eigenspectrum(H_spa);
  U = spa_spectrum.first;

  MatrixXcd H_rpa = construct_RPA(spa_spectrum.second, temperature);
  
  cout.precision(3);

  // for(int i=0; i<N*N/4; i++) cout << "(" << 
  // cout << H_rpa.unaryExpr(&filter).real() << endl << endl;

  cout <<  Viljk(3,0,0,3,temperature,spa_spectrum.second) << endl;
  cout <<  Viljk(0,0,3,3,temperature,spa_spectrum.second) << endl;
}