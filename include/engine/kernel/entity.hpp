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


#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <ostream>
#include <vector>
#include <set>
#include "commandobject.hpp"
#include "vector3.hpp"
#include "quaternion.hpp"
#include "component.hpp"

class Device;
class Component;

typedef enum {
    SPACE_LOCAL,
    SPACE_PARENT,
    SPACE_GLOBAL
} transform_space_t;

class Entity: public CommandObject {
public:
    friend class Component;
    friend std::ostream& operator<<(std::ostream& out, const Entity& rhs);

    typedef std::set<Entity*>::const_iterator const_child_iterator_t;
    typedef std::set<Entity*>::iterator child_iterator_t;

    Entity(Entity* parent, const std::string& objectName, const Device* device);
    ~Entity();

    const Entity* getParent() const;
    Entity* parent();
    const Vector3& getPositionAbs() const;
    const Vector3& getPositionRel() const;
    const Quaternion& getOrientationAbs() const;
    const Quaternion& getOrientationRel() const;
    const Component* getComponent(const component_t i) const;
    const_child_iterator_t getChildrenBegin() const;
    child_iterator_t getChildrenBegin();
    const_child_iterator_t getChildrenEnd() const;
    child_iterator_t getChildrenEnd();

    void setParent(Entity* parent);
    void setPositionAbs(const Vector3& position);
    void setPositionAbs(const scalar_t& posX, const scalar_t& posY, const scalar_t& posZ);
    void setPositionRel(const Vector3& position);
    void setPositionRel(const scalar_t& posX, const scalar_t& posY, const scalar_t& posZ);
    void setOrientationAbs(const Quaternion& orientation);
    void setOrientationAbs(const scalar_t& w, const scalar_t& x, const scalar_t& y, const scalar_t& z);
    void setOrientationAbs(const Vector3& axis, const scalar_t& angle);
    void setOrientationAbs(const scalar_t& yawRad, const scalar_t& pitchRad, const scalar_t& rollRad);
    void setOrientationRel(const Quaternion& orientation);
    void setOrientationRel(const scalar_t& w, const scalar_t& x, const scalar_t& y, const scalar_t& z);
    void setOrientationRel(const Vector3& axis, const scalar_t& angle);
    void setOrientationRel(const scalar_t& yawRad, const scalar_t& pitchRad, const scalar_t& rollRad);

    void translate(const Vector3& displacement, const transform_space_t& relativeTo = SPACE_LOCAL);
    void translate(const scalar_t& distX, const scalar_t& distY, const scalar_t& distZ, const transform_space_t& relativeTo = SPACE_LOCAL);
    void translateX(const scalar_t& distX, const transform_space_t& relativeTo = SPACE_LOCAL);
    void translateY(const scalar_t& distY, const transform_space_t& relativeTo = SPACE_LOCAL);
    void translateZ(const scalar_t& distZ, const transform_space_t& relativeTo = SPACE_LOCAL);

    void rotate(const Quaternion& deltaRotation, const transform_space_t& relativeTo = SPACE_LOCAL);
    void rotate(const scalar_t& w, const scalar_t& x, const scalar_t& y, const scalar_t& z, const transform_space_t& relativeTo = SPACE_LOCAL);
    void rotate(const Vector3& axis, const scalar_t& angle, const transform_space_t& relativeTo = SPACE_LOCAL);
    void yaw(const scalar_t& radians, const transform_space_t& relativeTo = SPACE_LOCAL);
    void pitch(const scalar_t& radians, const transform_space_t& relativeTo = SPACE_LOCAL);
    void roll(const scalar_t& radians, const transform_space_t& relativeTo = SPACE_LOCAL);
    void lookAt(const Vector3& target, const Vector3& up = VECTOR3_UNIT_Y);

    Entity* addChild(const std::string& childName);
    void removeChild(Entity* const child);
    void removeAllChildren();
    std::string treeToString(const size_t indent) const;

private:
    Entity* m_parent;
    const Device* m_device;
    std::set<Entity*> m_children;
    std::vector<Component*> m_components;
    Vector3 m_positionAbs;
    Vector3 m_positionRel;
    Quaternion m_orientationAbs;
    Quaternion m_orientationRel;
    Quaternion m_lastOrientation;

    Entity(const Entity& rhs);
    Entity& operator=(const Entity&);

    void applyTranslationToChildren();
    void applyOrientationToChildren();
    void applyTransformToPhysicsComponent();

    void cmdPositionAbs(const std::string& arg);
    void cmdPositionRel(const std::string& arg);
    void cmdOrientationAbsYPR(const std::string& arg);
    void cmdOrientationRelYPR(const std::string& arg);
    void cmdMoveXYZ(const std::string& arg);
    void cmdMoveX(const std::string& arg);
    void cmdMoveY(const std::string& arg);
    void cmdMoveZ(const std::string& arg);
    void cmdMoveXYZ_parent(const std::string& arg);
    void cmdMoveX_parent(const std::string& arg);
    void cmdMoveY_parent(const std::string& arg);
    void cmdMoveZ_parent(const std::string& arg);
    void cmdMoveXYZ_global(const std::string& arg);
    void cmdMoveX_global(const std::string& arg);
    void cmdMoveY_global(const std::string& arg);
    void cmdMoveZ_global(const std::string& arg);
    void cmdYaw(const std::string& arg);
    void cmdPitch(const std::string& arg);
    void cmdRoll(const std::string& arg);
    void cmdYaw_parent(const std::string& arg);
    void cmdPitch_parent(const std::string& arg);
    void cmdRoll_parent(const std::string& arg);
    void cmdYaw_global(const std::string& arg);
    void cmdPitch_global(const std::string& arg);
    void cmdRoll_global(const std::string& arg);
    void cmdRemoveAllChildren(const std::string&);
};



inline const Entity* Entity::getParent() const {
    return m_parent;
}

inline Entity* Entity::parent() {
    return m_parent;
}

inline const Vector3& Entity::getPositionAbs() const {
    return m_positionAbs;
}

inline const Vector3& Entity::getPositionRel() const {
    return m_positionRel;
}

inline const Quaternion& Entity::getOrientationAbs() const {
    return m_orientationAbs;
}

inline const Quaternion& Entity::getOrientationRel() const {
    return m_orientationRel;
}

inline const Component* Entity::getComponent(const component_t i) const {
    return m_components[i];
}

inline Entity::const_child_iterator_t Entity::getChildrenBegin() const {
    return m_children.begin();
}

inline Entity::child_iterator_t Entity::getChildrenBegin() {
    return m_children.begin();
}

inline Entity::const_child_iterator_t Entity::getChildrenEnd() const {
    return m_children.end();
}

inline Entity::child_iterator_t Entity::getChildrenEnd() {
    return m_children.end();
}



inline void Entity::setParent(Entity* parent) {
    m_parent = parent;
}

inline void Entity::setPositionAbs(const Vector3& position) {
    m_positionAbs = position;
    m_positionRel = m_positionAbs - m_parent->m_positionAbs;
    applyTransformToPhysicsComponent();
    applyTranslationToChildren();
}

inline void Entity::setPositionAbs(const scalar_t& posX, const scalar_t& posY, const scalar_t& posZ) {
    setPositionAbs(Vector3(posX, posY, posZ));
}

inline void Entity::setPositionRel(const Vector3& position) {
    m_positionRel = position;
    m_positionAbs = m_positionRel + m_parent->m_positionAbs;
    applyTransformToPhysicsComponent();
    applyTranslationToChildren();
}

inline void Entity::setPositionRel(const scalar_t& posX, const scalar_t& posY, const scalar_t& posZ) {
    setPositionRel(Vector3(posX, posY, posZ));
}

inline void Entity::setOrientationAbs(const Quaternion& orientation) {
    m_lastOrientation = m_orientationAbs;
    m_orientationAbs = orientation.normalized();
    m_orientationRel = (m_parent->m_orientationAbs.inverse() * m_orientationAbs).normalized();
    applyTransformToPhysicsComponent();
    applyOrientationToChildren();
}

inline void Entity::setOrientationAbs(const scalar_t& w, const scalar_t& x, const scalar_t& y, const scalar_t& z) {
    setOrientationAbs(Quaternion(w, x, y, z));
}

inline void Entity::setOrientationAbs(const Vector3& axis, const scalar_t& angle) {
    setOrientationAbs(axis, angle);
}

inline void Entity::setOrientationAbs(const scalar_t& yawRad, const scalar_t& pitchRad, const scalar_t& rollRad) {
    setOrientationAbs(Quaternion(yawRad, pitchRad, rollRad));
}

inline void Entity::setOrientationRel(const Quaternion& orientation) {
    m_lastOrientation = m_orientationAbs;
    m_orientationRel = orientation.normalized();
    m_orientationAbs = (m_parent->m_orientationAbs * m_orientationRel).normalized();
    applyTransformToPhysicsComponent();
    applyOrientationToChildren();
}

inline void Entity::setOrientationRel(const scalar_t& w, const scalar_t& x, const scalar_t& y, const scalar_t& z) {
    setOrientationRel(Quaternion(w, x, y, z));
}

inline void Entity::setOrientationRel(const Vector3& axis, const scalar_t& angle) {
    setOrientationRel(axis, angle);
}

inline void Entity::setOrientationRel(const scalar_t& yawRad, const scalar_t& pitchRad, const scalar_t& rollRad) {
    setOrientationRel(Quaternion(yawRad, pitchRad, rollRad));
}



inline void Entity::translate(const scalar_t& distX, const scalar_t& distY, const scalar_t& distZ, const transform_space_t& relativeTo) {
    translate(Vector3(distX, distY, distZ), relativeTo);
}

inline void Entity::translateX(const scalar_t& distX, const transform_space_t& relativeTo) {
    translate(Vector3(distX, 0, 0), relativeTo);
}

inline void Entity::translateY(const scalar_t& distY, const transform_space_t& relativeTo) {
    translate(Vector3(0, distY, 0), relativeTo);
}

inline void Entity::translateZ(const scalar_t& distZ, const transform_space_t& relativeTo) {
    translate(Vector3(0, 0, distZ), relativeTo);
}

inline void Entity::rotate(const scalar_t& w, const scalar_t& x, const scalar_t& y, const scalar_t& z, const transform_space_t& relativeTo) {
    rotate(Quaternion(w, x, y, z), relativeTo);
}

inline void Entity::rotate(const Vector3& axis, const scalar_t& angle, const transform_space_t& relativeTo) {
    rotate(Quaternion(axis, angle), relativeTo);
}

inline void Entity::yaw(const scalar_t& radians, const transform_space_t& relativeTo) {
    rotate(VECTOR3_UNIT_Y, radians, relativeTo);
}

inline void Entity::pitch(const scalar_t& radians, const transform_space_t& relativeTo) {
    rotate(VECTOR3_UNIT_X, radians, relativeTo);
}

inline void Entity::roll(const scalar_t& radians, const transform_space_t& relativeTo) {
    rotate(VECTOR3_UNIT_Z, radians, relativeTo);
}



inline void Entity::cmdRemoveAllChildren(const std::string&) {
    removeAllChildren();
}

#endif // ENTITY_HPP
