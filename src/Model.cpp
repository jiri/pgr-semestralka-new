#include "model.h"

#include <fmt/format.h>

#include <boost/filesystem.hpp>
using namespace boost::filesystem;

#include <tiny_obj_loader.h>

namespace {
    tuple<vector<GLfloat>, vector<GLuint>, material> loadFromFile(path p) {
        tinyobj::attrib_t attrib;
        vector<tinyobj::shape_t> shapes;
        vector<tinyobj::material_t> materials;
        string err;

        auto mtl_base = p.parent_path() / "/";
        bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, p.c_str(), mtl_base.c_str());

        if (!err.empty()) {
            cerr << err << endl;
        }

        if (!ret) {
            throw "Failed to open file";
        }

        vector<GLfloat> vertices;
        vector<GLuint> indices;

        for (const auto &shape : shapes) {
            for (const auto &index : shape.mesh.indices) {
                vertices.push_back(attrib.vertices[3 * index.vertex_index + 0]);
                vertices.push_back(attrib.vertices[3 * index.vertex_index + 1]);
                vertices.push_back(attrib.vertices[3 * index.vertex_index + 2]);

                if (!attrib.normals.empty()) {
                    vertices.push_back(attrib.normals[3 * index.normal_index + 0]);
                    vertices.push_back(attrib.normals[3 * index.normal_index + 1]);
                    vertices.push_back(attrib.normals[3 * index.normal_index + 2]);
                } else {
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                }

                if (!attrib.texcoords.empty()) {
                    vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 0]);
                    vertices.push_back(attrib.texcoords[2 * index.texcoord_index + 1]);
                } else {
                    vertices.push_back(0.0f);
                    vertices.push_back(0.0f);
                }

                indices.push_back(indices.size());
            }

            if (attrib.texcoords.empty()) {

            }

            if (attrib.vertices.empty()) {
                fmt::print(stderr, "Warning: Model doesn't contain any vertices\n");
            }

            if (attrib.normals.empty()) {
                fmt::print(stderr, "Warning: Model doesn't contain any normals\n");
            }

            if (attrib.texcoords.empty()) {
                fmt::print(stderr, "Warning: Model doesn't contain any texCoords\n");
            }
        }

        return { move(vertices), move(indices), shadeless { Texture() } };
    };
}

model::model(boost::filesystem::path p)
    : model { loadFromFile(p) }
{ }

model::model(tuple<vector<GLfloat>, vector<GLuint>, material> t)
    : g { get<0>(t), get<1>(t) }
    , m { get<2>(t) }
{ }
