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


#include "engine/renderer/renderer.hpp"

#include <iostream>
#include <GL/glew.h>
#include "engine/kernel/device.hpp"
#include "engine/kernel/devicemanager.hpp"
#include "engine/kernel/entity.hpp"
#include "engine/kernel/matrix3x3.hpp"
#include "engine/renderer/rendermanager.hpp"
#include "engine/renderer/camera.hpp"
#include "engine/renderer/renderablemesh.hpp"
#include "engine/renderer/light.hpp"
#include "engine/resources/model.hpp"
#include "engine/resources/texture.hpp"

using namespace std;

typedef enum {
    DATA_UPLOAD_VERTEX_ARRAY, // 1.1
    DATA_UPLOAD_VERTEX_BUFFER_OBJECT_EXT, // extension
    DATA_UPLOAD_VERTEX_BUFFER_OBJECT // 1.5
} data_upload_t;

typedef enum {
    MIPMAP_GENERATION_GLU, // 1.0
    MIPMAP_GENERATION_TEX_PARAMETER_EXT, // extension
    MIPMAP_GENERATION_TEX_PARAMETER // 1.4
} mipmap_generation_t;

data_upload_t gDataUploadMode = DATA_UPLOAD_VERTEX_ARRAY;
mipmap_generation_t gMipMapGenerationMode = MIPMAP_GENERATION_GLU;


void Renderer::setAmbientLight(const float r, const float g, const float b, const float a) {
    GLfloat global_ambient[] = {r, g, b, a};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, global_ambient);
}

void Renderer::initLighting() const {
    // enable lighting for legacy lights
//     glLightModeli(GL_LIGHT_MODEL_COLOR_CONTROL, GL_SEPARATE_SPECULAR_COLOR); // 1.4

    set<Light*>::const_iterator it = m_lights.begin();
    for (size_t i = 0; i < m_lights.size(); ++i) {
        GLenum lightEnum;
        switch (i) {
        case 0:
            lightEnum = GL_LIGHT0;
            break;
        case 1:
            lightEnum = GL_LIGHT1;
            break;
        case 2:
            lightEnum = GL_LIGHT2;
            break;
        case 3:
            lightEnum = GL_LIGHT3;
            break;
        case 4:
            lightEnum = GL_LIGHT4;
            break;
        case 5:
            lightEnum = GL_LIGHT5;
            break;
        case 6:
            lightEnum = GL_LIGHT6;
            break;
        case 7: default:
            lightEnum = GL_LIGHT7;
            break;
        }
        glEnable(lightEnum);
        glLightfv(lightEnum, GL_AMBIENT, (*it)->getAmbient());
        glLightfv(lightEnum, GL_DIFFUSE, (*it)->getDiffuse());
        glLightfv(lightEnum, GL_SPECULAR, (*it)->getSpecular());
        ++it;
    }
}

void Renderer::uploadModel(unsigned int& meshId, unsigned int& indicesId, const Mesh& mesh) {
    int bufferSize;
    size_t verticesBytes = mesh.getVerticesBytes();
    size_t normalsBytes = mesh.getNormalsBytes();
    size_t uvCoordsBytes = mesh.getUvCoordsBytes();
    size_t indicesBytes = mesh.getIndicesBytes();
    switch (gDataUploadMode) {
        case DATA_UPLOAD_VERTEX_BUFFER_OBJECT:
            glGenBuffers(1, &meshId);
            glBindBuffer(GL_ARRAY_BUFFER, meshId);
            glBufferData(GL_ARRAY_BUFFER,
                         verticesBytes + normalsBytes + uvCoordsBytes,
                         0,
                         GL_STATIC_DRAW);
            glBufferSubData(GL_ARRAY_BUFFER,
                            0,
                            verticesBytes,
                            mesh.getVerticesPtr());
            glBufferSubData(GL_ARRAY_BUFFER,
                            verticesBytes,
                            normalsBytes,
                            mesh.getNormalsPtr());
            glBufferSubData(GL_ARRAY_BUFFER,
                            verticesBytes + normalsBytes,
                            uvCoordsBytes,
                            mesh.getUvCoordsPtr());
            glGetBufferParameteriv(GL_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
            if(verticesBytes + normalsBytes + uvCoordsBytes != (size_t)bufferSize)
                cerr << "Error: data size is mismatch with input array" << endl;

            glGenBuffers(1, &indicesId);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesId);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                         indicesBytes,
                         mesh.getIndicesPtr(),
                         GL_STATIC_DRAW);
            glGetBufferParameteriv(GL_ELEMENT_ARRAY_BUFFER, GL_BUFFER_SIZE, &bufferSize);
            if(indicesBytes != (size_t)bufferSize)
                cerr << "Error: data size is mismatch with input array" << endl;
            break;
        case DATA_UPLOAD_VERTEX_BUFFER_OBJECT_EXT:
            glGenBuffersARB(1, &meshId);
            glBindBufferARB(GL_ARRAY_BUFFER_ARB, meshId);
            glBufferDataARB(GL_ARRAY_BUFFER_ARB,
                            verticesBytes + normalsBytes + uvCoordsBytes,
                            0,
                            GL_STATIC_DRAW_ARB);
            glBufferSubDataARB(GL_ARRAY_BUFFER_ARB,
                               0,
                               verticesBytes,
                               mesh.getVerticesPtr());
            glBufferSubDataARB(GL_ARRAY_BUFFER_ARB,
                               verticesBytes,
                               normalsBytes,
                               mesh.getNormalsPtr());
            glBufferSubDataARB(GL_ARRAY_BUFFER_ARB,
                               verticesBytes + normalsBytes,
                               uvCoordsBytes,
                               mesh.getUvCoordsPtr());
            glGetBufferParameterivARB(GL_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &bufferSize);
            if(verticesBytes + normalsBytes + uvCoordsBytes != (size_t)bufferSize)
                cerr << "Error: data size is mismatch with input array" << endl;

            glGenBuffersARB(1, &indicesId);
            glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indicesId);
            glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB,
                            indicesBytes,
                            mesh.getIndicesPtr(),
                            GL_STATIC_DRAW_ARB);
            glGetBufferParameterivARB(GL_ELEMENT_ARRAY_BUFFER_ARB, GL_BUFFER_SIZE_ARB, &bufferSize);
            if(indicesBytes != (size_t)bufferSize)
                cerr << "Error: data size is mismatch with input array" << endl;
            break;
        case DATA_UPLOAD_VERTEX_ARRAY:
            break;
        default:
            cerr << "Error: invalid data_upload_t: " << gDataUploadMode << endl;
    }
}

void Renderer::deleteModel(const unsigned int meshId, const unsigned int indicesId) {
    switch (gDataUploadMode) {
        case DATA_UPLOAD_VERTEX_BUFFER_OBJECT:
            glDeleteBuffers(1, &meshId);
            glDeleteBuffers(1, &indicesId);
            break;
        case DATA_UPLOAD_VERTEX_BUFFER_OBJECT_EXT:
            glDeleteBuffersARB(1, &meshId);
            glDeleteBuffersARB(1, &indicesId);
            break;
        case DATA_UPLOAD_VERTEX_ARRAY:
            break;
        default:
            cerr << "Error: invalid data_upload_t: " << gDataUploadMode << endl;
    }
}

void Renderer::uploadTexture(unsigned int& textureId, const Texture& texture) {
    GLenum textureFormat;

    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    switch (texture.getTextureFormat()) {
    case TEXTURE_FORMAT_RGBA:
        textureFormat = GL_RGBA;
        break;
    case TEXTURE_FORMAT_RGB:
        textureFormat = GL_RGB;
        break;
    case TEXTURE_FORMAT_BGRA:
        textureFormat = GL_BGRA;
        break;
    case TEXTURE_FORMAT_BGR:
        textureFormat = GL_BGR;
        break;
    default:
        cerr << "Error: invalid texture_format_t: " << texture.getTextureFormat() << endl;
        textureFormat = GL_RGBA;
    }
    switch (gMipMapGenerationMode) {
    case MIPMAP_GENERATION_TEX_PARAMETER:
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_TRUE); // 1.4
        glTexImage2D(GL_TEXTURE_2D, 0, texture.getBytesPerPixel(), texture.getWidth(),
                     texture.getHeight(), 0, textureFormat, GL_UNSIGNED_BYTE, texture.getPixels());
        break;
    case MIPMAP_GENERATION_TEX_PARAMETER_EXT:
        glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP_SGIS, GL_TRUE);
        glTexImage2D(GL_TEXTURE_2D, 0, texture.getBytesPerPixel(), texture.getWidth(),
                     texture.getHeight(), 0, textureFormat, GL_UNSIGNED_BYTE, texture.getPixels());
        break;
    case MIPMAP_GENERATION_GLU:
        gluBuild2DMipmaps(GL_TEXTURE_2D, texture.getBytesPerPixel(), texture.getWidth(),
                          texture.getHeight(), textureFormat, GL_UNSIGNED_BYTE, texture.getPixels());
        break;
    default:
        cerr << "Error: invalid mipmap_generation_t: " << gMipMapGenerationMode << endl;
    }
}

void Renderer::deleteTexture(const unsigned int textureId) {
    glDeleteTextures(1, &textureId);
}

void Renderer::draw() const {
    if (m_activeCamera->hasChanged()) {
        initCamera();
        m_activeCamera->setHasChanged(false);
    }

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    float m[16];

    // set camera
    Entity& cam = m_activeCamera->getEntity();
    setOpenGLMatrix(m, VECTOR3_ZERO, cam.getOrientationAbs().inverse());
    glMultMatrixf(m);
    glTranslatef(-cam.getPositionAbs().getX(), -cam.getPositionAbs().getY(), -cam.getPositionAbs().getZ());

    // set lights
    displayLegacyLights();

    // set meshes
    set<RenderableMesh*>::const_iterator it;
    for (it = m_model.begin(); it != m_model.end(); ++it) {
        const Model& model = (*it)->getModel();
        const Entity& entity = (*it)->getEntity();

        glPushMatrix();
        setOpenGLMatrix(m, entity.getPositionAbs(), entity.getOrientationAbs());
        glMultMatrixf(m);
        for (size_t n = 0; n < model.getTotalMeshes(); ++n) {
            const Mesh& mesh = model.getMesh(n);
            const Material& mtl = mesh.getMaterial();

            // set material
            glMaterialfv(GL_FRONT, GL_DIFFUSE, mtl.getColor(MATERIAL_COLOR_DIFFUSE).rgb);
            glMaterialfv(GL_FRONT, GL_SPECULAR, mtl.getColor(MATERIAL_COLOR_SPECULAR).rgb);
            glMaterialfv(GL_FRONT, GL_AMBIENT, mtl.getColor(MATERIAL_COLOR_AMBIENT).rgb);
            glMaterialfv(GL_FRONT, GL_EMISSION, mtl.getColor(MATERIAL_COLOR_EMISSIVE).rgb);
            glMaterialf(GL_FRONT, GL_SHININESS, mtl.getShininess());

            // set textures
            if (mtl.getTextureMap(MATERIAL_DIFFUSE_MAP) != 0)
                glBindTexture(GL_TEXTURE_2D, mtl.getTextureMap(MATERIAL_DIFFUSE_MAP)->getId());
            else
                glBindTexture(GL_TEXTURE_2D, 0);

            // draw mesh
            switch (gDataUploadMode) {
            case DATA_UPLOAD_VERTEX_BUFFER_OBJECT:
                glBindBuffer(GL_ARRAY_BUFFER, mesh.getMeshId());
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.getIndicesId());

                glVertexPointer(3, GL_FLOAT, 0, 0);
                glNormalPointer(GL_FLOAT, 0, (GLvoid*)(mesh.getVerticesBytes()));
                glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*)(mesh.getVerticesBytes() + mesh.getNormalsBytes()));
                glDrawElements(GL_TRIANGLES, mesh.getIndicesSize(), GL_UNSIGNED_INT, 0);

                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
                glBindBuffer(GL_ARRAY_BUFFER, 0);
                break;
            case DATA_UPLOAD_VERTEX_BUFFER_OBJECT_EXT:
                glBindBufferARB(GL_ARRAY_BUFFER_ARB, mesh.getMeshId());
                glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, mesh.getIndicesId());

                glVertexPointer(3, GL_FLOAT, 0, 0);
                glNormalPointer(GL_FLOAT, 0, (GLvoid*)(mesh.getVerticesBytes()));
                glTexCoordPointer(2, GL_FLOAT, 0, (GLvoid*)(mesh.getVerticesBytes() + mesh.getNormalsBytes()));
                glDrawElements(GL_TRIANGLES, mesh.getIndicesSize(), GL_UNSIGNED_INT, 0);

                glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, 0);
                glBindBufferARB(GL_ARRAY_BUFFER_ARB, 0);
                break;
            case DATA_UPLOAD_VERTEX_ARRAY:
                glVertexPointer(3, GL_FLOAT, 0, mesh.getVerticesPtr());
                glNormalPointer(GL_FLOAT, 0, mesh.getNormalsPtr());
                glTexCoordPointer(2, GL_FLOAT, 0, mesh.getUvCoordsPtr());
                glDrawElements(GL_TRIANGLES, mesh.getIndicesSize(), GL_UNSIGNED_INT, mesh.getIndicesPtr());
                break;
            default:
                cerr << "Error: invalid data_upload_t: " << gDataUploadMode << endl;
            }
        }
        glPopMatrix();
    }
}

string Renderer::listsToString() const {
    stringstream ss;
    ss << "Renderer Cameras List:" << endl;
    set<Camera*>::const_iterator itCam;
    for (itCam = m_cameras.begin(); itCam != m_cameras.end(); ++itCam) {
        ss << "  " << (*itCam)->getDescription();
        if (*itCam == m_activeCamera)
            ss << " *";
        ss << endl;
    }

    ss << "Renderer Meshes List:" << endl;
    set<RenderableMesh*>::const_iterator itMesh;
    for (itMesh = m_model.begin(); itMesh != m_model.end(); ++itMesh)
        ss << "  " << (*itMesh)->getDescription() << endl;

    ss << "Lights List:" << endl;
    set<Light*>::const_iterator itLight;
    for (itLight = m_lights.begin(); itLight != m_lights.end(); ++itLight)
        ss << "  " << (*itLight)->getDescription() << endl;

    return ss.str();
}

Renderer::Renderer():
    m_activeCamera(0),
    m_cameras(),
    m_lights(),
    m_model()
{
    double openGLVersion;
    double shaderLanguageVersion;
    GLint integer;

    stringstream ss;
    ss << glGetString(GL_SHADING_LANGUAGE_VERSION) << " " << glGetString(GL_VERSION);
    ss >> shaderLanguageVersion >> openGLVersion;

    // general information
    cout << endl;
    cout << "OpenGL Driver Info" << endl;
    cout << "==================" << endl;
    cout << "Vendor: " << glGetString(GL_VENDOR) << endl;
    cout << "Version: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
    cout << "Renderer: " << glGetString(GL_RENDERER) << endl;

    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &integer);
    cout << "Max texture size: " << integer << "x" << integer << endl;
    glGetIntegerv(GL_MAX_LIGHTS, &integer);
    cout << "Max lights: " << integer << endl;
    glGetIntegerv(GL_MAX_CLIP_PLANES, &integer);
    cout << "Max clip planes: " << integer << endl;
    glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH, &integer);
    cout << "Max ModelView stack depth: " << integer << endl;
    glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH, &integer);
    cout << "Max Projection stack depth: " << integer << endl;
    glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH, &integer);
    cout << "Max Attribute stack depth: " << integer << endl;
    glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH, &integer);
    cout << "Max Texture stack depth: " << integer << endl;
    glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &integer);
    cout << "Max vertices: " << integer << endl;
    glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &integer);
    cout << "Max indices: " << integer << endl;

    // capabilities
    if (openGLVersion >= 3.0)
        cout << "Using OpenGL Core " << openGLVersion << endl;
    else
        cout << "Using OpenGL Legacy " << openGLVersion << endl;

    // default techniques
    gDataUploadMode = DATA_UPLOAD_VERTEX_ARRAY;
    gMipMapGenerationMode = MIPMAP_GENERATION_GLU;

    // version techniques
    int openGLVersionInt = floor(openGLVersion * 10.0 + 0.5);
    switch (openGLVersionInt) {
    case 43: // 4.3
    case 42: // 4.2
    case 41: // 4.1
    case 40: // 4.0
    case 33: // 3.3
    case 32: // 3.2
    case 31: // 3.1
    case 30: // 3.0
    case 21: // 2.1
    case 20: // 2.0
    case 15: // 1.5
        gDataUploadMode = DATA_UPLOAD_VERTEX_BUFFER_OBJECT;
    case 14: // 1.4
        gMipMapGenerationMode = MIPMAP_GENERATION_TEX_PARAMETER;
        break;
    case 13: // 1.3
    case 12: // 1.2
    case 11: // 1.1
        gDataUploadMode = DATA_UPLOAD_VERTEX_ARRAY;
        gMipMapGenerationMode = MIPMAP_GENERATION_GLU;
        break;
    default:
        cerr << "Error: unsupported OpenGL version: " << openGLVersion << endl;
    }

    // extension techniques
    glGetIntegerv(GL_NUM_EXTENSIONS, &integer);
    cout << "Extensions: " << integer << endl;
    if (glewInit() != GLEW_OK)
        cerr << "Error: failed to initialize GLEW" << endl;
    if (gDataUploadMode == DATA_UPLOAD_VERTEX_ARRAY &&
        glewIsSupported("GL_ARB_vertex_buffer_object"))
        gDataUploadMode = DATA_UPLOAD_VERTEX_BUFFER_OBJECT_EXT;
    if (gMipMapGenerationMode == MIPMAP_GENERATION_GLU &&
        glewIsSupported("GL_SGIS_generate_mipmap"))
        gMipMapGenerationMode = MIPMAP_GENERATION_TEX_PARAMETER_EXT;

    switch (gDataUploadMode) {
    case DATA_UPLOAD_VERTEX_ARRAY:
        cout << "Using Vertex Arrays" << endl;
        break;
    case DATA_UPLOAD_VERTEX_BUFFER_OBJECT_EXT:
        cout << "Using Vertex Buffer Objects EXT" << endl;
        break;
    case DATA_UPLOAD_VERTEX_BUFFER_OBJECT:
        cout << "Using Vertex Buffer Objects" << endl;
        break;
    default:
        cerr << "Error: invalid data_upload_t: " << gDataUploadMode << endl;
    }
    switch (gMipMapGenerationMode) {
    case MIPMAP_GENERATION_GLU:
        cout << "Using gluBuild2DMipmaps" << endl;
        break;
    case MIPMAP_GENERATION_TEX_PARAMETER_EXT:
        cout << "Using GL_GENERATE_MIPMAP_SGIS" << endl;
        break;
    case MIPMAP_GENERATION_TEX_PARAMETER:
        cout << "Using GL_GENERATE_MIPMAP" << endl;
        break;
    default:
        cerr << "Error: invalid mipmap_generation_t: " << gMipMapGenerationMode << endl;
    }
    cout << endl;
}

Renderer::Renderer(const Renderer& rhs):
    m_activeCamera(rhs.m_activeCamera),
    m_cameras(rhs.m_cameras),
    m_lights(rhs.m_lights),
    m_model(rhs.m_model)
{
    cerr << "Renderer copy constructor should not be called" << endl;
}

Renderer& Renderer::operator=(const Renderer& rhs) {
    cerr << "Renderer assignment operator should not be called" << endl;
    if (this == &rhs)
        return *this;
    m_activeCamera = rhs.m_activeCamera;
    m_cameras = rhs.m_cameras;
    m_lights = rhs.m_lights;
    m_model = rhs.m_model;
    return *this;
}

void Renderer::initialize() {
    // always
    glFrontFace(GL_CCW); // redundant
    glCullFace(GL_BACK); // redundant
    glEnable(GL_CULL_FACE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
}

void Renderer::deinitialize() {
    set<Camera*>::const_iterator itCam;
    for (itCam = m_cameras.begin(); itCam != m_cameras.end(); ++itCam)
        delete *itCam;

    set<Light*>::const_iterator itLight;
    for (itLight = m_lights.begin(); itLight != m_lights.end(); ++itLight)
        delete *itLight;

    set<RenderableMesh*>::const_iterator itMesh;
    for (itMesh = m_model.begin(); itMesh != m_model.end(); ++itMesh)
        delete *itMesh;
}

void Renderer::initCamera() const {
    Device& dev = DeviceManager::getDevice();
    m_activeCamera->setViewport(0, 0, dev.getWinWidth(), dev.getWinHeight());
    viewport_t view = m_activeCamera->getViewport();
    glViewport(view.posX, view.posY, view.width, view.height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    switch (m_activeCamera->getType()) {
        case CAMERA_ORTHOGRAPHIC:
            glOrtho(
                -m_activeCamera->getOrthoWidth(),
                    m_activeCamera->getOrthoWidth(),
                    -m_activeCamera->getOrthoHeight(),
                    m_activeCamera->getOrthoHeight(),
                    m_activeCamera->getNearDistance(),
                    m_activeCamera->getFarDistance()
            );
            break;
        case CAMERA_PROJECTION:
            gluPerspective(
                m_activeCamera->getPerspectiveFOV(),
                           m_activeCamera->getAspectRatio(),
                           m_activeCamera->getNearDistance(),
                           m_activeCamera->getFarDistance()
            );
            break;
    }
    glMatrixMode(GL_MODELVIEW);
}

void Renderer::displayLegacyLights() const {
    set<Light*>::const_iterator itLight = m_lights.begin();
    for (size_t i = 0; i < m_lights.size(); ++i) {
        GLenum lightEnum;
        switch (i) {
            case 0:
                lightEnum = GL_LIGHT0;
                break;
            case 1:
                lightEnum = GL_LIGHT1;
                break;
            case 2:
                lightEnum = GL_LIGHT2;
                break;
            case 3:
                lightEnum = GL_LIGHT3;
                break;
            case 4:
                lightEnum = GL_LIGHT4;
                break;
            case 5:
                lightEnum = GL_LIGHT5;
                break;
            case 6:
                lightEnum = GL_LIGHT6;
                break;
            case 7: default:
                lightEnum = GL_LIGHT7;
                break;
        }
        const Vector3& pos = (*itLight)->getEntity().getPositionAbs();
        float lightPosition[] = {float(pos.getX()), float(pos.getY()), float(pos.getZ()), 1.0f};
        glLightfv(lightEnum, GL_POSITION, lightPosition);
        ++itLight;
    }
}

void Renderer::setOpenGLMatrix(float*const m, const Vector3& pos, const Quaternion& rot) const {
    Matrix3x3 temp(rot);
    m[0]  = static_cast<float>(temp.getRow(0).getX());
    m[1]  = static_cast<float>(temp.getRow(1).getX());
    m[2]  = static_cast<float>(temp.getRow(2).getX());
    m[3]  = 0.0f;
    m[4]  = static_cast<float>(temp.getRow(0).getY());
    m[5]  = static_cast<float>(temp.getRow(1).getY());
    m[6]  = static_cast<float>(temp.getRow(2).getY());
    m[7]  = 0.0f;
    m[8]  = static_cast<float>(temp.getRow(0).getZ());
    m[9]  = static_cast<float>(temp.getRow(1).getZ());
    m[10] = static_cast<float>(temp.getRow(2).getZ());
    m[11] = 0.0f;
    m[12] = static_cast<float>(pos.getX());
    m[13] = static_cast<float>(pos.getY());
    m[14] = static_cast<float>(pos.getZ());
    m[15] = 1.0f;
}