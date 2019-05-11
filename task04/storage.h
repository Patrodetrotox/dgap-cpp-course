#include<cstring>

template <typename T>
class Storage
{
public:
    Storage() = default;
    Storage(const T &val);
    ~Storage() {

    }

    void SetVal(const T &val);
    T GetVal();
private:
    T m_val;
};

template <>
class Storage <char*> {
public:
	Storage() {
		m_val = 0;
	}
	Storage(const char* val);
	~Storage();

	void SetVal(const char* val);
	char* GetVal();
private:
	char* m_val;
};

template <typename T>
Storage<T>::Storage(const T& val) {
	m_val = val;
}

template <typename T>
void Storage<T>::SetVal(const T& val) {
	m_val = val;
}

template <typename T>
T Storage<T>::GetVal() {
	return m_val;
}

Storage<char*>::Storage(const char* val) {
	m_val = new char[strlen(val) + 1];
	std::strcpy(m_val, val);
}

Storage<char*>::~Storage() {
	if(m_val != 0) {
		delete[] m_val;
	}
}

void Storage<char*>::SetVal(const char* val) {
	if(m_val != 0) {
		delete[] m_val;
	}
	m_val = new char[strlen(val) + 1];
	std::strcpy(m_val, val);
}

char* Storage<char*>::GetVal() {
	if(m_val == 0) {
		throw std::range_error("Storage is empty");
	}
	char* val = new char[strlen(m_val) + 1];
	std::strcpy(val, m_val);
	return val;
}
