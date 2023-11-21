#include "liph/numeric/big_integer.h"

#include "gtest/gtest.h"

using liph::big_integer;

TEST(big_integer, constructor_long_long) {
    std::vector<long long> nums{0, 1, -1, 255, -255, 256, -256, 257, -257, std::numeric_limits<long long int>::max(),
            std::numeric_limits<long long int>::max() - 1, std::numeric_limits<long long int>::min(),
            std::numeric_limits<long long int>::min() + 1};
    for (long long num : nums) EXPECT_EQ(big_integer(num).string(), std::to_string(num));
}

TEST(big_integer, constructor_str) {
    std::vector<std::string> strs{"0", "1234", "-7890", "-65536", "-231617616677661", "2367218788786236",
            "99999999999999999999", "-100000000000000000000", "-1231764716617112974197417",
            "123456789012345678901234567890", "-123456789012345678901234567890",
            "237147184653621751983148175877127471478932233900712641598172478148"};
    for (const std::string& str : strs) EXPECT_EQ(big_integer(str).string(), str);
}

TEST(big_integer, move_constructor) {
    big_integer m(1024);
    big_integer n(std::move(m));
    EXPECT_TRUE(m.zero());
    EXPECT_EQ(n, big_integer(1024));
}

TEST(big_integer, compare) {
    EXPECT_TRUE(big_integer(9999) < big_integer(10000));
    EXPECT_TRUE(big_integer(9999) <= big_integer(10000));
    EXPECT_TRUE(big_integer(9999) != big_integer(10000));
    EXPECT_TRUE(big_integer(-9999) > big_integer(-10000));
    EXPECT_TRUE(big_integer(-9999) >= big_integer(-10000));
    EXPECT_TRUE(big_integer(-9999) != big_integer(-10000));
    EXPECT_TRUE(big_integer(9999) != big_integer(-10000));
    EXPECT_TRUE(big_integer(9999) == big_integer(9999));
    EXPECT_TRUE(big_integer(-10000) == big_integer(-10000));
}

TEST(big_integer, add) {
    std::vector<std::vector<std::string>> strs{{"0", "0", "0"}, {"0", "123", "123"}, {"123", "0", "123"},
            {"1", "-1", "0"}, {"-1", "1", "0"}, {"12345678", "87654321", "99999999"},
            {"732567612345326", "712375618537518438671865713490", "712375618537519171239478058816"},
            {"-763452634", "-23472673562", "-24236126196"},
            {"32346378788356845731840", "180850296447005356265", "32527229084803851088105"}, {"9999", "-1111", "8888"},
            {"13617516173", "-217361746", "13400154427"}, {"-125154551", "-9823917", "-134978468"}};
    for (size_t i = 0; i < strs.size(); i++)
        EXPECT_EQ(big_integer(strs[i][0]) + big_integer(strs[i][1]), big_integer(strs[i][2]));
}

TEST(big_integer, sub) {
    std::vector<std::vector<std::string>> strs{{"0", "0", "0"}, {"0", "123", "-123"}, {"123", "0", "123"},
            {"122", "122", "0"}, {"-122", "-122", "0"}, {"13746572364856", "-235738945", "13746808103801"},
            {"9999", "1234", "8765"}, {"1234", "9999", "-8765"}, {"12345", "9999", "2346"},
            {"123462757265", "263457821657", "-139995064392"}};
    for (size_t i = 0; i < strs.size(); i++)
        EXPECT_EQ(big_integer(strs[i][0]) - big_integer(strs[i][1]), big_integer(strs[i][2]));
}

TEST(big_integer, multiply) {
    std::vector<std::vector<std::string>> strs{{"0", "0", "0"}, {"0", "123", "0"}, {"123", "0", "0"},
            {"9999", "1234", "12338766"}, {"1234", "9999", "12338766"}, {"-1234", "9999", "-12338766"},
            {"12345", "9999", "123437655"}, {"13746572364856", "-235738945", "-3240602466657308516920"},
            {"1", "4294967296", "4294967296"}, {"123462757265", "263457821657", "32527229084803851088105"},
            {"3746523747123854610346751230760127457627467340772567340670",
                    "8234758136457813486798978000000001278461263456",
                    "30851716910060577452279963882019518794474976243402807923418832522033227330342835913472945497810173"
                    "555520"}};
    for (size_t i = 0; i < strs.size(); i++)
        EXPECT_EQ(big_integer(strs[i][0]) * big_integer(strs[i][1]), big_integer(strs[i][2]));
}

TEST(big_integer, divide) {
    std::vector<std::vector<std::string>> strs{{"0", "123", "0"}, {"123", "1253124", "0"}, {"-343", "234826", "0"},
            {"100", "100", "1"}, {"300", "100", "3"}, {"361426", "643", "562"}, {"12346231", "3261", "3786"},
            {"61342399", "1", "61342399"},
            {"213654193745671657193476156767716", "234512346512345236146108329845", "911"},
            {"2361971239877651266120812737923425151", "1", "2361971239877651266120812737923425151"},
            {"999999999999999999999999999999999999999999999999999999999999999999999999", "1",
                    "999999999999999999999999999999999999999999999999999999999999999999999999"}};
    for (size_t i = 0; i < strs.size(); i++)
        EXPECT_EQ(big_integer(strs[i][0]) / big_integer(strs[i][1]), big_integer(strs[i][2]));
}

TEST(big_integer, extra) {
    big_integer i(
            "2967449566868551055015417464290533273077199179985304335099507553127683875317177019959423859642812118803366"
            "4754"
            "218345562493168782882");
    big_integer i1 = i + big_integer(1);
    big_integer i2 = i * big_integer(313) + big_integer(1);
    big_integer i3 = i * big_integer(353) + big_integer(1);
    big_integer n = i1 * i2 * i3;
    EXPECT_EQ(n.string(),
            "28871482380507712126714295971303939919776094592797227009265160241974323037991527331163289831446392259419"
            "77803110929349655578418949441740933805615113979999421542416933972905423711002751042080134966731755152859"
            "22696291677532547504444585610194940420003990443211677661994962953925045269871932907037356403227370127845"
            "3899126120309244841494728976885406024976768122077071687938121709811322297802059565867");
}

TEST(big_integer, literal) { EXPECT_EQ(big_integer("7124174278192181984714"), "7124174278192181984714"_bi); }

#include <iostream>
TEST(big_integer, rand) {
    std::cout << big_integer::rand(-1).string() << std::endl;
    std::cout << big_integer::rand(0).string() << std::endl;
    std::cout << big_integer::rand(1).string() << std::endl;
    std::cout << big_integer::rand(64).string() << std::endl;
    std::cout << big_integer::rand(128).string() << std::endl;
}
