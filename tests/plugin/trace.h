#ifndef __TRACE_H__
#define __TRACE_H__
#include <string.h>
#include "cpregs.h"

typedef enum RegType {
    INT_REG, 
    FP_REG, 
    SYS_REG,
    UNKOWN
} RegType;

typedef struct DstRegInfo{
    RegType type;
    union reg_info{
        typedef sys_reg{
            std::string reg_name;
            bool isread;
            int rt;
            int op1;
            int crn;
            int crm;
            int op2;
            int op0;
        }sys_reg;
        int rd;
    };
} DstRegInfo;


#endif