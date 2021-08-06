#include <vector>
#include <iostream>
#include <string>
using namespace std;


class Solution {
public:
    vector<vector<int>> result;
    vector<int> path;

    void backtracking(vector<int> &nums, int index, vector<bool> &used)
    {
        result.push_back(path);

        for(int i = index; i < nums.size(); i++)
        {
            if (i > 0 && nums[i] == nums[i - 1] && used[i - 1] == false) {
                continue;
            }
            path.push_back(nums[i]);
            used[i] = true;
            backtracking(nums, i + 1, used);
            path.pop_back();
            used[i] = false;
        }
    }

    vector<vector<int>> subsetsWithDup(vector<int>& nums) {
        vector<bool> used(nums.size(), false);
        sort(nums.begin(), nums.end()); // 去重需要排序
        backtracking(nums, 0, used);
        return result;
    }
};

int main()
{
    Solution a;
    vector<int> nums = {1,2,2};
    vector<vector<int>> vec = a.subsetsWithDup(nums);

    vector<vector<int>>::iterator it = vec.begin();
    for(it; it != vec.end(); it++)
    {
        vector<int>::iterator it2 = (*it).begin();
        for(it2; it2 != (*it).end(); it2++)
        {
            cout << *it2 << " ";
        }
        cout << endl;
    }

    return 0;
}