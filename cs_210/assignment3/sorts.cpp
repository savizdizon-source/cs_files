#include <vector>
#include <iostream>
#include <chrono>
#include <random>
#include <iomanip>
#include <string>
#include <functional>
 
class Sorts {
    public:
        bool isSorted(const std::vector<int>& values) {
            for (size_t i = 0; i + 1 < values.size(); i++) {
                if (values[i] > values[i + 1]) {
                    return false;
                }
            }
            return true;
        }
 
        void bubbleSort(std::vector<int>& values) {
            size_t n = values.size();
            bool swapped = true;
            while (swapped) {
                swapped = false;
                for (size_t i = 0; i + 1 < n; i++) {
                    if (values[i + 1] < values[i]) {
                        int temp = values[i];
                        values[i] = values[i + 1];
                        values[i + 1] = temp;
                        swapped = true;
                    }
                }
                n--; 
            }
        }

        void selectionSort(std::vector<int>& values) {
            for (size_t i = 0; i < values.size(); i++) {
                size_t minIdx = i;
                for (size_t j = i + 1; j < values.size(); j++) {
                    if (values[j] < values[minIdx]) {
                        minIdx = j;
                    }
                }
                if (minIdx != i) {
                    int temp = values[i];
                    values[i] = values[minIdx];
                    values[minIdx] = temp;
                }
            }
        }
 
        void insertionSort(std::vector<int>& values) {
            for (size_t i = 1; i < values.size(); i++) {
                int key = values[i];
                int j = i - 1;
 
                while (j >= 0 && values[j] > key) {
                    values[j + 1] = values[j];
                    j--;
                }
 
                values[j + 1] = key;
            }
        }
 
        void swap(int& a, int& b) {
            int temp = a;
            a = b;
            b = temp;
        }
 
        int partition(std::vector<int>& arr, int low, int high) {
            int pivot = arr[high];
            int i = low - 1;
 
            for (int j = low; j < high; j++) {
                if (arr[j] <= pivot) {
                    i++;
                    swap(arr[i], arr[j]);
                }
            }
            swap(arr[i + 1], arr[high]);
            return i + 1;
        }
 
        void quickSort(std::vector<int>& values, int low, int high) {
            if (low < high) {
                int pi = partition(values, low, high);
                quickSort(values, low, pi - 1);
                quickSort(values, pi + 1, high);
            }
        }
};
 
std::vector<int> makeRandom(size_t n, std::mt19937& rng) {
    std::vector<int> v(n);
    std::uniform_int_distribution<int> dist(0, 1000000);
    for (size_t i = 0; i < n; i++) v[i] = dist(rng);
    return v;
}
 
std::vector<int> makeSorted(size_t n) {
    std::vector<int> v(n);
    for (size_t i = 0; i < n; i++) v[i] = (int)i;
    return v;
}
 
std::vector<int> makeReverse(size_t n) {
    std::vector<int> v(n);
    for (size_t i = 0; i < n; i++) v[i] = (int)(n - i);
    return v;
}
 
struct Result {
    std::string algo;
    std::string dist;
    size_t n;
    double ms;
    bool correct;
};
 
int main() {
    Sorts s;
    std::mt19937 rng(42); 
 
    std::vector<size_t> sizes = {1000, 5000, 10000};
    std::vector<std::string> dists = {"random", "sorted", "reverse"};
    std::vector<Result> results;
 
    for (size_t n : sizes) {
        for (const auto& distName : dists) {
            std::vector<int> base;
            if (distName == "random") base = makeRandom(n, rng);
            else if (distName == "sorted") base = makeSorted(n);
            else base = makeReverse(n);
 
            // bubbleSort
            {
                std::vector<int> arr = base;
                auto start = std::chrono::high_resolution_clock::now();
                s.bubbleSort(arr);
                auto end = std::chrono::high_resolution_clock::now();
                double ms = std::chrono::duration<double, std::milli>(end - start).count();
                results.push_back({"bubbleSort", distName, n, ms, s.isSorted(arr)});
            }
            // selectionSort
            {
                std::vector<int> arr = base;
                auto start = std::chrono::high_resolution_clock::now();
                s.selectionSort(arr);
                auto end = std::chrono::high_resolution_clock::now();
                double ms = std::chrono::duration<double, std::milli>(end - start).count();
                results.push_back({"selectionSort", distName, n, ms, s.isSorted(arr)});
            }
            // insertionSort
            {
                std::vector<int> arr = base;
                auto start = std::chrono::high_resolution_clock::now();
                s.insertionSort(arr);
                auto end = std::chrono::high_resolution_clock::now();
                double ms = std::chrono::duration<double, std::milli>(end - start).count();
                results.push_back({"insertionSort", distName, n, ms, s.isSorted(arr)});
            }
            // quickSort
            {
                std::vector<int> arr = base;
                auto start = std::chrono::high_resolution_clock::now();
                s.quickSort(arr, 0, (int)arr.size() - 1);
                auto end = std::chrono::high_resolution_clock::now();
                double ms = std::chrono::duration<double, std::milli>(end - start).count();
                results.push_back({"quickSort", distName, n, ms, s.isSorted(arr)});
            }
        }
    }
 
    std::cout << std::left
              << std::setw(16) << "Algorithm"
              << std::setw(10) << "Input"
              << std::setw(10) << "N"
              << std::setw(14) << "Time (ms)"
              << "Correct" << "\n";
    std::cout << std::string(60, '-') << "\n";
    for (const auto& r : results) {
        std::cout << std::left
                  << std::setw(16) << r.algo
                  << std::setw(10) << r.dist
                  << std::setw(10) << r.n
                  << std::setw(14) << std::fixed << std::setprecision(3) << r.ms
                  << (r.correct ? "yes" : "NO") << "\n";
    }
 
    return 0;
}