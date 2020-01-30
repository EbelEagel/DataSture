#include <iostream>

using namespace std;
void Assert(bool val, string s)
{                // If "val" is false, print a message and terminate the program
    if (!val) {  // Assertion failed -- close the program
        cout << "Assertion Failed: " << s << endl;
        exit(-1);
    }
}

template <typename E>
class List  // List ADT
{
private:
    void operator=(const List&) {}  // Protect assignment
    List(const List&) {}            // Protect copy constructor

public:
    List() {}           // Default constructor
    virtual ~List() {}  // Base destructor

    virtual void clear() = 0;

    // Insert item at the current location
    virtual void insert(const E& item) = 0;

    // Append item at the end of the list
    virtual void append(const E& item) = 0;

    // Remove and return the current element
    virtual E remove() = 0;

    // Set the current position to the start of the list
    virtual void moveToStart() = 0;

    // Set the current position to the end of the list
    virtual void moveToEnd() = 0;

    // Move the current position one step left
    // No change if already at beginning
    virtual void prev() = 0;

    // Move the current position one step right
    // No change if already at end
    virtual void next() = 0;

    // Return the number of elements in the list
    virtual int length() const = 0;

    // Return the position of the current element
    virtual int currPos() const = 0;

    // Set current position
    virtual void moveToPos(int pos) = 0;

    // Return the current element
    virtual const E& getValue() const = 0;
};

template <typename E>  // Array-based list implementation
class Alist : public List<E>
{
private:
    int maxSize;
    int listSize;
    int curr;
    E* listArray;

public:
    Alist(int size = 5)
    {
        maxSize = size;
        listSize = curr = 0;
        listArray = new E[maxSize];
    }

    ~Alist() { delete[] listArray; }

    void clear()
    {
        delete[] listArray;
        listSize = curr = 0;
        listArray = new E[maxSize];
    }

    void insert(const int& it)
    {
        Assert(listSize < maxSize, "List capacity exceeded");
        for (int i = listSize; i > curr; i--)
            listArray[i] = listArray[i - 1];
        listArray[curr] = it;
        listSize++;
    }

    void append(const E& it)
    {
        Assert(listSize < maxSize, "List capacity exceeded");
        listArray[listSize++] = it;
    }

    int remove()
    {
        Assert((curr >= 0) && (curr < listSize), "No element");
        E it = listArray[curr];
        for (int i = curr; i < listSize - 1; i++)
            listArray[i] = listArray[i + 1];
        listSize--;
        return it;
    }

    void moveToStart() { curr = 0; }
    void moveToEnd() { curr = listSize; }
    void prev()
    {
        if (curr != 0)
            curr--;
    }
    void next()
    {
        if (curr < listSize)
            curr++;
    }

    int length() const { return listSize; }
    int currPos() const { return curr; }

    void moveToPos(int pos)
    {
        Assert((pos >= 0) && (pos <= listSize), "Pos out of range");
        curr = pos;
    }

    const E& getValue() const
    {
        Assert((curr >= 0) && (curr < listSize), "No current element");
        return listArray[curr];
    }
};

/*
测试程序问题描述：
    在已实现顺序表的基础上，编写一个函数，用于合并两个顺序表。输入的顺序表按照其元素从小到大排序，
    输出的顺表要求按照元素从大到小排序。
*/

template <typename E>
void listUnion(Alist<E>& a, Alist<E>& b, Alist<E>& c);

int main()
{
    int m, n;
    cout << "Input m and n: ";  // m、n分别为顺序表La和Lb的容量
    cin >> m >> n;
    Alist<int> La(m);
    Alist<int> Lb(n);
    Alist<int> Lc(m + n);

    cout << "Input the elements of La: ";
    for (int i = 0; i < m; i++) {
        int element;
        cin >> element;
        La.insert(element);
        La.next();
    }
    cout << "Input the elements of La: ";
    for (int i = 0; i < n; i++) {
        int element;
        cin >> element;
        Lb.insert(element);
        Lb.next();
    }
    listUnion(La, Lb, Lc);

    cout << "The elements of La are: " << endl;
    La.moveToStart();
    for (int i = 0; i < m; i++) {
        cout << La.getValue() << " ";
        La.next();
    }
    cout << endl << "The elements of Lb are: " << endl;
    Lb.moveToStart();
    for (int i = 0; i < n; i++) {
        cout << Lb.getValue() << " ";
        Lb.next();
    }
    cout << endl << "The elements of Lc are: " << endl;
    Lc.moveToStart();
    for (int i = 0; i < m + n; i++) {
        cout << Lc.getValue() << " ";
        Lc.next();
    }
    system("pause");
    return 0;
}

template <typename E>
void listUnion(Alist<E>& a, Alist<E>& b, Alist<E>& c)
{
    a.moveToStart();
    b.moveToStart();
    while (a.currPos() != a.length() && b.currPos() != b.length()) {
        int valueA = a.getValue();
        int valueB = b.getValue();
        if (valueA == valueB) {  // 若a b相同，则插入c中
            c.insert(valueA);
            c.moveToStart();  // 为了保证倒序
            a.next();
            b.next();
        } else if (valueA > valueB) {  // 若b小，则将b的值插入c中，b后移，a不变
            c.insert(valueB);
            c.moveToStart();
            b.next();
        } else {  // 若a小，则将b的值插入c中，a后移，b不变
            c.insert(valueA);
            c.moveToStart();
            a.next();
        }
    }
    while (a.currPos() != a.length()) {  // 复制剩余的
        c.insert(a.getValue());
        c.moveToStart();
        a.next();
    }
    while (b.currPos() != b.length()) {  // 复制剩余的
        c.insert(b.getValue());
        c.moveToStart();
        b.next();
    }
}