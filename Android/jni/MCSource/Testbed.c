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
#include "MCSort.h"
#include "MCBST.h"
#include "MCHeap.h"

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
//    while (current != null) {
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

static void testBasics()
{
    debug_log("------------------\n");
    debug_log("sizeof(MCGeneric)=%ld\n", sizeof(MCGeneric));
    debug_log("sizeof(MCFloat)=%ld\n", sizeof(MCFloat));
    debug_log("sizeof(MCULongLong)=%ld\n", sizeof(MCULongLong));
    debug_log("sizeof(MCLongLong)=%ld\n", sizeof(MCLongLong));
    debug_log("sizeof(MCSizeT)=%ld\n", sizeof(MCSizeT));
    debug_log("sizeof(MCULong)=%ld\n", sizeof(MCULong));
    debug_log("sizeof(MCLong)=%ld\n", sizeof(MCLong));
    debug_log("sizeof(MCVoidPtr)=%ld\n", sizeof(MCVoidPtr));
    debug_log("sizeof(MCFuncPtr)=%ld\n", sizeof(MCFuncPtr));
    debug_log("sizeof(MCObject*)=%ld\n", sizeof(MCObject*));
    debug_log("sizeof(MCUInt)=%ld\n", sizeof(MCUInt));
    debug_log("sizeof(MCInt)=%ld\n", sizeof(MCInt));
    debug_log("sizeof(MCHash)=%ld\n", sizeof(MCHash));
    debug_log("sizeof(MCBool)=%ld\n", sizeof(MCBool));
    debug_log("------------------\n");
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
    
    printf("p1b=%d p2b=%d bool=%d A==B(%d)\n", p1b, p2b, testMCBool(5,5), MCVector3Equal(a, b));
    
    MCVector3 data[] =
        {
            {-1.380090,0.574324,-1.020060},
            {-1.365010,0.315159,-1.028980},
            {-1.384510,0.307746,-1.027600},
            {-1.384780,0.314364,-1.027590},
            {-1.373570,0.528954,-1.027800},
        };
    
    MCPolygon Poly = {};
    MCPolygonInit(&Poly, data, 5);
    
    MCTriangle tresult[3];
    size_t iresult[5];
    size_t count = MCPolygonResolveConcave(&Poly, tresult, iresult);
    
    exit(-1);
}

static void testsort()
{
    MCLogTypeSet(MC_DEBUG);

    MCGeneric A[] = {12.1,20.0,13.6,29.7,23.4,-15.4,22.4,35.2,40.4,26.8,-51.3,19.0,17.3,111,234};
    size_t acount = sizeof(A) / sizeof(MCGeneric);
    
    MCGeneric B[] = {5,6,-3,4,2,1,8,9,-7,10,11,32,90,64,-23,22,14,19,38,24,55,17};
    size_t bcount = sizeof(B) / sizeof(MCGeneric);
    
    MCSort* sort = ff(new(MCSort), initWithArray, A, acount);
    ff(sort, quickSort, 0);
    ff(sort, printArray, 0);
    release(sort);
    
    MCBST* bstree = new(MCBST);
    for (int i=0; i<bcount; i++) {
        ff(bstree, insertValue, B[i]);
    }
    ff(bstree, printTree, 0);
    release(bstree);
    debug_log("testsort finish\n");
    
    MCGeneric heapvals[] = {5,6,3,4,2,1,8,9,7,10,11,32,90,64,23,22,14,19,38,24,55,17};
    
    int hcount = sizeof(heapvals)/sizeof(MCGeneric);
    MCHeap* hp = ff(new(MCHeap), initWithMaxcount, hcount);
    for (int i=0; i<hcount; i++) {
        ff(hp, insertValue, heapvals[i]);
    }
    
    ff(hp, printAll, 0);
    
    MCArray* ha = MCHeap_copySortAscend(0, hp, 0);
    ff(ha, printAll, "/");
    
    
    release(hp);
    
    exit(-1);
}

static void testhash()
{
    printf("this_test(%d)\n", hash("this_test"));
    exit(-1);
}

void starttest()
{
    //testBasics();
    //testclass();
    //testparser();
    //testCArrayLinkedList();
    //testGeometry();
    //testhash();
    //testsort();
}
