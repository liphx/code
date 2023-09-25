#include <filesystem>
#include <fstream>
#include <iostream>

#include "liph/json.h"

namespace liph {

struct Conf {
    struct Item {
        std::string name;
        std::string color;
    };
    std::string name;
    std::vector<Item> list;

    bool load(const std::filesystem::path& path) {
        try {
            const auto value = json::load(path);
            if (!value.is_object()) return false;
            name = value["name"].string_ref();
            const auto& list_value = value["list"].array_ref();
            for (auto it = list_value.begin(); it != list_value.end(); ++it) {
                if (!it->is_object()) return false;
                std::vector<std::string> name_list;
                const auto& name = it->at("name");
                if (name.is_string()) {
                    name_list.emplace_back(name.string_ref());
                } else if (name.is_array()) {
                    name_list.resize(name.array_ref().size());
                    for (size_t i = 0; i < name_list.size(); ++i) {
                        name_list[i] = name.array_ref()[i].string_ref();
                    }
                }
                int count = it->at("count").i64_ref();
                if (count <= 0) return false;
                std::vector<std::string> color_list;
                auto color_it = it->object_ref().find("color");
                if (color_it == it->object_ref().end()) {
                    color_list.emplace_back();
                } else if (color_it->second.is_string()) {
                    color_list.emplace_back(color_it->second.string_ref());
                } else if (color_it->second.is_array()) {
                    color_list.resize(color_it->second.array_ref().size());
                    for (size_t i = 0; i < color_list.size(); ++i) {
                        color_list[i] = color_it->second.array_ref()[i].string_ref();
                    }
                } else {
                    return false;
                }
                assert(!color_list.empty());
                for (int i = 0; i < count; ++i) {
                    for (const auto& name : name_list) {
                        for (const auto& color : color_list) {
                            list.emplace_back(Item{name, color});
                        }
                    }
                }
            }
        } catch (...) {
            return false;
        }
        return true;
    }
};

}  // namespace liph

int main(int argc, char **argv) {
    liph::Conf conf;
    conf.load(argv[1]);
    std::cout << conf.name << std::endl;
    std::cout << conf.list.size() << std::endl;
    for (const auto& item : conf.list) {
        std::cout << item.name << "," << (item.color.empty() ? "null" : item.color) << std::endl;
    }
}
