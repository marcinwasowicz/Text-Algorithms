#include "RadixSort.h"

RadixSort::RadixSort(){}

RadixSort::~RadixSort(){}

void RadixSort::sort(vector<vector<int>> &array, int minValue, int maxValue, int fromColumn, int toColumn) {
    for(int column = toColumn; column>=fromColumn; column--){
        this->stableCountingSort(array, minValue, maxValue, column);
    }
}

void RadixSort::stableCountingSort(vector<vector<int>> &array, int minValue, int maxValue, int column) {
    vector<int> C((maxValue-minValue)+1, 0);

    for(auto tuple : array){
        C[tuple[column]-minValue]++;
    }

    for(int i = 1; i<C.size(); i++){
        C[i] += C[i-1];
    }

    vector<vector<int>> B(array.size(), vector<int>());

    for(int i = array.size()-1; i>=0; i--){
        B[C[array[i][column]-minValue]-1] = array[i];
        C[array[i][column]-minValue]--;
    }

    for(int i = 0; i<B.size(); i++){
        array[i] = B[i];
    }
}

