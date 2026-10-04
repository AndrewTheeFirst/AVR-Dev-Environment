
#include <stddef.h>

#define CONTAINER_OF(ptr, type, member) (type*)((char*)ptr - offsetof(type, member))

struct inode{
    struct inode* left, *right;
};

void init_list(struct inode* head);

void link_inode(struct inode* head, struct inode* new_node);

void unlink_inode(struct inode* node);

bool is_sentinal(struct inode* head);