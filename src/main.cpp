#include "bignum.hpp"

int main(void)
{
  BigNum left(10), right(2);
  //left.print();
  //right.print();
  BigNum result = left / right;
  result.print();

}
