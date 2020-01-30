/**
 * 循环队列的顺序存储结构及基本操作实现
 * 参照教材64页
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
#define MAXQSIZE 100  // 最大队列长度

typedef int QElemType;
typedef int Status;

/**
 * 定义循环队列
 * */
typedef struct {
    QElemType *base;  // 初始化的动态分配存储空间
    int front;        // 头指针，若队列不空，指向队列头元素
    int rear;  // 尾指针，若队列不空，指向队列尾元素的下一个位置
} CyclicQueue;

/**
 * 构造一个空的循环队列Q
 */
Status InitQueue(CyclicQueue &Q)
{
    Q.base = (QElemType *)malloc(MAXQSIZE * sizeof(QElemType));
    if (!Q.base)
        return ERROR;
    Q.front = 0;
    Q.rear = 0;
    return OK;
}  // InitQueue

/**
 * 销毁队列
 */
Status DestroyQueue(CyclicQueue &Q)
{
    free(Q.base);
    Q.base = NULL;
    return OK;
}  // DestroyQueue

/**
 * 清空队列
 */
Status ClearQueue(CyclicQueue &Q)
{
    Q.front = Q.rear = 0;
    return OK;
}  // ClearQueue

/**
 * 判断队列是否为空
 */
Status QueueEmpty(CyclicQueue &Q)
{
    if (Q.rear == Q.front)
        return TRUE;
    return FALSE;
}  // QueueEmpty

/**
 * 返回队列Q的元素个数， 即队列的长度
 */
Status QueueLength(CyclicQueue Q)
{
    return (Q.rear - Q.front + MAXQSIZE) % MAXQSIZE;
}  // QueueLength

/**
 * 得到队列头元素，用e返回，队列为空返回ERROR，否则返回OK
 */
Status GetHead(CyclicQueue Q, QElemType &e)
{
    if (Q.rear == Q.front)
        return ERROR;
    e = *(Q.base + Q.front);
    return OK;
}  // GetHead

/**
 * 插入元素e为Q的新的队尾元素，队列已满返回ERROR，否则返回OK
 */
Status EnQueue(CyclicQueue &Q, QElemType e)
{
    if ((Q.rear + 1) % MAXQSIZE == Q.front)  // 队列满
        return ERROR;
    *(Q.base + Q.rear) = e;
    Q.rear = (Q.rear + 1) % MAXQSIZE;
    return OK;
}  // EnQueue

/**
 * 若队列不空，则删除队头元素，用e返回其值，并返回OK，否则返回ERROR
 */
Status DeQueue(CyclicQueue &Q, QElemType &e)
{
    if (Q.front == Q.rear)
        return ERROR;
    e = *(Q.base + Q.front);
    Q.front = (Q.front + 1) % MAXQSIZE;
    return OK;
}  // DeQueue

/**
 * 从队头到队尾遍历Q中每个元素调用visit(), visit()失败则操作失败
 */
Status QueueTraverse(CyclicQueue Q, Status (*visit)(QElemType &e))
{
    int p;
    p = Q.front;
    while (p != Q.rear) {
        if (!(*visit)(*(Q.base + p)))
            return ERROR;
        p = (p + 1) % MAXQSIZE;
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