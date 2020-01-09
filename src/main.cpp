#include <iostream>
#include "bignum.hpp"

int main(void)
{
  BigNum left(10), right(2);
  BigNum result = left / right;
  std::cout << result.repr() << std::endl;

}
