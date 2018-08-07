#ifndef _T_STRING_H
#define _T_STRING_H

namespace t_std {

class string {
public:
    string(const char *src = "");
    string(const string& str);
    string(const string& str, int pos, int len = npos);
    string(const char *src, int n);
    string(int n, char c);
    string(string&& str);
    ~string();

    string& operator= (const string& str);
    string& operator= (const char* str);
    string& operator= (char c);
    string& operator= (string&& str);

    // Iterators
// begin()
// end()
// rbegin()
// rend()

    // Capacity
    int size() { return _length; }
    int length() { return _length; }
    int max_size() { return 0x7fffffff; }
    void resize(int n, char c=0);
    int capacity() { return _capacity; }
    void reserve(int n = 0) { if (n > (_length + 1)) resize_buffer(n + 1); }
    void clear();
    bool empty() const { return (_length == 0); }

    // Element access
    char& operator[] (int pos) { if (pos >= 0 && pos < _length) return _buffer[pos]; else return (char&)_null; }
    char& at(int pos) { if (pos >= 0 && pos < _length) return _buffer[pos]; else return (char&)_null; }
    
    // Modifiers
    string& operator+= (const string& str);
    string& operator+= (const char* s);
    string& operator+= (char c);
    //append
    //push_back
    //assign
    //insert
    //erase
    //replace
    void swap(string& str);

    // String operations
    const char * c_str() const { return _buffer; }
    const char * data() const { return _buffer; }
    
    // Member constants
    static const int npos = -1;

private:
    static const char _null = 0;
    char *_buffer;
    int _capacity;
    int _length;
    void resize_buffer(int size);
};

string operator+ (const string& lhs, const string& rhs);
string operator+ (const string& lhs, const char *  rhs);
string operator+ (const char *  lhs, const string& rhs);
string operator+ (const string& lhs, char          rhs);
string operator+ (char          lhs, const string& rhs);

bool operator== (const string& lhs, const string& rhs);
bool operator== (const char*   lhs, const string& rhs);
bool operator== (const string& lhs, const char*   rhs);
bool operator!= (const string& lhs, const string& rhs);
bool operator!= (const char*   lhs, const string& rhs);
bool operator!= (const string& lhs, const char*   rhs);
bool operator<  (const string& lhs, const string& rhs);
bool operator<  (const char*   lhs, const string& rhs);
bool operator<  (const string& lhs, const char*   rhs);
bool operator<= (const string& lhs, const string& rhs);
bool operator<= (const char*   lhs, const string& rhs);
bool operator<= (const string& lhs, const char*   rhs);
bool operator>  (const string& lhs, const string& rhs);
bool operator>  (const char*   lhs, const string& rhs);
bool operator>  (const string& lhs, const char*   rhs);
bool operator>= (const string& lhs, const string& rhs);
bool operator>= (const char*   lhs, const string& rhs);
bool operator>= (const string& lhs, const char*   rhs);

void swap(string& s1, string& s2);

}

#endif // _T_STRING_H
