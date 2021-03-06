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


#ifndef SCENE_HPP
#define SCENE_HPP

#include <string>
#include <set>
#include <boost/property_tree/ptree.hpp>
#include "commandobject.hpp"

class Entity;
class Component;
class ComponentFactory;
class Device;
class Renderer;
class PhysicsWorld;

class Scene: public CommandObject {
public:
    friend class Entity;

    Scene(const std::string& objectName,
          const std::string& rootNodeName,
          const ComponentFactory* componentFactory,
          const Device* device,
          Renderer* renderer,
          PhysicsWorld* physicsWorld);
    ~Scene();

    const Entity* getRoot() const;

    Entity* root();

    void saveToXML(const std::string& fileName) const;
    bool loadFromXML(const std::string& fileName);
    bool findEntity(const std::string& name, Entity*& entity);
    std::string sceneGraphToString();

protected:
    const ComponentFactory* m_componentFactory;
    const Device* m_device;
    Renderer* m_renderer;
    PhysicsWorld* m_physicsWorld;
    std::string m_rootName;
    Entity* m_root;
    static std::map<std::string, Entity*> ms_entities;

private:
    Scene(const Scene& rhs);
    Scene& operator=(const Scene&);

    void saveToPTree(const std::string& path,
                     boost::property_tree::ptree& tree,
                     const Entity* node) const;
    bool loadFromPTree(const std::string& path,
                       const boost::property_tree::ptree& tree,
                       Entity* node,
                       Entity* parent,
                       std::set<std::string>& names,
                       bool& isCameraFound);

    std::string cmdSaveXML(std::deque<std::string>& args);
    std::string cmdLoadXML(std::deque<std::string>& args);
};



inline const Entity* Scene::getRoot() const {
    return m_root;
}


inline Entity* Scene::root() {
    return m_root;
}



inline std::string Scene::cmdSaveXML(std::deque<std::string>& args) {
    if (args.size() < 1)
        return "Error: too few arguments";
    saveToXML(args[0]);
    return "";
}

inline std::string Scene::cmdLoadXML(std::deque<std::string>& args) {
    if (args.size() < 1)
        return "Error: too few arguments";
    loadFromXML(args[0]);
    return "";
}

#endif // SCENE_HPP
