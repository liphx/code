#include <cfloat>
#include <iomanip>
#include <iostream>

#include "gflags/gflags.h"
#include "liph/liph.h"

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

DEFINE_double(base_max, 33891, "社保公积金上限");
DEFINE_double(ggj_rate, 0.12, "公积金比例");
DEFINE_double(tax_free_start, 5000, "个税起征点");
DEFINE_double(pay_others_rate, 0.105, "缴纳保险比例");  // 北京：8%(养老保险) + 0.5%(失业保险) + 2%(基本医疗保险)
DEFINE_double(pay_others_extra, 3, "大额医疗互助资金");
DEFINE_double(base, 12000, "工资基数");
DEFINE_double(tax_free_extra, 0, "专项附加扣除");
DEFINE_double(pre_tax_extra, 0, "工资基数外的收入或扣除");
DEFINE_double(pre_tax_total, 0, "起始税前总额(去除免税部分)");
DEFINE_double(tax_total, 0, "起始总税");
DEFINE_int32(month_begin, 1, "起始月份");
DEFINE_int32(month_end, 12, "结束月份");

static int tax_rate_table_idx(double pre_tax) {
    int idx = 0;
    while (pre_tax > tax_rate_table[idx + 1].start) {
        ++idx;
    }
    return idx;
}

int main(int argc, char **argv) {
    google::ParseCommandLineFlags(&argc, &argv, true);
    double tax_free = FLAGS_tax_free_start + FLAGS_tax_free_extra;
    double pay_others =
            std::min(FLAGS_base_max, FLAGS_base) * (FLAGS_pay_others_rate + FLAGS_ggj_rate) + FLAGS_pay_others_extra;
    double pre_tax_total = FLAGS_pre_tax_total;
    double salary_total = 0;
    double tax_total = FLAGS_tax_total;
    for (int i = FLAGS_month_begin; i <= FLAGS_month_end; i++) {
        double pre_tax = FLAGS_base + FLAGS_pre_tax_extra - tax_free - pay_others;
        if (pre_tax < 0) pre_tax = 0; // FLAGS_pre_tax_extra扣除很多
        pre_tax_total += pre_tax;
        int idx = tax_rate_table_idx(pre_tax_total);
        double tax = pre_tax_total * tax_rate_table[idx].rate - tax_rate_table[idx].deduction - tax_total;
        double salary = FLAGS_base + FLAGS_pre_tax_extra - pay_others - tax;
        tax_total += tax;
        salary_total += salary;
        std::cout << std::left << std::setw(2) << i << " salary: " << salary << ", tax: " << tax << std::endl;
    }
    std::cout << "salary total before tax: "
              << (FLAGS_month_end - FLAGS_month_begin + 1) * (FLAGS_base + FLAGS_pre_tax_extra) << std::endl;
    std::cout << "salary total: " << salary_total << std::endl;
    std::cout << "tax total: " << tax_total << std::endl;
    std::cout << "pre tax total after free: " << pre_tax_total << std::endl;
    return 0;
}
