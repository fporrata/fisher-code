#include <iostream>

#include "scene.h"
#include "loadscene.h"

#include "raytracer.h"

#include "image_writer.h"

using namespace std;

int main(int argc, char *argv[]) {
  if(argc<3) {
    cerr << "usage: ./pa3 input.scene output.png" << endl;
    return -1;
  }
  cerr << argv[0] << endl;
  cerr << argv[1] << endl;
  cerr << argv[2] << endl;

  SceneT scene;
  CameraT cam;
  OptionsT opts;

  /// Loads the scene
  if(!LoadScene(scene, cam, opts, argv[1]))
    return 1;

  /// Constructs an image object
  XImageT<XVec3b> xim;
  xim.Allocate(cam.m_nx, cam.m_ny);

  /// constructs the raytracer instance, using the read scene and parameters
  RayTracerT rt(scene, opts);

  /// Perform the raytracing computation
  rt.TraceAll(cam, xim);

  cerr << "done raytracing." << endl;

  /// Output the image to the file
  ImageWriter<XImageT<XVec3b> >::WritePNG(xim, argv[2]);

  return 0;
}


