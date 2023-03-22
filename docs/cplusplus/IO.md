# IO

## iostream

```cpp
namespace std {

// #include <ios>
class ios_base {
public:
    ios_base(const ios_base&) = delete;  // noncopyable
    virtual ~ios_base();

protected:
    ios_base();
};

template <class CharT, class Traits = char_traits<CharT> >
class basic_ios : public ios_base {};

typedef basic_ios<char> ios;
typedef basic_ios<wchar_t> wios;

// #include <ostream>
template <class CharT, class Traits = std::char_traits<CharT> >
class basic_ostream : virtual public std::basic_ios<CharT, Traits> {};

typedef basic_ostream<char> ostream;
typedef basic_ostream<wchar_t> wostream;

extern ostream cout;
extern ostream cerr;
extern ostream clog;
extern wostream wcout;
extern wostream wcerr;
extern wostream wclog;

// #include <istream>
template <class CharT, class Traits = std::char_traits<CharT> >
class basic_istream : virtual public std::basic_ios<CharT, Traits> {};

typedef basic_istream<char> istream;
typedef basic_istream<wchar_t> wistream;

extern istream cin;
extern wistream wcin;

template <class CharT, class Traits = std::char_traits<CharT> >
class basic_iostream : public basic_istream<CharT, Traits>, public basic_ostream<CharT, Traits> {};

typedef basic_iostream<char> iostream;
typedef basic_iostream<wchar_t> wiostream;

}  // namespace std
```

## cstdio

```cpp
// #include <cstdio>
```