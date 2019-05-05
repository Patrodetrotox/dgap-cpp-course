
#include <cstddef>
#include <iterator>

template <typename T>
class List {
public:
    List():
        m_head(NULL),
        m_tail(NULL),
        m_size(0)
    {}

    ~List()
    {
        Node *cur = m_head;
        while (cur) {
            Node *next = cur->next;
            delete cur;
            cur = next;
        }
        m_size = 0;
    }

    void push_front(const T& data);
    void push_back(const T& data);

    T pop_front();
    T pop_back();

    T& front();
    T& back();

    unsigned int size() { return m_size; };

private:
    struct Node;
public:

    class iterator: public std::iterator<std::bidirectional_iterator_tag, List>
    {
    public:
        friend class List;
        iterator(const iterator &c) = default;
        // operators: *, ++, --, ==, !=

        typedef std::bidirectional_iterator_tag iterator_category;

        iterator &operator++();
        iterator &operator--();
        bool operator==(const iterator &rit);
        bool operator!=(const iterator &rit);
        T &operator*();

    private:
        iterator(Node *ptr, bool end = false):
            m_ptr(ptr),
            m_end(end) { }; // accessible from List
        struct Node *m_ptr;
        bool m_end;
    };

    // Iterators:
    iterator begin();
    iterator end();

private:
    struct Node {
        Node(const T &d, Node * p = NULL, Node *n = NULL):
            data(d),
            prev(p),
            next(n)
        {}

        T data;
        Node *prev;
        Node *next;
    };

    Node *m_head;
    Node *m_tail;
    unsigned int m_size;
};

// Add element to list head
template <typename T>
void List<T>::push_front(const T &data)
{
    Node *node = new Node(data, NULL, m_head);
    if (m_head)
        m_head->prev = node;
    else
        m_tail = node;
    m_head = node;
    m_size++;
}

template <typename T>
void List<T>::push_back(const T &data)
{
    Node *node = new Node(data, m_tail, NULL);
    if (m_tail)
        m_tail->next = node;
    else
        m_head = node;
    m_tail = node;
    m_size++;
}

template <typename T>
T List<T>::pop_front()
{
    // TODO exceptions
    T buf = m_head->data;
    m_head = m_head->next;
    delete m_head->prev;
    m_head->prev = NULL;
    m_size--;
    if (!m_size)
        m_tail = NULL;
    return buf;
}

template <typename T>
T List<T>::pop_back()
{
    T buf = m_tail->data;
    m_tail = m_tail->prev;
    delete m_tail->next;
    m_tail->next = NULL;
    m_size--;
    if (!m_size)
        m_head = NULL;
    return buf;
}

template <typename T>
T& List<T>::front()
{
    return m_head->data;
}

template <typename T>
T& List<T>::back()
{
    return m_tail->data;
}

template <typename T>
typename List<T>::iterator List<T>::begin()
{
    return iterator(m_head, false);
}

template <typename T>
typename List<T>::iterator List<T>::end()
{
    return iterator(m_tail, true);
}

template <typename T>
typename List<T>::iterator &List<T>::iterator::operator++()
{
    // TODO Exceptions
    if (m_ptr->next)
        m_ptr = m_ptr->next;
    else
        m_end = true;
    return *this;
}

template <typename T>
auto List<T>::iterator::operator--() -> List<T>::iterator&
{
    if (m_end)
        m_end = false;
    else if (m_ptr->prev)
        m_ptr = m_ptr->prev;
    return *this;
}

template <typename T>
bool List<T>::iterator::operator==(const List<T>::iterator &rit)
{
    if (m_end && rit.m_end)
        return true;
    else if (m_end != rit.m_end)
        return false;
    else
        return m_ptr == rit.m_ptr;
}

template <typename T>
bool List<T>::iterator::operator!=(const List<T>::iterator &rit)
{
    return !(*this == rit);
}

template <typename T>
T &List<T>::iterator::operator*()
{
    return m_ptr->data;
}

