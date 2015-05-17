//
//  Shader.fsh
//  monkcGame
//
//  Created by SunYuLi on 5/15/15.
//  Copyright (c) 2015 oreisoft. All rights reserved.
//

varying lowp vec4 colorVarying;

void main()
{
    gl_FragColor = colorVarying;
}
