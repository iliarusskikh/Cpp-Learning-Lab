//  C++20 CONCEPTS - REVISION FILE
//
//  Big idea: a concept is a NAMED, COMPILE-TIME YES/NO QUESTION about a type
//  (or value). Templates use it to say "only accept types that ...", and the
//  compiler gives a short, readable error instead of a wall of template noise.
//
//  Contents
//    1. Defining concepts (requires expressions: 4 kinds of requirement)
//    2. Testing concepts with static_assert
//    3. Four ways to constrain a template
//    4. Overloading: the more constrained version wins
//    5. Standard library concepts (std::integral, ...)
//    6. Combining concepts (&&, ||, !)
//    7. Concepts on non-type template parameters (constexpr + IsPrime)
//    8. Ad-hoc "requires requires"
//    9. Common gotchas
// =============================================================================

#include <concepts>     // std::integral, std::same_as, std::convertible_to, ...
#include <cstddef>      // std::size_t
#include <iostream>
#include <list>
#include <set>
#include <string>
#include <vector>

// -----------------------------------------------------------------------------
// 1. DEFINING CONCEPTS
// -----------------------------------------------------------------------------
// Syntax:  template<params> concept Name = <compile-time bool expression>;
// A `requires` expression returns true if EVERYTHING inside it is valid code.
// Parameters after `requires` are just "pretend objects" used for the checks.

// (a) Simple requirement: "this expression must compile".
template<typename T>
concept HasPushBack = requires(T c, typename T::value_type v) {
    c.push_back(v);                      // vector, list, string: yes. set: no.
};

// (b) Type requirement: "this nested type must exist".
// (c) Compound requirement: "{ expr } -> constraint" checks the expression's
//     TYPE too, not just that it compiles.
template<typename T>
concept SizedContainer = requires(T c) {
    typename T::value_type;                              // type requirement
    { c.size() } -> std::convertible_to<std::size_t>;    // compound requirement
    c.begin();                                           // simple requirement
    c.end();
};

// Compound requirement on an operator: "can I stream it to std::cout?"
template<typename T>
concept Printable = requires(std::ostream& os, const T& x) {
    { os << x } -> std::same_as<std::ostream&>;
};

// (d) Nested requirement: an inner `requires` checks a bool condition.
//     Without the inner `requires`, it would only check the expression compiles
//     (sizeof(T) <= 8 always compiles!), so it would never be false.
template<typename T>
concept SmallType = requires {
    requires sizeof(T) <= 8;
};

// -----------------------------------------------------------------------------
// 2. TESTING CONCEPTS WITH static_assert (zero runtime cost, checked on build)
// -----------------------------------------------------------------------------
struct NoPrint {};           // no operator<<
struct Big { char data[64]; };

static_assert(HasPushBack<std::vector<int>>);
static_assert(HasPushBack<std::list<int>>);
static_assert(!HasPushBack<std::set<int>>);        // sets use insert()

static_assert(SizedContainer<std::vector<int>>);
static_assert(!SizedContainer<int>);

static_assert(Printable<int>);
static_assert(Printable<std::string>);
static_assert(!Printable<NoPrint>);

static_assert(SmallType<int>);
static_assert(!SmallType<Big>);

// -----------------------------------------------------------------------------
// 3. FOUR WAYS TO CONSTRAIN A TEMPLATE (all mean the same thing)
// -----------------------------------------------------------------------------

// (a) requires clause, placed before the function
template<typename C, typename T>
requires HasPushBack<C>
void addA(C& c, const T& val) { c.push_back(val); }

// (b) trailing requires clause, placed after the parameter list
template<typename C, typename T>
void addB(C& c, const T& val) requires HasPushBack<C> { c.push_back(val); }

// (c) constrained template parameter: concept name replaces `typename`
template<HasPushBack C, typename T>
void addC(C& c, const T& val) { c.push_back(val); }

// (d) abbreviated function template: concept + `auto` (no template<> needed)
void addD(HasPushBack auto& c, const auto& val) { c.push_back(val); }

// -----------------------------------------------------------------------------
// 4. OVERLOADING: THE MORE CONSTRAINED VERSION WINS
// -----------------------------------------------------------------------------
// Both overloads accept a vector, but the constrained one is "more specialised",
// so the compiler picks it. No ambiguity, no SFINAE tricks.

template<typename C, typename T>       // fallback: anything else (e.g. std::set)
void add(C& c, const T& val) {
    c.insert(val);
    std::cout << "  add: used insert()\n";
}

template<HasPushBack C, typename T>    // preferred when push_back exists
void add(C& c, const T& val) {
    c.push_back(val);
    std::cout << "  add: used push_back()\n";
}

// -----------------------------------------------------------------------------
// 5. STANDARD LIBRARY CONCEPTS (use these before writing your own)
// -----------------------------------------------------------------------------
//   std::integral<T>            bool, char, int, long ... (NOT double)
//   std::floating_point<T>      float, double, long double
//   std::same_as<T, U>          exactly the same type
//   std::convertible_to<From,To>
//   std::derived_from<D, B>
//   std::invocable<F, Args...>  F can be called with Args
//   std::regular, std::copyable, std::movable ...
//   (<iterator> / <ranges> add std::input_iterator, std::ranges::range, ...)

// Abbreviated syntax: `std::integral auto` = "any integral type".
void print(std::integral auto value) {
    std::cout << "  Printing value: " << value << "\n";
}

// Same idea, but with a named template parameter so we can reuse T.
template<std::floating_point T> T halve(T x) {
    return x / 2;
}

// Constrained variable declaration also works:
//   std::integral auto n = 5;      // OK
//   std::integral auto d = 3.5;    // error: double is not integral

// -----------------------------------------------------------------------------
// 6. COMBINING CONCEPTS with && || !
// -----------------------------------------------------------------------------
template<typename T>
concept Number = std::integral<T> || std::floating_point<T>;

template<Number T>
T square(T x) { return x * x; }

// Combine inline too:
template<typename T>
requires std::integral<T> && (!std::same_as<T, bool>)    // integral but not bool
T increment(T x) { return x + 1; }

// -----------------------------------------------------------------------------
// 7. CONCEPTS ON NON-TYPE TEMPLATE PARAMETERS (values, not types)
// -----------------------------------------------------------------------------
// `constexpr` lets the function run at compile time, so it can feed a concept.
constexpr bool isPrime(int val) {
    for (int i = 2; i <= val / 2; ++i) {
        if (val % i == 0) {
            return false;
        }
    }
    return val > 1;          // 0 and 1 are not prime; 2 and 3 are
}

static_assert(isPrime(7));   // plain constexpr call, evaluated by the compiler
static_assert(!isPrime(1));

template<auto Val>           // `auto` NTTP: the value's type is deduced
concept IsPrime = Val > 0 && isPrime(Val);

static_assert(IsPrime<7>);
static_assert(!IsPrime<6>);
static_assert(!IsPrime<-3>); // Val > 0 fails first, isPrime is never evaluated

// A class that can only be instantiated with a prime number:
template<auto Val>
requires IsPrime<Val>
class PrimeBox {
public:
    static constexpr auto value = Val;
};

PrimeBox<7> primeBox;        // OK
// PrimeBox<6> badBox;       // error: constraint IsPrime<6> not satisfied

// -----------------------------------------------------------------------------
// 8. AD-HOC "requires requires" (one-off check, no named concept)
// -----------------------------------------------------------------------------
// First `requires` = clause, second `requires` = expression.
// Prefer a named concept if you will reuse it.
template<typename T>
requires requires(T a) { a + a; }
T twice(T a) { return a + a; }

// -----------------------------------------------------------------------------
// 9. COMMON GOTCHAS
// -----------------------------------------------------------------------------
//  * A concept checks that code COMPILES, not that it is semantically right.
//    (HasPushBack says nothing about what push_back actually does.)
//  * bool and char are integral, so print(true) and print('c') are accepted.
//  * Concepts are not checked inside the function body, only at the call site.
//  * Always require C++20: -std=c++20 (older modes will not know `concept`).
//  * Don't forget semicolons after declarations and statements!

// -----------------------------------------------------------------------------
// DEMO
// -----------------------------------------------------------------------------
int main() {
    std::cout << "--- add overloads ---\n";
    std::vector<int> v;
    std::set<int> s;
    add(v, 1);               // constrained overload (push_back)
    add(s, 2);               // fallback overload (insert)

    addA(v, 2); addB(v, 3); addC(v, 4); addD(v, 5);
    std::cout << "  vector size after addA..addD: " << v.size() << "\n";   // 5

    std::cout << "--- std::integral ---\n";
    print(3);                // OK
    print('c');              // OK: char is integral
    // print(3.521);         // error: double does not satisfy std::integral

    std::cout << "--- Number / floating_point ---\n";
    std::cout << "  square(4)    = " << square(4) << "\n";
    std::cout << "  square(1.5)  = " << square(1.5) << "\n";
    std::cout << "  halve(5.0)   = " << halve(5.0) << "\n";
    // halve(5);             // error: int is not floating_point
    // square(std::string("x"));  // error: std::string is not a Number

    std::cout << "--- increment (integral, not bool) ---\n";
    std::cout << "  increment(41) = " << increment(41) << "\n";
    // increment(true);      // error: bool excluded

    std::cout << "--- IsPrime ---\n";
    std::cout << "  PrimeBox<7>::value = " << PrimeBox<7>::value << "\n";
    std::cout << "  IsPrime<13> = " << IsPrime<13> << ", IsPrime<12> = " << IsPrime<12> << "\n";

    std::cout << "--- requires requires ---\n";
    std::cout << "  twice(21) = " << twice(21) << "\n";
    std::cout << "  twice(string) = " << twice(std::string("ab")) << "\n";
    // twice(NoPrint{});     // error: NoPrint has no operator+

    return 0;
}
