#ifndef TIME_H_
#define TIME_H_

#include <SDL2/SDL.h>

class Time {
public:
    static Time& getInstance() {
        static Time instance;
        return instance;
    }

    Time(const Time&) = delete;
    Time& operator=(const Time&) = delete;

    void computeDeltaTime() {
        float currentFrame = SDL_GetTicks() / 1000.0f; // Convert ms to seconds
        m_deltaTime = currentFrame - m_lastFrame;
        m_lastFrame = currentFrame;
    }

    float getDeltaTime() const {
        return m_deltaTime;
    }


private:
    float m_deltaTime{};
    float m_lastFrame{};

    Time() {
        // m_lastFrame = SDL_GetTicks() / 1000.0f;
    }
    
    ~Time() {}
};

#endif
