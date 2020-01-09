#include <vector>
#include <sstream>
#include <iostream>

typedef short sdigit;
typedef unsigned short digit;
typedef unsigned int twodigits;

#define DIGIT_SHIFT 2
#define DIGIT_BASE (1<<DIGIT_SHIFT)
#define DIGIT_MASK (DIGIT_BASE-1)

#define SORTSIZE(FUNCTION)\
  if (thisbigger) {\
    FUNCTION(this, other);\
  }\
  else {\
    FUNCTION(other, this);\
  }

class BigNum {
  
private:
  std::vector<digit> digits;
  bool negative;

  static BigNum add (const BigNum *bigger, const BigNum *smaller);
  static BigNum subtract(const BigNum *bigger, const BigNum *smaller);
  static BigNum multiply(const BigNum *bigger, const BigNum *smaller);
  static void divrem(const BigNum *bigger, const BigNum *smaller, BigNum *quotient, BigNum *remainder);

  void normalise()
  {
    // remove leading zero-valued digits.
    while (this->digits.back() == 0)
      this->digits.pop_back();
  }

public:
  BigNum()
  {
    this->negative = false;
  }

  BigNum(long value)
  {
    this->negative = value < 0;
    if (this->negative) value = -value;

    long carry = value;
    while(carry) {
      this->digits.push_back(carry & DIGIT_MASK);
      carry = carry >> DIGIT_SHIFT;
    }
  }

  BigNum(std::string value);

  BigNum operator+(const BigNum &other_ref)
  {
    BigNum result;
    const BigNum *other = &other_ref;
    bool thisbigger = this->get_size() > other->get_size();
    if (this->negative) {
      if (other->negative) {
        SORTSIZE(result = BigNum::add);
        result.negative = true;
      }
      else {
        SORTSIZE(result = BigNum::subtract);
      }
    }
    else {
      if (other->negative) {
        SORTSIZE(result = BigNum::subtract);
      }
      else {
        SORTSIZE(result = BigNum::add);
      }
    }

    return result;
  }

  BigNum operator-(const BigNum &other_ref)
  {
    BigNum result;
    const BigNum *other = &other_ref;
    bool thisbigger = this->get_size() > other->get_size();
    if (this->negative) {
      if (!other->negative) {
        SORTSIZE(result = BigNum::add);
        result.negative = true;
      }
      else {
        SORTSIZE(result = BigNum::subtract);
      }
    }
    else {
      if (!other->negative) {
        SORTSIZE(result = BigNum::subtract);
      }
      else {
        SORTSIZE(result = BigNum::add);
      }
    }

    result.negative = negative;
    return result;
  }

  BigNum operator*(const BigNum &other)
  {
    BigNum result = BigNum::multiply(this, &other);
    result.negative = (other.negative != this->negative);
    return result;
  }

  BigNum operator/(const BigNum &other)
  {
    BigNum quotient;
    BigNum::divrem(this, &other, &quotient, NULL);
    quotient.negative = (other.negative != this->negative);
    return quotient;
  }

  BigNum operator%(const BigNum &other)
  {
    BigNum remainder;
    BigNum::divrem(this, &other, NULL, &remainder);
    if (other.negative) {
      remainder = remainder - other;
    }
    return remainder;
  }

  bool operator>(const BigNum &other)
  {
    if (other.negative != this->negative)
      return other.negative;

    size_t this_size = this->get_size_abs(), other_size = other.get_size_abs();

    if (this_size > other_size)
      return !this->negative; // this is bigger: if NOT negative then this is greater than.

    if (this_size < other_size)
      return this->negative; // this is smaller: if negative then this is greater than.

    if (this_size == 0)
      return false; // both the same: both zero

    int i = this->get_size_abs();
    while (--i >= 0 && this->digits[i] == other.digits[i]);

    if (i < 0)
      return false; // both the same

    return this->digits[i] > other.digits[i];
  }

  bool operator==(const BigNum &other)
  {
    if (this->get_size() != other.get_size()) {
      return false;
    }

    for (size_t i = 0; i < this->get_size_abs(); i++) {
      if (this->digits[i] != other.digits[i])
        return false;
    }

    return true;
  }

  bool operator!=(const BigNum &other)
  {
    return !((*this) == other);
  }

  bool operator>=(const BigNum &other)
  {
    return ((*this) > other) || ((*this) == other);
  }

  bool operator<(const BigNum &other)
  {
    // less than is not greater or equal to
    return !((*this) >= other);
  }

  bool operator<=(const BigNum &other)
  {
    // less than or equal is not greater
    return !((*this) > other);
  }

  std::string repr() const {

    // TODO: this solution is temporary: it will over- and underflow if bigger
    // numbers than a long long is capable of storing is used.
    int i = this->get_size_abs();
    if (i == 0)
      return "0";

    long long res = 0;
    for (i--; i >= 0; i--) {
      res <<= DIGIT_SHIFT;
      res += this->digits[i];
    }

    std::stringstream ss;
    if (this->negative_p()) ss << "-";
    ss << res;
    return ss.str();
  }

  void print() const
  {
    std::cout << this->repr() << std::endl;
  }

  size_t get_size_abs() const
  {
    return this->digits.size();
  }
  
  int get_size() const
  {
    int size = this->get_size_abs();
    return this->negative_p() ? -size : size;
  }

  bool negative_p() const
  {
    return this->negative;
  }
  
  
};
