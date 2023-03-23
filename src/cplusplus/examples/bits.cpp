/*
 * csapp.cs.cmu.edu/3e/labs.html
 * operators allowed = !  ̃ & ˆ | + << >>
 *
 * 1. bitXor
 * 2. tmin
 * 3. isTmax
 * 4. allOddBits
 * 5. negate
 * 6. isAsciiDigit
 * 7. conditional
 * 8. isLessOrEqual
 * 9. logicalNeg
 * 10. howManyBits
 * 11. floatScale2
 * 12. floatFloat2Int
 * 13. floatPower2
 */

/*
 * bitXor - x^y using only ~ and &
 *   Example: bitXor(4, 5) = 1
 *   Legal ops: ~ &
 *   Max ops: 14
 *   Rating: 1
 */
int bitXor(int x, int y) {
    // x, y 不都为0且不都为1的位
    return ~(~x & ~y) & ~(x & y);
}

/*
 * tmin - return minimum two's complement integer
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 4
 *   Rating: 1
 */
int tmin() { return 1 << 31; }

/*
 * isTmax - returns 1 if x is the maximum, two's complement number,
 *     and 0 otherwise
 *   Legal ops: ! ~ & ^ | +
 *   Max ops: 10
 *   Rating: 1
 */
int isTmax(int x) {
    // tmax + 1 = tmin
    // tmin = -tmin
    // 0 = -0
    int a = x + 1;
    return !((~a + 1) ^ a) & (!!a);
}

/*
 * allOddBits - return 1 if all odd-numbered bits in word set to 1
 *   where bits are numbered from 0 (least significant) to 31 (most significant)
 *   Examples allOddBits(0xFFFFFFFD) = 0, allOddBits(0xAAAAAAAA) = 1
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 2
 */
int allOddBits(int x) { return !((x & 0xAAAAAAAA) ^ (0xAAAAAAAA)); }

/*
 * negate - return -x
 *   Example: negate(1) = -1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 5
 *   Rating: 2
 */
int negate(int x) { return ~x + 1; }

/*
 * isAsciiDigit - return 1 if 0x30 <= x <= 0x39 (ASCII codes for characters '0' to '9')
 *   Example: isAsciiDigit(0x35) = 1.
 *            isAsciiDigit(0x3a) = 0.
 *            isAsciiDigit(0x05) = 0.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 15
 *   Rating: 3
 */
int isAsciiDigit(int x) {
    int a = x + (~0x30 + 1);
    int b = 0x39 + (~x + 1);
    return !(a & 0x80000000) & !(b & 0x80000000);
}

/*
 * conditional - same as x ? y : z
 *   Example: conditional(2,4,5) = 4
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 16
 *   Rating: 3
 */
int conditional(int x, int y, int z) {
    int mask = !x << 31 >> 31;
    return (mask & z) + (~mask & y);
}

/*
 * isLessOrEqual - if x <= y  then return 1, else return 0
 *   Example: isLessOrEqual(4,5) = 1.
 *   Legal ops: ! ~ & ^ | + << >>
 *   Max ops: 24
 *   Rating: 3
 */
int isLessOrEqual(int x, int y) {
    int diff = (x >> 31) ^ (y >> 31);
    int a = y + ~x + 1;
    return !!(diff & (x >> 31)) + ((!diff) & !(a >> 31));
}

/*
 * logicalNeg - implement the ! operator, using all of
 *              the legal operators except !
 *   Examples: logicalNeg(3) = 0, logicalNeg(0) = 1
 *   Legal ops: ~ & ^ | + << >>
 *   Max ops: 12
 *   Rating: 4
 */
int logicalNeg(int x) {
    int tmp = ~(x | (~x + 1));
    return (tmp >> 31) & 1;
}

/* howManyBits - return the minimum number of bits required to represent x in
 *             two's complement
 *  Examples: howManyBits(12) = 5
 *            howManyBits(298) = 10
 *            howManyBits(-5) = 4
 *            howManyBits(0)  = 1
 *            howManyBits(-1) = 1
 *            howManyBits(0x80000000) = 32
 *  Legal ops: ! ~ & ^ | + << >>
 *  Max ops: 90
 *  Rating: 4
 */
int howManyBits(int x) {
    int b16, b8, b4, b2, b1, b0;
    int sign = x >> 31;
    x = (sign & ~x) | (~sign & x);
    b16 = !!(x >> 16) << 4;
    x = x >> b16;
    b8 = !!(x >> 8) << 3;
    x = x >> b8;
    b4 = !!(x >> 4) << 2;
    x = x >> b4;
    b2 = !!(x >> 2) << 1;
    x = x >> b2;
    b1 = !!(x >> 1);
    x = x >> b1;
    b0 = x;
    return b16 + b8 + b4 + b2 + b1 + b0 + 1;
}

/*
 * floatScale2 - Return bit-level equivalent of expression 2*f for
 *   floating point argument f.
 *   Both the argument and result are passed as unsigned int's, but
 *   they are to be interpreted as the bit-level representation of
 *   single-precision floating point values.
 *   When argument is NaN, return argument
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatScale2(unsigned uf) {
    unsigned s, exp, frac;
    s = uf & 0x80000000;
    exp = (uf >> 23) & 0x000000FF;
    frac = uf & 0x007FFFFF;
    if (exp == 0xFF) return uf;
    if (exp == 0) {
        if (frac & 0x00400000) exp++;
        frac = (frac << 1) & 0x007FFFFF;
    } else {
        exp++;
        if (exp == 0xFF) frac = 0;
    }
    return s | (exp << 23) | frac;
}

/*
 * floatFloat2Int - Return bit-level equivalent of expression (int) f
 *   for floating point argument f.
 *   Argument is passed as unsigned int, but
 *   it is to be interpreted as the bit-level representation of a
 *   single-precision floating point value.
 *   Anything out of range (including NaN and infinity) should return
 *   0x80000000u.
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. also if, while
 *   Max ops: 30
 *   Rating: 4
 */
int floatFloat2Int(unsigned uf) {
    int s, exp, frac, bias_exp;
    s = uf & 0x80000000;
    exp = (uf >> 23) & 0x000000FF;
    frac = uf & 0x007FFFFF;
    bias_exp = exp - 127;
    frac = frac | 1 << 23;

    if (exp == 0 || bias_exp < 0) return 0;
    if (exp == 255 || bias_exp >= 31) return 0x80000000U;
    if (bias_exp >= 24)
        frac = frac << (bias_exp - 23);
    else
        frac = frac >> (23 - bias_exp);
    if (s != 0) frac = ~frac + 1;

    return frac;
}

/*
 * floatPower2 - Return bit-level equivalent of the expression 2.0^x
 *   (2.0 raised to the power x) for any 32-bit integer x.
 *
 *   The unsigned value that is returned should have the identical bit
 *   representation as the single-precision floating-point number 2.0^x.
 *   If the result is too small to be represented as a denorm, return
 *   0. If too large, return +INF.
 *
 *   Legal ops: Any integer/unsigned operations incl. ||, &&. Also if, while
 *   Max ops: 30
 *   Rating: 4
 */
unsigned floatPower2(int x) {
    int exp = x + 127;
    if (exp >= 255) return 0xFF << 23;
    if (exp <= 0) return 0;
    return exp << 23;
}

int main() {}
