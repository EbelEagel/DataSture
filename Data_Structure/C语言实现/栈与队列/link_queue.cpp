/**
 * 链队列的基本操作实现
 * 参照教材61页
 * */

#include <stdio.h>
#include <stdlib.h>

/**
 * 宏定义及类型定义
 * */
#define ERROR 0
#define OK 1
#define TRUE 1
#define FALSE 0

typedef int QElemType;
typedef int Status;

typedef struct QNode {
    QElemType data;
    struct QNode *next;
} QNode, *QueuePtr;

/**
 * 定义链队列
 * */
typedef struct {
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
}  // InitQueue

/**
 * 销毁队列Q，Q不再存在
 */
Status DestroyQueue(LinkQueue &Q)
{
    while (Q.front) {
        Q.rear = Q.front->next;
        free(Q.front);
        Q.front = Q.rear;
    }
    return OK;
}  // DestroyQueue

/**
 * 将Q清为空队列, 并释放结点空间
 */
Status ClearQueue(LinkQueue &Q)
{
    QueuePtr p, temp;
    p = Q.front->next;
    Q.front->next = NULL;
    Q.rear = Q.front;
    while (p) {
        temp = p->next;
        free(p);
        p = temp;
    }
    return OK;
}  // ClearQueue

/**
 * 判断Q是否为空队列, 是返回TRUE，否则返回FALSE
 */
Status QueueEmpty(LinkQueue Q)
{
    if (Q.front == Q.rear)
        return TRUE;
    else
        return FALSE;
}  // QueueEmpty

/**
 * 返回Q的元素个数，即队列的长度
 */
int QueueLength(LinkQueue Q)
{
    QueuePtr p;
    int len = 0;
    p = Q.front->next;
    while (p) {
        len++;
        p = p->next;
    }
    return len;
}  // QueueLength

/**
 * 若队列不空，则用e返回Q的队头元素，并返回OK；否则返回ERROR
 */
Status GetHead(LinkQueue Q, QElemType &e)
{
    if (Q.front == Q.rear)
        return ERROR;
    e = Q.front->next->data;
    return OK;
}  // GetHead

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
}  // EnQueue

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
}  // DeQueue

/**
 * 从队头到队尾依次对Q中每个元素调用visit(), visit()失败则操作失败
 */
Status QueueTraverse(LinkQueue Q, Status (*visit)(QElemType &e))
{
    QueuePtr p;
    p = Q.front->next;
    while (p) {
        if (!(*visit)(p->data))
            return ERROR;
        p = p->next;
    }
    return OK;
}  // QueueTraverse

/**
 * 打印元素
 */
Status visit_display(QElemType &e)
{
    printf("%d ", e);
    return OK;
}

int main()
{
    LinkQueue queue1;
    InitQueue(queue1);
    int arr1[8] = {33, 12, 75, 0, 49, 67, 8, 999};
    for (int i = 0; i < sizeof(arr1) / sizeof(int); i++)
        EnQueue(queue1, arr1[i]);

    printf("链队列queue1中有%d个元素, 分别为: \n", QueueLength(queue1));
    QueueTraverse(queue1, visit_display);

    int head;
    GetHead(queue1, head);
    printf("\n第一个元素的值为: %d", head);

    // 删除队头元素
    DeQueue(queue1, head);
    GetHead(queue1, head);
    printf("\n删除队头元素后, 第一个元素的值为: %d", head);

    // 在队尾插入元素52，并遍历
    int tail = 52;
    EnQueue(queue1, tail);
    printf("\n插入新元素后的queue1为: \n");
    QueueTraverse(queue1, visit_display);

    DestroyQueue(queue1);

    return 0;
}