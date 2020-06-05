#include <vector>
using namespace std;

class RadixSort{
public:
    RadixSort();
    ~RadixSort();
    void sort(vector<vector<int>>& array, int minValue, int maxValue, int fromColumn, int toColumn);
private:
    void stableCountingSort(vector<vector<int>>& array, int minValue, int maxValue, int column);
};