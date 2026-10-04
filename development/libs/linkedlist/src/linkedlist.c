#include "linkedlist.h"
#include <stddef.h>

bool is_sentinal(struct inode* head){
    return head == head->right;
}

void init_list(struct inode* head){
    head->right = head;
    head->left = head;
}

static void __link_inode(struct inode* left, struct inode* head, struct inode* right){
    left->right = head;
    head->left = left;
    head->right = right;
    right->left = head;
}

void link_inode(struct inode* head, struct inode* new_node){
    __link_inode(head, new_node, head->right);
}

static void __unlink_inode(struct inode* left, struct inode* right){
    left->right = right;
    right->left = left;
}

void unlink_inode(struct inode* node){
    __unlink_inode(node->left, node->right);
    node->left = NULL;
    node->right = NULL;
}