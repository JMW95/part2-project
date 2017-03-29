#include <algorithm>

#include "Util.h"

static Vector3 computeVertIntersection(Vector3 p1, Vector3 p2, float xc){
    float x0 = p1.vals[0];
    float y0 = p1.vals[1];
    float z0 = p1.vals[2];
    float x1 = p2.vals[0];
    float y1 = p2.vals[1];
    float z1 = p2.vals[2];
    Vector3 tmp;
    tmp.vals[0] = xc;
    tmp.vals[1] = y0 + (xc - x0)*((y1-y0)/(x1-x0));
    tmp.vals[2] = z0 + (xc - x0)*((z1-z0)/(x1-x0));
    return tmp;
}

static Vector3 computeHorizIntersection(Vector3 p1, Vector3 p2, float yc){
    float x0 = p1.vals[0];
    float y0 = p1.vals[1];
    float z0 = p1.vals[2];
    float x1 = p2.vals[0];
    float y1 = p2.vals[1];
    float z1 = p2.vals[2];
    Vector3 tmp;
    tmp.vals[0] = x0 + (yc - y0)*((x1-x0)/(y1-y0));
    tmp.vals[1] = yc;
    tmp.vals[2] = z0 + (yc - y0)*((z1-z0)/(y1-y0));
    return tmp;
}

static void clipVert(std::vector<Vector3> &outputList, float xc, bool lessIsIn){
    if(outputList.size() < 3) return;
    std::vector<Vector3> inputList = outputList;
    outputList.clear();
    Vector3 S = inputList.back();
    for(auto E = inputList.begin(); E < inputList.end(); ++E){
        if((lessIsIn && E->vals[0] < xc) || (!lessIsIn && E->vals[0] >= xc)){
            if((lessIsIn && S.vals[0] >= xc) || (!lessIsIn && S.vals[0] < xc)){
                outputList.push_back(computeVertIntersection(S, *E, xc));
            }
            outputList.push_back(*E);
        }else if((lessIsIn && S.vals[0] < xc) || (!lessIsIn && S.vals[0] >= xc)){
            outputList.push_back(computeVertIntersection(S, *E, xc));
        }
        S = *E;
    }
}

static void clipHoriz(std::vector<Vector3> &outputList, float yc, bool lessIsIn){
    if(outputList.size() < 3) return;
    std::vector<Vector3> inputList = outputList;
    outputList.clear();
    Vector3 S = inputList.back();
    for(auto E = inputList.begin(); E < inputList.end(); ++E){
        if((lessIsIn && E->vals[1] < yc) || (!lessIsIn && E->vals[1] >= yc)){
            if((lessIsIn && S.vals[1] >= yc) || (!lessIsIn && S.vals[1] < yc)){
                outputList.push_back(computeHorizIntersection(S, *E, yc));
            }
            outputList.push_back(*E);
        }else if((lessIsIn && S.vals[1] < yc) || (!lessIsIn && S.vals[1] >= yc)){
            outputList.push_back(computeHorizIntersection(S, *E, yc));
        }
        S = *E;
    }
}

void Util::transform(const Model &m, const Matrix4 &modelView, const Matrix4& proj,
                std::vector<Triangle2D> &renderfaces){
    Vector3 camnormal(0,0,1);
    Triangle2D tri;
    auto T = proj * modelView;
    auto NMV = modelView.topleft().inverse().transpose();
    
    for(auto it = m.faces.begin(); it < m.faces.end(); ++it){
        auto f = Face();
        
        f.normal = (NMV * (*it).normal).normalise();
        auto col = -camnormal.dot(f.normal);
        if(col <= 0) continue; // Back-face culling
        
        bool valid = true;
        
        float minx = 1e10;
        float maxx = -1e10;
        float miny = 1e10;
        float maxy = -1e10;
        
        // Transform each vertex of this face
        for(int i=0; i<3; i++){
            auto v = T * (*it).vertices[i];
            v.vals[0] /= v.vals[3];
            v.vals[1] /= v.vals[3];
            v.vals[2] /= v.vals[3];
            f.vertices[i] = v;
            if(v.vals[2] < 0){
                valid = false;
                break;
            }
            
            // Bounding box
            if(v.vals[0] < minx) minx = v.vals[0];
            if(v.vals[0] > maxx) maxx = v.vals[0];
            if(v.vals[1] < miny) miny = v.vals[1];
            if(v.vals[1] > maxy) maxy = v.vals[1];
        }
        
        if(!valid) continue;
        
        // Check bounding box and only clip if necessary
        if (minx > 1 || maxx < -1 || miny > 1 || maxy < -1){
            continue;
        }
        
        // Input -> triangle to be clipped
        // Output -> list of vertices in clipped polygon
        std::vector<Vector3> outputList;
        outputList.push_back(f.vertices[0]);
        outputList.push_back(f.vertices[1]);
        outputList.push_back(f.vertices[2]);
        
        if (minx >= -1 && maxx <= 1 && miny >= -1 && maxy <= 1){
            // Don't need to clip!
        }else{
            // left edge
            clipVert(outputList, -1, false);
            // top edge
            clipHoriz(outputList, -1, false);
            // right edge
            clipVert(outputList, 1, true);
            // bottom edge
            clipHoriz(outputList, 1, true);
        }
        
        if(outputList.size() >= 3){ // At least 1 triangle to draw
            auto v0 = outputList[0];
            for(unsigned int i=0; i<outputList.size()-2; i++){
                auto v1 = outputList[i+1];
                auto v2 = outputList[i+2];
                
                tri.points[0].x = 479 * ((v0.vals[0] + 1) / 2);
                tri.points[0].y = 271 * ((v0.vals[1] + 1) / 2);
                tri.depths[0] = v0.vals[2];
                
                tri.points[1].x = 479 * ((v1.vals[0] + 1) / 2);
                tri.points[1].y = 271 * ((v1.vals[1] + 1) / 2);
                tri.depths[1] = v1.vals[2];
                
                tri.points[2].x = 479 * ((v2.vals[0] + 1) / 2);
                tri.points[2].y = 271 * ((v2.vals[1] + 1) / 2);
                tri.depths[2] = v2.vals[2];
                
                tri.color = m.color*32 + ((col*32) < 31 ? (col*32) : 31);
                
                float mindepth = 1e10;
                if(v0.vals[2] <= v1.vals[2] && v0.vals[2] <= v2.vals[2]){
                    mindepth = v0.vals[2];
                }
                if(v1.vals[2] <= v0.vals[2] && v1.vals[2] <= v2.vals[2]){
                    mindepth = v1.vals[2];
                }
                if(v2.vals[2] <= v0.vals[2] && v2.vals[2] <= v1.vals[2]){
                    mindepth = v2.vals[2];
                }
                
                tri.mindepth = mindepth;
                
                renderfaces.push_back(tri);
            }
        }
    }
}

static bool trizcomp(Triangle2D a, Triangle2D b){
    return a.mindepth < b.mindepth;
}

void Util::sort_triangles(std::vector<Triangle2D> &renderfaces){
    std::sort(renderfaces.begin(), renderfaces.end(), trizcomp);
}

float Util::deg2rad(float degrees){
    return degrees * 3.14159 / 180.0;
}
