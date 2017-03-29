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

static Vector4 computeZIntersection(Vector4 p1, Vector4 p2, float zc){
    float x0 = p1.vals[0];
    float y0 = p1.vals[1];
    float z0 = p1.vals[2];
    float x1 = p2.vals[0];
    float y1 = p2.vals[1];
    float z1 = p2.vals[2];
    Vector4 tmp;
    tmp.vals[0] = x0 + (zc - z0)*((x1-x0)/(z1-z0));
    tmp.vals[1] = y0 + (zc - z0)*((y1-y0)/(z1-z0));
    tmp.vals[2] = zc;
    tmp.vals[3] = 1;
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

static void clipZ(std::vector<Vector4> &outputList, float zc){
    if(outputList.size() < 3) return;
    std::vector<Vector4> inputList = outputList;
    outputList.clear();
    Vector4 S = inputList.back();
    for(auto E = inputList.begin(); E < inputList.end(); ++E){
        if(E->vals[2] >= zc){
            if(S.vals[2] < zc){
                outputList.push_back(computeZIntersection(S, *E, zc));
            }
            outputList.push_back(*E);
        }else if(S.vals[2] >= zc){
            outputList.push_back(computeZIntersection(S, *E, zc));
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
        std::vector<Vector4> transformed; // Up to 4 vectors, if z-clipped
        
        auto facenormal = (NMV * (*it).normal).normalise();
        
        // View vector to the triangle
        auto view4 = modelView * (*it).vertices[0];
        Vector3 view(view4.vals[0] / view4.vals[3], view4.vals[1] / view4.vals[3], view4.vals[2] / view4.vals[3]);
        
        auto viewdot = -view.dot(facenormal);
        if(viewdot <= 0) continue; // Back-face culling
        
        bool valid = false;
        bool needszclip = false;
        
        float minx = 1e10;
        float maxx = -1e10;
        float miny = 1e10;
        float maxy = -1e10;
        
        // Transform each vertex of this face
        for(int i=0; i<3; i++){
            auto v = T * (*it).vertices[i];
            
            // In front of the camera
            if(v.vals[3] < 0){
                valid = true;
            }
            // Behind camera
            if (v.vals[3] > 0){
                needszclip = true;
            }
            
            v.vals[0] /= v.vals[3];
            v.vals[1] /= v.vals[3];
            v.vals[2] /= v.vals[3];
            transformed.push_back(v);
            
            // Bounding box
            if(v.vals[0] < minx) minx = v.vals[0];
            if(v.vals[0] > maxx) maxx = v.vals[0];
            if(v.vals[1] < miny) miny = v.vals[1];
            if(v.vals[1] > maxy) maxy = v.vals[1];
        }
        
        if(!valid) continue;
        
        // Triangle leaves viewing frustum
        if(needszclip){
            // Repeat the transformation, but clip against near z-plane before projecting
            minx = 1e10;
            maxx = -1e10;
            miny = 1e10;
            maxy = -1e10;
            
            // Clipping is done in view-space
            for(int i=0; i<3; i++){
                auto v = modelView * (*it).vertices[i];
                
                v.vals[0] /= v.vals[3];
                v.vals[1] /= v.vals[3];
                v.vals[2] /= v.vals[3];
                transformed[i] = v;
            }
            
            // Clip against the z=1 plane in viewing-space
            clipZ(transformed, 1);
            
            // Perspective-project the clipped polygon
            for(unsigned int i=0; i<transformed.size(); i++){
                auto v = proj * transformed[i];
                
                v.vals[0] /= v.vals[3];
                v.vals[1] /= v.vals[3];
                v.vals[2] /= v.vals[3];
                
                transformed[i] = v;
                
                // Bounding box
                if(v.vals[0] < minx) minx = v.vals[0];
                if(v.vals[0] > maxx) maxx = v.vals[0];
                if(v.vals[1] < miny) miny = v.vals[1];
                if(v.vals[1] > maxy) maxy = v.vals[1];
            }
        }
        
        // Check bounding box and only clip if necessary
        if (minx > 1 || maxx < -1 || miny > 1 || maxy < -1){
            continue;
        }
        
        // Input -> polygon to be clipped
        // Output -> list of vertices in clipped polygon
        std::vector<Vector3> outputList;
        for(unsigned int i=0; i<transformed.size(); i++){
            Vector4 t = transformed[i];
            outputList.push_back(Vector3(t.vals[0], t.vals[1], t.vals[2]));
        }
        
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
        
        // Shading
        auto camdot = -camnormal.dot(facenormal);
        if (camdot < 0) camdot = 0;
        float shade = 0.3 + 0.7 * (camdot);
        int col = shade * 31;
        
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
                
                tri.color = m.color*32 + col;
                
                float maxdepth = -1e10;
                if(v0.vals[2] <= v1.vals[2] && v0.vals[2] <= v2.vals[2]){
                    maxdepth = v0.vals[2];
                }
                if(v1.vals[2] <= v0.vals[2] && v1.vals[2] <= v2.vals[2]){
                    maxdepth = v1.vals[2];
                }
                if(v2.vals[2] <= v0.vals[2] && v2.vals[2] <= v1.vals[2]){
                    maxdepth = v2.vals[2];
                }
                
                tri.maxdepth = maxdepth;
                
                renderfaces.push_back(tri);
            }
        }
    }
}

static bool trizcomp(Triangle2D a, Triangle2D b){
    return a.maxdepth < b.maxdepth;
}

void Util::sort_triangles(std::vector<Triangle2D> &renderfaces){
    std::sort(renderfaces.begin(), renderfaces.end(), trizcomp);
}

float Util::deg2rad(float degrees){
    return degrees * 3.14159 / 180.0;
}
