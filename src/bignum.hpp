#include <vector>
#include <sstream>
#include <iostream>

// Define some types. A 'digit' is the smallest unit of integer: the storage
// size of each element in the vector representing the big integer. 'twodigits'
// is as it sounds: a datatype large enough to hold something twice as big as a
// digit.
typedef unsigned short digit;
typedef unsigned int twodigits;

// DIGIT_SHIFT defines the bit-width of a number in a single digit. So moving
// from a digit in the LSB position to LSB+1 position left-shifts by DIGIT_SHIFT
// amount. In the Python implementation, they choose a digit shift of 15 or 30
// as this allows them to use more complicated algorithms giving performance
// boosts.
#define DIGIT_SHIFT 15

// DIGIT_BASE is the largest number representable by the digit, plus one. If
// DIGIT_SHIFT was 3, then our base would be 8, the maximum representable number
// 0b111 (decimal 7).
#define DIGIT_BASE (1<<DIGIT_SHIFT)

// DIGIT_MASK is the number where all the bits of the digit are one: the larges
// number representable by the digit.
#define DIGIT_MASK (DIGIT_BASE-1)


// Some of the algorithms used require the operands to be in order of number of
// digits. This macro will run function on pointer 'this' and pointer 'other' in
// varying orders depending on the result of the size comparison stored in
// variable 'thisbigger'.
#define SORTSIZE(FUNCTION)\
  if (thisbigger) {\
    FUNCTION(this, other);\
  }\
  else {\
    FUNCTION(other, this);\
  }


// BigNum();
// BigNum(long number);
// 
// A BigNum represents an Integer accross any number of bytes. Methods used here
// follow mostly along with that used by the Python developers, simplified as
// necessary.
class BigNum {
  
private:
  // The integer is describe entirely by this vector of digits, and a sign.
  std::vector<digit> digits;
  bool negative;

  // Class methods described here are defined in add.cpp, subtract.cpp,
  // multiply.cpp, and divide.cpp
  static BigNum add (const BigNum *bigger, const BigNum *smaller);
  static BigNum subtract(const BigNum *bigger, const BigNum *smaller);
  static BigNum multiply(const BigNum *bigger, const BigNum *smaller);
  static void divrem(const BigNum *bigger, const BigNum *smaller, BigNum *quotient, BigNum *remainder);

  
  // BigNum::normalise();
  // Removes leading zeros from the digit vector.
  void normalise()
  {
    while (this->digits.back() == 0)
      this->digits.pop_back();
  }

public:

  // Default constructor: creates a big number with value '0'.
  BigNum()
  {
    this->negative = false;
  }


  // Initialise constructor: creates a BigNum from the long value given.
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


  // Initialise constructor: create a BigNum from the string.
  BigNum(std::string value); // TODO


  // Addition. This code wraps the unsigned addition function defined in add.cpp
  // and unsigned subtraction in subtract.cpp.  Adding two positive, or two
  // negative numbers, is addition. Adding a positive to a negative, or a
  // negative to a positive, is subtraction.
  BigNum operator+(const BigNum &other_ref)
  {
    BigNum result;
    const BigNum *other = &other_ref;
    bool thisbigger = this->get_size_abs() > other->get_size_abs();
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
  

  // Subtraction. Similar to addition above, but a negation operation is applied
  // to the right hand operand.
  BigNum operator-(const BigNum &other_ref)
  {
    BigNum result;
    const BigNum *other = &other_ref;
    bool thisbigger = this->get_size_abs() > other->get_size_abs();
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


  // Multiplication. Need to sort the operands into bigger-first-order, then
  // call on the algorithm defined in multiply.cpp.
  BigNum operator*(const BigNum &other_ref)
  {
    const BigNum *other = &other_ref;
    bool thisbigger = this->get_size_abs() > other->get_size_abs();
    BigNum result;
    SORTSIZE(result = BigNum::multiply)
    result.negative = (other->negative != this->negative);
    return result;
  }
  

  // Division. No size-sorting this time, just call the divrem class method
  // (defined in divide.cpp) providing location reference for the quotient to be
  // returned.
  BigNum operator/(const BigNum &other)
  {
    BigNum quotient;
    BigNum::divrem(this, &other, &quotient, NULL);
    quotient.negative = (other.negative != this->negative);
    return quotient;
  }


  // Modulo. Same algorithm as division (divrem in divide.cpp), except we
  // provide the remainder address instead of the quotient.
  BigNum operator%(const BigNum &other)
  {
    BigNum remainder;
    BigNum::divrem(this, &other, NULL, &remainder);
    if (other.negative) {
      remainder = remainder - other;
    }
    return remainder;
  }


  // Greater than comparison.
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


  // Equality comparison.
  bool operator==(const BigNum &other)
  {
    if (this->get_size_abs() != other.get_size_abs()) {
      return false;
    }

    if (this->negative != other.negative) {
      return false;
    }

    for (size_t i = 0; i < this->get_size_abs(); i++) {
      if (this->digits[i] != other.digits[i])
        return false;
    }

    return true;
  }


  // not equal to
  bool operator!=(const BigNum &other)
  {
    return !((*this) == other);
  }


  // greater than or equal to
  bool operator>=(const BigNum &other)
  {
    return ((*this) > other) || ((*this) == other);
  }


  // less than
  bool operator<(const BigNum &other)
  {
    // less than is not greater or equal to
    return !((*this) >= other);
  }


  // less than or equal to
  bool operator<=(const BigNum &other)
  {
    // less than or equal is not greater
    return !((*this) > other);
  }


  // String representation. Show the value of the BigNum as a string.
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
    if (this->is_negative()) ss << "-";
    ss << res;
    return ss.str();
  }


  // Get the 'size' of the BigNum: this gives the number of digits used to store
  // the number. This is used in the maths operations to compare objects and to
  // decide result sizes.
  size_t get_size_abs() const
  {
    return this->digits.size();
  }


  // Get if the number is negative or not.
  bool is_negative() const
  {
    return this->negative;
  }
  
};
