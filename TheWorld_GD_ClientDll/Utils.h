#pragma once

#include <cfloat>
#include <assert.h> 

#include <Godot.hpp>

static constexpr float kPi = 3.14159265358979323846f;
static constexpr float kPi2 = 6.28318530717958647692f;
static constexpr float kEpsilon = 0.0001f;
static constexpr float kAreaEpsilon = FLT_EPSILON;
static constexpr float kNormalEpsilon = 0.001f;

static int align(int x, int a) {
	return (x + a - 1) & ~(a - 1);
}

template <typename T>
static T max(const T& a, const T& b) {
	return a > b ? a : b;
}

template <typename T>
static T min(const T& a, const T& b) {
	return a < b ? a : b;
}

template <typename T>
static T max3(const T& a, const T& b, const T& c) {
	return max(a, max(b, c));
}

/// Return the maximum of the three arguments.
template <typename T>
static T min3(const T& a, const T& b, const T& c) {
	return min(a, min(b, c));
}

/// Clamp between two values.
template <typename T>
static T clamp(const T& x, const T& a, const T& b) {
	return min(max(x, a), b);
}

template <typename T>
static void swap(T& a, T& b) {
	T temp = a;
	a = b;
	b = temp;
}

union FloatUint32 {
	float f;
	uint32_t u;
};

static bool isFinite(float f) {
	FloatUint32 fu;
	fu.f = f;
	return fu.u != 0x7F800000u && fu.u != 0x7F800001u;
}

static bool isNan(float f) {
	return f != f;
}

// Robust floating point comparisons:
// http://realtimecollisiondetection.net/blog/?p=89
static bool equal(const float f0, const float f1, const float epsilon) {
	//return fabs(f0-f1) <= epsilon;
	return fabs(f0 - f1) <= epsilon * max3(1.0f, fabsf(f0), fabsf(f1));
}

static int ftoi_ceil(float val) {
	return (int)ceilf(val);
}

static bool isZero(const float f, const float epsilon) {
	return fabs(f) <= epsilon;
}

static float square(float f) {
	return f * f;
}

/** Return the next power of two.
* @see http://graphics.stanford.edu/~seander/bithacks.html
* @warning Behaviour for 0 is undefined.
* @note isPowerOfTwo(x) == true -> nextPowerOfTwo(x) == x
* @note nextPowerOfTwo(x) = 2 << log2(x-1)
*/
static uint32_t nextPowerOfTwo(uint32_t x) {
	assert(x != 0);
	// On modern CPUs this is supposed to be as fast as using the bsr instruction.
	x--;
	x |= x >> 1;
	x |= x >> 2;
	x |= x >> 4;
	x |= x >> 8;
	x |= x >> 16;
	return x + 1;
}

/*
 * Case Insensitive String Comparision
 */
static bool caseInSensStringEqual(std::string& str1, std::string& str2)
{
	return boost::iequals(str1, str2);
}

/*
 * Case Insensitive String Comparision
 */
static bool caseInSensWStringEqual(std::wstring& str1, std::wstring& str2)
{
	return boost::iequals(str1, str2);
}
