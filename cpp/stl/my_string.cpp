/*

*/
#include <iostream>
#include <ostream>
#include <istream>
#include <memory>

namespace my_std
{
    template <class T>
    class basic_string
    {
    public:
        // (constructor) constructs a basic_string
        basic_string()
        {
            // defalut alloc 8 object
            this->_alloc.allocate(8);
        }

        basic_string(T *input)
        {
            std::cout << "basic_string constructor ..." << std::endl;

            size_t input_len = 0;
            T *cur = input;
            while (*cur != '\0')
            {
                cur++;
                input_len++;
            }
            this->do_alloc(input_len);

            cur = input;
            for (int i = 0; i < input_len; i++)
            {
                this->_alloc.construct(this->_end, *cur);
                cur++;
            }

            std::cout << input_len;
            return;
        }

        basic_string(T t) : basic_string(t, 1)
        {
        }

        basic_string(T t, size_t n)
        {
            this->do_alloc(n);
            return;
        }

        void do_alloc(size_t len)
        {
            // 8 16 32 64 128...
            int alloc_len = 8;
            while (len > alloc_len)
            {
                alloc_len *= 2;
            }
            this->_begin = _alloc.allocate(alloc_len);
            this->_end = this->_begin;
            return;
        }

        void construct_func()
        {
            // this->_alloc.construct(this->_end, T);
        }

        size_t max_size()
        {
            return this->_alloc.max_size();
        }

        // (destructor)
        ~basic_string()
        {
            // free this->_data;
            // this->_len = 0;
        }
        size_t size()
        {
            return this->_len;
        }

        std::ostream &operator<<(std::ostream &cout)
        {
            // if (str.size() == 0)
            // {
            //     cout << "";
            // }
            // else
            // {
            //     T *cur = this->_begin;
            //     while (cur != this->_end)
            //     {
            //         cout << *cur;
            //     }
            // }

            return cout;
        }

        // friend std::ostream &operator<<(std::ostream &os, const basic_string &s);
        // friend std::istream &operator>>(std::istream &is, basic_string &s);

        const T *begin() const
        {
            return this->_begin;
        }

        const T *end() const
        {
            return this->_end;
        }

        // private:
        void flush_meta()
        {
        }
        T *_begin = NULL;
        T *_end = NULL;
        size_t _capacity = 0;

        size_t _len = 0;

        std::allocator<T> _alloc; // 1.
    };

    // Member functions
    // (constructor)
    // Construct basic_string object (public member function )
    // (destructor)
    // String destructor (public member function )
    // operator=
    // String assignment (public member function )

    // Iterators:
    // begin
    // Return iterator to beginning (public member function )
    // end
    // Return iterator to end (public member function )
    // rbegin
    // Return reverse iterator to reverse beginning (public member function )
    // rend
    // Return reverse iterator to reverse end (public member function )
    // cbegin
    // Return const_iterator to beginning (public member function )
    // cend
    // Return const_iterator to end (public member function )
    // crbegin
    // Return const_reverse_iterator to reverse beginning (public member function )
    // crend
    // Return const_reverse_iterator to reverse end (public member function )

    // Capacity:
    // size
    // Return size (public member function )
    // length
    // Return length of string (public member function )
    // max_size
    // Return maximum size (public member function )
    // resize
    // Resize string (public member function )
    // capacity
    // Return size of allocated storage (public member function )
    // reserve
    // Request a change in capacity (public member function )
    // clear
    // Clear string (public member function )
    // empty
    // Test whether string is empty (public member function )
    // shrink_to_fit
    // Shrink to fit (public member function )

    // Element access:
    // operator[]
    // Get character of string (public member function )
    // at
    // Get character of string (public member function )
    // back
    // Access last character (public member function )
    // front
    // Access first character (public member function )

    // Modifiers:
    // operator+=
    // Append to string (public member function )
    // append
    // Append to string (public member function )
    // push_back
    // Append character to string (public member function )
    // assign
    // Assign content to string (public member function )
    // insert
    // Insert into string (public member function )
    // erase
    // Erase characters from string (public member function )
    // replace
    // Replace portion of string (public member function )
    // swap
    // Swap string values (public member function )
    // pop_back
    // Delete last character (public member function )

    // String operations:
    // c_str
    // Get C-string equivalent
    // data
    // Get string data (public member function )
    // get_allocator
    // Get allocator (public member function )
    // copy
    // Copy sequence of characters from string (public member function )
    // find
    // Find first occurrence in string (public member function )
    // rfind
    // Find last occurrence in string (public member function )
    // find_first_of
    // Find character in string (public member function )
    // find_last_of
    // Find character in string from the end (public member function )
    // find_first_not_of
    // Find non-matching character in string (public member function )
    // find_last_not_of
    // Find non-matching character in string from the end (public member function )
    // substr
    // Generate substring (public member function )
    // compare
    // Compare strings (public member function )

    // Non-member function overloads
    // operator+
    // Concatenate strings (function template )
    // relational operators
    // Relational operators for basic_string (function template )
    // swap
    // Exchanges the values of two strings (function template )
    // operator>>
    // Extract string from stream (function template )
    // operator<<
    // Insert string into stream (function template )
    // getline
    // Get line from stream into string (function template )

    // Member constants
    // npos
    // Maximum value of size_type (public static member constant )

    //    concatenates two strings or a basic_string and a T(function template)
    //    operator==
    //    operator!=
    //    operator<
    //    operator>
    //    operator<=
    //    operator>=
    //    operator<=>
    using string = basic_string<char>;

};

// std::istream &operator>>(std::istream &cin, my_std::string &str)
// {
//     if (str.string != NULL)
//     {
//         delete[] str.string;
//         str.string = NULL;
//     }
//     char temp[10];
//     cin >> temp;
//     str.string = new char[strlen(temp) + 1];
//     strcpy_s(str.string, strlen(temp) + 1, temp);
//     str.m_size = strlen(temp);
//     return cin;
// }

std::ostream &operator<<(std::ostream &os, const my_std::string &s)
{
    const char *b = s.begin();
    const char *e = s.end();
    std::cout << "b: " << &b << std::endl;
    std::cout << "e: " << &e << std::endl;
    while (b != e)
    {
        os << *b;
        os << 111;
        b++;
    }
    return os;
}

int main()
{

    my_std::string str("hello");
    std::cout << str << std::endl;

    return 0;
}