//
//  Testbed.c
//  monkcGame
//
//  Created by Sun YuLi on 16/1/10.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "Testbed.h"

static void testMethod(mc_message_arg(MCObject), MCChar arg1, MCInt arg2, MCGeneric arg3, MCLongLong arg4, MCPtr arg5, MCFuncPtr arg6)
//static void testMethod(mc_message_arg(MCObject), ...)
{
    char c = arg1;
    double f = arg3.mcdouble.d;
    
    
}

static void testclass()
{
    
    mc_message msg;
    msg.object = (MCObject*)new(MCObject);
    msg.address = MCFuncPtr(testMethod);
    _push_jump(msg, 'c', 1, 0.5879808908098, 4, 8, 16);
}

void starttest()
{
    testclass();
}
