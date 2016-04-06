//
//  Testbed.c
//  monkcGame
//
//  Created by Sun YuLi on 16/1/10.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "Testbed.h"

static void test(mc_message_arg(MCObject), MCChar arg1, MCInt arg2, MCGeneric arg3, MCLongLong arg4, MCPtr arg5, MCFuncPtr arg6)
//static void testpublic(mc_message_arg(MCObject), ...)
{
    char c = arg1;
    double f = arg3.mcdouble.d;
    
    
}

static void testclass()
{
    
    mc_message msg;
    msg.object = (MCObject*)new(MCObject);
    msg.address = MCFuncPtr(test);
    _push_jump(msg, 'c', 1, 0.5879808908098, 4, 8, 16);
}

//static void testparser()
//{
//    const char* str = "v 123 345";
//    char buff[256];
//    char* w = getWord(str, buff);
//    
//}

void starttest()
{
    //testclass();
    //testparser();
}
