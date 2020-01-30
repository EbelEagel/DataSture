/**
 * 迷宫问题求解算法的实现
 * 部分内容参照教材51页
 */

#include <stdio.h>
#include <stdlib.h>

#define ERROR 0
#define OK 1
#define TRUE 1
#define FALSE 0
#define OVERFLOW -1
#define STACK_INIT_SIZE 100  // 栈初始化值
#define STACKINCREMENT 10    // 栈容量每次增加的值
#define SIZE 10              // 定义迷宫地图大小

typedef struct {
    int row;  // 行，0起始
    int col;  // 列，0起始
} PosType;

typedef struct {
    int ord;       // 通道块在路径上的序号
    PosType seat;  // 通道块在迷宫中的坐标位置
    int di;        // 从此通道走向下一通道块的方向
} SElemType;

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

extern SqStack S;
extern int Map[SIZE][SIZE];

/**
 * 判断当前位置能不能通过，当前位置可通过是指该位置未曾到达过
 */
bool Pass(PosType curpos)
{
    if (1 == Map[curpos.row][curpos.col])
        return false;
    return true;
}

/**
 * 在走过的地方留下足迹
 */
void FootPrint(PosType curpos) { Map[curpos.row][curpos.col] = 1; }

/**
 * 下一个位置，di从1到4表示从东到北
 */
PosType NextPos(PosType pos, int di)
{
    PosType curpos;
    switch (di) {
        case 1:  // 东, 列加1
            curpos.row = pos.row;
            curpos.col = pos.col + 1;
            break;
        case 2:  // 南，行加1
            curpos.row = pos.row + 1;
            curpos.col = pos.col;
            break;
        case 3:  // 西，列减1
            curpos.row = pos.row;
            curpos.col = pos.col - 1;
            break;
        case 4:  // 北，行减1
            curpos.row = pos.row - 1;
            curpos.col = pos.col;
            break;
    }
    return curpos;
}

/**
 * 算法3.3，迷宫算法
 */
Status MazePath(PosType start, PosType end)
{
    PosType curpos;
    int curstep = 1;  // 探索第一步
    curpos = start;
    SElemType e;  // 通道块
    do {
        if (Pass(curpos)) {     // 如果当前位置能通过
            FootPrint(curpos);  // 留下足迹
            e.ord = curstep;
            e.seat = curpos;
            e.di = 1;
            Push(S, e);  // 加入路径
            if (curpos.row == end.row && curpos.col == end.col)
                return (OK);              // 到达终点
            curpos = NextPos(curpos, 1);  // 将当前位置的东邻设为下一位置
            curstep++;                    // 探索下一步
        } else {                          // 如果当前位置不能通过
            if (!StackEmpty(S)) {
                Pop(S, e);
                while (4 == e.di && !StackEmpty(S))
                    Pop(S, e);  // 即该位置的4方向都已经探索完成，就再退回一步
                if (e.di < 4) {  // 换一个方向探索
                    e.di++;
                    Push(S, e);
                    curpos = NextPos(e.seat, e.di);
                }
            }
        }
    } while (!StackEmpty(S));

    return ERROR;
}

Status display_path(SElemType e)
{
    printf("(%d, %d) ", e.seat.row, e.seat.col);
    switch (e.di) {
        case 1:
            printf("东\n");
            break;
        case 2:
            printf("南\n");
            break;
        case 3:
            printf("西\n");
            break;
        case 4:
            printf("北\n");
            break;
    }
    return OK;
}

int Map[SIZE][SIZE] = {{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
                       {1, 0, 0, 1, 0, 0, 0, 1, 0, 1},
                       {1, 0, 0, 1, 0, 0, 0, 1, 0, 1},
                       {1, 0, 0, 0, 0, 1, 1, 0, 0, 1},
                       {1, 0, 1, 1, 1, 0, 0, 0, 0, 1},
                       {1, 0, 0, 0, 1, 0, 0, 0, 0, 1},
                       {1, 0, 1, 0, 0, 0, 1, 0, 0, 1},
                       {1, 0, 1, 1, 1, 0, 1, 1, 0, 1},
                       {1, 1, 0, 0, 0, 0, 0, 0, 0, 1},
                       {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};  // 迷宫地图
SqStack S;                                               // 路径栈

int main()
{
    InitStack(S);
    PosType start;
    PosType end;
    start.row = 1;
    start.col = 1;

    end.row = 8;
    end.col = 8;

    MazePath(start, end);
    StackTraverse(S, display_path);
    return 0;
}