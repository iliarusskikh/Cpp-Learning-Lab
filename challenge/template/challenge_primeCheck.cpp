#include <type_traits>

template <unsigned long long p, unsigned long long i>
struct is_prime_impl : std::conditional_t<(p % i == 0), std::false_type,
        is_prime_impl<p, i - 1>> {};

// Base case: got all the way down to i == 1 without finding a divisor
template <unsigned long long p>
struct is_prime_impl<p, 1> : std::true_type {};

template <unsigned long long p>
struct is_prime : is_prime_impl<p, p / 2> {};

// p/2 recursion breaks for 0 and 1 (i would hit 0, causing p % 0)
template <> struct is_prime<0> : std::false_type {};
template <> struct is_prime<1> : std::false_type {};

// usage
static_assert(is_prime<97>::value);
static_assert(!is_prime<91>::value);   // 7 * 13


/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/
/*----------------------------------------------------------------------*/

// modern way using consexpr
 
 constexpr unsigned long long isqrt(unsigned long long n) {
     if (n < 2) return n;
     unsigned long long lo = 1, hi = n;
     while (lo < hi) {
         unsigned long long mid = lo + (hi - lo + 1) / 2;
         if (mid <= n / mid) lo = mid; else hi = mid - 1;
     }
     return lo;
 }

 constexpr bool is_prime2(unsigned long long p) {
     if (p < 2)
         return false;
     if (p % 2 == 0)
         return p == 2;
     const auto r = isqrt(p);
     for (unsigned long long i = 3; i <= r; i += 2)
         if (p % i == 0)  return false;
     return true;
 }

 static_assert(is_prime2(97));
 static_assert(!is_prime2(91));   // 7 * 13
 constexpr bool runtime_ok = is_prime2(104729);
 

