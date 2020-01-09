#include "bignum.hpp"

BigNum BigNum::multiply(const BigNum *bigger, const BigNum *smaller)
{
  size_t size_bigger = bigger->get_size_abs(), size_smaller = smaller->get_size_abs();

  BigNum result;
  for (unsigned int i = 0; i < (size_bigger + size_smaller); i++)
    result.digits.push_back(0);

  unsigned int i_result, i_smaller;
  for (unsigned int i_bigger = 0; i_bigger < size_bigger; i_bigger++) {
    twodigits carry = 0;
    twodigits f = bigger->digits[i_bigger];
    i_result = i_bigger;
    i_smaller = 0;

    for (i_smaller = 0; i_smaller < size_smaller; i_smaller++) {
      carry += result.digits[i_result] + smaller->digits[i_smaller] * f;
      result.digits[i_result++] = (digit)(carry & DIGIT_MASK);
      carry = carry >> DIGIT_SHIFT;
    }
    
    if (carry)
      result.digits[i_result] += (digit)(carry & DIGIT_MASK);

  }

  return result;
}
