/**
 * 数组的顺序存储表示和实现
 * 参照教材93页
 * */

// <stdarg> 为标准头文件，提供宏 va_start、 va_arg、 va_end，用于存取变长参数表
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

/**
 * 宏定义及类型定义
 * */
#define OK 1
#define ERROR -1
#define OVERFLOW -1
#define MAX_ARRAY_DIM 8  // 假设数组维数的最大值为8

typedef int ElemType;
typedef int Status;

/**
 * 定义顺序数组
 * */
typedef struct {
    ElemType *base;  // 数组元素基址，由InitArray分配
    int dim;         // 数组维数
    int *bounds;  // 数组维界基址，由InitArray分配（存放每一维的个数）
    int *constants;  // 数组映像函数常量基址，由InitArray分配
} Array;

/**
 * 初始化数组，若维数dim和随后的各维长度合法，则构造相应的数组A，并返回OK
 */
Status InitArray(Array &A, int dim, ...)
{
    va_list ap;
    int i;
    int elemtotal = 1;  // 保存数组的元素个数
    if (dim < 1 || dim > MAX_ARRAY_DIM)
        return ERROR;
    A.dim = dim;
    A.bounds = (int *)malloc(dim * sizeof(int));
    if (!A.bounds)
        exit(OVERFLOW);
    va_start(ap, dim);  // ap为va_list类型，是存放变长参数信息的数组
    for (i = 0; i < dim; i++) {
        A.bounds[i] = va_arg(ap, int);  // 读入一个变长参数
        if (A.bounds[i] < 0)
            return ERROR;
        elemtotal *= A.bounds[i];
    }

    va_end(ap);
    A.base = (ElemType *)malloc(elemtotal * sizeof(ElemType));
    if (!A.base)
        exit(OVERFLOW);
    // 下面求映像函数Ci,并存入A.constants[i-1], i=1,...,dim
    A.constants = (int *)malloc(dim * sizeof(int));
    if (!A.constants)
        exit(OVERFLOW);
    A.constants[dim - 1] = 1;  // L=1，指针的增减以元素的大小为单位
    for (i = dim - 2; i >= 0; i--)  // 对应于93页顶上那个东西
        A.constants[i] = A.bounds[i + 1] * A.constants[i + 1];

    return OK;
}

/**
 * 销毁数组A
 */
Status DestroyArray(Array &A)
{
    if (!A.base)
        return ERROR;
    free(A.base);
    A.base = NULL;

    if (!A.bounds)
        return ERROR;
    free(A.bounds);
    A.bounds = NULL;

    if (!A.constants)
        return ERROR;
    free(A.constants);
    A.constants = NULL;

    return OK;
}

/**
 * 若ap指示的各下标值合法，则求出该元素在A中的相对地址off
 */
Status Locate(Array A, va_list ap, int &off)
{
    int i, ind;
    off = 0;
    for (i = 0; i < A.dim; i++) {
        ind = va_arg(ap, int);
        if (ind < 0 || ind >= A.bounds[i])
            return OVERFLOW;
        off += A.constants[i] * ind;
    }

    return OK;
}

/**
 * A是n维数组，e为元素变量，随后是n个下标值，若下标不越界，则e赋值为所指定的A的元素值，并返回OK
 */
Status Value(Array A, ElemType &e, ...)
{
    int result;
    int off;
    va_list ap;
    va_start(ap, e);
    result = Locate(A, ap, off);
    if (OVERFLOW == result)
        return ERROR;
    e = *(A.base + off);

    return OK;
}

/**
 * 若下标不越界，将e的值赋给所指定的A的元素，并返回OK
 */
Status Assign(Array &A, ElemType e, ...)
{
    int result, off;
    va_list ap;
    va_start(ap, e);
    result = Locate(A, ap, off);
    if (OVERFLOW == result)
        return ERROR;
    *(A.base + off) = e;

    return OK;
}

// TODO: 该函数还未完善
/**
 * 遍历一个数组的全部元素并输出
 */
void ArrayTraverse(const Array A, int dim, ...)
{
    va_list ap;
    int ele = 0;  // 保存数组的元素个数
    if (dim < 1 || dim > MAX_ARRAY_DIM)
        exit(ERROR);

    int *dimension = new int[dim + 1];  // dimension数组存放每一维的维数
    for (int i = 0; i < dim + 1; i++)
        dimension[i] = 0;
    va_start(ap, dim);  // ap为va_list类型，是存放变长参数信息的数组
    for (int i = 0; i < dim; i++) {
        dimension[i] = va_arg(ap, int);  // 读入一个变长参数
        if (A.bounds[i] < 0)
            exit(ERROR);
    }
    va_end(ap);

    // for()
}

int main(void)
{
    Array arr1;  // 定义一个数组arr1
    InitArray(arr1, 3, 3, 3, 3);

    // 为3*3*3维的数组赋值，依次赋为1~27
    for (int i = 0; i < 3; i++)
        for (int j = 0; j < 3; j++)
            for (int k = 0; k < 3; k++) {
                ElemType ele = i * 9 + j * 3 + k + 1;
                Assign(arr1, ele, i, j, k);
            }

    // 将数组中的每个元素的值依次输出
    for (int i = 0; i < 3; i++) {
        printf("第%d层元素为: \n", i + 1);
        for (int j = 0; j < 3; j++) {
            for (int k = 0; k < 3; k++) {
                ElemType ele = 0;
                Value(arr1, ele, i, j, k);
                printf("%d  ", ele);
            }
            printf("\n");
        }
        printf("\n");
    }

    return 0;
}