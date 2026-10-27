// C++ program to demonstrate the creation of descending
// order set container
// set - balanced tree (red-black tree) | logO
// multiset - repeated items | logO
// unordered_set - hash table(an array of buckets containing linked lists for collisions).

#include <iostream>
#include <set>
#include <unordered_set>
#include <algorithm>
using namespace std;

struct S{
    int i;
    float b;
};

struct MyHash{
    std::size_t operator()(S const&s) const noexcept{
        std::size_t h1 = std::hash<int>{}(s.i);
        std::size_t h2 = std::hash<int>{}(s.b);
        return h1 ^ (h2 << 1); //or use boost::hash_combine
    }
};


struct Custom{
    int field1;
    int field2;
    
    // Constructor
    Custom(int _field1, int _field2): field1(_field1), field2(_field2){}
    
    bool operator<(const Custom& rhs) const{
        int lhs_sum = field1+field2;
        int rhs_sum = rhs.field1_rhs.field2;
        return lrs_sum < rhs_sum;
    }
};

struct DescendingOrder{
    //overloading comparator
    bool operator()(const int& a, const int& b)const{
        return a>b;
    }
};

void printDuplicates(int arr[], int n)
{
    // declaring unordered sets for checking and storing
    // duplicates
    unordered_set<int> intSet;
    unordered_set<int> duplicate;

    // looping through array elements
    for (int i = 0; i < n; i++) {
        // if element is not there then insert that
        if (intSet.find(arr[i]) == intSet.end())
            intSet.insert(arr[i]);

        // if element is already there then insert into
        // duplicate set
        else
            duplicate.insert(arr[i]);
    }

    // printing the result
    cout << "Duplicate item are : ";
    unordered_set<int>::iterator itr;

    // iterator itr loops from begin() till end()
    for (itr = duplicate.begin(); itr != duplicate.end();
         itr++)
        cout << *itr << " ";
        //duplicate.erase(it);
}


int main()
{
    std::set<int> initList{1,2,3,4,5,6};
    std::set<int> initList2{7,8,9,0};

    initList2.merge(initList1);
    
    std::cout << initList.contains(4) << "\n"; //true/false, same as count
    //unique values only
    set<int, greater<int> > s1;
    s1.insert(10);
    s1.insert(5);
    s1.insert(12);
    s1.insert(4);

    auto it = s1.find(5);
    //*it

    for (auto& i : s1) {
        cout << i << ' '; //12,10,5,4.. discending
    }
    
    
    s1.erase(s1.begin(), s1.find(30));//erase all elements until 30 found

    // remove element with value 50 in s2
    s1.erase(50);
    
    int arr[] = { 1, 5, 2, 1, 4, 3, 1, 7, 2, 8, 9, 5 };
    int n = sizeof(arr) / sizeof(int);

    printDuplicates(arr, n);
    if (s1.find(10) != s1.end()) {
        cout << "element found." << endl;
    }
    
    
    // multiset
    std::multiset<int> q;
    q.insert(10);
    q.insert(10);
    q.insert(10);
    // it will remove only one value of
    // 10 from multiset
    q.erase(q.find(10));
    q.erase(10); //otherwise it removes all instances of 10
    
    // it will give output 2
    std::cout << q.count(10) << endl;
    
    //.size()
    //.empty()
    //.count(10);
    //.lower_bound(10);
    
    
    //using custom comparators, in case you cannot modify the class
    std::set<int, DescendingOrder> descNumbers;
    descNumbers.insert({5,19,12,11});
    
    //operator() within custom structure
    std::multiset<Custom> myy;
    Custom c0(1,3);
    Custom c1(3,6);
    multi.insert(c0);
    multi.insert(c2);
    
    
    //19,12,11,5
    for(const auto& num : descNumbers){
        std::cout << num << " ";
    }
    
    
    
    //for unordered_set
    /*
     //when multiple index are allocated to the same bucket, they form a linked list
     bucket_count() - number of buckets in the container
     max_bucket_count()
     bucket_size(n)
     rehash(n)
     ..
     
     //we can iterate through all buckets
     for(int i=0; i<s.bucket_count(); ++i){
        std::cout << s.bucket_size(i) << "\n"; //size of the particular bucket, watch for linked-lists
     }
     
    */

    //if using custom datastructure, requires operator=
    //also HASH function needed
    std::unordered_set<int>numbers;
    int current = 0;

    // Populates 5 elements dynamically using a mutable lambda
    std::generate_n(std::back_inserter(numbers), 5, [current]() mutable {
        return current += 2;
    });
    
    std::unordered_multiset<S,MyHash,MyEq>numb;

    return 0;
}
