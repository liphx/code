#include "liph/csv.h"

#include <cassert>
#include <sstream>
#include <stdexcept>

namespace liph {

namespace {

bool split(const std::string& str, std::vector<std::string>& out, char delim) {
    std::string s;
    int status = 0;
    for (char ch : str) {
        if (status == 0 && ch != delim && ch != '"') {
            s += ch;
            status = 1;
        } else if (status == 0 && ch == delim) {
            out.emplace_back();
        } else if (status == 0 && ch == '"') {
            status = 2;
        } else if (status == 1 && ch != delim) {
            s += ch;
        } else if (status == 1 && ch == delim) {
            out.emplace_back(s);
            s.clear();
            status = 0;
        } else if (status == 2 && ch != '"' && ch != '\\') {
            s += ch;
        } else if (status == 2 && ch == '"') {
            status = 4;
        } else if (status == 2 && ch == '\\') {
            status = 3;
        } else if (status == 3) {
            switch (ch) {
            case 'b':
                s += '\b';
                break;
            case 'f':
                s += '\f';
                break;
            case 'n':
                s += '\n';
                break;
            case 'r':
                s += '\r';
                break;
            case 't':
                s += '\t';
                break;
            default:
                s += ch;
            }
            status = 2;
        } else if (status == 4 && ch == delim) {
            out.emplace_back(s);
            s.clear();
            status = 0;
        } else if (status == 4 && ch != delim) {
            return false;
        } else {
            assert(0);
        }
    }

    if (!s.empty()) {
        out.emplace_back(s);
    }

    return true;
}

}  // namespace

csv::csv(std::istream& is, char delim) : status_(true), delim_(delim), is_(is) {
    if (delim_ == '"') throw std::invalid_argument("delim can not be \"");
}

std::string csv::get_row() {
    col_.clear();
    std::getline(is_, row_);
    if (!is_)
        status_ = false;
    else
        status_ = split(row_, col_, delim_);
    return row_;
}

csv::operator bool() const { return status_; }

size_t csv::col_size() const { return col_.size(); }

std::string csv::get_col(size_t n) const { return n < col_.size() ? col_[n] : std::string(); }

}  // namespace liph
