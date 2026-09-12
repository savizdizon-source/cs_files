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

        return -1;
    }
}

int main() {
    int numbers[] = {2, 4, 7, 10, 11, 32, 45, 87};
    const int numbersSize = sizeof(numbers) / sizeof(numbers[0]);
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
    }
}

