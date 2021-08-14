#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
using namespace std;

class Solution {
public:
    int minStoneSum(vector<int>& piles, int k) {
        priority_queue<int> que(piles.begin(), piles.end());
        while(k--)
        {
            int n = que.top();
            que.pop();
            que.push(n - (n / 2));
        }

        int num = 0;
        while(!que.empty())
        {
            num += que.top();
            que.pop();
        }

        return num;
    }
};

//class Solution {
//public:
//    int minStoneSum(vector<int>& piles, int k) {
//        sort(piles.rbegin(), piles.rend());
//        for(auto &n : piles)
//        {
//            cout << n << endl;
//        }
//        for(int i = 0; i < k; i++)
//        {
//            piles[i] = piles[i] - (piles[i] / 2);
//        }
//
//        int num = 0;
//        for(auto &s : piles)
//        {
//            num += s;
//        }
//
//        return num;
//    }
//};

int main()
{
    Solution a;
    vector<int> vec = {4122,9928,3477,9942};
    std::cout << a.minStoneSum(vec, 6) << std::endl;
    return 0;
}