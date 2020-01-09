#include "bignum.hpp"

BigNum BigNum::subtract(const BigNum *a, const BigNum *b)
{
  const BigNum *bigger = NULL, *smaller = NULL;
  int size_bigger, size_smaller, size_a = a->get_size_abs(), size_b = b->get_size_abs(), i;
  bool negative = false;

  if (size_b > size_a) {
    bigger = b;
    smaller = a;
    size_bigger = size_b;
    size_smaller = size_a;
    negative = true;
  }
  else if (size_a == size_b) {
    i = size_a;
    while (--i >= 0 && a->digits[i] == b->digits[i]);
    if (i < 0)
      return BigNum(0);
    if (a->digits[i] < b->digits[i]) {
      negative = true;
      bigger = b;
      smaller = a;
    }
    else {
      bigger = a;
      smaller = b;
    }
    size_bigger = size_smaller = i+1;
  }
  else { /* size_a > size_b */
    bigger = a;
    smaller = b;
    size_bigger = size_a;
    size_smaller = size_b;
  }

  BigNum result;
  digit borrow = 0;
  for (i = 0; i < size_smaller; i++) {
    borrow = bigger->digits[i] - smaller->digits[i] - borrow;
    result.digits.push_back(borrow & DIGIT_MASK);
    borrow = borrow >> DIGIT_SHIFT;
    borrow &= 1;
  }

  for (; i < size_bigger; i++) {
    borrow = bigger->digits[i] - borrow;
    result.digits.push_back(borrow & DIGIT_MASK);
    borrow = borrow >> DIGIT_SHIFT;
    borrow &= 1;
  }

  if (borrow != 0) {
    std::cerr << "borrow not zero; something's not right." << std::endl;
    exit(1);
  }

  result.negative = negative;
  result.normalise();
  return result;
}
