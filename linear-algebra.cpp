#include <iomanip>
#include <iostream>

#include <Eigen/Cholesky>
#include <Eigen/Core>
#include <Eigen/LU>

int main(int argc, char **argv) {
  int dim = 1000;
  Eigen::MatrixXd M = Eigen::MatrixXd::Random(dim, dim);
  // Make sure our matrix is SPD
  Eigen::MatrixXd A = 0.5 * (M + M.adjoint().eval());
  A = A * A.adjoint();
  Eigen::VectorXd b = Eigen::VectorXd::Random(dim);

  // Solve using fully pivoted LU decomposition
  Eigen::VectorXd x_flu = A.fullPivLu().solve(b);
  double relative_error_flu = (A * x_flu - b).norm() / b.norm();

  // Solve using partially pivoted LU decomposition
  Eigen::VectorXd x_plu = A.partialPivLu().solve(b);
  double relative_error_plu = (A * x_plu - b).norm() / b.norm();

  // Solve using Cholesky decomposition
  Eigen::VectorXd x_llt = A.llt().solve(b);
  double relative_error_llt = (A * x_llt - b).norm() / b.norm();

  // Solve using robust Cholesky decomposition
  Eigen::VectorXd x_ldlt = A.ldlt().solve(b);
  double relative_error_ldlt = (A * x_ldlt - b).norm() / b.norm();

  std::cout << "Linear system solvers comparison " << std::endl;
  std::cout << "  Relative error |Ax - b| / |b|  " << std::endl;
  std::cout << "---------------------------------" << std::endl;
  std::cout << "FullPivLU    " << std::setprecision(6) << relative_error_flu << std::endl;
  std::cout << "PartialPivLU " << std::setprecision(6) << relative_error_plu << std::endl;
  std::cout << "LLT          " << std::setprecision(6) << relative_error_llt << std::endl;
  std::cout << "LDLT         " << std::setprecision(6) << relative_error_ldlt << std::endl;
  std::cout << std::endl;
  std::cout << "  Error wrt FullPivLU |x - x_flu| / |x_flu|  " << std::endl;
  std::cout << "---------------------------------------------" << std::endl;
  std::cout << "PartialPivLU " << std::setprecision(6) << (x_plu - x_flu).norm() / x_flu.norm() << std::endl;
  std::cout << "LLT          " << std::setprecision(6) << (x_llt - x_flu).norm() / x_flu.norm() << std::endl;
  std::cout << "LDLT         " << std::setprecision(6) << (x_ldlt - x_flu).norm() / x_flu.norm() << std::endl;

  return 0;
}
