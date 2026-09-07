
//# Knapsack problems
//# Partition to two equal sum subsets

//Coin change, optimisation.
 // you are given a list of coins of different denominations and a total amount of money amount
 //write a function to compute the fewest number of coins that you need to make up that amount. Each coin can be used any number of times. If amount cannot be made up by any combination of the coins, return 01.
// Example input coins = [1,2,5] ,amount=11;
// Output : 3
 
 #include <vector>
 #include <algorithm>

 int coinChange(std::vector<int>& coins, int amount) {
     // Base case: 0 coins are needed to make an amount of 0
     if (amount == 0) return 0;
     
     // Create DP table initialized to amount + 1 (acting as infinity)
     std::vector<int> dp(amount + 1, amount + 1);
     
     // Base case state initialization
     dp[0] = 0;
     //dp[i] - represents the min amount of coins required to reach i value;
     
     // Iteratively build up combinations for every amount up to target
     for (int i = 1; i <= amount; ++i) {
         for (int& coin : coins) {
             // Check if the coin value can fit into the current target amount 'i'
             if (i >= coin) {
                 // The minimum coins for amount 'i' is either its current value,
                 // or 1 coin plus the optimal solution for the remaining amount (i - coin)
                 dp[i] = std::min(dp[i], 1 + dp[i - coin]);
             }
         }
     }
     
     // If dp[amount] is still greater than amount, it means the target was unreachable
     return dp[amount] > amount ? -1 : dp[amount];
 }
