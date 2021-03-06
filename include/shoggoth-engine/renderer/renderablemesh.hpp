/*
 *    Copyright (c) 2012 David Cavazos <davido262@gmail.com>
 *
 *    Permission is hereby granted, free of charge, to any person
 *    obtaining a copy of this software and associated documentation
 *    files (the "Software"), to deal in the Software without
 *    restriction, including without limitation the rights to use,
 *    copy, modify, merge, publish, distribute, sublicense, and/or sell
 *    copies of the Software, and to permit persons to whom the
 *    Software is furnished to do so, subject to the following
 *    conditions:
 *
 *    The above copyright notice and this permission notice shall be
 *    included in all copies or substantial portions of the Software.
 *
 *    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 *    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
 *    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 *    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
 *    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
 *    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 *    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 *    OTHER DEALINGS IN THE SOFTWARE.
 */


#ifndef RENDERABLEMESH_HPP
#define RENDERABLEMESH_HPP

#include <vector>
#include "shoggoth-engine/kernel/component.hpp"

const std::string COMPONENT_RENDERABLEMESH = "renderablemesh";

const std::string RENDERABLEMESH_BOX_DESCRIPTION = "$box";
const std::string RENDERABLEMESH_FILE_DESCRIPTION = "$file";

class Renderer;
class Model;
class Material;

class RenderableMesh: public Component {
public:
    RenderableMesh(Entity* const _entity, Renderer* renderer);
    ~RenderableMesh();

    const Model* getModel() const;
    const Material* getMaterial(const size_t meshIndex) const;
    Model* model();

    void loadBox(const double lengthX, const double lengthY, const double lengthZ);
    void loadFromFile(const std::string& fileName);
    void assignMaterial(const size_t meshIndex, const std::string& fileName);

    void loadFromPtree(const std::string& path, const boost::property_tree::ptree& tree);
    void saveToPtree(const std::string& path, boost::property_tree::ptree& tree) const;

private:
    Renderer* m_renderer;
    Model* m_model;
    std::vector<Material*> m_materials;

    RenderableMesh(const RenderableMesh& rhs);
    RenderableMesh& operator=(const RenderableMesh&);

    std::string cmdLoadModelBox(std::deque<std::string>& args);
    std::string cmdLoadModelFile(std::deque<std::string>& args);
};



inline const Model* RenderableMesh::getModel() const {
    return m_model;
}

inline const Material* RenderableMesh::getMaterial(const size_t meshIndex) const {
    return m_materials[meshIndex];
}

inline Model* RenderableMesh::model() {
    return m_model;
}



inline std::string RenderableMesh::cmdLoadModelFile(std::deque<std::string>& args) {
    if (args.size() < 1)
        return "Error: too few arguments";
    loadFromFile(args[0]);
    return "";
}

#endif // RENDERABLEMESH_HPP
