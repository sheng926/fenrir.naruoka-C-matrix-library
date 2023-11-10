#include <iostream>
#include "matrix.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */
 
Matrix<double> some_matrix_op()
{
  Matrix<double> m1(3, 3); //m1 is a 3 by 3 zero matrix.
  {
      m1(0, 0) = 1.; // I hate Matlab, like C style indexes :-)
      m1(1, 1) = 2.;
      m1(2, 2) = 3.;
      m1(0, 1) = 4.;
  }
  //Matrix<double> m2(Matrix::getI(3)); // m2 is a 3 by 3 unit matrix.
  Matrix<double> m2(m2.getI(3)); // m2 is a 3 by 3 unit matrix.
  return m1.transpose() * m2; // => {1, 0, 0; 4, 2, 0; 0, 0, 3}
}
  
int main(int argc, char** argv) 
{
	some_matrix_op();
	return 0;
}