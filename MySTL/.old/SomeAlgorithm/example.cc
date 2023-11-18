#include <iostream>
#include <vector>
#include <string.h>
#include <algorithm>
#define NOT_FOUND -1
using namespace std;

template<typename Constains>
void changListValue(Constains& lst, int p)
{
    auto itr = lst.begin();
    while(itr != lst.end()) {
        *itr++ = p;
    }
}
template<typename Contains>
void removeEveryOtherItem(Contains& lst)
{
    auto itr = lst.begin();

    while(itr != lst.end()) {
        itr = lst.erase(itr);
        if(itr != lst.end()) ++itr;
    }
}

// template<typename Contains>
// auto begin(Contains& c) ->decltype(c.begin())
// {
//     return c.begin();
// }

// template<typename Contains>
// auto begin1(const Contains& c) ->decltype(c.begin())
// {
//     return c.begin();
// }

template<typename Contains>
void print(Contains& c, ostream& os = cout)
{
    if(c.empty())
        os << "This Contains is empty" << endl;
    else {
        auto itr = begin(c);
        os << "[" << *itr++;
        while(itr != c.end()) {
            os << ',' << *itr++ ;
        }
        os << ']' << endl;
    }
}




int f(vector<int>&);
int f1(vector<int>&);
int f_sum(const vector<int>&,int,int);
int fs(const vector<int>&);
int f4(const vector<int>&);

int main()
{
    // vector<int> vec {-2, 11, -4, 13, -5, -2};
    // vector<int> vec {-2, 11, -4, 13, 88, 99, 300, -5, -2};
    // print(vec);
    // vector<int>::iterator itr = vec.begin();
    // while(itr !=  vec.end())
    // cout << *itr++ << endl;
    // int vec[7]{-1, 0, 1, 2, 3, 4};
    // cout << "------------" << endl;
    // 4(cin>>a) {
    // vec.push_back(a);
    // }
    // changListValue(vec,999);
    // vector<int>::iterator itrs = vec.begin();
    // while(itrs !=  vec.end())
    // cout << *itrs++ << endl;

    return 0;
}


int f(vector<int>& vec)
{
    int maxsum = 0;
    for(int i=0; i<vec.size(); ++i) {
        for(int j=i; j<vec.size(); ++j) {
            int thissum = 0;
            for(int k=i; k<=j; ++k) {
                thissum += vec[k];
            }
            if(maxsum < thissum)
                maxsum = thissum;
        }

    }

    return maxsum;
}


int f1(vector<int>& vec)
{
    int max_sum = 0;
    for(int i = 0; i < vec.size(); ++i) {
        int this_sum = 0;
        for(int j = i; j < vec.size(); ++j) {
            this_sum += vec[j];
            if(this_sum > max_sum) max_sum = this_sum;
        }
    }
    return max_sum;
}


int f_sum(const vector<int>& vec, int left, int right)
{
    if(left == right) {
        if(vec[left] > 0)
            return vec[left];
        else
            return 0;
    }

    int center = (left+right)/2;

    int LeftSum = f_sum(vec, left, center);
    int RightSum = f_sum(vec, center+1, right);

    int LeftBordSum = 0, LeftMaxSum = 0;
    for(int i = center; i >= left; --i) {
        LeftBordSum += vec[i];
        if(LeftBordSum > LeftMaxSum) {
            LeftMaxSum = LeftBordSum;
        }
    }

    int RightBordSum = 0, RightMaxsum = 0;
    for(int j = center+1; j <= right; ++j) {
        RightBordSum += vec[j];
        if(RightBordSum > RightMaxsum) {
            RightMaxsum = RightBordSum;
        }
    }

    return max({LeftSum, RightSum, LeftMaxSum+RightMaxsum});
}

int fs(const vector<int>& vec)
{
    return f_sum(vec, 0, vec.size()-1);
}


int f4(const vector<int>& vec)
{
    int max_sum = 0, this_sum = 0;
    for(int i = 0; i < vec.size(); ++i) {
        this_sum += vec[i];

        if(this_sum > max_sum) {
            max_sum = this_sum;
        }
        else if(this_sum < 0){
            this_sum = 0;
        }

    }
    return max_sum;
}