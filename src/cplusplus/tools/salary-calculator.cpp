#include <cfloat>
#include <iomanip>
#include <iostream>

// 级数 累计预扣预缴应纳税所得额        预扣率（%） 速算扣除数
// 1    不超过36000元的部分             3           0
// 2    超过36000元至144000元的部分     10          2520
// 3    超过144000元至300000元的部分    20          16920
// 4    超过300000元至420000元的部分    25          31920
// 5    超过420000元至660000元的部分    30          52920
// 6    超过660000元至960000元的部分    35          85920
// 7    超过960000元的部分              45          181920

struct tax_rate {
    double start;
    double end;
    double rate;
    double deduction;
};

static const tax_rate tax_rate_table[] = {{0, 36000, 0.03, 0}, {36000, 144000, 0.1, 2520}, {144000, 300000, 0.2, 16920},
        {300000, 420000, 0.25, 31920}, {420000, 660000, 0.3, 52920}, {660000, 960000, 0.35, 85920},
        {960000, DBL_MAX, 0.45, 181920}, {DBL_MAX, 0, 0, 0}};
static const double housing_fund_limit = 3826;  // 31884*0.12
static const double housing_fund_rate = 0.12;
// static const double housing_fund_rate = 0.05; // 华为是这样的
static const double tax_free_start = 5000;
static const double pay_others_rate = 0.105;

static int tax_rate_table_idx(double pre_tax) {
    int idx = 0;
    while (pre_tax > tax_rate_table[idx + 1].start) {
        ++idx;
    }
    return idx;
}

int main(int argc, char **argv) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " salary [tax_free_extra]" << std::endl;
        std::cerr << "Example: " << argv[0] << " 12000" << std::endl;
        std::cerr << "         " << argv[0] << " 12000 1500(租房减免)" << std::endl;
        return 1;
    }
    double base = std::strtod(argv[1], nullptr);
    double tax_free_extra = 0;
    if (argc > 2) {
        tax_free_extra = std::strtod(argv[2], nullptr);
    }
    double pre_tax_extra = 0;
    if (argc > 3) {
        pre_tax_extra = std::strtod(argv[3], nullptr);
    }
    double tax_free = tax_free_start + tax_free_extra;
    double pay_others = base * pay_others_rate + 3;  // 医疗保险个人缴纳比例2% + 3元
    pay_others += std::min(housing_fund_limit, base * housing_fund_rate);
    double pre_tax_total = 0;
    double salary_total = 0;
    double tax_total = 0;
    for (int i = 0; i < 12; i++) {
        double pre_tax = base + pre_tax_extra - tax_free - pay_others;
        pre_tax_total += pre_tax;
        int idx = tax_rate_table_idx(pre_tax_total);
        double tax = pre_tax_total * tax_rate_table[idx].rate - tax_rate_table[idx].deduction - tax_total;
        double salary = base + pre_tax_extra - pay_others - tax;
        tax_total += tax;
        salary_total += salary;
        std::cout << std::left << std::setw(2) << i + 1 << " salary: " << salary << ", tax: " << tax << std::endl;
    }
    std::cout << "salary total before tax: " << 12 * (base + pre_tax_extra) << std::endl;
    std::cout << "salary total: " << salary_total << std::endl;
    std::cout << "tax total: " << tax_total << std::endl;
    return 0;
}
