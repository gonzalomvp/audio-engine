#pragma once

#include <al.h>

inline void setListenerPosition   (float x, float y, float z) { alListener3f(AL_POSITION, x, y, z); }
inline void setListenerVelocity   (float x, float y, float z) { alListener3f(AL_VELOCITY, x, y, z); }
inline void setListenerOrientation(float x, float y, float z) { alListener3f(AL_ORIENTATION, x, y, z); }
