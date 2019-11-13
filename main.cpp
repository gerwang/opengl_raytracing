#include <memory>
#include "OpenGLDisplay.h"


int main() {
    auto display = std::make_unique<OpenGLDisplay>();
    display->mainLoop();
    return 0;
}