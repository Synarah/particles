// Bryn Mawr College, Synarah Sitaf, 2020
//

#include <cmath>
#include <string>
#include <vector>
#include "agl/window.h"

using namespace std;
using namespace glm;
using namespace agl;

struct Particle {
  glm::vec3 pos;
  glm::vec3 vel;
  glm::vec4 color;
  float rot;
  float size;
  float ls;
  bool fin = false;
};

class Viewer : public Window {
public:
  Viewer() : Window() {
  }

  void setup() {
    setWindowSize(1000, 1000);
    renderer.setDepthTest(false);
    renderer.blendMode(agl::BLEND);
    createConfetti(1);
    renderer.loadTexture("boom", "../textures/bubbles.png", 0);
    renderer.loadTexture("particle", "../textures/star4.png", 0);
  }

  void createConfetti(int size)
  {
    for (int i = 0; i < size; i++){
      Particle p;
      p.color = vec4(0.65, 0.3, 0.95, 0);
      p.size = 0.25f;
      p.pos = vec3(0, -0.5f, 0);
      p.vel = vec3(0, .01f, 0) * 10.0f;
      mParticles.push_back(p);
    }
  }

  void createExplosion(){
    for(int i = 0; i < 6; i++){
        Particle p;
        p.color = vec4(0.9, 0.9, 1.0, 1.0);
        p.size = 0.25f;
        p.pos = vec3(0, 0.5, 0);
        p.vel = agl::randomUnitCube() * 0.05f;
        p.vel.z = 0;
        explosions.push_back(p);
    }
  }

  void updateConfetti(){
    for(int i = 0; i < mParticles.size(); i++ ){
        if(mParticles[i].pos.y < .50f){
        mParticles[i].pos.y += mParticles[i].vel.y;   
        }
        else{
            ready = true;
        }
    }
  }

  void updateExplosion(){
      for (int i = 0; i < explosions.size(); i ++){
          explosions[i].pos.x += explosions[i].vel.x;
          explosions[i].pos.y += explosions[i].vel.y;
          explosions[i].pos.z += explosions[i].vel.z;
      }
  }

  void drawConfetti()
  {
    renderer.texture("image", "particle");
    for (int i = 0; i < mParticles.size(); i++)
    {
      Particle particle = mParticles[i];
      renderer.sprite(particle.pos, particle.color, particle.size, particle.rot);
    }
  }

  void drawExplosion()
  {
    renderer.texture("image", "boom");
    for (int i = 0; i < explosions.size(); i++)
    {
      Particle particle = explosions[i];
      renderer.sprite(particle.pos, particle.color, particle.size, particle.rot);
    }
  }

  void mouseMotion(int x, int y, int dx, int dy) {
  }

  void mouseDown(int button, int mods) {
  }

  void mouseUp(int button, int mods) {
  }

  void scroll(float dx, float dy) {
    eyePos.z += dy;
  }

  void keyUp(int key, int mods) {
  }

  void draw() {
    renderer.beginShader("sprite");
    position = vec3(0,0,0);
    float aspect = ((float)width()) / height();
    renderer.perspective(glm::radians(60.0f), aspect, 0.1f, 50.0f);
    /**if(mParticles.size() < 90){
      createConfetti(1);
    }*/
    renderer.lookAt(eyePos, lookPos, up);
    if (ready){
        createExplosion();
    }
    //renderer.sprite(position, vec4(1.0f), 0.25f);
    updateConfetti();
    updateExplosion();
    drawConfetti();
    drawExplosion();
    renderer.endShader();
  }

protected:

  vec3 eyePos = vec3(0, 0, 3);
  vec3 lookPos = vec3(0, 0, 0);
  vec3 up = vec3(0, 1, 0);
  vec3 position = vec3(1, 0, 0);
  bool ready = false;

  std::vector<Particle> explosions;

  std::vector<Particle> mParticles;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
