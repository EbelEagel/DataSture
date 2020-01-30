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

/**
 * 教材49页，3.2.2 括号匹配的检验, 匹配返回OK，不匹配返回ERROR
 */
Status checkBracket(char *expr)
{
    int i = 0;
    SqStack S;
    InitStack(S);
    SElemType e;
    while ('\0' != *(expr + i)) {
        switch (*(expr + i)) {
            case '(':
                Push(S, -1);
                break;
            case '[':
                Push(S, -2);
                break;
            case '{':
                Push(S, -3);
                break;
            case ')':
                if (OK == GetTop(S, e)) {  // 栈非空，得到栈顶元素
                    if (-1 == e)           // 栈顶元素与‘）’匹配
                        Pop(S, e);         // 删除栈顶元素
                    else                   // 不匹配，返回ERROR
                        return ERROR;
                } else
                    return ERROR;
                break;
            case ']':
                if (OK == GetTop(S, e)) {
                    if (-2 == e)
                        Pop(S, e);
                    else
                        return ERROR;
                } else
                    return ERROR;
                break;
            case '}':
                if (OK == GetTop(S, e)) {
                    if (-3 == e)
                        Pop(S, e);
                    else
                        return ERROR;
                } else
                    return ERROR;
                break;
            default:
                break;
        }  // switch

        i++;
    }  // while

    if (StackEmpty(S))
        return OK;
    else
        return ERROR;
}

/** 
 * 输出括号匹配的检验结果
 */
void outputResult(Status i, char *k)
{
    OK == i ? printf("表达式 \"%s\" 左右括号数匹配。\n", k)
            : printf("表达式 \"%s\" 左右括号数不匹配。\n", k);
}

int main()
{
    char a[] = "class A {private: char a; int b[]]; public: A(){} ~A(){}}";
    char c[] = "int main() { float a[] = {1.0, 5.2}; return 0;}";
    outputResult(checkBracket(a), a);
    outputResult(checkBracket(c), c);
    return 0;
}