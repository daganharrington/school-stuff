/*
a wee little stub to compute bhattacharyya coefficient using eigen
measures 'overlap' between two random samples, generally
two multivariate gaussians here
CXXFLAGS =  -I/usr/include/eigen3 -lm
dagan
*/

#include <iostream>
#include <cmath>
#include <Eigen/Dense>

double Bhat (Eigen::MatrixXd A_mean, Eigen::MatrixXd A_var,
	     Eigen::MatrixXd B_mean, Eigen::MatrixXd B_var){
  Eigen::MatrixXd P(2,2);
  P = (A_var + B_var) / 2;
  Eigen::MatrixXd P_inverse(2,2);
  P_inverse = P.inverse(); //no check
  
  Eigen::MatrixXd mu0mu1_t(1,2);
  mu0mu1_t = (A_mean - B_mean).transpose();
  Eigen::MatrixXd mu0mu1(2,1);
  mu0mu1 = (A_mean - B_mean);
  Eigen::MatrixXd temp(1,2);
  temp = mu0mu1_t * P_inverse;
  
  double x = temp(0,0)*mu0mu1(0,0)+temp(0,1)*mu0mu1(1,0);

  return 
    std::exp( (-1.0/8.0)* x) *
    std::sqrt ( std::sqrt (A_var.determinant() * B_var.determinant()) /
	   P.determinant());
}

int main()
ex{  
  Eigen::MatrixXd A_mean(2,1);
  A_mean << 5000, 5500;
  Eigen::MatrixXd A_var(2,2);
  A_var << 100000, 0, 0, 100000;
  
  Eigen::MatrixXd B_mean(2,1);
  B_mean << 6000, 5500;
  Eigen::MatrixXd B_var(2,2);
  B_var << 100000, 0, 0, 100000;
  
  std::cout << "Overlapping proportion is : "<< 
    Bhat(A_mean,A_var,B_mean,B_var) << std::endl;
  
  return 0;
}
