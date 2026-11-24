#include <iostream>
#include <vector>

#include <unordered_map>

//custom iterator

template<typename T>
class MyIter{
public:
    using iterator_category = std::forward_iterator;
    using value_type = T;
    using reference = T&;
    using pointer = T*;
    using difference_type = std::ptrdiff_t;
    
    //required operations
    MyIter& operator++();
    reference operator*() const;
    bool operator==(const MyIter&) const;
};


int main() {
    std::vector<int> values = {1,2,4,6,87,2,5,7,33};
    
    /*
    for (int i=0; i< values.size(), i++){
        std::cout << values[i] <<"\n";
    }
    
    for (int value: values){
        std::cout << value << "\n";
    }*/
    
    for (std::vector<int>::iterator it =values.begin(); it!= values.end(); it++){
        std::cout << *it << "\n";//iterators useful when need to manupulate the collection of elements
        *it = *it  +1 ; //to modify via iterator
    }
    
    for (auto it =values.begin(); it!= values.end(); it++){/* */} //use of auto
    
    
    //types of iterators
    
    //std::istream_iterator
    //std::ostream_iterator
    //std::forward_list::iterator
    //std::list::iterator, std::map::iterator    bidirectional
    
    //random access iterators
    std::vector<int> nuum = {10,2030,40,04,30,40,44,21};
    auto iter = nuum.begin();
    int first =  *iter; //or it[0];
    //distance it-nuum.begin();
     
    //begin(), .end(), cbegin() const iterator, rbegin() rend();
    // Range-based for-loop
    for(int& num : nuum){
        //const int& - for efficiency
        num*=2;// reference for modification
    }
    
    auto ite = std::find(nuum.begin(),nuum.end(), 8);
    
    auto itad = nuum.begin();
    std::advance(itad, 2);
    std::cout << *itad << '\n';
    std::cout << std::distance(itad, vec.end()) << '\n';
    
    //transform elements
    
    //iterators may become invalid if container is modified - resized or push, etc
    
    
    std::vector<int> val = {1,2,4,6,87,2,5,7,33};
    auto it = val.begin();
    auto it2 = cbegin(val); // const mode, does not allow modifying
    auto it3 = rbegin(val); // reverse iterator
    auto myData = data(val);
    for (size_t i=0; i<size(val);i++){
        std::cout << myData[i] << std::endl;
    }
    
    
    std::unordered_map<std::string,int> map;
    map["lel"] = 5;
    map["kek"] = 2;
    
    
    using ScoreMap =  std::unordered_map<std::string,int>;
    for (ScoreMap::const_iterator it =  map.begin(). it!= map.end(); it++){
        auto& key = it->first;//& not copying the value
        auto& value = it->second;
    }

    for (auto kv:map){
        auto& key = kv.first;//& not copying the value
        auto& value = kv.second;
    }
    
    
    for (auto [key, value] :map){
        //auto& key = kv.first;//& not copying the value
        //auto& value = kv.second;
    }
    
    return 0;
}
