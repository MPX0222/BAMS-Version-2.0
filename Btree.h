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

#define MaxM 10   //B��������


typedef struct BTree_node {  //B����B���������
    int* keys;              //���ؼ���ָ��
    struct BTree_node** childrens;  //����ָ��
    int num;                //�ؼ��ָ���
    int leaf;               //Ҷ���
}BTree_node;

typedef struct BTree {
    BTree_node* root;
    int t;
}BTree;

//����(kΪҪ���ҵ�����)
int BTree_Search(BTree* T, BTree_node* node, int k, int layer); 
//����B�����
BTree_node* BTree_CreateNode(int t, int leaf);  
//����B��
void BTree_CreateTree(BTree* T, int t);  
//�ݻ�B��
void BTree_Destory(BTree_node* node); 
//���ѽ��x�ĵ�i������
void BTree_SplitChild(BTree* T, BTree_node* x, int i);  
//����ǰ�Թؼ��ֵ�����
void BTree_Insert_Nonfull(BTree* T, BTree_node* x, int k);  
//��B���в���ؼ���
void BTree_Insert(BTree* T, int key);  
//����B���������������ļ�
void BTree_Traverse(BTree* T, BTree_node* node, int layer);  
//��ӡ��ʾ������
void BTree_Print(BTree* T, BTree_node* node, int layer);  
//B��ɾ���ؼ���
void BTree_DeleteKey(BTree* T, BTree_node* node, int key);     
//ɾ��B��
int BTree_DeleteTree(BTree* T, int key);   



int BTree_Search(BTree* T, BTree_node* node, int k, int layer) // kΪҪ���ҵ����ݣ�layerĬ��Ϊ0�����ҳɹ����عؼ��ֽ�㣨int����ʧ�ܷ���-1
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

BTree_node* BTree_CreateNode(int t, int leaf)  //����B�����
{
    BTree_node* node = (BTree_node*)calloc(1, sizeof(BTree_node));  //�����ڴ�ռ䣬����ָ��ָ�룬�����÷�����ڴ�Ϊ0
    if (node == NULL) assert(0);  //�����ڽ�㣬����ֹͣ����

    node->leaf = leaf;
    node->keys = (int*)calloc(1, (2 * t - 1) * sizeof(int));  //�����������ڴ��þ�
    node->childrens = (BTree_node**)calloc(1, (2 * t) * sizeof(BTree_node));  //�����ӷ����ڴ�ռ�
    node->num = 0;

    return node;
}

void BTree_CreateTree(BTree* T, int t)   //����B��
{
    T->t = t;
    BTree_node* x = BTree_CreateNode(t, 1);
    T->root = x;
}

void BTree_Destory(BTree_node* node) //�ݻ�B��
{
    assert(node);  //������node��������ִ�У��������ڣ���ֹ����

    free(node->childrens);
    free(node->keys);
    free(node);
}

void BTree_SplitChild(BTree* T, BTree_node* x, int i)  //���ѽ��x�ĵ�i������
{
    int t = T->t;
    BTree_node* y = x->childrens[i];
    BTree_node* z = BTree_CreateNode(t, y->leaf);  //����һ��B�����

    z->num = t - 1;
    int j = 0;
    for (j = 0;j < t - 1;j++)
    {
        z->keys[j] = y->keys[j + t];
    }
    if (y->leaf == 0)  //���Ҷ���Ϊ0�������ӽ������һ��
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

void BTree_Insert_Nonfull(BTree* T, BTree_node* x, int k) //����ǰ�Թؼ��ֵ�����
{
    int i = x->num - 1;    //iΪ�ؼ��ָ���-1�������м�Ϊ�ؼ��ֶ�Ӧ�±�
    if (x->leaf == 1)
    {
        while (i >= 0 && x->keys[i] > k)  //�Ƚ�ǰ��ؼ��ִ�С������
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
            BTree_SplitChild(T, x, i + 1);     //���ѽ��
            if (k > x->keys[i + 1])
                i++;
        }
        BTree_Insert_Nonfull(T, x->childrens[i + 1], k);  //�ݹ飬���Ѻ���������ؼ���
    }
}

void BTree_Insert(BTree* T, int key)  //��B���в���ؼ���
{
    BTree_node* r = T->root;
    if (r->num == 2 * T->t - 1)   // ���ý���йؼ���������ڽ���-1����������ķ��ѡ����м�ؼ���Ϊ�罫���һ��Ϊ��������һ���½�㣬�����м�ؼ�����뵽�����(��-1��)��
    {
        BTree_node* node = BTree_CreateNode(T->t, 0);
        T->root = node;

        node->childrens[0] = r;
        BTree_SplitChild(T, node, 0);     //����Ҷ�ӽ�������B�����ʣ���Ҫ��Ҷ�ӽ����з���
        int i = 0;
        if (node->keys[0] < key)
            i++;
        BTree_Insert_Nonfull(T, node->childrens[i], key);
    }
    else
    {
        BTree_Insert_Nonfull(T, r, key);   //�ؼ������С�ڽ���-1��ֱ�Ӳ���
    }
}

FILE* fp;

void BTree_Traverse(BTree* T, BTree_node* node, int layer)  //����B��д����ļ�
{
    BTree_node* p = node;
    int  i;
    if (p) {
        for (i = 0;i < node->num;i++)
            fprintf(fp, "%d\n", node->keys[i]);  //д���ļ�
        layer++;
        for (i = 0;i <= p->num;i++)
            if (p->childrens[i])
                BTree_Traverse(T, p->childrens[i], layer);
    }
    else printf("��\n");
}

void BTree_Print(BTree* T, BTree_node* node, int layer)
{
    BTree_node* p = node;
    int  i;
    if (p) {
        printf("\n���� = %d �ؼ��ָ��� = %d �Ƿ�Ҷ��� = %d\n", layer, p->num, p->leaf);
        for (i = 0;i < node->num;i++)
            printf("%d ", p->keys[i]);
        printf("\n");
        layer++;
        for (i = 0;i <= p->num;i++)
            if (p->childrens[i])
                BTree_Print(T, p->childrens[i], layer);
    }
    else printf("����Ϊ����\n");
}

void BTree_Merge(BTree* T, BTree_node* node, int idx)  //���ڽ��node��idx�����Ӻ�idx+1�����Ӻϲ�
{
    BTree_node* left = node->childrens[idx];  //��idx������
    BTree_node* right = node->childrens[idx + 1];  //��idx+1������

    int i = 0;
    left->keys[T->t - 1] = node->keys[idx];       //���ݺϲ�
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
    BTree_Destory(right);       //Ĩȥ�Ҳ���

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

void BTree_DeleteKey(BTree* T, BTree_node* node, int key)     //B��ɾ���ؼ���
{
    if (node == NULL) return;

    int idx = 0, i;
    while (idx<node->num && key>node->keys[idx])
    {
        idx++;
    }
    if (idx < node->num && key == node->keys[idx]) {
        if (node->leaf) {      //�ؼ��� key �ڽ�� node ��,node��Ҷ���, ֱ���� node ��ɾ�� key
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
        //���nodeǰ�� key ���ӽ�� left ���ٺ��� t ���ؼ���, �ҳ� key ���� left Ϊ���������е�ǰ�� key-1, �ݹ��ɾ��key-1, 
        //����node����key-1 ����key
        {
            BTree_node* left = node->childrens[idx];
            node->keys[idx] = left->keys[left->num - 1];
            BTree_DeleteKey(T, left, left->keys[left->num - 1]);
        }
        else if (node->childrens[idx + 1]->num >= T->t) 
        /*�ԳƵ����, ���left������ t-1 �����, ����� node �к��� key ���ӽ�� right. ��� right �� t ���ؼ���, 
        ���ҳ� key ���� right Ϊ���������еĺ�� key+1. �ݹ��ɾ�� key+1, ���� node �� �� key+1 ���� key*/
        {
            BTree_node* right = node->childrens[idx + 1];
            node->keys[idx] = right->keys[0];
            BTree_DeleteKey(T, right, right->keys[0]);
        }
        else 
        /*left��right��ֻ����t-1 �����, ��� key �� right �ϲ������� left, ��ʱ�� node ʧȥ�� key ��ָ�� right ��ָ��, 
        �� left ����t-1���ؼ���. �ͷ�right, �ݹ�ش�left��ɾ�� key*/
        {
            BTree_Merge(T, node, idx);
            BTree_DeleteKey(T, node->childrens[idx], key);
        }
    }
    else 
    /*�ؼ��� key �����ڲ���� node ��, ��ذ�����node->childrens��. ��� node->childrens ֻ�� t-1 ���ؼ���, 
    ��ôִ����������֤�½���һ�����ٰ��� t ���ؼ��ֵĽ��*/
    {
        BTree_node* child = node->childrens[idx];
        if (child == NULL)
        {
            printf("�޷�ɾ����%d\n", key);
            return;
        }
        /*���node->childrenֻ�� t-1 ���ؼ���, �������ֵ������� t ���ؼ���, ���node�е�ĳ����
        �����½���node->children��, �� node->children ���������ֵܻ����ֵܵ�һ���ؼ������� node, �����ֵ�
        ����Ӧ�ĺ���ָ���Ƶ� node->children��, ʹnode->children����һ���ؼ��� */
        /*��� node->children �Լ� node->children �������ֵܽ�㶼ֻ�� t-1 ���ؼ���, ��� node->children ��һ���ֵܺ�
        ��, ���� node ��һ���ؼ����Ƶ��ºϲ��Ľ��, ʹ֮��Ϊ�ý����м�ؼ���*/
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
                /*��� node->children �Լ� node->children �������ֵܽ�㶼ֻ�� t-1 ���ؼ���, ��� node->children ��һ���ֵܺ�
                ��, ���� node ��һ���ؼ����Ƶ��ºϲ��Ľ��, ʹ֮��Ϊ�ý����м�ؼ���*/
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

int BTree_DeleteTree(BTree* T, int key)   //ɾ��B��
{
    if (!T->root)
        return -1;
    BTree_DeleteKey(T, T->root, key);
    return 0;
}

