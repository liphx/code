#include "list.h"

#include "common.h"

struct people {
    unsigned int age;
    char name[128];
    struct list_head list;
};

#define err_quit(msg) err_exit(msg, 1)

int main(int argc, char *argv[]) {
    struct list_head lists;
    INIT_LIST_HEAD(&lists);

    struct people *Alice = (struct people *)malloc(sizeof(struct people));
    if (Alice == NULL) {
        err_quit("malloc fail");
    }
    Alice->age = 29;
    strcpy(Alice->name, "Alice");
    list_add(&Alice->list, &lists);

    struct people *River = (struct people *)malloc(sizeof(struct people));
    if (River == NULL) {
        err_quit("malloc fail");
    }
    River->age = 7;
    strcpy(River->name, "River");
    list_add_tail(&River->list, &lists);

    struct people *Edith = (struct people *)malloc(sizeof(struct people));
    if (Edith == NULL) {
        err_quit("malloc fail");
    }
    Edith->age = 17;
    strcpy(Edith->name, "Edith");
    list_add_tail(&Edith->list, &lists);

    struct people *pos, *next;
    list_for_each_entry_safe(pos, next, &lists, list) {
        if (pos->age < 10) {
            list_del(&pos->list);
        }
    }

    list_for_each_entry(pos, &lists, list) { printf("%s\n", pos->name); }
}
