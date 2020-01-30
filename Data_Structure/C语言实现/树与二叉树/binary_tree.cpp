/**
 * 二叉树的二叉链表存储表示与实现
 * 参照教材127页
 * */

#include <stdio.h>
#include <stdlib.h>

/**
 * 宏定义及类型定义
 * */
#define ERROR -1
#define OK 1
#define TRUE 1
#define FALSE 0
typedef int Status;
typedef char TElemType;  // 定义二叉树结点的数据元素类型

/**
 * 定义二叉树的结点
 * */
typedef struct BiTNode {
    TElemType data;
    struct BiTNode *lchild, *rchild;  // 左右孩子指针
} BiTNode, *BiTree;

typedef BiTree QElemType;
// 定义链队列结点
typedef struct QNode {
    QElemType data;
    struct QNode *next;
} QNode, *QueuePtr;

typedef struct {     // 定义链队列
    QueuePtr front;  // 队头指针
    QueuePtr rear;   // 队尾指针
} LinkQueue;

/**
 * 构造一个空队列Q
 */
Status InitQueue(LinkQueue &Q)
{
    Q.front = (QNode *)malloc(sizeof(QNode));
    if (!Q.front)
        return ERROR;
    Q.front->next = NULL;
    Q.rear = Q.front;
    return OK;
}

/**
 * 判断Q是否为空, 是返回TRUE，否则返回FALSE
 */
Status QueueEmpty(LinkQueue Q)
{
    if (Q.front == Q.rear)
        return TRUE;
    else
        return FALSE;
}

/**
 * 插入元素e为Q的新的队尾元素
 */
Status EnQueue(LinkQueue &Q, QElemType e)
{
    QueuePtr p;
    p = (QNode *)malloc(sizeof(QNode));  // 生成新的结点
    if (!p)
        return ERROR;
    p->data = e;
    p->next = NULL;
    Q.rear->next = p;
    Q.rear = p;
    return OK;
}

/**
 * 若队列不空，删除队列Q的队头元素，用e返回其值，返回OK，否则返回ERROR
 */
Status DeQueue(LinkQueue &Q, QElemType &e)
{
    QueuePtr p;
    p = Q.front->next;
    if (Q.front == Q.rear)
        return ERROR;
    e = p->data;
    if (Q.front->next == Q.rear)  // 队列中只有一个元素时，还要修改尾指针
        Q.rear = Q.front;
    Q.front->next = p->next;
    free(p);
    p = NULL;
    return OK;
}

/**
 * 算法6.4，按先序顺序构造二叉树
 */
Status CreateBiTree(BiTree &T)
{
    char ch;
    scanf("%c", &ch);
    if (' ' == ch)
        T = NULL;
    else {
        T = (BiTNode *)malloc(sizeof(BiTNode));
        if (!T)
            return ERROR;
        T->data = ch;
        CreateBiTree(T->lchild);  // 构造左子树
        CreateBiTree(T->rchild);  // 构造右子树
    }
    return OK;
}

/**
 * 打印一个元素
 */
Status display(TElemType e)
{
    printf("%c", e);
    return OK;
}

/**
 * 算法6.1，先序遍历二叉树，对每个元素调用函数visit
 */
Status PreOrderTraverse(BiTree T, Status (*visit)(TElemType e))
{
    if (T) {
        if (OK == visit(T->data))
            if (OK == PreOrderTraverse(T->lchild, visit))
                if (OK == PreOrderTraverse(T->rchild, visit))
                    return OK;
        return ERROR;
    } else
        return OK;
}

/**
 * 中序遍历二叉树，对每个元素调用函数visit
 */
Status InOrderTraverse(BiTree T, Status (*visit)(TElemType e))
{
    if (T) {
        if (OK == InOrderTraverse(T->lchild, visit))
            if (OK == visit(T->data))
                if (OK == InOrderTraverse(T->rchild, visit))
                    return OK;
        return ERROR;
    } else
        return OK;
}

/**
 * 后序遍历二叉树，对每个元素调用函数visit
 */
Status PostOrderTraverse(BiTree T, Status (*visit)(TElemType e))
{
    if (T) {
        if (OK == PostOrderTraverse(T->lchild, visit))
            if (OK == PostOrderTraverse(T->rchild, visit))
                if (OK == visit(T->data))
                    return OK;
        return ERROR;
    } else
        return OK;
}

/**
 * 层序遍历二叉树，利用队列实现
 */
Status LevelOrderTraverse(BiTree T, Status (*visit)(TElemType e))
{
    BiTree p;
    LinkQueue Q;
    InitQueue(Q);
    p = T;
    while (p || !QueueEmpty(Q)) {
        if (p) {
            visit(p->data);
            if (p->lchild)
                EnQueue(Q, p->lchild);
            if (p->rchild)
                EnQueue(Q, p->rchild);
            if (!QueueEmpty(Q))
                DeQueue(Q, p);
            else  // 队列为空时，退出while循环
                break;
        }
    }
    return OK;
}

// /**
//  * 非递归法实现先序遍历二叉树T
//  */
// Status PreOrderTraverse2(BiTree T, Status (*visit)(TElemType e))
// {
//     SqStack S;
//     BiTree p;
//     InitStack(S);
//     p = T;
//     while (p || !StackEmpty(S)) {
//         if (p) {  // 访问根指针，左指针进栈
//             if (ERROR == visit(p->data))
//                 return ERROR;
//             Push(S, p);
//             p = p->lchild;
//         } else {
//             Pop(S, p);
//             p = p->rchild;
//         }
//     }
//     return OK;
// }

// /**
//  * 算法6.2，非递归法实现中序遍历二叉树T
//  */
// Status InOrderTraverse2(BiTree T, Status (*visit)(TElemType e))
// {
//     SqStack S;
//     BiTree p;
//     InitStack(S);
//     Push(S, T);  // 根指针进栈
//     while (!StackEmpty(S)) {
//         while (OK == GetTop(S, p) && p)  // 向左走到尽头
//             Push(S, p->lchild);
//         Pop(S, p);  // 空指针退栈
//         if (!StackEmpty(S)) {
//             Pop(S, p);
//             if (ERROR == visit(p->data))
//                 return ERROR;
//             Push(S, p->rchild);
//         }  // if
//     }      // while
//     return OK;
// }

// /**
//  * 算法6.3，非递归法实现中序遍历二叉树T
//  */
// Status InOrderTraverse3(BiTree T, Status (*visit)(TElemType e))
// {
//     SqStack S;
//     BiTree p;
//     InitStack(S);
//     p = T;
//     while (p || !StackEmpty(S)) {
//         if (p) {  // 根指针进栈，遍历左子树
//             Push(S, p);
//             p = p->lchild;
//         } else {  // 根指针退栈，访问根结点，遍历右子树
//             Pop(S, p);
//             if (ERROR == visit(p->data))
//                 return ERROR;
//             p = p->rchild;
//         }  // if
//     }      // while
//     return OK;
// }

// /**
//  * 非递归法实现后序遍历二叉树T
//  */
// Status PostOrderTraverse2(BiTree T, Status (*visit)(TElemType e))
// {
//     SqStack S;
//     InitStack(S);
//     BiTree p;
//     int Tag[20];  // 标志栈
//     int t = 1;
//     p = T;
//     while (p || !StackEmpty(S)) {
//         while (p) {  // 向左走到尽头
//             Push(S, p);
//             p = p->lchild;
//             Tag[t++] = 0;
//         }

//         while (!StackEmpty(S) &&
//                1 == Tag[t - 1]) {  // 结点标志为1表示右子树已经访问过
//             Pop(S, p);
//             t##;
//             if (ERROR == visit(p->data))
//                 return ERROR;
//         }

//         if (!StackEmpty(S)) {  //
//         结点标志为0，则访问右子树，并将结点标志置为1
//             Tag[t - 1] = 1;
//             GetTop(S, p);
//             p = p->rchild;
//         } else
//             break;
//     }

//     return OK;
// }

int main()
{
    BiTree t1, t2;

    printf(
        "请按先序次序输入二叉树 t1 "
        "中结点的值（一个字符），空格字符表示空树：\n");
    // 在交互界面输入如下语句："I-oY  o  vu  !  Le  -  "
    CreateBiTree(t1);
    printf("二叉树 t1 的层序遍历结果为：\n");
    LevelOrderTraverse(t1, display);  // 测试二叉树的层序遍历

    printf(
        "\n\n请按先序次序输入二叉树 t2 "
        "中结点的值（一个字符），空格字符表示空树：\n");
    // 在交互界面输入如下语句：",n-oY  u  a  -d  em   -hon  t  ni  g  teb  t  re  !   "
    CreateBiTree(t2);
    printf("二叉树 t2 的中序遍历结果为：\n");
    InOrderTraverse(t2, display);

    return 0;
}