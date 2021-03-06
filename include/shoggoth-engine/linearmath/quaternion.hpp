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


#ifndef QUATERNION_HPP
#define QUATERNION_HPP

#include <bullet/LinearMath/btQuaternion.h>
#include "scalar.hpp"

class Vector3;

class Quaternion {
public:
    Quaternion();
    Quaternion(const Quaternion& q);
    Quaternion(const scalar_t& w, const scalar_t& x, const scalar_t& y, const scalar_t& z);
    Quaternion(const Vector3& axis, const scalar_t& _angle);
    Quaternion(const scalar_t& yaw, const scalar_t& pitch, const scalar_t& roll);
    Quaternion(const btQuaternion& q);

    Quaternion& operator=(const Quaternion& q);
    Quaternion operator-() const;
    Quaternion& operator+=(const Quaternion& q);
    Quaternion& operator-=(const Quaternion& q);
    Quaternion& operator*=(const Quaternion& q);
    Quaternion& operator*=(const scalar_t& q);
    Quaternion& operator/=(const scalar_t& q);
    const Quaternion operator+(const Quaternion& q) const;
    const Quaternion operator-(const Quaternion& q) const;
    const Quaternion operator*(const Quaternion& q) const;
    const Quaternion operator*(const scalar_t& s) const;
    const Quaternion operator/(const scalar_t& s) const;
    bool operator==(const Quaternion& q) const;
    bool operator!=(const Quaternion& q) const;

    const scalar_t& getW() const;
    const scalar_t& getX() const;
    const scalar_t& getY() const;
    const scalar_t& getZ() const;
    Vector3 getAxis() const;
    scalar_t getAngle() const;
    scalar_t getYaw() const;
    scalar_t getPitch() const;
    scalar_t getRoll() const;
    void setW(const scalar_t& w);
    void setX(const scalar_t& x);
    void setY(const scalar_t& y);
    void setZ(const scalar_t& z);
    void setValue(const scalar_t& w, const scalar_t& x, const scalar_t& y, const scalar_t& z);
    void setValue(const Quaternion& q);
    void setAxisAngle(const Vector3& axis, const scalar_t& _angle);
    void setEuler(const scalar_t& yaw, const scalar_t& pitch, const scalar_t& roll);
    void setValue(const btQuaternion& q);

    scalar_t length() const;
    scalar_t lengthSquared() const;
    scalar_t angle(const Quaternion& q) const;
    scalar_t dot(const Quaternion& q) const;
    Quaternion inverse() const;
    Quaternion& normalize();
    Quaternion normalized() const;
    Quaternion slerp(const Quaternion& q, const scalar_t& t) const;

private:
    scalar_t m_data[4];
};

const Quaternion QUATERNION_IDENTITY = Quaternion(1, 0, 0, 0);

#include "vector3.hpp"



inline std::ostream& operator<<(std::ostream& out, const Quaternion& q) {
    out << q.getW() << " " << q.getX() << " " << q.getY() << " " << q.getZ();
    return out;
}

inline std::istream& operator>>(std::istream& in, Quaternion& q) {
    scalar_t w, x, y, z;
    in >> w >> x >> y >> z;
    q.setValue(w, x, y, z);
    return in;
}



inline Quaternion::Quaternion() {
}

inline Quaternion::Quaternion(const Quaternion& q) {
    setValue(q);
}

inline Quaternion::Quaternion(const scalar_t& w, const scalar_t& x, const scalar_t& y, const scalar_t& z) {
    setValue(w, x, y, z);
}

inline Quaternion::Quaternion(const Vector3& axis, const scalar_t& _angle) {
    setAxisAngle(axis, _angle);
}

inline Quaternion::Quaternion(const scalar_t& yaw, const scalar_t& pitch, const scalar_t& roll) {
    setEuler(yaw, pitch, roll);
}

inline Quaternion::Quaternion(const btQuaternion& q) {
    setValue(q);
}



inline Quaternion& Quaternion::operator=(const Quaternion& q) {
    setValue(q);
    return *this;
}

inline Quaternion Quaternion::operator-() const {
    return Quaternion(-getW(), -getX(), -getY(), -getZ());
}

inline Quaternion& Quaternion::operator+=(const Quaternion& q) {
    m_data[0] += q.m_data[0];
    m_data[1] += q.m_data[1];
    m_data[2] += q.m_data[2];
    m_data[3] += q.m_data[3];
    return *this;
}

inline Quaternion& Quaternion::operator-=(const Quaternion& q) {
    m_data[0] -= q.m_data[0];
    m_data[1] -= q.m_data[1];
    m_data[2] -= q.m_data[2];
    m_data[3] -= q.m_data[3];
    return *this;
}

inline Quaternion& Quaternion::operator*=(const Quaternion& q) {
    setValue(
        getW() * q.getW() - getX() * q.getX() - getY() * q.getY() - getZ() * q.getZ(),
        getW() * q.getX() + getX() * q.getW() + getY() * q.getZ() - getZ() * q.getY(),
        getW() * q.getY() + getY() * q.getW() + getZ() * q.getX() - getX() * q.getZ(),
        getW() * q.getZ() + getZ() * q.getW() + getX() * q.getY() - getY() * q.getX()
    );
    return *this;
}

inline Quaternion& Quaternion::operator*=(const scalar_t& q) {
    m_data[0] *= q;
    m_data[1] *= q;
    m_data[2] *= q;
    m_data[3] *= q;
    return *this;
}

inline Quaternion& Quaternion::operator/=(const scalar_t& q) {
    return *this *= ONE / q;
}

inline const Quaternion Quaternion::operator+(const Quaternion& q) const {
    return Quaternion(*this) += q;
}

inline const Quaternion Quaternion::operator-(const Quaternion& q) const {
    return Quaternion(*this) -= q;
}

inline const Quaternion Quaternion::operator*(const Quaternion& q) const {
    return Quaternion(*this) *= q;
}

inline const Quaternion Quaternion::operator*(const scalar_t& s) const {
    return Quaternion(*this) *= s;
}

inline const Quaternion Quaternion::operator/(const scalar_t& s) const {
    return Quaternion(*this) /= s;
}

inline bool Quaternion::operator==(const Quaternion& q) const {
    return getW() == q.getW() &&
           getX() == q.getX() &&
           getY() == q.getY() &&
           getZ() == q.getZ();
}

inline bool Quaternion::operator!=(const Quaternion& q) const {
    return !(*this == q);
}



inline const scalar_t& Quaternion::getW() const {
    return m_data[0];
}

inline const scalar_t& Quaternion::getX() const {
    return m_data[1];
}

inline const scalar_t& Quaternion::getY() const {
    return m_data[2];
}

inline const scalar_t& Quaternion::getZ() const {
    return m_data[3];
}

inline Vector3 Quaternion::getAxis() const {
    const scalar_t s_squared = ONE - getW() * getW();

    if (s_squared < static_cast<scalar_t>(10.0) * EPSILON) //Check for divide by zero
        return VECTOR3_UNIT_X;  // Arbitrary
    const scalar_t s = ONE / std::sqrt(s_squared);
    return Vector3(getX() * s, getY() * s, getZ() * s);
}

inline scalar_t Quaternion::getAngle() const {
    return TWO * std::acos(getW());
}

inline void Quaternion::setW(const scalar_t& w) {
    m_data[0] = w;
}

inline void Quaternion::setX(const scalar_t& x) {
    m_data[1] = x;
}

inline void Quaternion::setY(const scalar_t& y) {
    m_data[2] = y;
}

inline void Quaternion::setZ(const scalar_t& z) {
    m_data[3] = z;
}

inline void Quaternion::setValue(const scalar_t& w, const scalar_t& x, const scalar_t& y, const scalar_t& z) {
    setW(w);
    setX(x);
    setY(y);
    setZ(z);
}

inline void Quaternion::setValue(const Quaternion& q) {
    setValue(q.getW(), q.getX(), q.getY(), q.getZ());
}

inline void Quaternion::setAxisAngle(const Vector3& axis, const scalar_t& _angle) {
    const scalar_t halfAngle = _angle * HALF;
    const scalar_t s = std::sin(halfAngle) / axis.length();
    setValue(std::cos(halfAngle), axis.getX() * s, axis.getY() * s, axis.getZ() * s);
}

inline void Quaternion::setEuler(const scalar_t& yaw, const scalar_t& pitch, const scalar_t& roll) {
    const scalar_t halfYaw = yaw * HALF;
    const scalar_t halfPitch = pitch * HALF;
    const scalar_t halfRoll = roll * HALF;
    const scalar_t cosYaw = std::cos(halfYaw);
    const scalar_t sinYaw = std::sin(halfYaw);
    const scalar_t cosPitch = std::cos(halfPitch);
    const scalar_t sinPitch = std::sin(halfPitch);
    const scalar_t cosRoll = std::cos(halfRoll);
    const scalar_t sinRoll = std::sin(halfRoll);
    setValue(
        cosRoll * cosPitch * cosYaw + sinRoll * sinPitch * sinYaw,
        cosRoll * sinPitch * cosYaw + sinRoll * cosPitch * sinYaw,
        cosRoll * cosPitch * sinYaw - sinRoll * sinPitch * cosYaw,
        sinRoll * cosPitch * cosYaw - cosRoll * sinPitch * sinYaw
    );
}

inline void Quaternion::setValue(const btQuaternion& q) {
    setValue(q.getW(), q.getX(), q.getY(), q.getZ());
}



inline scalar_t Quaternion::length() const {
    return std::sqrt(lengthSquared());
}

inline scalar_t Quaternion::lengthSquared() const {
    return dot(*this);
}

inline scalar_t Quaternion::angle(const Quaternion& q) const {
    scalar_t s = std::sqrt(lengthSquared() * q.lengthSquared());
    return std::acos(dot(q) / s);
}

inline scalar_t Quaternion::dot(const Quaternion& q) const {
    return getW() * q.getW() +
           getX() * q.getX() +
           getY() * q.getY() +
           getZ() * q.getZ();
}

inline Quaternion Quaternion::inverse() const {
    return Quaternion(getW(), -getX(), -getY(), -getZ());
}

inline Quaternion& Quaternion::normalize() {
    return *this /= length();
}

inline Quaternion Quaternion::normalized() const {
    return *this / length();
}

inline scalar_t Quaternion::getYaw() const {
    return std::asin(-TWO * (getX() * getZ() - getW() * getY()));
}

inline scalar_t Quaternion::getPitch() const {
    return std::atan2(TWO * (getY() * getZ() + getW() * getX()),
            getW() * getW() - getX() * getX() - getY() * getY() + getZ() * getZ());
}

inline scalar_t Quaternion::getRoll() const {
    return std::atan2(TWO * (getX() * getY() + getW() * getZ()),
            getW() * getW() + getX() * getX() - getY() * getY() - getZ() * getZ());
}

inline Quaternion Quaternion::slerp(const Quaternion& q, const scalar_t& t) const {
    scalar_t product = dot(q) / std::sqrt(lengthSquared() * q.lengthSquared());
    if (std::abs(product) != ONE) {
        // Take care of long angle case see http://en.wikipedia.org/wiki/Slerp
        const scalar_t sign = (product < 0) ? -ONE : ONE;

        const scalar_t theta = std::acos(sign * product);
        const scalar_t s1 = std::sin(sign * t * theta);
        const scalar_t d = ONE / std::sin(theta);
        const scalar_t s0 = std::sin((ONE - t) * theta);

        return Quaternion(
            (getW() * s0 + q.getW() * s1) * d,
            (getX() * s0 + q.getX() * s1) * d,
            (getY() * s0 + q.getY() * s1) * d,
            (getZ() * s0 + q.getZ() * s1) * d
        );
    }
    else
        return *this;
}



inline Quaternion operator*(const Quaternion& q, const Vector3& v) {
    return Quaternion(-q.getX() * v.getX() - q.getY() * v.getY() - q.getZ() * v.getZ(),
                       q.getW() * v.getX() + q.getY() * v.getZ() - q.getZ() * v.getY(),
                       q.getW() * v.getY() + q.getZ() * v.getX() - q.getX() * v.getZ(),
                       q.getW() * v.getZ() + q.getX() * v.getY() - q.getY() * v.getX());
}

inline Quaternion operator*(const Vector3& v, const Quaternion& q) {
    return Quaternion(-v.getX() * q.getX() - v.getY() * q.getY() - v.getZ() * q.getZ(),
                       v.getX() * q.getW() + v.getY() * q.getZ() - v.getZ() * q.getY(),
                       v.getY() * q.getW() + v.getZ() * q.getX() - v.getX() * q.getZ(),
                       v.getZ() * q.getW() + v.getX() * q.getY() - v.getY() * q.getX());
}

#endif // QUATERNION_HPP
