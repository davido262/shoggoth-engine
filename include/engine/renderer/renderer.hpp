/*
    <one line to give the library's name and an idea of what it does.>
    Copyright (C) 2012  David Cavazos <davido262@gmail.com>

    This library is free software; you can redistribute it and/or
    modify it under the terms of the GNU Lesser General Public
    License as published by the Free Software Foundation; either
    version 2.1 of the License, or (at your option) any later version.

    This library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
    Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public
    License along with this library; if not, write to the Free Software
    Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/


#ifndef RENDERER_HPP
#define RENDERER_HPP

#include <string>
#include <set>

class Vector3;
class Quaternion;
class RenderManager;
class Camera;
class Light;
class RenderableMesh;
class Mesh;
class Texture;

class Renderer {
public:
    friend class RenderManager;
    friend class Camera;
    friend class Light;
    friend class RenderableMesh;

    void setAmbientLight(const float r, const float g, const float b, const float a = 1.0f);
    void initLighting() const;
    void uploadModel(unsigned int& meshId, unsigned int& indicesId, const Mesh& mesh);
    void deleteModel(const unsigned int meshId, const unsigned int indicesId);
    void uploadTexture(unsigned int& textureId, const Texture& texture);
    void deleteTexture(const unsigned int textureId);
    void draw() const;
    std::string listsToString() const;

private:
    Renderer();
    Renderer(const Renderer& rhs);
    Renderer& operator=(const Renderer& rhs);

    void initialize();
    void deinitialize();
    void initCamera() const;
    void displayLegacyLights() const;
    void setOpenGLMatrix(float* const m, const Vector3& pos, const Quaternion& rot) const;

    Camera* m_activeCamera;
    std::set<Camera*> m_cameras;
    std::set<Light*> m_lights;
    std::set<RenderableMesh*> m_model;
};

#endif // RENDERER_HPP