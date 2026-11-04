#include <iostream>
#include <vector>
#include <algorithm>
#include <numeric>       // for std::accumulate, std::reduce
#include <random>
#include <string>
#include <ranges>        // C++20 ranges
#include <iomanip>

int main()
{
    // ────────────────────────────────────────────────
    // Typical working set
    // ────────────────────────────────────────────────
    std::vector<int> v = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};

    std::cout << "Original:  ";
    for (int x : v) std::cout << x << ' ';
    std::cout << "\n";

    
    // 1. Sorting (most used algorithm family)
    std::sort(v.begin(), v.end());
    // or C++20: std::ranges::sort(v);

    std::cout << "Sorted:    ";
    std::ranges::copy(v, std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";

    
    //std::stable_sort, partial_sort, nth_element)
    
    
    // 2. Binary search (requires sorted range)
    if (std::binary_search(v.begin(), v.end(), 5)) {
        std::cout << "5 is present\n";
    }

    
    // 3. lower_bound / upper_bound (very common in real code)
    auto it = std::lower_bound(v.begin(), v.end(), 5);
    std::cout << "First >= 5 is at position " << (it - v.begin())
              << " → value " << *it << "\n";

    
    // 4. count / count_if
    std::cout << "Number of 5s: " << std::count(v.begin(), v.end(), 5) << "\n";

    auto is_odd = [](int x) { return x % 2 != 0; }; //lambda function
    std::cout << "Number of odds: " << std::count_if(v.begin(), v.end(), is_odd) << "\n";

    
    // 5. min_element / max_element
    auto [min_it, max_it] = std::minmax_element(v.begin(), v.end());
    std::cout << "Min = " << *min_it << ", Max = " << *max_it << "\n";

    
    // 6. accumulate / reduce (numeric algorithms)
    int sum = std::accumulate(v.begin(), v.end(), 0);
    std::cout << "Sum = " << sum << "\n";

    int multy = std::accumulate(v.begin(), v.end(), 1, std::multiplies<int>());

    
    
    // C++17 std::reduce (can be parallelized)
    int prod = std::reduce(v.begin(), v.end(), 1, std::multiplies{});
    std::cout << "Product = " << prod << "\n";

    
    // 7. transform (very frequent)
    std::vector<std::string> words{"apple", "banana", "cherry", "date"};

    std::transform(words.begin(), words.end(), words.begin(),
        [](std::string s) {
            s[0] = std::toupper(s[0]);
            return s;
        });

    std::cout << "Capitalized: ";
    for (const auto& w : words) std::cout << w << " ";
    std::cout << "\n";

    
    // 8. remove / erase idiom (very common pattern)
    words.erase(std::remove_if(words.begin(), words.end(),
        [](const std::string& s) { return s.size() <= 5; }),
        words.end());

    std::cout << "After remove_if (<=5 chars): ";
    for (const auto& w : words) std::cout << w << " ";
    std::cout << "\n";

    
    // 9. find_if / any_of / all_of / none_of
    bool has_long = std::any_of(words.begin(), words.end(),
        [](const auto& s) { return s.size() >= 7; });

    std::cout << "Any word >= 7 chars? " << std::boolalpha << has_long << "\n";

    auto it= std::find(v.begin(),v.end(),4);
    if (it != v.end()){
        //do things
    }
    
    
    std::vector<int> dataa = {1,2,3,4,5,5,6,7};
    std::vector<int> seq = {3,4,5};
    auto it = std::search(dataa.begin(),dataa.end(),seq.begin());
    
    //find_end - find the last occurence of a sequence
    //equal - determines if two ranges are equal
    //mismatch = returns position where two ranges differ
    
    
    
    
    
    //merge two vectors sorted!
    std::vector<int> a = {1, 3, 5, 7, 9};
    std::vector<int> b = {2, 4, 6, 8, 10};

    std::vector<int> result;
    result.reserve(a.size() + b.size());           // good practice

    // Merge two sorted ranges into a new sorted range
    std::merge(a.begin(), a.end(),
               b.begin(), b.end(),
               std::back_inserter(result));
    
    //remove duplicates
    std::set_union(a.begin(), a.end(),
                   b.begin(), b.end(),
                   std::back_inserter(result));
    
    
    //or resize one of the vectors,and merge into it
    
    
    // 10. C++20 ranges version (very modern & readable)
    std::cout << "\nC++20 ranges style:\n";

    auto even = v | std::views::filter([](int x){ return x % 2 == 0; })
                  | std::views::transform([](int x){ return x * 10; });

    std::cout << "Even numbers ×10: ";
    for (int x : even) std::cout << x << ' ';
    std::cout << "\n";

    
    
    //std::iota - fills a range with sequential values incremented by 1 from initial value
    std::vector<int> vv(5);
    std::iota(vv.beging(),vv.end(),10);
    
    
    
    // Bonus: shuffle + partial_sort (common in top-k problems)
    std::random_device rd;
    std::mt19937 g(rd());

    std::shuffle(v.begin(), v.end(), g);
    std::partial_sort(v.begin(), v.begin() + 4, v.end());

    std::cout << "4 smallest elements (after shuffle + partial_sort): ";
    std::copy_n(v.begin(), 4, std::ostream_iterator<int>(std::cout, " "));
    std::cout << "\n";
    
    
    
    //adjescent difference - current element minus previous element
    std::vector<int> v1 = {3, 1, 4, 1, 5, 9, 2, 6, 5, 3, 5};
    std::vector<int> differences(v1.size());

    std::adjacent_difference(v1.begin(),v1.end(), difference.begin());
    

    /* ----------------------- */
    
    std::vector<int> a = {1, 3, 5, 7, 9};
    std::vector<int> b = {2, 3, 4, 7, 8};

    std::cout << "Input:\n";
    print(a, "a = ");
    print(b, "b = ");

    // ────────────────────────────────────────────────
    // 1. set_union
    // ────────────────────────────────────────────────
    // All elements from both ranges, sorted, duplicates removed
    std::vector<int> union_result;
    std::set_union(a.begin(), a.end(),
                   b.begin(), b.end(),
                   std::back_inserter(union_result));

    print(union_result, "set_union         → ");

    // ────────────────────────────────────────────────
    // 2. set_intersection
    // ────────────────────────────────────────────────
    // Only elements present in BOTH ranges
    std::vector<int> intersection_result;
    std::set_intersection(a.begin(), a.end(),
                          b.begin(), b.end(),
                          std::back_inserter(intersection_result));

    print(intersection_result, "set_intersection  → ");

    // ────────────────────────────────────────────────
    // 3. set_difference (a - b)
    // ────────────────────────────────────────────────
    // Elements in a that are NOT in b
    std::vector<int> diff_ab;
    std::set_difference(a.begin(), a.end(),
                        b.begin(), b.end(),
                        std::back_inserter(diff_ab));

    print(diff_ab, "set_difference a-b → ");

    // Bonus: b - a
    std::vector<int> diff_ba;
    std::set_difference(b.begin(), b.end(),
                        a.begin(), a.end(),
                        std::back_inserter(diff_ba));

    print(diff_ba, "set_difference b-a → ");

    // ────────────────────────────────────────────────
    // 4. set_symmetric_difference
    // ────────────────────────────────────────────────
    // Elements that are in a OR b, but NOT in both
    // (like XOR for sets)
    std::vector<int> symdiff_result;
    std::set_symmetric_difference(a.begin(), a.end(),
                                  b.begin(), b.end(),
                                  std::back_inserter(symdiff_result));

    print(symdiff_result, "symmetric diff    → ");
    
    return 0;
}
