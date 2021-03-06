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


#ifndef DEVICE_HPP
#define DEVICE_HPP

#include <string>
#include <set>
#include "commandobject.hpp"
#include "inputs.hpp"

struct SDL_Surface;

class Device: public CommandObject {
public:
    Device(const std::string& objectName);
    ~Device();

    Inputs* getInputs();
    double getDeltaTime() const;
    double getFps() const;

    void onFrameStart();
    void onFrameEnd();
    void swapBuffers() const;
    size_t videoMemKB();
    void setTitle(const std::string& title);
    void setFullscreen(const bool useFullscreen = true);
    void setResolution(const size_t width, const size_t height);
    size_t getWinWidth() const;
    size_t getWinHeight() const;
    void processEvents(bool& isRunning);

protected:
    size_t m_width;
    size_t m_height;
    size_t m_halfWidth;
    size_t m_halfHeight;
    size_t m_depth;
    std::set<size_t> m_keysPressed;
    std::set<size_t> m_mouseButtonsPressed;
    static Inputs ms_inputs;
    static SDL_Surface* ms_screen;
    double m_startTime;
    double m_deltaTime;
    double m_fps;

    std::string cmdSwapBuffers(std::deque<std::string>&);
    std::string cmdTitle(std::deque<std::string>& args);
    std::string cmdFullscreen(std::deque<std::string>& args);
    std::string cmdResolution(std::deque<std::string>& args);
};



inline Inputs* Device::getInputs() {
    return &ms_inputs;
}

inline double Device::getDeltaTime() const {
    return m_deltaTime;
}

inline double Device::getFps() const {
    return m_fps;
}



inline std::string Device::cmdSwapBuffers(std::deque<std::string>&) {
    swapBuffers();
    return "";
}

inline std::string Device::cmdTitle(std::deque< std::string >& args) {
    if (args.size() < 1)
        return "Error: too few arguments";
    setTitle(args[0]);
    return std::string("New title: ") + args[0];
}

#endif // DEVICE_HPP
