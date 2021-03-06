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


#include "shoggoth-engine/kernel/component.hpp"

#include <iostream>
#include <ostream>
#include "shoggoth-engine/kernel/entity.hpp"

using namespace std;

Component::Component(const string& type, Entity* const _entity):
    m_entity(_entity),
    m_type(type),
    m_description()
{
    m_entity->m_components.insert(pair<string, Component*>(m_type, this));
}

Component::~Component() {
    m_entity->m_components.erase(m_type);
}

Component::Component(const Component& rhs):
    m_entity(rhs.m_entity),
    m_type(rhs.m_type),
    m_description(rhs.m_description)
{
    cerr << "Error: Component copy constructor should not be called!" << endl;
}

Component& Component::operator=(const Component&) {
    cerr << "Error: Component assignment operator should not be called!" << endl;
    return *this;
}
