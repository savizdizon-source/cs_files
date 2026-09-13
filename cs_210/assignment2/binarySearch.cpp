#include <iostream>
using namespace std;

int binarySearch_R(const int* numbers, int low, int high, int key) {
    if (low > high) {
        return -1;
    }

    int mid = (low + high) / 2;
    if (numbers[mid] < key) {
        return binarySearch_R(numbers, mid + 1, high, key);
    } else if (numbers[mid] > key) {
        return binarySearch_R(numbers, low, mid - 1, key);
    }
    return mid;
}

int binarySearch_I(const int* numbers, int numberSize, int key) {
    int low = 0;
    int high = numberSize - 1;

    while (high >= low) {
        int mid = (high + low) / 2;
        if (numbers[mid] < key) {
            low = mid + 1;
        } else if (numbers[mid] > key) {
            high = mid - 1;
        } else {
            return mid;
        }

        
    }
    
    return -1;
}

int main() {
    int numbers[] = {2, 4, 7, 10, 11, 32, 45, 87};
    const int numbersSize = sizeof(numbers) / sizeof(numbers[0]);
    // Test at least five searches, including: first element, last element, middle element, missing value below the range, and missing value inside the range.
    int key = 7;
    int keyIndex1 = binarySearch_I(numbers, numbersSize, key);
    int keyIndex2 = binarySearch_R(numbers, 0, numbersSize - 1, key);

    cout << "Key Index #1 (Iterative)" << endl;
    if (keyIndex1 == -1) {
        cout << key << " was not found." << endl;
    } else {
        cout << "Found " << key << " at index " << keyIndex1 << "." << endl;
    }

    cout << "Key Index #2 (Recursive)" << endl;
    if (keyIndex2 == -1) {
        cout << key << "was not found." << endl;
    } else {
    cout << "Found " << key << " at index " << keyIndex2 << "." << endl;
    }    

    int keyFirstIndex = binarySearch_I(numbers, numbersSize, 2);
    int keyLastIndex = binarySearch_I(numbers, numbersSize, 87);
    int keyMiddleIndex = binarySearch_I(numbers, numbersSize, 11);
    int keyBelowIndex = binarySearch_I(numbers, numbersSize, 1);
    int keyAbove = binarySearch_I(numbers, numbersSize, 50);


    cout << "Test first value: " << keyFirstIndex << endl;
    cout << "Test last value: " << keyLastIndex << endl;
    cout << "Test middle value: " << keyMiddleIndex << endl;
    cout << "Test below range value: " << keyBelowIndex << endl;
    cout << "Test above range value: " << keyAbove << endl;

}

