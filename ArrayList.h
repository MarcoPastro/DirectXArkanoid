#pragma once
class GameObject;

class ArrayList {
public:
    /*
        @brief
        create an arraylist of gameobject with an initial capacity
    */
    ArrayList();
    /*
        @brief
        Destroy only the array not the objects
    */
    ~ArrayList();
    /*
        @brief
        change the pointer to the other arraylist and do NOT delete the objects
        @param other the other arraylist
    */
    ArrayList& operator=(const ArrayList& other);
    /*
        @brief
        add an element and update the capacity if needed
        @param element the element to add
    */
    void Add(GameObject* element);
    /*
        @brief
        get an element with a index
        @param index the index
        @return the GameObject or null
    */
    GameObject* Get(int index);
    /*
        @brief
        get the size of the array
        @return the size
    */
    int GetSize();

private:
    static const int INITIAL_CAPACITY = 25;
    GameObject** arr;
    int capacity;
    int size;
};
