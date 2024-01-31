#include <chrono>
#include <iostream>
#include <vector>

#include "liph/terminal.h"
using namespace std;
using namespace std::chrono;

string to(month m) {
    static string name[]{"January", "February", "March", "April", "May", "June", "July", "August", "September",
            "October", "November", "December"};
    unsigned i{m};
    if (i < 1 || i > 12) return {};
    return name[i - 1];
}

string to(day d) {
    unsigned i{d};
    string s = to_string(i);
    if (s.length() < 2) s = ' ' + s;
    return s;
}

void begin(weekday wd) {
    if (wd == Monday)
        cout << "   ";
    else if (wd == Tuesday)
        cout << "      ";
    else if (wd == Wednesday)
        cout << "         ";
    else if (wd == Thursday)
        cout << "            ";
    else if (wd == Friday)
        cout << "               ";
    else if (wd == Saturday)
        cout << "                  ";
}

void show(year_month_day ymd) {
    year y = ymd.year();
    month m = ymd.month();
    day d = ymd.day();
    cout << "    " << to(m) << " " << y << '\n';
    cout << "Su Mo Tu We Th Fr Sa\n";
    vector<string> output;
    day first{1};
    day last = year_month_day_last{y, month_day_last{m}}.day();
    begin(weekday{year_month_day{y, m, first}});
    bool one = true;
    while (true) {
        weekday wd{year_month_day{y, m, first}};
        if (!one) cout << ' ';
        one = false;
        if (first == d)
            cout << liph::text_italic() << to(first) << liph::color_reset();
        else
            cout << to(first);
        if (wd == Saturday) {
            one = true;
            if (first != last) cout << '\n';
        }
        if (first == last) break;
        ++first;
    }
    cout << "\n\n";
}

int main() {
    time_point now = system_clock::now();
    const time_zone *zone = current_zone();
    zoned_time zt{zone, now};
    auto local_time = zt.get_local_time();
    year_month_day ymd{floor<days>(local_time)};
    show(ymd);
    return 0;
}
