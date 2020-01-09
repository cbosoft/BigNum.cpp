#include "bignum.hpp"

BigNum BigNum::add(const BigNum *bigger, const BigNum *smaller)
  {
    size_t size_bigger = bigger->get_size_abs(), size_smaller = smaller->get_size_abs(), i = 0;

    BigNum result;
    digit carry = 0;

    for (i = 0; i < size_smaller; i++) {
      carry += bigger->digits[i] + smaller->digits[i];
      result.digits.push_back(carry & DIGIT_MASK);
      carry = carry >> DIGIT_SHIFT;
    }

    for (; i < size_bigger; i++) {
      carry += bigger->digits[i];
      result.digits.push_back(carry & DIGIT_MASK);
      carry = carry >> DIGIT_SHIFT;
    }

    result.digits.push_back(carry);

    result.normalise();
    return result;
  }
