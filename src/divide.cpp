#include "bignum.hpp"

// This is a very, VERY, simple algorithm to calculate the quotient of a
// unsigned integer dividend divided by an unisgned integer divisor. The
// remainder is also output.
void BigNum::divrem(const BigNum *dividend_ptr, const BigNum *divisor_ptr, BigNum *quotient, BigNum *remainder)
{
  BigNum _remainder = (*dividend_ptr);
  BigNum _quotient(0);
  const BigNum one(1);
  const BigNum divisor = (*divisor_ptr);
  while (_remainder > BigNum(0)) {
    _remainder = _remainder - divisor;
    _quotient = _quotient + one;
  }

  if (quotient != NULL)
    (*quotient) = _quotient;

  if (remainder != NULL)
    (*remainder) = _remainder;

}

