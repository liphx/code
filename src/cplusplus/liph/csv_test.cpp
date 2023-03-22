#include "csv.h"

#include <sstream>

#include "gtest/gtest.h"

namespace liph {
namespace test {

TEST(csv, get_row) {
    std::vector<std::vector<std::string>> data = {{"abc", "123", "hello"}, {"4", "world", "000"}};
    std::stringstream ss;
    for (const auto& row : data) {
        std::string sep = "";
        for (const auto& col : row) {
            ss << sep << col;
            sep = ",";
        }
        ss << "\n";
    }
    csv csv_parser(ss);
    for (const auto& row : data) {
        csv_parser.get_row();
        EXPECT_TRUE(csv_parser);
        EXPECT_EQ(csv_parser.col_size(), row.size());
        for (size_t i = 0; i < row.size(); i++) {
            EXPECT_EQ(csv_parser.get_col(i), row[i]);
        }
    }
}

TEST(csv, tsv) {
    std::stringstream ss;
    ss << "1\t2\t3\n";
    csv tsv_parser(ss, '\t');
    tsv_parser.get_row();
    EXPECT_EQ(tsv_parser.col_size(), 3u);
}

TEST(csv, blank) {
    std::stringstream ss;
    csv csv_parser(ss);
    csv_parser.get_row();
    EXPECT_FALSE(csv_parser);
}

TEST(csv, special) {
    std::stringstream ss;
    ss << R"("111\t\n  \"","   213",   213
11"22"
"111"22
)";
    csv csv_parser(ss);
    csv_parser.get_row();
    EXPECT_TRUE(csv_parser);
    EXPECT_EQ(csv_parser.get_col(0), "111\t\n  \"");
    EXPECT_EQ(csv_parser.get_col(1), "   213");
    EXPECT_EQ(csv_parser.get_col(2), "   213");
    csv_parser.get_row();
    EXPECT_TRUE(csv_parser);
    EXPECT_EQ(csv_parser.get_col(0), "11\"22\"");
    csv_parser.get_row();
    EXPECT_FALSE(csv_parser);
}

}  // namespace test
}  // namespace liph
