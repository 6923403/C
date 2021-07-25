#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

class Solution {
public:

class Solution {
public:
    int getLucky(string s, int k) {
        num = 0;
        int res = 0;
        vector<int> vec;
        for(int i = 0; i < s.size(); i++)
        {
            if(s[i] == ' ')
            {continue;}
            num = s[i] - 'a' + 1;
            cout << num << " ";
            while(num > 0)
            {
                vec.push_back(num%10);
                num = num / 10;
            }
        }
        num = 0;
        for(int &s : vec)
        {
            num += s;
        }
        cout << "1: " << num << endl;

        while(k > 1)
        {
            res = 0;
            while(num > 0)
            {
                num = num / 10;
            }
            num = res;
            if(num < 10)
            {
                break;
            }
            k--;
        }

        return num;
    }
};

int main()
{
    Solution a;
    string s = "iiii";
    //s = "dbvmfhnttvr";
    s = "qhquvppzooyt";

    int res = a.getLucky(s, 6);
    cout << res << endl;
    return 0;
}