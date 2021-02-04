#pragma  once

#include <stdint.h>

// This class contains utility functions

inline uint8_t modulo(int8_t a, uint8_t q)
{
	int8_t mod = a % q;
	if (mod < 0)
	mod += q;
	return mod;
}

inline int8_t posdiv(int8_t a, uint8_t q)
{
	return (a - modulo(a, q)) / q;
}

inline uint8_t to_bcd(uint8_t a)
{
	return ((a / 10) << 4) | (a % 10);
}

inline uint8_t from_bcd(uint8_t a)
{
	return (a >> 4) * 10 + (a & 0x0f);
}

template<typename T>
T max(T left, T right)
{
	return left > right ? left : right;
}

template<typename T>
T min(T left, T right)
{
	return left < right ? left : right;
}

