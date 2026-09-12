// C++ program to illustrate the begin and end iterator
#include <iostream>
#include <map> //self balancing binary search tree - elements end up sorted
#include <string>
#include <unordered_map>//hash table - may be faster than map
#include <sstream>


//multimap - same keys allowed; sorted order

using namespace std;


//for unordered_map custom hash
struct CityRecord{
    std::string Name;
    uint64_t Population;
    double Latitude, Longitude;
};

//everytime your map needs a hash function for custom structure
namespace std{
    tempalte<>
    struct hash<CityRecord>
    {
        size_t operator()(const& key){
            return hash<std::string>()(key.Name);
        }
        
    }
}



int main()
{
    // Create a map of strings to integers
    map<string, int> mp;
    
    // Insert some values into the map
    mp["one"] = 1;
    mp["two"] = 2;
    mp["three"] = 3;

    // Get an iterator pointing to the first element in the
    // map
    map<string, int>::iterator it = mp.begin();

    // Iterate through the map and print the elements
    while (it != mp.end())
    {
        mp[it->first]++;
        cout << "Key: " << it->first
             << ", Value: " << it->second << endl;
        ++it;
    }
    
    cout << "Size of map: " << mp.size() << endl;

    
    // empty map container
    map<int, int> gquiz1;

    // insert elements in random order
    gquiz1.insert(pair<int, int>(1, 40));
    gquiz1.insert(pair<int, int>(2, 30));
    gquiz1.insert(pair<int, int>(3, 60));
    gquiz1.insert(pair<int, int>(4, 20));
    gquiz1.insert(pair<int, int>(5, 50));
    gquiz1.insert(pair<int, int>(6, 50));
    map<int, int> gquiz2(gquiz1.begin(), gquiz1.end());
    gquiz2.erase(gquiz2.begin(), gquiz2.find(3));

    if (mp.count("four") > 0)
    {
         cout << "Key 'four' is in the map" << endl;
    }
    
    for (auto a : mp)
    {
        mp[a.first]++;
    }
    
    
    multimap<int, int>::iterator it2; // empty multimap container

    multimap<int, int> mumap; // empty multimap container
    // insert elements in random order
    mumap.insert(pair<int, int>(1, 40));
    mumap.insert(pair<int, int>(2, 30));
    mumap.insert(pair<int, int>(3, 60));
    mumap.insert(pair<int, int>(6, 10));
    mumap.insert(pair<int, int>(6, 10));
    while (it2 != mumap.end())
    {
        //mumap[it2->second]++;
        cout << "Key: " << it2->first << ", Value: " << it2->second << endl;
        ++it2;
    }
    
    
    
    unordered_map<string, double> umap = {  //inserting element directly in map
     {"One", 1},
     {"Two", 2},
     {"Three", 3}
   };

     // inserting values by using [] operator
     umap["PI"] = 3.14;
     umap["root2"] = 1.414;
     umap["root3"] = 1.732;
     umap["log10"] = 2.302;
     umap["loge"] = 1.0;

     // inserting value by insert function
     umap.insert(make_pair("e", 2.718));

     string key = "PI";

     // If key not found in map iterator
     // to end is returned
     if (umap.find(key) == umap.end())
       cout << key << " not found\n\n";
    
    
    
    
    
    
    
    
    
    //print frequencies
    string str = "geeks for geeks geeks quiz "
                 "practice qa for";
    
    // each word is mapped to its frequency
    unordered_map<string, int> wordFreq;
    
    // breaking input into word using
    // string stream
    // Used for breaking words
    stringstream ss(str);
    
    // To store individual words
    string word;
    while (ss >> word)
      wordFreq[word]++;

    // now iterating over word, freq pair
    // and printing them in <, > format
    unordered_map<string, int>:: iterator p;
    for (p = wordFreq.begin(); p != wordFreq.end(); p++)
      cout << "(" << p->first << ", " << p->second << ")\n";
    
    string nn;
    double gg;
    for (auto& [nn,gg] : umap){
        std::cout << nn << " " << gg << "\n";
    }
    
    
    
    
    
    
    //unordered map
    //unordered map is a container that stores key-value pairs in a hash table providing fast access by key
    //hash based implementation O(1) average look up
    //no guaranteed element order
    //keys must be unique
    unordered_map<CityRecord, string> mmap; //cannot hash custom data, alternatively use as a pointer CityRecord* or provide a hash implementation
    
    std::unordered_map<std::string,int>scores {
        {"Alice",99},
        {"Bob",22},
        {"Geg",12},
    };
    scores["Alice"] = 99;
    if(scores.count("Dave")>0){
        //key exists
    }
    int aliceScore = scores["Alice"];//if using at() would through exception if not found
    scores.erase("Alice");
    auto it = scores.find("Bob");
    
    
    for (const auto& pair:scores){
        std::cout << pair.first <<" "<<pair.second<<std::endl;
    }
    
    
    for (const auto& [name,score] :scores){
        std::cout << name <<" "<<score<<std::endl;
    }
    
    //performance
    //Lookup O(1) -> O(n) worst
    //Insertion  O(1) -> O(n);
    //Deletion O(1) -> O(n);
    //worst case occurs when many elements hash to the same bucket collision
    //performance degrades as load factor increases
    
    //optimising performance
    //reserve capacity to avoid rehashing
    std::unordered_map<std::string,int>map;
    map.reserve(1000); //reserve for 1000 elements
    
    //control load factor to balance the memroy usage vs speed
    //get current load factor
    map.load_factor();//set maximum load factor;
    map.max_load_factor(0.7);//use custom hash functions for complex keys
    //lower factor (increase buckets)reduce collisions but increases memory consumption
    //to use the custom type as key, a hash function needs to be defined
    
    
    
    
    
    
    /*-------------------------------------------------*/
    
    for (auto& kv : umap){
        std::string& namee = kv.first;
        double kkkk = kv.second;
    }
    
    //iterating through a vector is still faster than a map
    
    
    
    
    
    
    
    return 0;
}

//theory_maps.cpp


/*
 
 #include <iostream>
 #include <map>
 #include <string>
 #include <tuple>

 struct Position {
     int qty;
     std::string sym;

     // std::map needs operator< (via std::less<Position> by default)
     bool operator<(const Position& other) const {
         return std::tie(sym, qty) < std::tie(other.sym, other.qty);
     }
 };

 int main() {
     std::map<Position, double> positionMap; // no custom comparator needed now

     positionMap[{100, "AAPL"}] = 150.25;
     positionMap[{50, "GOOG"}] = 2800.50;
     positionMap[{75, "AAPL"}] = 151.00; // different qty, same sym -> distinct key

     for (const auto& [pos, price] : positionMap) {
         std::cout << pos.sym << " qty=" << pos.qty << " -> $" << price << "\n";
     }
 }
 */





struct NewStruct{
    int a;
    double k;
};

struct MyHas {
    size_t operator()(const NewStruct& s1) const noexcept {
        size_t h1 = std::hash<int>()(s1.a);
        size_t h2 = std::hash<double>()(s1.k);
        
        // Combine hashes (using a standard bit-shift hash combine pattern)
        return h1 ^ (h2 + 0x9e3779b9 + (h1 << 6) + (h1 >> 2));
    }
};

struct MyEq {
    bool operator()(const NewStruct& a, const NewStruct& b) const noexcept {
        return (a.a == b.a) && (a.k == b.k);
    }
};

// Map instantiation
std::unordered_map<NewStruct, int, MyHas, MyEq> mmp;
