// #include "DConsole.h"
#include <iostream>
#include <fstream>
using namespace std;


template <typename Callback>
struct callback_ostreambuf : public std::streambuf
{
    using callback_t = Callback;
    callback_ostreambuf(Callback cb, void* user_data = nullptr): 
       callback_(cb), user_data_(user_data) {}

protected:
    std::streamsize xsputn(const char_type* s, std::streamsize n) override 
    { 
        return callback_(s, n, user_data_); // returns the number of characters successfully written.
    };

    int_type overflow(int_type ch) override 
    { 
        return callback_(&ch, 1, user_data_); // returns the number of characters successfully written.
    }

private:
    Callback callback_;
    void* user_data_;
};

template <typename Callback>
auto make_callback_ostreambuf(Callback cb, void* user_data = nullptr)
{
    return callback_ostreambuf<Callback>(cb, user_data);
}
int main()
{
    // cout << "Hello";
    // DConsole dc;
    // std::streambuf *sb = std::cout.rdbuf(&dc);

    auto cbsbuf = make_callback_ostreambuf([](const void *buf, std::streamsize sz, void *user_data)
                                           {
                                               fstream f("test.txt", ios::in | ios::out | ios::app);
                                               f << "Custom Boi";
                                               f.write(reinterpret_cast<const char *>(buf),sz);
                                               f.close();
                                               //std::cout.write(reinterpret_cast<const char *>(buf), sz);
                                               return sz; // return the numbers of characters written.
                                           });
    std::cout.rdbuf(&cbsbuf);
    cout << "TEST " << 42;
    cout << "Test123";
}