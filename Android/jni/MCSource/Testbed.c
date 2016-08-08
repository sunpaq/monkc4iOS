//
//  Testbed.c
//  monkcGame
//
//  Created by Sun YuLi on 16/1/10.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "Testbed.h"
#include "MCLinkedList.h"
#include "MCGeometry.h"

//static void test(mc_message_arg(MCObject), MCChar arg1, MCInt arg2, MCGeneric arg3, MCLongLong arg4, MCPtr arg5, MCFuncPtr arg6)
//static void testmethod(mc_message_arg(MCObject), ...)
//{
//    char c = arg1;
//    double f = arg3.mcdouble.d;
//    
//    
//}
//
//static void testclass()
//{
//    
//    mc_message msg;
//    msg.object = (MCObject*)new(MCObject);
//    msg.address = MCFuncPtr(test);
//    _push_jump(msg, 'c', 1, 0.5879808908098, 4, 8, 16);
//}

//static void testparser()
//{
//    const char* str = "v 123 345";
//    char buff[256];
//    char* w = getWord(str, buff);
//    
//}

//static void testCArrayLinkedList()
//{
//    CArrayLinkedListItem array[5];
//    long values[] = {123,456,789,111,222};
//    CArrayLinkedListItem* head = initCArrayLinkedList(array, values, 5);
//
//    head = deleteCArrayLinkedListItem(array, &array[0]);
//    head = deleteCArrayLinkedListItem(array, &array[1]);
//    head = deleteCArrayLinkedListItem(array, &array[4]);
//
//    CArrayLinkedListItem* current = head;
//    while (current != mull) {
//        printf("current item value: %ld\n", current->value);
//        current = nextCArrayLinkedListItem(array, current);
//    }
//    
//    printf("test end\n");
//    exit(-1);
//}

MCBool testMCBool(int a, int b)
{
    return (a==b);
}

static void testGeometry()
{
    MCVector3 a, b, c, p1, p2;
    
    a = (MCVector3){0,0,0};
    b = (MCVector3){0,2,0};
    c = (MCVector3){2,0,0};
    p1 = (MCVector3){0.5,0.5,0};
    p2 = (MCVector3){1.5,1.5,0};

    MCBool p1b = MCTriangleContainsVertex(MCTriangleMake(a, b, c), p1);
    MCBool p2b = MCTriangleContainsVertex(MCTriangleMake(a, b, c), p2);
    
    printf("p1b=%d p2b=%d bool=%d\n A==B(%d)\n", p1b, p2b, testMCBool(5,5), MCVector3Equal(a, b));
    exit(-1);
}

void starttest()
{
    //testclass();
    //testparser();
    //testCArrayLinkedList();
    testGeometry();
}
