#include <iostream>
#include <vector>
using namespace std;

class Solution {
public:
    vector<vector<int>> result; // 存放符合条件结果的集合
    vector<int> path; // 用来存放符0合条件单一结果
    void backtracking(int n, int k, int index)
    {
        if (path.size() == k) {
            result.push_back(path);
            for(auto &s : path)
            {
                cout << s << " ";
            }
            cout << endl;

            return;
        }
        for(int i = index; i <= n - (k - path.size()) + 1; i++) { //(n - k + size) + 1
            path.push_back(i); // 处理节点 
            backtracking(n, k, i + 1); // 递归
            path.pop_back(); // 回溯，撤销处理的节点
        }
    }

    vector<vector<int>> combine(int n, int k) {
        backtracking(n, k, 1);
        return result;
    }
};

int main()
{
    int n = 4;
    int k = 2;
    Solution a;
    vector<vector<int>> vec = a.combine(n, k);
    vector<vector<int>>::iterator it = vec.begin();
//
//    for(it; it != vec.end(); it++)
//    {
//        vector<int>::iterator it2 = (*it).begin();
//        for(it2; it2 != (*it).end(); it2++)
//        {
//            cout << *it2 << " ";
//        }
//        cout << endl;
//    }

    return 0;
}