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

template <typename E>  // Singly linked list node
class Link
{
public:
    E element;   // Value for this node
    Link* next;  // Pointer to next node in list

    Link(const E& elemval, Link* nextval = NULL)
    {
        element = elemval;
        next = nextval;
    }
    Link(Link* nextval = NULL) { next = nextval; }
};

template <typename E>  // Linked list implementation
class LList : public List<E>
{
private:
    Link<E>* head;  // Pointer to list header
    Link<E>* tail;  // Pointer to last element
    Link<E>* curr;  // Access to current element
    int cnt;        // Size of list

    void init()  // Initialization helper method
    {
        curr = tail = head = new Link<E>;
        cnt = 0;
    }

    void removeall()  // Return link node to free store
    {
        while (head != NULL) {
            curr = head;
            head = head->next;
            delete curr;
        }
    }

public:
    LList(int size = 5) { init(); }  // Constructor
    ~LList() { removeall(); }        // Destructor

    void print() const;
    void clear()
    {
        removeall();
        init();
    }

    // Insert "it" at current position
    void insert(const E& it)
    {
        curr->next = new Link<E>(it, curr->next);
        if (tail == curr)
            tail = curr->next;  // New tail
    }

    void append(const E& it)
    {
        tail = tail->next = new Link<E>(it, NULL);
        cnt++;
    }

    // Remove and return current element
    E remove()
    {
        Assert(curr->next != NULL, "No element");
        E it = curr->next->element;   // Remember value
        Link<E>* ltemp = curr->next;  // Remember link node
        if (tail == curr->next)
            tail = curr;  // Reset tail
        curr->next = curr->next->next;
        delete ltemp;
        cnt--;
        return it;
    }

    void moveToStart() { curr = head; }
    void moveToEnd() { curr = tail; }

    void prev()
    {
        if (curr == head)
            return;
        Link<E>* temp = head;
        while (temp->next != curr)
            temp = temp->next;
        curr = temp;
    }

    void next()
    {
        if (curr != tail)
            curr = curr->next;
    }

    int length() const { return cnt; }

    int currPos() const
    {
        Link<E>* temp = head;
        int i;
        for (i = 0; curr != temp; i++)
            temp = temp->next;
        return i;
    }

    void moveToPos(int pos)
    {
        Assert((pos >= 0) && (pos <= cnt), "Position out of range");
        curr = head;
        for (int i = 0; i < pos; i++)
            curr = curr->next;
    }

    const E& getValue() const
    {
        Assert(curr->next != NULL, "No value");
        return curr->next->element;
    }
};