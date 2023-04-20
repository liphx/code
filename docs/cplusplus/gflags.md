# gflags

- http://gflags.github.io/gflags/
- https://github.com/gflags/gflags.git

## type

- bool -> bool
- int32/uint32/int64/uint64 -> int*_t
- double -> double
- string -> std::string

## declare

```cpp
#include "gflags/gflags_declare.h"

#define GFLAGS_NAMESPACE google
namespace fLS {
typedef std::string clstring;
}  // namespace fLS

#define DECLARE_VARIABLE(type, shorttype, name)                  \
    namespace fL##shorttype {                                    \
        extern GFLAGS_DLL_DECLARE_FLAG type FLAGS_##name;        \
    }                                                            \
    using fL##shorttype::FLAGS_##name

#define DECLARE_bool(name)   DECLARE_VARIABLE(bool, B, name)
#define DECLARE_int32(name)  DECLARE_VARIABLE(::GFLAGS_NAMESPACE::int32, I, name)
#define DECLARE_uint32(name) DECLARE_VARIABLE(::GFLAGS_NAMESPACE::uint32, U, name)
#define DECLARE_int64(name)  DECLARE_VARIABLE(::GFLAGS_NAMESPACE::int64, I64, name)
#define DECLARE_uint64(name) DECLARE_VARIABLE(::GFLAGS_NAMESPACE::uint64, U64, name)
#define DECLARE_double(name) DECLARE_VARIABLE(double, D, name)
#define DECLARE_string(name)                                      \
    namespace fLS {                                               \
    using ::fLS::clstring;                                        \
    extern GFLAGS_DLL_DECLARE_FLAG ::fLS::clstring& FLAGS_##name; \
    }                                                             \
    using fLS::FLAGS_##name
```

## define

```cpp
#include "gflags/gflags.h"

class GFLAGS_DLL_DECL FlagRegisterer {
public:
    template <typename FlagType>
    FlagRegisterer(const char *name, const char *help, const char *filename, FlagType *current_storage,
            FlagType *defvalue_storage) {
        FlagValue *const current = new FlagValue(current_storage, false);
        FlagValue *const defvalue = new FlagValue(defvalue_storage, false);
        RegisterCommandLineFlag(name, help, filename, current, defvalue);
    }
};

#define DEFINE_VARIABLE(type, shorttype, name, value, help)                                  \
    namespace fL##shorttype {                                                                \
        static const type FLAGS_nono##name = value;                                          \
        GFLAGS_DLL_DEFINE_FLAG type FLAGS_##name = FLAGS_nono##name;                         \
        type FLAGS_no##name = FLAGS_nono##name;                                              \
        static GFLAGS_NAMESPACE::FlagRegisterer o_##name(                                    \
                #name, MAYBE_STRIPPED_HELP(help), __FILE__, &FLAGS_##name, &FLAGS_no##name); \
    }                                                                                        \
    using fL##shorttype::FLAGS_##name

#define DEFINE_bool(name, val, txt)                                                                         \
    namespace fLB {                                                                                         \
    typedef ::fLB::CompileAssert                                                                            \
            FLAG_##name##_value_is_not_a_bool[(sizeof(::fLB::IsBoolFlag(val)) != sizeof(double)) ? 1 : -1]; \
    }                                                                                                       \
    DEFINE_VARIABLE(bool, B, name, val, txt)

#define DEFINE_int32(name, val, txt)  DEFINE_VARIABLE(GFLAGS_NAMESPACE::int32, I, name, val, txt)

#define DEFINE_uint32(name, val, txt) DEFINE_VARIABLE(GFLAGS_NAMESPACE::uint32, U, name, val, txt)

#define DEFINE_int64(name, val, txt)  DEFINE_VARIABLE(GFLAGS_NAMESPACE::int64, I64, name, val, txt)

#define DEFINE_uint64(name, val, txt) DEFINE_VARIABLE(GFLAGS_NAMESPACE::uint64, U64, name, val, txt)

#define DEFINE_double(name, val, txt) DEFINE_VARIABLE(double, D, name, val, txt)

#define DEFINE_string(name, val, txt)                                                                                  \
    namespace fLS {                                                                                                    \
    using ::fLS::clstring;                                                                                             \
    using ::fLS::StringFlagDestructor;                                                                                 \
    static union {                                                                                                     \
        void *align;                                                                                                   \
        char s[sizeof(clstring)];                                                                                      \
    } s_##name[2];                                                                                                     \
    clstring *const FLAGS_no##name = ::fLS::dont_pass0toDEFINE_string(s_##name[0].s, val);                             \
    static GFLAGS_NAMESPACE::FlagRegisterer o_##name(                                                                  \
            #name, MAYBE_STRIPPED_HELP(txt), __FILE__, FLAGS_no##name, new (s_##name[1].s) clstring(*FLAGS_no##name)); \
    static StringFlagDestructor d_##name(s_##name[0].s, s_##name[1].s);                                                \
    extern GFLAGS_DLL_DEFINE_FLAG clstring& FLAGS_##name;                                                              \
    using fLS::FLAGS_##name;                                                                                           \
    clstring& FLAGS_##name = *FLAGS_no##name;                                                                          \
    }                                                                                                                  \
    using fLS::FLAGS_##name
```

for example

```cpp
DEFINE_int32(number, 0, "default is 0");
/////
DEFINE_VARIABLE(GFLAGS_NAMESPACE::int32, I, number, 0, "default is 0");
/////

namespace fLI {
static const int32_t FLAGS_nononumber = 0;
int32_t FLAGS_number = FLAGS_nononumber;
static int32_t FLAGS_nonumber = FLAGS_nononumber;
static google::FlagRegisterer o_number("number", "default is 0", __FILE__, &FLAGS_number, &FLAGS_nonumber);
}  // namespace fLI
using fLI::FLAGS_number;
```

## RegisterFlagValidator

```cpp
#include "gflags/gflags.h"

DEFINE_string(module, "aa", "one of aa|ab|bc");
DEFINE_validator(module, [](const char *, const std::string& value) {
    if (value != "aa" && value != "ab" && value != "bc") {
        return false;
    }
    return true;
});

int main(int argc, char **argv) {
    google::ParseCommandLineFlags(&argc, &argv, true);
}
```

```shell
./a.out --module=bb
ERROR: failed validation of new value 'bb' for flag 'module'
```

## ParseCommandLineFlags

```cpp
uint32_t ParseCommandLineFlags(int* argc, char*** argv, bool remove_flags);

DEFINE_string(module, "aa", "one of aa|ab|bc");

static void print_args(int argc, char **argv) {
    std::cout << "argc: " << argc << ", argv:";
    for (int i = 1; i < argc; i++) {
        std::cout << " " << argv[i];
    }
    std::cout << std::endl;
}

int main(int argc, char **argv) {
    print_args(argc, argv);
    google::ParseCommandLineFlags(&argc, &argv, true);
    print_args(argc, argv);
}
```

```shell
# remove_flags is true
$ ./a.out 1 2 --module=ab 3 4
argc: 6, argv: 1 2 --module=ab 3 4
argc: 5, argv: 1 2 3 4
# remove_flags is false, flags will be the beginning in argv
./a.out 1 2 --module=ab 3 4
argc: 6, argv: 1 2 --module=ab 3 4
argc: 6, argv: --module=ab 1 2 3 4
```
