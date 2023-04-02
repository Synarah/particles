// Bryn Mawr College, alinen, 2020
//

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);
    renderer.loadShader("simple-texture",
      "../shaders/simple-texture.vs",
      "../shaders/simple-texture.fs");

    Image img;
    img.load("../textures/tree.png", true);
    renderer.loadTexture("tree", img, 0);
    // TODO: Use the width and the height of the image to scale the billboard
    h = img.height();
    w = img.width();
    renderer.loadTexture("grass", "../textures/grass.png", 0);
    renderer.blendMode(agl::BLEND);
  }


  void mouseMotion(int x, int y, int dx, int dy) {
    if (click) {
      e = e + dy * 0.1;
      a = a - dx * 0.1;
      float x = rad * sin(a) * cos(e);
      float y = rad * sin(e);
      float z = rad * cos(a) * cos(e);
      eyePos = vec3(x, y, z);
    }
  }

    void mouseDown(int button, int mods) {
      click = true;
    }

    void mouseUp(int button, int mods) {
      click = false;
    }

  void scroll(float dx, float dy) {
  }

  void draw() {
    renderer.beginShader("simple-texture");

    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
    renderer.lookAt(eyePos, lookPos, up);

    // draw plane
    renderer.texture("Image", "grass");
    renderer.push();
    renderer.translate(vec3(0.0, -0.5, 0));
    renderer.scale(vec3(2.0f));
    renderer.plane();
    renderer.pop();

    // draw tree
    renderer.texture("Image", "tree");
    renderer.push();
    renderer.rotate(atan2(normalize(eyePos - lookPos).x, normalize(eyePos - lookPos).z), vec3(0.0, 1.0, 0.0)); //normalize(eyePos - lookPos)
    renderer.translate(vec3(-0.5, -0.5, 0));
    renderer.scale(vec3((float)w/(float)h, 1.0, 1.0));
    renderer.quad(); // vertices span from (0,0,0) to (1,1,0)
    renderer.pop();

    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 2);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  bool click = false;
  float e = 0;
  float a = 0;
  float rad = 10;
  float h;
  float w;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
