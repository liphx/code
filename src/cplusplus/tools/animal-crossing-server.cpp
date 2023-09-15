#include <algorithm>
#include <filesystem>
#include <string>
#include <vector>

#include "httplib.h"
#include "liph/liph.h"

using json = liph::json;

json array_to_json(const std::vector<int>& active_time) {
    json ret = json::array;
    for (int i : active_time) ret.array_ref().emplace_back(i);
    return ret;
}

struct Fish {
    Fish(int id, const std::string& name, const std::vector<int>& active_time, const std::string& location)
            : id{id}, name{name}, active_time{active_time}, location{location} {}

    int id;
    std::string name;
    std::vector<int> active_time;
    std::string location;
};

struct Insect {
    Insect(int id, const std::string& name, const std::vector<int>& active_time)
            : id{id}, name{name}, active_time{active_time} {}

    int id;
    std::string name;
    std::vector<int> active_time;
};

struct Halobios {
    Halobios(int id, const std::string& name, const std::vector<int>& active_time)
            : id{id}, name{name}, active_time{active_time} {}

    int id;
    std::string name;
    std::vector<int> active_time;
};

static const std::vector<Fish> all_fish = {{1, "红目鲫", {1, 2, 3, 11, 12}, "河流"},
        {2, "溪哥", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, "河流"},
        {3, "鲫鱼", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, "河流"},
        {4, "雅罗鱼", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, "河流"},
        {5, "鲤鱼", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, "池塘"},
        {6, "锦鲤", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, "池塘"},
        {7, "金鱼", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, "池塘"},
        {8, "龙晴金鱼", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, "池塘"},
        {9, "兰寿金鱼", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, "池塘"}, {10, "稻田鱼", {4, 5, 6, 7, 8}, "池塘"},
        {11, "淡水龙虾", {4, 5, 6, 7, 8, 9}, "池塘"}, {12, "鳖", {8, 9}, "河流"},
        {13, "拟鳄龟", {4, 5, 6, 7, 8, 9, 10}, "河流"}, {14, "蝌蚪", {3, 4, 5, 6, 7}, "池塘"},
        {15, "青蛙", {5, 6, 7, 8}, "池塘"}, {16, "塘鳢鱼", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, "河流"},
        {17, "泥鳅", {3, 4, 5}, "河流"}, {18, "鲶鱼", {5, 6, 7, 8, 9, 10}, "池塘"}, {19, "黑鱼", {6, 7, 8}, "池塘"},
        {20, "蓝鳃太阳鱼", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, "河流"},
        {21, "黄鲈鱼", {1, 2, 3, 10, 11, 12}, "河流"}, {22, "黑鲈鱼", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, "河流"},
        {23, "吴郭鱼", {6, 7, 8, 9, 10}, "河流"}, {24, "白斑狗鱼", {9, 10, 11, 12}, "河流"},
        {25, "西太公鱼", {1, 2, 12}, "河流"}, {26, "香鱼", {7, 8, 9}, "河流"},
        {27, "樱花钩吻鲑", {3, 4, 5, 6, 9, 10, 11}, "悬崖上"}, {28, "花羔红点鲑", {3, 4, 5, 6, 9, 10, 11}, "悬崖上"},
        {29, "金鳟", {3, 4, 5, 9, 10, 11}, "悬崖上"}, {30, "远东哲罗鱼", {1, 2, 3, 12}, "悬崖上"},
        {31, "鲑鱼", {9}, "入海口"}, {32, "帝王鲑", {9}, "入海口"}, {33, "中华绒螯蟹", {9, 10, 11}, "河流"},
        {34, "孔雀鱼", {4, 5, 6, 7, 8, 9, 10, 11}, "河流"}, {35, "温泉医生鱼", {5, 6, 7, 8, 9}, "河流"},
        {36, "神仙鱼", {5, 6, 7, 8, 9, 10}, "河流"}, {37, "斗鱼", {5, 6, 7, 8, 9, 10}, "河流"},
        {38, "霓虹灯鱼", {4, 5, 6, 7, 8, 9, 10, 11}, "河流"}, {39, "彩虹鱼", {5, 6, 7, 8, 9, 10}, "河流"},
        {40, "食人鱼", {6, 7, 8, 9}, "河流"}, {41, "骨舌鱼", {6, 7, 8, 9}, "河流"},
        {42, "黄金河虎", {6, 7, 8, 9}, "河流"}, {43, "雀鳝", {6, 7, 8, 9}, "池塘"},
        {44, "巨骨舌鱼", {6, 7, 8, 9}, "河流"}, {45, "恩氏多鳍鱼", {6, 7, 8, 9}, "河流"},
        {46, "鲟鱼", {1, 2, 3, 9, 10, 11, 12}, "入海口"}, {47, "海天使", {1, 2, 3, 12}, "大海"},
        {48, "海马", {4, 5, 6, 7, 8, 9, 10, 11}, "大海"}, {49, "小丑鱼", {4, 5, 6, 7, 8, 9}, "大海"},
        {50, "拟刺尾鲷", {4, 5, 6, 7, 8, 9}, "大海"}, {51, "耳带蝴蝶鱼", {4, 5, 6, 7, 8, 9}, "大海"},
        {52, "苏眉鱼", {7, 8}, "大海"}, {53, "狮子鱼", {4, 5, 6, 7, 8, 9, 10, 11}, "大海"},
        {54, "河豚", {1, 2, 11, 12}, "大海"}, {55, "刺豚", {7, 8, 9}, "大海"},
        {56, "凤尾鱼", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, "大海"},
        {57, "竹荚鱼", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, "大海"},
        {58, "条石鲷", {3, 4, 5, 6, 7, 8, 9, 10, 11}, "大海"},
        {59, "鲈鱼", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, "大海"},
        {60, "鲷鱼", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, "大海"}, {61, "鲽鱼", {1, 2, 3, 4, 10, 11, 12}, "大海"},
        {62, "比目鱼", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, "大海"},
        {63, "鱿鱼", {1, 2, 3, 4, 5, 6, 7, 8, 12}, "大海"}, {64, "裸胸鳝", {8, 9, 10}, "大海"},
        {65, "五彩鳗", {6, 7, 8, 9, 10}, "大海"}, {66, "鲔鱼", {1, 2, 3, 4, 11, 12}, "码头"},
        {67, "旗鱼", {1, 2, 3, 4, 7, 8, 9, 11, 12}, "码头"}, {68, "白面弄鱼", {5, 6, 7, 8, 9, 10}, "码头"},
        {69, "鬼头刀", {5, 6, 7, 8, 9, 10}, "码头"}, {70, "翻车鱼", {7, 8, 9}, "大海"},
        {71, "鳐鱼", {8, 9, 10, 11}, "大海"}, {72, "锯鲨", {6, 7, 8, 9}, "大海"}, {73, "双髻鲨", {6, 7, 8, 9}, "大海"},
        {74, "鲨鱼", {6, 7, 8, 9}, "大海"}, {75, "鲸鲨", {6, 7, 8, 9}, "大海"}, {76, "吸盘鱼", {6, 7, 8, 9}, "大海"},
        {77, "灯笼鱼", {1, 2, 3, 11, 12}, "大海"}, {78, "皇带鱼", {1, 2, 3, 4, 5, 12}, "大海"},
        {79, "太平洋桶眼鱼", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, "大海"},
        {80, "矛尾鱼", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}, "大海（雨天）"}};

static const std::vector<Insect> all_insect = {{1, "白粉蝶", {1, 2, 3, 4, 5, 6, 9, 10, 11, 12}},
        {2, "斑缘点粉蝶", {3, 4, 5, 6, 9, 10}}, {3, "凤蝶", {3, 4, 5, 6, 7, 8, 9}}, {4, "乌鸦凤蝶", {3, 4, 5, 6}},
        {5, "青带凤蝶", {4, 5, 6, 7, 8}}, {6, "大白斑蝶", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}},
        {7, "大紫蛱蝶", {5, 6, 7, 8}}, {8, "大桦斑蝶", {9, 10, 11}}, {9, "大蓝闪蝶", {1, 2, 3, 6, 7, 8, 9, 12}},
        {10, "彩袄蛱蝶", {4, 5, 6, 7, 8, 9}}, {11, "红颈凤蝶", {1, 2, 4, 5, 6, 7, 8, 9, 12}},
        {12, "亚历山大凤蝶", {5, 6, 7, 8, 9}}, {13, "飞蛾", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}},
        {14, "皇蛾", {4, 5, 6, 7, 8, 9}}, {15, "日落蛾", {4, 5, 6, 7, 8, 9}},
        {16, "中华剑角蝗", {4, 5, 6, 7, 8, 9, 10, 11}}, {17, "飞蝗", {8, 9, 10, 11}}, {18, "稻蝗", {8, 9, 10, 11}},
        {19, "蚱蜢", {7, 8, 9}}, {20, "蟋蟀", {9, 10, 11}}, {21, "铃虫", {9, 10}},
        {22, "螳螂", {3, 4, 5, 6, 7, 8, 9, 10, 11}}, {23, "兰花螳螂", {3, 4, 5, 6, 7, 8, 9, 10, 11}},
        {24, "蜜蜂", {3, 4, 5, 6, 7}}, {25, "黄蜂", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}}, {26, "油蝉", {7, 8}},
        {27, "斑透翅蝉", {7, 8}}, {28, "熊蝉", {7, 8}}, {29, "寒蝉", {8, 9}}, {30, "暮蝉", {7, 8}},
        {31, "蝉蜕", {7, 8}}, {32, "红蜻蜓", {9, 10}}, {33, "绿胸晏蜓", {4, 5, 6, 7, 8, 9, 10}},
        {34, "无霸勾蜓", {5, 6, 7, 8, 9, 10}}, {35, "豆娘", {1, 2, 11, 12}}, {36, "萤火虫", {6}},
        {37, "蝼蛄", {1, 2, 3, 4, 5, 11, 12}}, {38, "水黾", {5, 6, 7, 8, 9}}, {39, "龙虱", {5, 6, 7, 8, 9}},
        {40, "田鳖", {4, 5, 6, 7, 8, 9}}, {41, "椿象", {3, 4, 5, 6, 7, 8, 9, 10}},
        {42, "人面椿象", {3, 4, 5, 6, 7, 8, 9, 10}}, {43, "瓢虫", {3, 4, 5, 6, 10}},
        {44, "虎甲虫", {2, 3, 4, 5, 6, 7, 8, 9, 10}}, {45, "吉丁虫", {4, 5, 6, 7, 8}},
        {46, "提琴虫", {5, 6, 9, 10, 11}}, {47, "星天牛", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}},
        {48, "琉璃星天牛", {5, 6, 7, 8, 9}}, {49, "宝石象鼻虫", {7, 8}}, {50, "蜣螂", {1, 2, 12}},
        {51, "雪隐金龟", {7, 8, 9}}, {52, "宝石金龟", {7, 8}}, {53, "日铜锣金花龟", {6, 7, 8}},
        {54, "歌利亚大角花金龟", {6, 7, 8, 9}}, {55, "锯锹形虫", {7, 8}}, {56, "深山锹形虫", {7, 8}},
        {57, "大锹形虫", {7, 8}}, {58, "彩虹锹形虫", {6, 7, 8, 9}}, {59, "细身赤锹形虫", {7, 8}},
        {60, "黄金鬼锹形虫", {7, 8}}, {61, "长颈鹿锹形虫", {7, 8}}, {62, "独角仙", {7, 8}},
        {63, "高卡萨斯南洋大兜虫", {7, 8}}, {64, "象兜虫", {7, 8}}, {65, "长戟大兜虫", {7, 8}},
        {66, "竹节虫", {7, 8, 9, 10, 11}}, {67, "叶竹节虫", {7, 8, 9}},
        {68, "蓑衣虫", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}}, {69, "蚂蚁", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}},
        {70, "寄居蟹", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}},
        {71, "海蟑螂", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}}, {72, "苍蝇", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}},
        {73, "蚊子", {6, 7, 8, 9}}, {74, "跳蚤", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}},
        {75, "蜗牛", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}}, {76, "鼠妇", {1, 2, 3, 4, 5, 6, 9, 10, 11, 12}},
        {77, "蜈蚣", {1, 2, 3, 4, 5, 6, 9, 10, 11, 12}}, {78, "蜘蛛", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}},
        {79, "狼蛛", {1, 2, 3, 4, 11, 12}}, {80, "蝎子", {5, 6, 7, 8, 9, 10}}};

static const std::vector<Halobios> all_halobios = {{1, "裙带菜", {1, 2, 3, 4, 5, 6, 7, 10, 11, 12}},
        {2, "海葡萄", {6, 7, 8, 9}}, {3, "海参", {1, 2, 3, 4, 11, 12}}, {4, "海猪", {1, 2, 11, 12}},
        {5, "海星", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}}, {6, "海胆", {5, 6, 7, 8, 9}},
        {7, "石笔海胆", {5, 6, 7, 8, 9}}, {8, "海葵", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}},
        {9, "海月水母", {7, 8, 9}}, {10, "海蛞蝓", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}},
        {11, "马氏珠母贝", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}}, {12, "贻贝", {6, 7, 8, 9, 10, 11, 12}},
        {13, "牡蛎", {1, 2, 9, 10, 11, 12}}, {14, "虾夷扇贝", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}},
        {15, "花螺", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}}, {16, "角蝾螺", {3, 4, 5, 9, 10, 11, 12}},
        {17, "鲍鱼", {1, 6, 7, 8, 9, 10, 11, 12}}, {18, "大砗磲", {5, 6, 7, 8, 9}},
        {19, "鹦鹉螺", {3, 4, 5, 6, 9, 10, 11}}, {20, "章鱼", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}},
        {21, "扁面蛸", {3, 4, 5, 9, 10, 11}}, {22, "吸血鬼乌贼", {5, 6, 7, 8}}, {23, "萤火鱿", {3, 4, 5, 6}},
        {24, "梭子蟹", {6, 7, 8, 9, 10, 11}}, {25, "珍宝蟹", {1, 2, 3, 4, 5, 11, 12}},
        {26, "松叶蟹", {1, 2, 3, 4, 11, 12}}, {27, "帝王蟹", {1, 2, 3, 11, 12}},
        {28, "藤壶", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}}, {29, "高脚蟹", {3, 4}}, {30, "日本对虾", {6, 7, 8, 9}},
        {31, "甜虾", {1, 2, 9, 10, 11, 12}}, {32, "虾蛄", {1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12}},
        {33, "伊势龙虾", {10, 11, 12}}, {34, "龙虾", {1, 4, 5, 6, 12}}, {35, "大王具足虫", {7, 8, 9, 10}},
        {36, "鲎", {7, 8, 9}}, {37, "海蛸", {4, 5, 6, 7, 8}}, {38, "花园鳗", {5, 6, 7, 8, 9, 10}},
        {39, "海扁虫", {8, 9}}, {40, "偕老同穴", {1, 2, 10, 11, 12}}};

namespace fs = std::filesystem;

std::string FLAGS_data_path;
int32_t FLAGS_port;
std::string FLAGS_host;

bool checkUserName(const std::string& user) {
    if (user.empty()) return false;
    if (std::all_of(user.begin(), user.end(), [](char ch) { return isalnum(ch); })) return true;
    LOG << "not valid name: " << user;
    return false;
}

void MyFish(const httplib::Request& req, httplib::Response& res) {
    json body;
    std::string user, data;
    try {
        body = json::parse(req.body);
        user = body.at("user").string_ref();
        data = body.at("data").string_ref();
    } catch (std::exception& e) {
        LOG << e.what();
        return;
    }

    if (!checkUserName(user)) {
        return;
    }

    std::ofstream os(FLAGS_data_path + "/fish-" + user + ".json");
    os << data;
}

void MyInsect(const httplib::Request& req, httplib::Response& res) {
    json body;
    std::string user, data;
    try {
        body = json::parse(req.body);
        user = body.at("user").string_ref();
        data = body.at("data").string_ref();
    } catch (std::exception& e) {
        LOG << e.what();
        return;
    }

    if (!checkUserName(user)) {
        return;
    }

    std::ofstream os(FLAGS_data_path + "/insect-" + user + ".json");
    os << data;
}

void MyHalobios(const httplib::Request& req, httplib::Response& res) {
    json body;
    std::string user, data;
    try {
        body = json::parse(req.body);
        user = body.at("user").string_ref();
        data = body.at("data").string_ref();
    } catch (std::exception& e) {
        LOG << e.what();
        return;
    }

    if (!checkUserName(user)) {
        return;
    }

    std::ofstream os(FLAGS_data_path + "/halobios-" + user + ".json");
    os << data;
}

#define READ_JSON(file)                                                            \
    do {                                                                           \
        std::string str;                                                           \
        if (liph::read_file(file, str)) {                                          \
            try {                                                                  \
                res.set_content(json::parse(str).to_string(), "application/json"); \
                return;                                                            \
            } catch (...) {                                                        \
            }                                                                      \
        }                                                                          \
    } while (0)

void AllFish(const httplib::Request& req, httplib::Response& res) {
    LOG << "GET /api/all_fish";
    std::string user;
    if (req.has_param("user")) {
        user = req.get_param_value("user");
    }

    if (checkUserName(user)) {
        READ_JSON(FLAGS_data_path + "/fish-" + user + ".json");
    }

    json ret = json::array;
    for (auto it = all_fish.begin(); it != all_fish.end(); ++it) {
        json item = json::array;
        item.array_ref().emplace_back(it->id);
        item.array_ref().emplace_back(it->name);
        item.array_ref().emplace_back(false);
        ret.array_ref().emplace_back(item);
    }
    res.set_content(ret.to_string(), "application/json");
}

void AllInsect(const httplib::Request& req, httplib::Response& res) {
    std::string user;
    if (req.has_param("user")) {
        user = req.get_param_value("user");
    }

    if (checkUserName(user)) {
        READ_JSON(FLAGS_data_path + "/insect-" + user + ".json");
    }

    json ret = json::array;
    for (auto it = all_insect.begin(); it != all_insect.end(); ++it) {
        json item = json::array;
        item.array_ref().emplace_back(it->id);
        item.array_ref().emplace_back(it->name);
        item.array_ref().emplace_back(false);
        ret.array_ref().emplace_back(item);
    }
    res.set_content(ret.to_string(), "application/json");
}

void AllHalobios(const httplib::Request& req, httplib::Response& res) {
    std::string user;
    if (req.has_param("user")) {
        user = req.get_param_value("user");
    }

    if (checkUserName(user)) {
        READ_JSON(FLAGS_data_path + "/halobios-" + user + ".json");
    }

    json ret = json::array;
    for (auto it = all_halobios.begin(); it != all_halobios.end(); ++it) {
        json item = json::array;
        item.array_ref().emplace_back(it->id);
        item.array_ref().emplace_back(it->name);
        item.array_ref().emplace_back(false);
        ret.array_ref().emplace_back(item);
    }
    res.set_content(ret.to_string(), "application/json");
}

void FishDetail(const httplib::Request& req, httplib::Response& res) {
    json body, ret;
    int id;
    try {
        body = json::parse(req.body);
        id = body.at("id").i64_ref();
    } catch (std::exception& e) {
        LOG << e.what();
        return;
    }

    int idx = id - 1;
    if (idx < 0 || (size_t)idx >= all_fish.size()) {
        return;
    }

    const auto& fish = all_fish[idx];
    ret["id"] = fish.id;
    ret["name"] = fish.name;
    ret["active_time"] = array_to_json(fish.active_time);
    ret["location"] = fish.location;
    res.set_content(ret.to_string(), "application/json");
}

void InsectDetail(const httplib::Request& req, httplib::Response& res) {
    json body, ret;
    int id;
    try {
        body = json::parse(req.body);
        id = body.at("id").i64_ref();
    } catch (std::exception& e) {
        LOG << e.what();
        return;
    }

    int idx = id - 1;
    if (idx < 0 || (size_t)idx >= all_insect.size()) {
        return;
    }

    const auto& insect = all_insect[idx];
    ret["id"] = insect.id;
    ret["name"] = insect.name;
    ret["active_time"] = array_to_json(insect.active_time);
    res.set_content(ret.to_string(), "application/json");
}

void HalobiosDetail(const httplib::Request& req, httplib::Response& res) {
    json body, ret;
    int id;
    try {
        body = json::parse(req.body);
        id = body.at("id").i64_ref();
    } catch (std::exception& e) {
        LOG << e.what();
        return;
    }

    int idx = id - 1;
    if (idx < 0 || (size_t)idx >= all_halobios.size()) {
        return;
    }

    const auto& halobios = all_halobios[idx];
    ret["id"] = halobios.id;
    ret["name"] = halobios.name;
    ret["active_time"] = array_to_json(halobios.active_time);
    res.set_content(ret.to_string(), "application/json");
}

int main(int argc, char *argv[]) {
    liph::flags flags;
    flags.register_string_flag("host", "0.0.0.0");
    flags.register_int32_flag("port", 9001);
    flags.register_string_flag("data_path", "/tmp");
    if (!flags.parse_flags(argc, &argv)) {
        std::cerr << flags.help() << std::endl;
        return 1;
    }
    FLAGS_host = flags.string_ref("host");
    FLAGS_port = flags.int32_ref("port");
    FLAGS_data_path = flags.string_ref("data_path");
    if (FLAGS_data_path.empty()) {
        std::cerr << "data_path empty" << std::endl;
        std::cerr << flags.help() << std::endl;
        return 1;
    }

    httplib::Server svr;

    svr.Get("/api/all_fish", AllFish);
    svr.Post("/api/my_fish", MyFish);
    svr.Post("/api/fish_detail", FishDetail);
    svr.Get("/api/all_insect", AllInsect);
    svr.Post("/api/my_insect", MyInsect);
    svr.Post("/api/insect_detail", InsectDetail);
    svr.Get("/api/all_halobios", AllHalobios);
    svr.Post("/api/my_halobios", MyHalobios);
    svr.Post("/api/halobios_detail", HalobiosDetail);

    svr.listen(FLAGS_host.c_str(), FLAGS_port);
}
