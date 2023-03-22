#ifndef LIPH_CSV_H_
#define LIPH_CSV_H_

#include <istream>
#include <string>
#include <vector>

namespace liph {

class csv {
public:
    csv(std::istream& is, char delim = ',');
    std::string get_row();
    size_t col_size() const;
    std::string get_col(size_t n) const;
    operator bool() const;

private:
    bool status_;
    char delim_;
    std::istream& is_;
    std::string row_;
    std::vector<std::string> col_;
};

}  // namespace liph

#endif  // LIPH_CSV_H_
