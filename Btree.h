#pragma once

// code by C

/**
 * Code by C
 * @version 1.35 2020-08-12
 * @author MPX
*/

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>

#define MaxM 10   //B树最大阶数


typedef struct BTree_node {  //B树和B树结点类型
    int* keys;              //结点关键字指针
    struct BTree_node** childrens;  //孩子指针
    int num;                //关键字个数
    int leaf;               //叶结点
}BTree_node;

typedef struct BTree {
    BTree_node* root;
    int t;
}BTree;

//查找(k为要查找的内容)
int BTree_Search(BTree* T, BTree_node* node, int k, int layer); 
//创建B树结点
BTree_node* BTree_CreateNode(int t, int leaf);  
//创建B树
void BTree_CreateTree(BTree* T, int t);  
//摧毁B树
void BTree_Destory(BTree_node* node); 
//分裂结点x的第i个孩子
void BTree_SplitChild(BTree* T, BTree_node* x, int i);  
//插入前对关键字的排序
void BTree_Insert_Nonfull(BTree* T, BTree_node* x, int k);  
//在B树中插入关键字
void BTree_Insert(BTree* T, int key);  
//遍历B树并将其结点输入文件
void BTree_Traverse(BTree* T, BTree_node* node, int layer);  
//打印显示整棵树
void BTree_Print(BTree* T, BTree_node* node, int layer);  
//B树删除关键字
void BTree_DeleteKey(BTree* T, BTree_node* node, int key);     
//删除B树
int BTree_DeleteTree(BTree* T, int key);   



int BTree_Search(BTree* T, BTree_node* node, int k, int layer) // k为要查找的内容，layer默认为0，查找成功返回关键字结点（int），失败返回-1
{
    BTree_node* p = node;
    int  i;
    if (p)
    {
        for (i = 0;i < node->num;i++)
        {
            if (p->keys[i] == k)  
                return p->keys[i];
        }
        layer++;
        for (i = 0;i <= p->num;i++)
        {
            if (p->childrens[i])
            {
                if ((BTree_Search(T, p->childrens[i], k, layer)) != -1)
                {
                    return BTree_Search(T, p->childrens[i], k, layer);
                    break;
                }
                else
                    continue;
            }
        }
    }
    return -1;
}

BTree_node* BTree_CreateNode(int t, int leaf)  //创建B树结点
{
    BTree_node* node = (BTree_node*)calloc(1, sizeof(BTree_node));  //分配内存空间，返回指向指针，并设置分配的内存为0
    if (node == NULL) assert(0);  //不存在结点，程序停止运行

    node->leaf = leaf;
    node->keys = (int*)calloc(1, (2 * t - 1) * sizeof(int));  //给键结点分配内存用尽
    node->childrens = (BTree_node**)calloc(1, (2 * t) * sizeof(BTree_node));  //给孩子分配内存空间
    node->num = 0;

    return node;
}

void BTree_CreateTree(BTree* T, int t)   //创建B树
{
    T->t = t;
    BTree_node* x = BTree_CreateNode(t, 1);
    T->root = x;
}

void BTree_Destory(BTree_node* node) //摧毁B树
{
    assert(node);  //若存在node，则向下执行；若不存在，终止程序

    free(node->childrens);
    free(node->keys);
    free(node);
}

void BTree_SplitChild(BTree* T, BTree_node* x, int i)  //分裂结点x的第i个孩子
{
    int t = T->t;
    BTree_node* y = x->childrens[i];
    BTree_node* z = BTree_CreateNode(t, y->leaf);  //创建一个B树结点

    z->num = t - 1;
    int j = 0;
    for (j = 0;j < t - 1;j++)
    {
        z->keys[j] = y->keys[j + t];
    }
    if (y->leaf == 0)  //如果叶结点为0，将孩子结点下移一层
    {
        for (j = 0;j < t;j++)
        {
            z->childrens[j] = y->childrens[j + t];
        }
    }
    y->num = t - 1;
    for (j = x->num;j >= i + 1;j--)
    {
        x->childrens[j + 1] = x->childrens[j];
    }
    x->childrens[i + 1] = z;
    for (j = x->num - 1;j >= i;j--)
    {
        x->keys[j + 1] = x->keys[j];
    }
    x->keys[i] = y->keys[t - 1];
    x->num += 1;
}

void BTree_Insert_Nonfull(BTree* T, BTree_node* x, int k) //插入前对关键字的排序
{
    int i = x->num - 1;    //i为关键字个数-1，数组中即为关键字对应下标
    if (x->leaf == 1)
    {
        while (i >= 0 && x->keys[i] > k)  //比较前后关键字大小，排序
        {
            x->keys[i + 1] = x->keys[i];
            i--;
        }
        x->keys[i + 1] = k;
        x->num += 1;
    }
    else
    {
        while (i >= 0 && x->keys[i] > k)
            i--;
        if (x->childrens[i + 1]->num == (2 * (T->t)) - 1)
        {
            BTree_SplitChild(T, x, i + 1);     //分裂结点
            if (k > x->keys[i + 1])
                i++;
        }
        BTree_Insert_Nonfull(T, x->childrens[i + 1], k);  //递归，分裂后重新排序关键字
    }
}

void BTree_Insert(BTree* T, int key)  //在B树中插入关键字
{
    BTree_node* r = T->root;
    if (r->num == 2 * T->t - 1)   // 若该结点中关键码个数等于阶数-1，则将引起结点的分裂。以中间关键码为界将结点一分为二，产生一个新结点，并把中间关键码插入到父结点(ｋ-1层)中
    {
        BTree_node* node = BTree_CreateNode(T->t, 0);
        T->root = node;

        node->childrens[0] = r;
        BTree_SplitChild(T, node, 0);     //插入叶子结点后不满足B树性质，需要对叶子结点进行分裂
        int i = 0;
        if (node->keys[0] < key)
            i++;
        BTree_Insert_Nonfull(T, node->childrens[i], key);
    }
    else
    {
        BTree_Insert_Nonfull(T, r, key);   //关键码个数小于阶数-1，直接插入
    }
}

FILE* fp;

void BTree_Traverse(BTree* T, BTree_node* node, int layer)  //遍历B树写入库文件
{
    BTree_node* p = node;
    int  i;
    if (p) {
        for (i = 0;i < node->num;i++)
            fprintf(fp, "%d\n", node->keys[i]);  //写入文件
        layer++;
        for (i = 0;i <= p->num;i++)
            if (p->childrens[i])
                BTree_Traverse(T, p->childrens[i], layer);
    }
    else printf("空\n");
}

void BTree_Print(BTree* T, BTree_node* node, int layer)
{
    BTree_node* p = node;
    int  i;
    if (p) {
        printf("\n层数 = %d 关键字个数 = %d 是否叶结点 = %d\n", layer, p->num, p->leaf);
        for (i = 0;i < node->num;i++)
            printf("%d ", p->keys[i]);
        printf("\n");
        layer++;
        for (i = 0;i <= p->num;i++)
            if (p->childrens[i])
                BTree_Print(T, p->childrens[i], layer);
    }
    else printf("该树为空树\n");
}

void BTree_Merge(BTree* T, BTree_node* node, int idx)  //把内结点node的idx个孩子和idx+1个孩子合并
{
    BTree_node* left = node->childrens[idx];  //第idx个孩子
    BTree_node* right = node->childrens[idx + 1];  //第idx+1个孩子

    int i = 0;
    left->keys[T->t - 1] = node->keys[idx];       //数据合并
    for (i = 0;i < T->t - 1;i++)
    {
        left->keys[T->t + i] = right->keys[i];
    }
    if (!left->leaf)
    {
        for (i = 0;i < T->t;i++) {
            left->childrens[T->t + i] = right->childrens[i];
        }
    }
    left->num += T->t;
    BTree_Destory(right);       //抹去右部分

    for (i = idx + 1;i < node->num;i++)
    {
        node->keys[i - 1] = node->keys[i];
        node->childrens[i] = node->childrens[i + 1];
    }
    node->childrens[i + 1] = NULL;
    node->num -= 1;

    if (node->num == 0)
    {
        T->root = left;
        BTree_Destory(node);
    }
}

void BTree_DeleteKey(BTree* T, BTree_node* node, int key)     //B树删除关键字
{
    if (node == NULL) return;

    int idx = 0, i;
    while (idx<node->num && key>node->keys[idx])
    {
        idx++;
    }
    if (idx < node->num && key == node->keys[idx]) {
        if (node->leaf) {      //关键字 key 在结点 node 中,node是叶结点, 直接在 node 中删除 key
            for (i = idx;i < node->num - 1;i++)
            {
                node->keys[i] = node->keys[i + 1];
            }
            node->keys[node->num - 1] = 0;
            node->num--;

            if (node->num == 0)
            {
                free(node);
                T->root = NULL;
            }
            return;
        }
        else if (node->childrens[idx]->num >= T->t) 
        //结点node前于 key 的子结点 left 至少含有 t 个关键字, 找出 key 在以 left 为根的子树中的前驱 key-1, 递归地删除key-1, 
        //并在node中用key-1 代替key
        {
            BTree_node* left = node->childrens[idx];
            node->keys[idx] = left->keys[left->num - 1];
            BTree_DeleteKey(T, left, left->keys[left->num - 1]);
        }
        else if (node->childrens[idx + 1]->num >= T->t) 
        /*对称的情况, 如果left中少于 t-1 个结点, 检查结点 node 中后于 key 的子结点 right. 如果 right 有 t 个关键字, 
        则找出 key 在以 right 为根的子树中的后继 key+1. 递归地删除 key+1, 并在 node 中 用 key+1 代替 key*/
        {
            BTree_node* right = node->childrens[idx + 1];
            node->keys[idx] = right->keys[0];
            BTree_DeleteKey(T, right, right->keys[0]);
        }
        else 
        /*left和right都只是有t-1 个结点, 则把 key 和 right 合并进入结点 left, 此时的 node 失去了 key 和指向 right 的指针, 
        且 left 包含t-1个关键字. 释放right, 递归地从left中删除 key*/
        {
            BTree_Merge(T, node, idx);
            BTree_DeleteKey(T, node->childrens[idx], key);
        }
    }
    else 
    /*关键字 key 不在内部结点 node 中, 则必包含于node->childrens中. 如果 node->childrens 只有 t-1 个关键字, 
    那么执行这两步保证下降到一个至少包含 t 个关键字的结点*/
    {
        BTree_node* child = node->childrens[idx];
        if (child == NULL)
        {
            printf("无法删除键%d\n", key);
            return;
        }
        /*如果node->children只有 t-1 个关键字, 但它的兄弟至少有 t 个关键字, 则把node中的某个关
        键字下降到node->children中, 讲 node->children 的相邻左兄弟或右兄弟的一个关键字升至 node, 讲该兄弟
        中相应的孩子指针移到 node->children中, 使node->children增加一个关键字 */
        /*如果 node->children 以及 node->children 的所有兄弟结点都只有 t-1 个关键字, 则把 node->children 与一个兄弟合
        并, 即把 node 的一个关键字移到新合并的结点, 使之成为该结点的中间关键字*/
        if (child->num == T->t - 1)
        {
            BTree_node* left = NULL;
            BTree_node* right = NULL;
            if (idx - 1 >= 0)
                left = node->childrens[idx - 1];
            if (idx + 1 <= node->num)
                right = node->childrens[idx + 1];
            if ((left && left->num >= T->t) || (right && right->num >= T->t))
            {
                int richR = 0;
                if (right)
                    richR = 1;
                if (left && right) {
                    if (right->num > left->num)
                        richR = 1;
                    else richR = 0;
                }
                if (right && right->num >= T->t && richR)
                {
                    child->keys[child->num] = node->keys[idx];
                    child->childrens[child->num + 1] = right->childrens[0];
                    child->num++;

                    node->keys[idx] = right->keys[0];
                    for (i = 0;i < right->num - 1;i++)
                    {
                        right->keys[i] = right->keys[i + 1];
                        right->childrens[i] = right->childrens[i + 1];
                    }
                    right->keys[right->num - 1] = 0;
                    right->childrens[right->num - 1] = right->childrens[right->num];
                    right->childrens[right->num] = NULL;
                    right->num--;
                }
                else
                /*如果 node->children 以及 node->children 的所有兄弟结点都只有 t-1 个关键字, 则把 node->children 与一个兄弟合
                并, 即把 node 的一个关键字移到新合并的结点, 使之成为该结点的中间关键字*/
                {
                    for (i = child->num;i > 0;i--)
                    {
                        child->keys[i] = child->keys[i - 1];
                        child->childrens[i + 1] = child->childrens[i];
                    }
                    child->childrens[1] = child->childrens[0];
                    child->childrens[0] = left->childrens[left->num];
                    child->keys[0] = node->keys[idx - 1];
                    child->num++;

                    left->keys[left->num - 1] = 0;
                    left->childrens[left->num] = NULL;
                    left->num--;
                }
            }
            else if ((!left || (left->num == T->t - 1)) && (!right || (right->num == T->t - 1)))
            {
                if (left && left->num == T->t - 1)
                {
                    BTree_Merge(T, node, idx - 1);
                    child = left;
                }
                else if (right && right->num == T->t - 1)
                {
                    BTree_Merge(T, node, idx);
                }
            }
        }
        BTree_DeleteKey(T, child, key);
    }

}

int BTree_DeleteTree(BTree* T, int key)   //删除B树
{
    if (!T->root)
        return -1;
    BTree_DeleteKey(T, T->root, key);
    return 0;
}

