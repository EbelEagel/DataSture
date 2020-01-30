/**
 * 顺序栈的基本操作实现
 * 参照教材46页
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
#define OVERFLOW -1
#define STACK_INIT_SIZE 100  // 栈存储空间初始分配量
#define STACKINCREMENT 10    // 栈容量每次增加的值

typedef int SElemType;
typedef int Status;

/**
 * 定义顺序栈
 * */
typedef struct {
    SElemType *base;  // 栈底指针，在栈构造前和销毁之后，base的值为NULL
    SElemType *top;  // 栈顶指针
    int stacksize;   // 当前已分配的存储空间，以元素为单位
} SqStack;

/**
 * 构造一个空栈S
 */
Status InitStack(SqStack &S)
{
    S.base = (SElemType *)malloc(STACK_INIT_SIZE * sizeof(SElemType));
    if (!S.base)
        return ERROR;  // 存储分配失败
    S.top = S.base;
    S.stacksize = STACK_INIT_SIZE;
    return OK;
}  // InitStack

/**
 * 销毁栈S，S不再存在
 */
Status DestroyStack(SqStack &S)
{
    free(S.base);
    S.base = NULL;
    return 0;
}  // DestroyStack

/**
 * 把S置为空栈
 */
Status ClearStack(SqStack &S)
{
    S.top = S.base;
    return 0;
}  // ClearStack

/**
 * 若栈S为空，返回TRUE， 否则返回FALSE
 */
Status StackEmpty(SqStack S) { return (S.base == S.top) ? TRUE : FALSE; }

/**
 * 返回S的元素个数，即栈的长度
 */
int StackLength(SqStack S) { return (S.top - S.base); }

/**
 * 若栈不空，则用e返回S的栈顶元素，并返回OK，否则返回ERROR
 */
Status GetTop(SqStack S, SElemType &e)
{
    if (S.base == S.top)  // 栈为空
        return ERROR;
    e = *(S.top - 1);  // 栈顶元素赋给e
    return OK;
}  // GetTop

/**
 * 若栈不空，返回S的栈顶元素
 */
int GetTop(SqStack S) { return *(S.top - 1); }

/**
 * 插入元素e为新的栈顶元素
 */
Status Push(SqStack &S, SElemType e)
{
    if (S.stacksize == StackLength(S)) {  // 栈达到了最大容量，自动增加容量
        S.base = (SElemType *)realloc(
            S.base, (S.stacksize + STACKINCREMENT) * sizeof(SElemType));
        if (!S.base)
            exit(OVERFLOW);  // 存储分配失败
        S.top = S.base + S.stacksize;
        S.stacksize += STACKINCREMENT;
    }
    *S.top++ = e;
    return OK;
}  // Push

/**
 * 若栈不空，则删除S的栈顶元素，用e返回其值，并返回OK，否则返回ERROR
 */
Status Pop(SqStack &S, SElemType &e)
{
    if (S.top == S.base)  // 栈为空
        return ERROR;
    e = *(--S.top);
    return OK;
}  // Pop

/**
 * 从栈底到栈顶每个元素依次调用visit()，旦visit失败，则操作失败
 */
Status StackTraverse(SqStack S, Status (*visit)(SElemType))
{
    SElemType *p;
    p = S.base;           // p指向栈底
    while (p != S.top) {  // 遍历栈的元素
        visit(*p);
        p++;
    }
    return OK;
}  // StackTraverse

/**
 * 打印元素
 */
Status visit_display(SElemType e)
{
    printf("%d ", e);
    return OK;
}

Status display_char(SElemType e)
{
    printf("%c ", e);
    return OK;
}

int main()
{
    SqStack stack1;
    InitStack(stack1);
    int arr1[8] = {33, 12, 75, 0, 49, 67, 8, 999};
    int arr2[3] = {24, 81, 100};
    for (int i = 0; i < sizeof(arr1) / sizeof(int); i++)
        Push(stack1, arr1[i]);

    printf("顺序栈stack1中有%d个元素，这%d个元素为: \n", StackLength(stack1),
           StackLength(stack1));
    StackTraverse(stack1, visit_display);

    // 将stack1的栈顶元素删除，并将arr2数组中的元素一次放入栈中，最后返回栈顶元素有
    int a1 = 0;
    Pop(stack1, a1);
    for (int i = 0; i < sizeof(arr2) / sizeof(int); i++)
        Push(stack1, arr2[i]);
    printf("\nstack1更新后有%d个元素\n其中栈顶元素为: %d", StackLength(stack1),
           GetTop(stack1));

    DestroyStack(stack1);

    return 0;
}