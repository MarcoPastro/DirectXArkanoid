#include "ArrayList.h"



ArrayList::ArrayList() : capacity(INITIAL_CAPACITY), size(0) {
    arr = new GameObject*[capacity];
}


ArrayList::~ArrayList() {
    /*for (int i = 0; i < size; ++i) {
        delete arr[i];
        arr[i] = nullptr;
    }*/
    delete[] arr;
    arr = nullptr;
}

ArrayList& ArrayList::operator=(const ArrayList& other) {
    if (this != &other) {
        /*for (int i = 0; i < size; ++i) {
            delete arr[i];
        }*/
        delete[] arr;
        arr = other.arr;
    }
    return *this;
}
void ArrayList::Add(GameObject* element) {
    if (size == capacity) {
        capacity *= 2;
        GameObject** newArr = new GameObject*[capacity];
        for (int i = 0; i < size; ++i) {
            newArr[i] = arr[i];
        }
        delete[] arr;
        arr = newArr;
    }
    arr[size] = element;
    ++size;
}

GameObject* ArrayList::Get(int index) {
    if (index < 0 || index >= size) {
        return nullptr;
    }
    return arr[index];
}

int ArrayList::GetSize() {
    return size;
}
