#include "bignum.hpp"

void BigNum::divrem(const BigNum *dividend_ptr, const BigNum *divisor_ptr)
{
  BigNum remainder = (*dividend_ptr);
  BigNum quotient(0);
  const BigNum one(1);
  const BigNum divisor = (*divisor_ptr);
  while (remainder > BigNum(0)) {
    remainder = remainder - divisor;
    quotient = quotient + one;
  }

  this->last_quotient = quotient.digits;
  this->last_remainder = remainder.digits;

}

