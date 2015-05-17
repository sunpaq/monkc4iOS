//
//  MC3DiOSDriver.h
//  monkcGame
//
//  Created by SunYuLi on 5/18/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//


#ifndef __monkcGame__MC3DiOSDriver__
#define __monkcGame__MC3DiOSDriver__

#include <stdio.h>
#include "monkc.h"
#include "MC3DType.h"

#define MCMatrix4ToGLKMatrix4(mat) (GLKMatrix4){(mat).m00, (mat).m01, (mat).m02, (mat).m03, \
                                                (mat).m10, (mat).m11, (mat).m12, (mat).m13, \
                                                (mat).m20, (mat).m21, (mat).m22, (mat).m23, \
                                                (mat).m30, (mat).m31, (mat).m32, (mat).m33}

#define MCMatrix4FromGLKMatrix4(mat) (MCMatrix4){(mat).m00, (mat).m01, (mat).m02, (mat).m03, \
                                                 (mat).m10, (mat).m11, (mat).m12, (mat).m13, \
                                                 (mat).m20, (mat).m21, (mat).m22, (mat).m23, \
                                                 (mat).m30, (mat).m31, (mat).m32, (mat).m33}

MCMatrix4 MCMatrix4Multiply(MCMatrix4 matrixLeft, MCMatrix4 matrixRight);

#endif /* defined(__monkcGame__MC3DiOSDriver__) */
