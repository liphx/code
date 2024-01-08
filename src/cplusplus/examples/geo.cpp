#include <cmath>
#include <iostream>
using namespace std;

// 单位为角度
float calc_geodist(float lat1, float lng1, float lat2, float lng2) {
    // 1. 计算三个参数
    float dx = lng1 - lng2;         // 经度差值
    float dy = lat1 - lat2;         // 纬度差值
    float b = (lat1 + lat2) / 2.0;  // 平均纬度
    // 2. 计算东西方向距离和南北方向距离(单位：米)，东西距离采用三阶多项式，南北采用一阶多项式即可
    float Lx = (0.05 * b * b * b - 19.16 * b * b + 47.13 * b + 110966) * dx;  // 东西距离
    float Ly = (17 * b + 110352) * dy;                                        // 南北距离
    // 3. 用平面的矩形对角距离公式计算总距离
    return sqrt(Lx * Lx + Ly * Ly);
}

int main() { cout << calc_geodist(121.524014, 31.275173, 2.12108, 0.545921) << endl; }
