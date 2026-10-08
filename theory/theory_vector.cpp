#include <iostream>
#include <vector>
#include <range>
#include <algorithm>


using namespace std;
//std vector stored in heap
//heap is slower than stack
//contiguous storage - reallocate if more elements neeeded
//vector resizable



void know_algos(){
    const std::vector<int> data = {12,3,-1,-44,6,33};
    const auto is_positive = [](const auto &x){return x>0;};
    auto fist_pos_it = std::find_if(data.cbegin(),data.cend(),is_positive);
    //use first position it
}


int main() {
    vector<char> v = {'a', 'c', 'f', 'd', 'z'};
    
    auto print_v = [](char n){std::cout << n << ",";};
    std::for_each(begin(v), end(v),print_v);
    
    // Deleting last element 'z'
      v.pop_back();
      for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;
  
      // Deleting element 'f'
    vector<char>::iterator it =find(v.begin(),v.end(),'w');
    cout << *it <<endl;
      v.erase(find(v.begin(), v.end(), 'f')); //needs an iterator pointer
    
      for (int i = 0; i < v.size(); i++) {
        cout << v[i] << " ";
    }
    
    
    // Vector resized to 7 and added
    // elements initialized to 9
    v.resize(7, 9);

    for (auto i : v)
        cout << i << " ";
    
    
    //too many allocations
    //dont copy vectors, use & where necessary
    //use const references instead of copying into other vectors
    //everytime when push_back, vector grows by 50%  for example 10->15...
    //grows geometrically. Capacity increases. First do reserve or resize then push_back();
    
    //another problem is copying, for example when push_back(Data(i)) //structure
    //use instead of that emplace_back()
    
    
    
    std::vector<int> veeec = {10,20,40};
    veeec.reserve(10);
    veeec.shrink_to_fit(); //capacity = size
    
    
    //2d vectors
    std::vector<std::vector<int>> matrix(3,std::vector<int>(4,0));
    

    /*-------------------*/
    
    //merging two vectors = sorted
    //1

    std::vector<int> a = {1, 3, 5, 7, 9};
    std::vector<int> b = {2, 4, 6, 8, 10};

    std::vector<int> result;
    result.reserve(a.size() + b.size());           // good practice

    // Merge two sorted ranges into a new sorted range
    std::merge(a.begin(), a.end(),
               b.begin(), b.end(),
               std::back_inserter(result));


    //2
    //if also removing duplicates
    std::set_union(a.begin(), a.end(),
                   b.begin(), b.end(),
                   std::back_inserter(result));
    
    //3
    std::ranges::merge(a, b, std::back_inserter(result));
    
    // std::vector<int> result;
    // or result.reserve(a.size() + b.size());
    // std::ranges::merge(a, b, std::back_inserter(result));   //this also sorts the result
    //v1.insert(v1.end(), v2.begin(), v2.end()); then sort 
    
    //vector.insert + sort -> avoid, O(nlogn)
    
    //unsorted vectors
    std::vector<int> aa = {5, 1, 8, 3, 9};
    std::vector<int> bb = {4, 7, 2, 6, 0};

    std::vector<int> v3;
    v3.reserve(aa.size() + bb.size());           // does not move .end() reference

    v3.insert(v3.end(), aa.begin(), aa.end()); // inserts all elements of a at the end, end =begin since its empty
    v3.insert(v3.end(), bb.begin(), bb.end());//insert BB

    std::sort(v3.begin(), v3.end()); // sort merged!
    
    
    return EXIT_SUCCESS;
}




// Function to merge two sorted vectors
std::vector<int> merge_sorted(const std::vector<int>& vec1, const std::vector<int>& vec2) {
    std::vector<int> result;
    result.reserve(vec1.size() + vec2.size()); // Avoid reallocations

    size_t i = 0, j = 0;
    while (i < vec1.size() && j < vec2.size()) {
        if (vec1[i] <= vec2[j]) {
            result.push_back(vec1[i]);
            ++i;
        } else {
            result.push_back(vec2[j]);
            ++j;
        }
    }

    // Append remaining elements from vec1
    while (i < vec1.size()) {
        result.push_back(vec1[i]);
        ++i;
    }

    // Append remaining elements from vec2
    while (j < vec2.size()) {
        result.push_back(vec2[j]);
        ++j;
    }

    return result;
}

/*
std::vector<int> vec = {1, 3, 5, 7, 3, 9};
int target = 3;

std::cout << "Vector: ";
print_vector(vec);
std::cout << "Searching for: " << target << "\n\n";

// 1. Using std::find
auto it = std::find(vec.begin(), vec.end(), target);
if (it != vec.end()) {
    std::cout << "std::find: Found " << target << " at index "
              << (it - vec.begin()) << "\n";
} else {
    std::cout << "std::find: " << target << " not found\n";
}

// 2. Using std::find_if (example: find first element > target)
auto it_if = std::find_if(vec.begin(), vec.end(),
                          [target](int x) { return x > target; });
*/



// Max element in vector
//cout << *max_element(v.begin(), v.end()) << endl;
//
// Max element in array
//cout << *max_element(arr, arr + n);




// Find the maximum element in the vector of structure
// based on the sno field
//St max = *max_element(v.begin(), v.end(),
//                [](const St &i, const St &j) {
//                    return i.sno < j.sno;
//                });


/*
 int main() {
     vector<int> v1 = {1, 5, 6, 8};
     vector<int> v2(v1.size());

     // Incrementing all elements of vector by 1
     transform(v1.begin(), v1.end(), v2.begin(), [](int a) {return a + 1;});

     for (auto i : v2)
        cout << i << " ";
     return 0;
 }
 */
