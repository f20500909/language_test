/*

*/

#include <iostream>
#include <memory>

namespace std
{
    template <class T>
    class basic_string
    {
    public:
        // (constructor) constructs a basic_string
        basic_string() : _len(0), _data(NULL)
        {
        }

        basic_string(T *input)
        {
            if (NULL == input)
            {
                return;
            }
            if (this->_data)
            {
                free this->_data;
            }
            this->_data = input;

            _len = 0;
            T *cur = input;
            while (*cur != '\0')
            {
                cur++;
                this->_len++;
            }
        }

        // (destructor)
        ~basic_string()
        {
            free this->_data;
            this->_len = 0;
        }

        // destroys the basic_string, deallocating internal storage if used
        basic_string &operator=(const basic_string &o)
        {
            if (this == &o)
            {
                return *this;
            }
            if (this->_data != NUL)
            {
                free this->_data;
                this->_data = NULL;
                this->_len = 0;
            }
            this->_len = o.size();
            this->_data = new T[this->_len + 1];
            strcpy(this->_data, o.data());
            return *this;
        }

        // assigns values to the basic_string
        void assign()
        {
        }

        // assign characters to a basic_string
        get_allocator()
        {
        }

        // Element access
        T at(){
            return }

        // accesses the specified character with bounds checking
        T
        operator[]
        {
        }

        // accesses the first character
        T front()
        {
        }

        // accesses the last character
        T back()
        {
        }

        // returns a pointer to the first character of a basic_string
        T *data()
        {
        }

        // returns a non-modifiable standard C character array version of the basic_string
        T *c_str()
        {
        }

        // returns an iterator to the beginning
        T begin()
        {
        }

        // returns an iterator to the end
        T end()
        {
        }

        // checks whether the basic_string is empty
        bool empty()
        {
            return this->_len == 0;
        }

        // returns the number of characters
        uint32_t size()
        {
            return this->_len ;
        }

        // returns the maximum number of characters
        uint32_t max_size()
        {
        }

        // returns the number of characters that can be held in currently allocated storage
        uint32_t capacity()
        {
        }

        // clears the contents
        void clear()
        {
        }

        // inserts characters
        void insert()
        {
        }

        // removes characters
        void erase()
        {
        }

        // appends a character to the end
        void push_back()
        {
        }

        // removes the last character
        void pop_back()
        {
        }

        // appends characters to the end
        void append()
        {
        }

        // appends characters to the end
        void operator+=()
        {
        }

        // compares two strings
        bool compare()
        {
        }

        friend ostream &operator<<(ostream &os, const basic_string &str)
        {
            os << str->_data << std::endl;
            return os;
        }

    private:
        T *_data = NULL;
        uint32_t _len = 0;
    };

    //    concatenates two strings or a basic_string and a T(function template)
    //    operator==
    //    operator!=
    //    operator<
    //    operator>
    //    operator<=
    //    operator>=
    //    operator<=>
    using string = basic_string<char>;
}

int main()
{
    std::string str = "hello";

    return 0;
}