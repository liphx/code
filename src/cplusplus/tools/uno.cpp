// Copyright (c) 2022 liph
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <algorithm>
#include <cassert>
#include <chrono>
#include <iostream>
#include <memory>
#include <numeric>
#include <sstream>
#include <string>
#include <thread>
#include <unordered_set>
#include <utility>
#include <vector>

namespace uno {

template <class T>
inline void random_sort(T arr[], int n) {
    for (int i = 0; i < n; i++) {
        int j = rand() % (n - i);
        std::swap(arr[j], arr[n - i - 1]);
    }
}

static inline void reset_input() {
    std::cin.clear();
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static bool ui_color = false;
enum Color { WILD = 0, BLUE, RED, GREEN, YELLOW, Normal };
static const std::string ColorString[] = {"WILD", "BLUE", "RED", "GREEN", "YELLOW"};
static const std::string ColorStringShort[] = {"w", "b", "r", "g", "y"};
static const std::string ColorStringTerminal[] = {
        "\033[;47m", "\033[;44m", "\033[;41m", "\033[;42m", "\033[;43m", "\033[0m"};

class NumCard;
class ActionCard;
class WildCard;
class UNO;

enum CardType { CardTypeNum, CardTypeAction, CardTypeWild, CardTypeInvalid };

class Card {
public:
    virtual Color color() const = 0;
    virtual ~Card() {}
    virtual std::string string() const = 0;
    std::string color_string() const {
        if (ui_color) {
            std::stringstream ss;
            ss.width(8);
            ss.setf(std::ios::right);
            ss << string() << "  ";
            return ColorStringTerminal[color()] + ss.str() + ColorStringTerminal[Normal];
        } else {
            return ColorStringShort[color()] + " " + string();
        }
    }
    virtual Card *clone() const = 0;
    virtual bool match(const Card *) const = 0;
    virtual bool match(const NumCard *) const = 0;
    virtual bool match(const ActionCard *) const = 0;
    virtual bool match(const WildCard *) const = 0;
    virtual CardType type() const = 0;
    virtual int score() const = 0;
};

class NumCard : public Card {
public:
    NumCard(int num, Color color) : num_(num), color_(color) {}
    NumCard(int num, int color) : num_(num), color_(Color(color)) {}
    Color color() const { return color_; }
    std::string string() const { return std::to_string(num_); }
    bool vaild() const { return num_ >= 0 && num_ <= 9 && color_ != WILD; }
    Card *clone() const { return new NumCard(num_, color_); }
    bool match(const Card *other) const { return other->match(this); }
    bool match(const NumCard *other) const { return (num_ == other->num_) || (color_ == other->color_); }
    bool match(const ActionCard *other) const;
    bool match(const WildCard *) const { return true; }
    CardType type() const { return CardTypeNum; }
    int num() const { return num_; }
    int score() const { return num_; }

private:
    int num_;
    Color color_;
};

class ActionCard : public Card {
public:
    enum ActionType { Skip = 0, DrawTwo, Reverse };
    constexpr static const char *const action_name[] = {"Skip", "+2", "Reverse"};

public:
    ActionCard(ActionType type, Color color) : type_(type), color_(color) {}
    ActionCard(ActionType type, int color) : type_(type), color_(Color(color)) {}
    Color color() const { return color_; }
    std::string string() const { return action_name[type_]; }
    bool vaild() const { return color_ != WILD; }
    Card *clone() const { return new ActionCard(type_, color_); }
    bool match(const Card *other) const { return other->match(this); }
    bool match(const NumCard *other) const { return color_ == other->color(); }
    bool match(const ActionCard *other) const { return (type_ == other->type_) || (color_ == other->color()); }
    bool match(const WildCard *) const { return true; }
    CardType type() const { return CardTypeAction; }
    ActionType action_type() const { return type_; }
    int score() const { return 20; }

private:
    ActionType type_;
    Color color_;
};

class WildCard : public Card {
public:
    enum WildType { Wild = 0, WildDrawFour };
    constexpr static const char *const wild_name[] = {"Wild", "+4"};

public:
    WildCard(WildType type) : type_(type) {}
    Color color() const { return WILD; }
    std::string string() const { return wild_name[type_]; }
    Card *clone() const { return new WildCard(type_); }
    bool match(const Card *other) const { return other->match(this); }
    bool match(const NumCard *) const { return true; }
    bool match(const ActionCard *) const { return true; }
    bool match(const WildCard *) const { return true; }
    CardType type() const { return CardTypeWild; }
    WildType wild_type() const { return type_; }
    int score() const { return 50; }

private:
    WildType type_;
};

inline bool NumCard::match(const ActionCard *other) const { return color_ == other->color(); }

inline void print_cards(const std::vector<std::unique_ptr<Card>>& cards) {
    std::cout << "======== cards(" << cards.size() << ") ========" << std::endl;
    int idx = 1;
    for (const auto& card : cards) {
        std::cout.width(4);
        std::cout << idx++ << ": " << card->color_string() << std::endl;
    }
    std::cout << "==========================" << std::endl;
}

class Player {
public:
    Player() {}
    virtual ~Player() {}

    void fetch_one_card(Card *card) {
        assert(card);
        cards_heap_.emplace_back(card);
    }

    virtual Card *give_one_card(Card *top) {
        Card *ret = nullptr;
        random_sort(cards_heap_.data(), cards_heap_.size());
        auto it = cards_heap_.begin();
        while (it != cards_heap_.end()) {
            if ((*it)->match(top)) {
                ret = it->release();
                cards_heap_.erase(it);
                break;
            }
            ++it;
        }
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(1000ms);
        return ret;
    }

    virtual Color change_color() const {
        Color colors[] = {BLUE, RED, GREEN, YELLOW};
        return colors[rand() % 4];
    }

    void swap_cards(std::shared_ptr<Player> other) { cards_heap_.swap(other->cards_heap_); }

    virtual void pick_and_swap_cards(const std::vector<std::pair<int, std::shared_ptr<Player>>>& players) {
        int select = 0;
        size_t min_left = -1;
        for (size_t i = 0; i < players.size(); i++) {
            if (players[i].second->left() <= min_left) {
                min_left = players[i].second->left();
                select = i;
            }
        }

        auto player = players[select];
        if (player.first != 0) {
            std::cout << "Swap cards with player " << player.first << "." << std::endl;
        } else {
            std::cout << "Swap cards with you." << std::endl;
        }
        swap_cards(player.second);
    }

    bool win() const { return cards_heap_.empty(); }
    size_t left() const { return cards_heap_.size(); }
    int score() const {
        return std::accumulate(cards_heap_.begin(), cards_heap_.end(), 0,
                [](int init, const auto& card) { return init + card->score(); });
    }

protected:
    std::vector<std::unique_ptr<Card>> cards_heap_;
};

class UserPlayer : public Player {
public:
    void sort_cards() {
        std::sort(cards_heap_.begin(), cards_heap_.end(),
                [](const std::unique_ptr<Card>& x, const std::unique_ptr<Card>& y) {
                    int i = x->color() * -CardTypeInvalid + x->type();
                    int j = y->color() * -CardTypeInvalid + y->type();
                    if (i != j) return i < j;
                    if (x->type() == CardTypeNum) {
                        return static_cast<NumCard *>(x.get())->num() < static_cast<NumCard *>(y.get())->num();
                    }
                    if (x->type() == CardTypeAction) {
                        return static_cast<ActionCard *>(x.get())->action_type() <
                               static_cast<ActionCard *>(y.get())->action_type();
                    }

                    return static_cast<WildCard *>(x.get())->wild_type() <
                           static_cast<WildCard *>(y.get())->wild_type();
                });
    }

    virtual Card *give_one_card(Card *top) {
        std::cout << "Your cards:" << std::endl;
        sort_cards();
        print_cards(cards_heap_);
        Card *ret = nullptr;
        size_t select = -1;
        while (true) {
            std::cout << "Your selection(0 for fetch one): ";
            std::cin >> select;
            if (std::cin.eof()) {
                std::cout << "Exit, goodbye..." << std::endl;
                exit(0);
            }
            if (std::cin.fail() || select > cards_heap_.size()) {
                std::cout << "No such card!" << std::endl;
                reset_input();
                continue;
            }
            if (select == 0) {
                return ret;
            }
            auto& card = cards_heap_[select - 1];
            if (card->match(top)) {
                ret = card.release();
                cards_heap_.erase(cards_heap_.begin() + select - 1);
                break;
            } else {
                std::cout << "Invalid card!" << std::endl;
                continue;
            }
        }
        return ret;
    }

    virtual void pick_and_swap_cards(const std::vector<std::pair<int, std::shared_ptr<Player>>>& players) {
        std::cout << "Select another player(1-" << players.size() << "): ";
        size_t select = 0;
        std::cin >> select;
        while (std::cin.fail() || --select >= players.size()) {
            std::cout << "Invalid input!" << std::endl;
            std::cout << "Select another player(1-" << players.size() << "): ";
            reset_input();
            std::cin >> select;
        }
        auto player = players[select];
        std::cout << "Swap cards with player " << player.first << std::endl;
        swap_cards(player.second);
    }

    virtual Color change_color() const {
        Color colors[] = {BLUE, RED, GREEN, YELLOW};
        std::cout << "Select one color to change: 1. BLUE, 2. RED, 3. GREEN, 4. YELLOW" << std::endl;
        size_t select = 0;
        std::cin >> select;
        while (--select > 4) {
            std::cout << "Invalid color!" << std::endl;
            std::cout << "Select one color to change: 1. BLUE, 2. RED, 3. GREEN, 4. YELLOW" << std::endl;
            reset_input();
            std::cin >> select;
        }
        return colors[select];
    }
};

class UNO {
public:
    constexpr static const int TotalCardNums = 108;
    constexpr static const int MinPlayersNums = 2;
    constexpr static const int MaxPlayersNums = 10;
    constexpr static const int InitialHandCardNums = 7;

public:
    struct Rule {
        Rule(int players_num = 4, bool enable_70 = true) : players_num(players_num), enable_70(enable_70) {
            players_num = std::max(MinPlayersNums, players_num);
            players_num = std::min(MaxPlayersNums, players_num);
        }

        void set_rule() {
            std::cout << "Set players num(2-10): ";
            std::cin >> players_num;
            while (std::cin.fail() || players_num < 2 || players_num > 10) {
                reset_input();
                std::cout << "Invalid input." << std::endl;
                std::cout << "Set players num(2-10): ";
                std::cin >> players_num;
            }

            std::cout << "Enable 7/0 rule?(y/Y/n/N): ";
            char ch;
            std::cin >> ch;
            while (std::cin.fail() || (ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N')) {
                reset_input();
                std::cout << "Invalid input." << std::endl;
                std::cout << "Enable 7/0 rule?(y/Y/n/N): ";
                std::cin >> ch;
            }
            reset_input();
            enable_70 = (ch == 'y' || ch == 'Y');
        }

        int players_num;
        bool enable_70;
    };

public:
    UNO(Rule rule = Rule()) : rule_(rule) {}

    void init() {
        init_players();
        init_cards_heap();
        init_player_hand_cards();
        init_top_and_first_player();
    }

    void start() {
        while (true) {
            auto player = next_player();
            auto *card = player->give_one_card(top_.get());
            std::cout << player_string() << " plays: ";
            if (card == nullptr) {
                player_fetch_one(player);
                std::cout << "null, fetch one and " << player->left() << " lefts." << std::endl;
            } else {
                top_.reset(card->clone());
                std::cout << top_->color_string();
                if (player->left() == 1) {
                    std::cout << ", uno!" << std::endl;
                } else {
                    std::cout << ", " << player->left() << " lefts." << std::endl;
                }
                if (player->win()) {
                    // TODO: score after +2 or +4
                    int score = std::accumulate(players_.begin(), players_.end(), 0,
                            [](int init, auto player) { return init + player->score(); });
                    std::cout << player_string() << " win, score: " << score << "!" << std::endl;
                    std::cout << "Game over!" << std::endl;
                    break;
                }

                discards_heap_.emplace_back(card);
                action(player);
            }
        }
    }

    std::shared_ptr<Player> next_player() {
        current_player_ += dirction_ ? 1 : -1;
        if (current_player_ == static_cast<int>(players_.size())) {
            current_player_ = 0;
        } else if (current_player_ == -1) {
            current_player_ = players_.size() - 1;
        }
        return players_[current_player_];
    }

    void action(std::shared_ptr<Player> player) {
        if (top_->type() == CardTypeNum && rule_.enable_70) {
            auto *card = static_cast<NumCard *>(top_.get());
            int num = card->num();
            if (num == 7) {
                std::vector<std::pair<int, std::shared_ptr<Player>>> players;
                for (int i = 0; i < static_cast<int>(players_.size()); i++) {
                    if (i != current_player_) {
                        players.emplace_back(std::make_pair(i, players_[i]));
                    }
                }
                player->pick_and_swap_cards(players);
            } else if (num == 0) {
                std::cout << "Everyone swap cards with next player!" << std::endl;
                for (size_t i = 0; i < players_.size(); i++) {
                    auto who = players_[current_player_];
                    who->swap_cards(next_player());
                }
            }
            return;
        } else if (top_->type() == CardTypeAction) {
            ActionCard *card = static_cast<ActionCard *>(top_.get());
            switch (card->action_type()) {
            case ActionCard::Reverse:
                dirction_ = !dirction_;
                std::cout << "Reverse!" << std::endl;
                break;
            case ActionCard::Skip:
                (void)next_player();
                std::cout << "Skip player " << current_player_ << std::endl;
                break;
            case ActionCard::DrawTwo:
                player = next_player();
                player_fetch_one(player);
                player_fetch_one(player);
                std::cout << player_string() << " +2 and skip, " << player->left() << " lefts." << std::endl;
                break;
            default:
                assert(0);
            }
        } else if (top_->type() == CardTypeWild) {
            WildCard *card = static_cast<WildCard *>(top_.get());
            Color color;
            switch (card->wild_type()) {
            case WildCard::Wild:
                color = player->change_color();
                break;
            case WildCard::WildDrawFour:
                color = player->change_color();
                player = next_player();
                player_fetch_one(player);
                player_fetch_one(player);
                player_fetch_one(player);
                player_fetch_one(player);
                std::cout << player_string() << " +4 and skip, " << player->left() << " lefts." << std::endl;
                break;
            default:
                assert(0);
            }
            top_.reset(new NumCard(-1, color));
            std::cout << "Color changes to "
                      << (ui_color ? (ColorStringTerminal[color] + "          " + ColorStringTerminal[Normal])
                                   : ColorString[color])
                      << std::endl;
        }
    }

private:
    void init_cards_heap() {
        cards_heap_.resize(TotalCardNums);
        int idx = 0;
        for (int i = 0; i < 4; i++) {
            cards_heap_[idx++] = std::make_unique<WildCard>(WildCard::Wild);
            cards_heap_[idx++] = std::make_unique<WildCard>(WildCard::WildDrawFour);
        }
        int color = BLUE;
        while (color <= YELLOW) {
            cards_heap_[idx++] = std::make_unique<NumCard>(0, color);
            for (int i = 0; i < 2; i++) {
                for (int num = 1; num <= 9; num++) {
                    cards_heap_[idx++] = std::make_unique<NumCard>(num, color);
                }
                cards_heap_[idx++] = std::make_unique<ActionCard>(ActionCard::Skip, color);
                cards_heap_[idx++] = std::make_unique<ActionCard>(ActionCard::DrawTwo, color);
                cards_heap_[idx++] = std::make_unique<ActionCard>(ActionCard::Reverse, color);
            }
            color++;
        }
        assert(idx == TotalCardNums);
        random_sort(cards_heap_.data(), cards_heap_.size());
    }

    void init_players() {
        players_.resize(rule_.players_num);
        players_[0] = std::make_shared<UserPlayer>();
        for (int i = 1; i < rule_.players_num; i++) {
            players_[i] = std::make_shared<Player>();
        }
    }

    void init_player_hand_cards() {
        for (int i = 0; i < InitialHandCardNums; i++) {
            for (const auto& player : players_) {
                player_fetch_one(player);
            }
        }
    }

    void init_top_and_first_player() {
        top_ = std::make_shared<NumCard>(rand() % 10, 1 + rand() % 4);
        std::cout << "Init top card: " << top_->color_string() << std::endl;
        current_player_ = rand() % players_.size();
    }

    void player_fetch_one(std::shared_ptr<Player> player) {
        if (cards_heap_.empty()) {
            assert(!discards_heap_.empty());
            cards_heap_.swap(discards_heap_);
            random_sort(cards_heap_.data(), cards_heap_.size());
        }
        player->fetch_one_card(cards_heap_.back().release());
        cards_heap_.pop_back();
    }

    std::string player_string() const {
        if (current_player_ == 0) {
            return "You";
        }
        return "Player " + std::to_string(current_player_);
    }

private:
    Rule rule_;
    std::vector<std::shared_ptr<Player>> players_;
    std::vector<std::unique_ptr<Card>> cards_heap_;
    std::vector<std::unique_ptr<Card>> discards_heap_;

    std::shared_ptr<Card> top_;
    bool dirction_{true};
    int current_player_{0};
};

}  // namespace uno

int main(int argc, char **argv) {
    std::unordered_set<std::string> args;
    for (int i = 0; i < argc; i++) {
        args.insert(argv[i]);
    }

    if (args.count("--help") > 0) {
        std::cout << "Usage: " << argv[0] << " [OPTION]..." << std::endl;
        std::cout << "Options:\n"
                     "  --help   display this help and exit\n"
                     "  --color  enable color\n"
                     "  --rule   set uno rules"
                  << std::endl;
        return 0;
    }

    if (args.count("--color") > 0) {
        uno::ui_color = true;
    }

    uno::UNO::Rule rule;
    if (args.count("--rule") > 0) {
        rule.set_rule();
    }
    uno::UNO game(rule);
    srand(time(nullptr));
    game.init();
    game.start();
}
