#pragma once

#ifdef WINDLL_EXPORTS
#define WINDLL_API __declspec(dllexport)
#else
#define WINDLL_API __declspec(dllimport)
#endif

// The Fibonacci recurrence relation describes a sequence F
// where F(n) is { n = 0, a
//               { n = 1, b
//               { n > 1, F(n-2) + F(n-1)
// for some initial integral values a and b.
// If the sequence is initialized F(0) = 1, F(1) = 1,
// then this relation produces the well-known Fibonacci
// sequence: 1, 1, 2, 3, 5, 8, 13, 21, 34, ...

// Initialize a Fibonacci relation sequence
// such that F(0) = a, F(1) = b.
// This function must be called before any other function.
extern "C" WINDLL_API bool readFileFoo(const char* path, const char* buffer, unsigned int from, unsigned int to);

// Produce the next value in the sequence.
// Returns true on success and updates current value and index;
// false on overflow, leaves current value and index unchanged.
extern "C" WINDLL_API bool writeFileFoo(const char* path, const char* buffer, unsigned int from, unsigned int howMany);