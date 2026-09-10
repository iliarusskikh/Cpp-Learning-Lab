//This code detects "fraudulent" user IDs based on co-occurrence frequency in pairs.
//It assumes each string in the input vector represents a transaction that contains two user IDs (the first two tokens),
//and it counts how many times each user ID appears across all such pairs.
//If a user ID appears in ≥ threshold transactions, it is considered suspicious ("fraudulent").

#include <algorithm>
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <iostream>

std::vector<std::string> get_fraud_id(const std::vector<std::string>& uid, int threshold) {
    
    std::unordered_map<std::string, int> count;
    std::vector<std::string> temp;

    for (const std::string& transaction : uid) { //using range
        std::stringstream ss(transaction); //single string appended into stream
        temp.clear(); //ensure temp vector is cleard

        std::string word;
        while (ss >> word) {
        //while (std::getline(ss, word, ' ')) {
            temp.push_back(word);
        }

        // Safety: skip invalid lines (less than 2 IDs)
        if (temp.size() < 2) {
            continue;
        } // valid: 2 ids

        const std::string& id1 = temp[0];
        const std::string& id2 = temp[1];

        if (id1 != id2) {
            count[id1]++;
            count[id2]++;
        } else {
            count[id1]++; //same ID for both IDs
        }
    }

    std::vector<std::string> result; //detected IDs over threshold
    for (const auto& [id, freq] : count) { //new style
        if (freq >= threshold) { //if more than threshold transactions
            result.push_back(id);
        }
    }

    // cannot iterate through map
    // Sort descending numerically (safest with stoull to avoid overflow)
    sort(result.begin(), result.end(),
         [](const std::string& a, const std::string& b) {
             return stoull(a) > stoull(b);
         });
    
    //OR rbegin()/rend() - reverse iterator
    //sort(result.rbegin(), result.rend())
    
    return result;
}


int main() {
    std::vector<std::string> id = {
        "345366 89921 45",
        "029323 38239 23",
        "38239 345366 15",
        "029323 38239 77",
        "345366 38239 23",
        "029323 345366 13",
        "38239 38239 23"
    };

    std::vector<std::string> res = get_fraud_id(id, 3);

    std::cout << "Fraudulent IDs (appearing >= 3 times): ";
    for (const std::string& s : res) {
        std::cout << s << " ";
    }
    std::cout << "\n";

    return 0;
}


/*
 std::vector<std::pair<std::string, int>> result(mp.begin(), mp.end()); //detected IDs over threshold

 sort(result.begin(), result.end(),
      [](const auto& a, const auto& b) {
          return stoull(a.first) > stoull(b.first);
      });
 
 
 
 */


#include <algorithm>
#include <unordered_map>
#include <string>
#include <string_view>
#include <vector>
#include <iostream>
#include <charconv>

std::vector<std::string> get_fraud_id2(const std::vector<std::string>& uid, int threshold) {
    std::unordered_map<std::string_view, int> count;
    count.reserve(uid.size()*2); // Avoid re-hashing overhead

    for (const std::string& transaction : uid) {
        std::string_view sv = transaction;

        // Find space boundaries for the first two tokens
        size_t pos1 = sv.find(' ');
        if (pos1 == std::string_view::npos) continue; // Invalid format

        size_t pos2 = sv.find(' ', pos1 + 1);
        
        std::string_view id1 = sv.substr(0, pos1);
        std::string_view id2 = (pos2 == std::string_view::npos) ? sv.substr(pos1 + 1) : sv.substr(pos1 + 1, pos2 - pos1 - 1);

        if (id1 != id2) {
            count[id1]++;
            count[id2]++;
        } else {
            count[id1]++;
        }
    }

    // Collect candidates with their numeric values for efficient sorting
    struct Candidate {
        std::string_view id;
        unsigned long long num_val;
    };
    std::vector<Candidate> candidates;

    for (const auto& [id, freq] : count) {
        if (freq >= threshold) {
            unsigned long long val = 0;
            std::from_chars(id.data(), id.data() + id.size(), val); // Faster than stoull, moves data into val
            candidates.push_back({id, val});
        }
    }

    // Sort numerically descending using pre-parsed values
    std::sort(candidates.begin(), candidates.end(), [](const Candidate& a, const Candidate& b) {
            return a.num_val > b.num_val;
        });

    // Materialize output std::vector<std::string>
    std::vector<std::string> result;
    result.reserve(candidates.size());
    for (const auto& cand : candidates) {
        result.emplace_back(cand.id);
    }

    return result;
}
