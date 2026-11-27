//pip install cppman for manuals
// example: cppman -h
#include <algorithm>
#include <cmath>
#include <iostream>
#include <iterator>
#include <vector>
#include <numeric> //iota
#include <list>

int main() {
    
    //std::transform
    std::vector<int> vecs = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    std::sort(vecs.begin(), vecs.end(), [](int a, int b) { return std::abs(a) < std::abs(b); });
    std::transform(vecs.begin(), vecs.end(), vecs.begin(), [](int n) { return n * n; });

    //std::merge
    std::vector<int> a = {1, 3, 5, 7};
    std::vector<int> b = {2, 4, 6};
    std::vector<int> result;
    std::merge(a.begin(), a.end(), b.begin(), b.end(), std::back_inserter(result));

    //std::unique
    std::vector<int> v = {1, 2, 2, 3, 3, 1, 4, 4};
    std::sort(v.begin(), v.end());
    v.erase(std::unique(v.begin(), v.end()), v.end());

    for (int n : result) {
        std::cout << n << ' ';
    }
    std::cout << "\nunique: ";
    for (int n : v) {
        std::cout << n << ' ';
    }
    std::cout << '\n';
    
    
    // std::find
    std::vector<int> w = {1, 2, 3, 3, 7, 8, 9 , 4, 5, 6, 7, 8, 9, 10};
    auto it = std::find(w.cbegin(),w.cend(),7); //const iterator, we dont modidying
    if (it!= w.end())
        std::cout<< *it <<'\n';
    
    auto is_even = [](int i){return i%2==0;}
    auto it2 = std::find_if(v.begin(), v.end(), auto);
    
    std::vector<int> seq{2,3,3};
    auto res = std::search(w.begin(),w.end(), seq.begin(),seq.end());
    
    //adjacent elemnents find
    auto res2 = std::adjacent_find(w.begin(),w.end());// 3
    auto dis = std::distance(w.begin(), res2);
    
    
    // std::mismatch
    std::vector<int> w2 = {1, 2, 3, 3, 7, 8, 9 , 4, 5, 6, 7, 8, 9, 10};
    std::vector<int> w3 = {1, 2, 4, 3, 7, 8, 9 , 4, 5, 6, 7, 8, 9, 10};

    auto res3 = std::mismatch(w2.begin(),w2.end(), w3.begin()); //return a first mismatched element it
    if(*res3.first == *res3.second){
        //no mismatch found
    }
    std::cout << res3.first << "!=" << res3.second << '\n';
    
    
    // std::equal
    bool res4 = std::equal(w2.begin(),w2.end(), w3.begin());
    //example for strings
    bool res5 = std::equal(w2.begin(),w2.end(), w3.begin(), w3.end(), [](char i, char j){return toupper(i)==toupper(j)});

    
    //std::lexicographical_compare - returns true if one range is lexi.. less than another
    std::string s1 = "apple";
    std::string s2 = "microsoft";
    bool res4 = std::lexicographical_compare(s1.begin(), s1.end(), s2.begin(), s2.end());
    
    
    //std::all_of, any_of, none_of
    std::vector<int> w4 = {1, 2, 4, 3, 7, 8, 9 , 4, 5, 6, 7, 8, 9, 10};
    auto res5 = std::all_of(w4.begin(), w4.end(), [](int i){return i>0});
    res5 = std::any_of(w4.begin(), w4.end(), [](int i){return i>8});
    res5 = std::none_of(w4.begin(), w4.end(), [](int i){return i<1});
    
    
    //std::count, count_if
    res5 = std::count(w4.begin(), w4.end(),4); //counts 4
    res5 = std::count_if(w4.begin(), w4.end(),[](int elem){return elem>=4}); //counts >4

    
    //std::copy copy_if ; copy_n(copies only n elements
    std::vector<int> from_vec(10);
    std::iota(from_vec.begin(), from_vec.end(), 0);
    std::vector<int> to_vec;
    std::copy(from_vec.begin(), from_vec.end(), std::back_inserter(to_vec));
    
    std::string s3 = "Hello my World";
    std::string s4;
    std::copy_if(s3.begin(), s3.end(), std::back_inserter(s4), [](char c){return c>64 && c>91;}) //upper case
    std::copy_n(s3.begin()+5, s3.end(), std::back_inserter(s4), s3.length()-5);
    
    
    //std::fill fill_n
    std::vector<int> ints;
    ints.reserve(100);
    std::fill(ints.begin(), ints.end(), 100); //same values set
    std::fill_n(ints.begin(), 3, 22); // fills first 3 values with 22

    
    //std::generate and generate_n

    int f(){
        static int i=0;
        return i++;
    }
    std::vector<int> ints2(10);//need to explicitly reserve
    std::generate(ints2.begin(),ints2.end(),f);
    std::generate(ints2.begin(),ints2.end(),[n=0]()mutable {return n++;});// mutable to set N as writable, captured by copy

    
    //std::reverse reverse_copy
    std::reverse(begin(ints2),end(ints2));
    std::reverse_copy(begin(ints2),end(ints2),begin(ints));

    
    //std::remove remove_if remove_copy
    using DataType = std::vector<int>;
    DataType collection{1,4,6,7,4,3,6,7,2};
    //pushes towards the end of the vector
    auto newEnd = std::remove(begin(collection), end(collection), 2);
    //now perform actual removal of the elements
    collection.erase(newEnd, end(collection));
    
    
    //std::sample
    std::vector<int> units{1,4,6,32,-1,3,7};
    std::vector<int> out;
    std::sample(begin(units),end(units),
                std::back_inserter(out),
                3,
                std::mt19937{std::random_device{}()}); //3 random ints
    
    
    //std::rotate
    std::vector<int> rot{1,4,6,32,-1,3,7};
    std::rotate(rot.begin(), rot.begin()+2, rot.end()); //6,32,-1,3,7,1,4 -> pop from begin, push at the end
    std::rotate(rot.begin(), rot.begin()+2, rot.end()-1);//skip last element
    
    
    //std::shuffle
    
    // 1. Obtain a non-deterministic seed from the OS/hardware
    std::random_device rd;
    // 2. Seed the Mersenne Twister PRNG engine with rd()
    std::mt19937 gen(rd());
    std::vector<int> shuf{1, 2, 3, 4, 5, 6, 8};
    std::shuffle(shuf.begin(), shuf.end(), gen);
    
    
    //std::unique and unique_copy
    std::vector<int> un{1, 2, 3, ,3,4, 5, 6, 11, 21, 123, 4, 5, 6, 8};

    
    
    return 0;
}

