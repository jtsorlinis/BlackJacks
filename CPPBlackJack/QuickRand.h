static unsigned int g_seed;

// Used to seed the generator.
inline void fast_srand(int seed) { g_seed = seed; }

// Compute a pseudorandom integer.
// Output value in range [0, 32767]
inline int fast_rand(void) {
  g_seed = (214013 * g_seed + 2531011);
  return (g_seed >> 16) & 0x7FFF;
}

static unsigned long x = 123456789, y = 362436069, z = 521288629;

unsigned long xorshf96(void) {  // period 2^96-1
  unsigned long t;
  x ^= x << 16;
  x ^= x >> 5;
  x ^= x << 1;

  t = x;
  x = y;
  y = z;
  z = t ^ x ^ y;

  return z;
}
