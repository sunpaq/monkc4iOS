//
//  BATrianglization.c
//  Sapindus
//
//  Created by Sun YuLi on 2016/10/11.
//  Copyright © 2016年 oreisoft. All rights reserved.
//

#include "BATrianglization.h"

size_t getTrianglesBuffSize(BAFace* faces, size_t facecounts)
{
    size_t trisize = 0;
    for (size_t i=0; i<facecounts; i++) {
        BAFace face = faces[i];
        if (face.vcount > 0) {
            size_t vertexCount = 0;
            vertexCount += face.vcount;
            if (vertexCount < 3) {
                error_log("BATrianglization - vertexCount < 3\n");
            }
            else if (vertexCount == 3) {
                trisize++;
            }
            else if (vertexCount > 3) {
                trisize += vertexCount - 2;
            }
        }else{
            error_log("BATrianglization - face vertex count is 0\n");
        }
    }
    return trisize;
}

//return face count;
size_t trianglization(BATriangle* triangles, BAFace* faces, size_t facecounts, MCVector3* vertexbuff)
{
    size_t tricounts = 0;
    for (int i=0; i<facecounts; i++) {
        BAFace* face = &faces[i];
        if (face->vcount < 9) {
            error_log("BAObjParser - v/n/t group less than 3\n");
            exit(-1);
        }
        else if (face->vcount == 9) {
            triangles[tricounts++] = (BATriangle) {
                face->data[0], face->data[1], face->data[2],
                face->data[3], face->data[4], face->data[5],
                face->data[6], face->data[7], face->data[8]
            };
        }
        else if (face->vcount > 9) {
            int count = (int)(face->vcount / 3);
            MCVector3  mcvertexes[LINE_MAX];
            MCTriangle mctriangles[LINE_MAX-2];
            
            for (int i=0; i<count; i++) {
                mcvertexes[i] = vertexbuff[face->data[i*3]];
            }
            
            MCPolygon Poly = {};
            MCPolygonInit(&Poly, mcvertexes, count);
            
            if (Poly.isConvex) {
                //debug_log("CONVEX poly: %s\n", linebuff);
                for (int i=0; i< face->vcount-6; i=i+3) {
                    //face
                    triangles[tricounts] = (BATriangle){
                        face->data[0  ], face->data[1  ], face->data[2  ],
                        face->data[i+3], face->data[i+4], face->data[i+5],
                        face->data[i+6], face->data[i+7], face->data[i+8]
                    };
                    tricounts++;
                }
            }else{
                size_t epcount = 0;
                size_t viresult[LINE_MAX];
                size_t mctricount = MCPolygonResolveConcave(&Poly, mctriangles, viresult);
                if (mctricount == 0) {
                    //error_log("poly: %s\n", linebuff);
                    MCPolygonDumpVertexData(&Poly);
                    error_log("error poly %d\n", epcount++);
                }else{
                    //debug_log("poly: %s\n", linebuff);
                }
                
                for (int i=0; i<mctricount; i++) {
                    
                    size_t vi1 = viresult[i*3+0];
                    size_t vi2 = viresult[i*3+1];
                    size_t vi3 = viresult[i*3+2];
                    
                    long v1 = face->data[vi1*3+0];
                    long t1 = face->data[vi1*3+1];
                    long n1 = face->data[vi1*3+2];
                    
                    long v2 = face->data[vi2*3+0];
                    long t2 = face->data[vi2*3+1];
                    long n2 = face->data[vi2*3+2];
                    
                    long v3 = face->data[vi3*3+0];
                    long t3 = face->data[vi3*3+1];
                    long n3 = face->data[vi3*3+2];
                    
                    triangles[tricounts] = (BATriangle){
                        v1, t1, n1,
                        v2, t2, n2,
                        v3, t3, n3
                    };
                    tricounts++;
                }
            }
        }
    }
    return tricounts;
}
