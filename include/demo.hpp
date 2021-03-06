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


#ifndef GAME_HPP
#define GAME_HPP

#include "shoggoth-engine/kernel/device.hpp"
#include "shoggoth-engine/kernel/scene.hpp"
#include "shoggoth-engine/renderer/renderer.hpp"
#include "shoggoth-engine/physics/physicsworld.hpp"
#include "testcomponentfactory.hpp"

class Demo: public CommandObject {
public:
    Demo(const std::string& objectName,
         const std::string& deviceName,
         const std::string& rendererName,
         const std::string& physicsWorldName,
         const std::string& sceneName,
         const std::string& rootNodeName);
    ~Demo();

    void loadScene();
    void bindInputs();
    void runMainLoop();

private:
    bool m_isRunning;
    Device m_device;
    Renderer m_renderer;
    PhysicsWorld m_physicsWorld;
    TestComponentFactory m_componentFactory;
    Scene m_scene;

    std::string cmdQuit(std::deque<std::string>&);
    std::string cmdRunCommand(std::deque<std::string>& args);
    std::string cmdPrint(std::deque<std::string>& args);
    std::string cmdOnMouseMotion(std::deque<std::string>&);
    std::string cmdFireCube(std::deque<std::string>&);
    std::string cmdFireModel(std::deque<std::string>&);
};

#endif // GAME_HPP
