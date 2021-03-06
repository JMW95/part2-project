#include "Model.h"

#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>
#include <string>

static Vector3 face_normal_from_vertices(Vector4 &a, Vector4 &b, Vector4 &c){
    auto v1 = b - a;
    auto v2 = c - a;
    return Vector3(v1).cross(Vector3(v2));
}

Face::Face(Vector4 a, Vector4 b, Vector4 c, unsigned char cid){
    vertices[0] = a;
    vertices[1] = b;
    vertices[2] = c;
    
    color_index = cid;
    
    normal = face_normal_from_vertices(a, b, c);
}

Model::Model(){
}

Model::Model(std::string filename, const std::map<std::string, unsigned char> texturemap){
    
    colormap = std::vector<char>(256);
    int colour_idx = 0;
    
    std::cout << "Loading model from " << filename << std::endl;
    
    std::ifstream file(filename);
    
    std::string line;
    while(std::getline(file, line)){
        std::istringstream iss(line);
        
        if(line.empty()) continue;
        if (line.at(0) == '#') continue;
        
        std::string item;
        if (std::getline(iss, item, ' ')) {
            if (item.compare("v") == 0){
                float a, b, c;
                iss >> a >> b >> c;
                vertices.push_back(Vector4(a, b, c, 1.0f));
            }else if(item.compare("f") == 0){
                std::vector<Vector4> face;
                std::string part;
                while(std::getline(iss, part, ' ')){ // read until the space
                    std::istringstream partss(part);
                    int idx;
                    partss >> idx;
                    face.push_back(vertices[idx - 1]);
                }
                for(unsigned int i=0; i<face.size()-2; i++){
                    faces.push_back(Face(face[0], face[i+1], face[i+2], colour_idx));
                }
            }else if(item.compare("vn") == 0){
                // TODO read vertex normals?
            }else if(item.compare("usemtl") == 0){
                std::string mtlname;
                iss >> mtlname;
                if(texturemap.count(mtlname) > 0){
                    colour_idx = texturemap.at(mtlname);
                    std::cout << "switching to " << mtlname << " as " << colour_idx << std::endl;
                }
            }
        }
    }
    
    file.close();
    
    std::cout << vertices.size() << " vertices, " << faces.size() << " faces" << std::endl;
}

// Counter-clockwise vertex ordering
void Model::_face(int a, int b, int c, unsigned char cid){
    faces.push_back(Face(vertices[a], vertices[b], vertices[c], cid));
}
