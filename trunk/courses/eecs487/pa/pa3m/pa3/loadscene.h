#ifndef _LOADSCENE_H
#define _LOADSCENE_H

#include "scene.h"
#include "raytracer.h"

bool LoadScene(SceneT& s, CameraT& cam, OptionsT& opts, const char* filename);

#endif
