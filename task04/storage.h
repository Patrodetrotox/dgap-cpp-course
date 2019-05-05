

template <typename T>
class Storage
{
public:
    Storage() = default;
    Storage(const T &val);
    ~Storage();

    void SetVal(const T &val);
    T GetVal();
private:
    T m_val;
};

