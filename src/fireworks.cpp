// Bryn Mawr College, Synarah Sitaf
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
    renderer.blendMode(agl::ADD);
    createConfetti(1);
    renderer.loadTexture("boom", "../textures/burst.png", 0);
    renderer.loadTexture("particle", "../textures/rocket.png", 0);
  }

  void createConfetti(int size)
  {
    for (int i = 0; i < size; i++){
      Particle p;
      p.color = vec4(1.0, 1.0, 1.0, .5);
      p.size = 0.25f;
      p.pos = vec3(0, -0.5f, 0);
      p.vel = vec3(0, .01f, 0);
      p.rot = 3.14;
      mParticles.push_back(p);
    }
  }

  void createExplosion(){
    for(int i = 0; i < 6; i++){
        Particle p;
        p.color = vec4(0.85, 0.7, 0.95, 0.5);
        p.size = 0.1f;
        p.pos = vec3(0, 0.5, 0);
        p.vel = agl::randomUnitSphere();
        p.vel.z = 0.0;
        explosions.push_back(p);
    }
  }

  void updateConfetti(){
    for(int i = 0; i < mParticles.size(); i++ ){
        if(mParticles[i].pos.y < .50f){
        mParticles[i].pos.y += mParticles[i].vel.y * 0.8;
        }
        else{
            mParticles[i].color.a = 0.0f;
            ready = true;
        }
    }
  }

  void updateExplosion(){
      if(glfwGetTime() - start > 5){
          stop = true;
      }
      for (int i = 0; i < explosions.size(); i ++){

            if(calculateDistance(explosions[i].pos, mParticles[0].pos) < 1){
                explosions[i].pos.x += explosions[i].vel.x * dt();
                explosions[i].pos.y += explosions[i].vel.y* dt();
                explosions[i].pos.z += explosions[i].vel.z* dt();
                explosions[i].vel.y += 0.005*dt();
            }
            else{  
                explosions[i].color = vec4(0.00, 0.3, 0.95, 0.5);
                explosions[i].size = 0.1f;
                explosions[i].pos = vec3(0, 0.5, 0);
                explosions[i].vel = agl::randomUnitSphere();
                explosions[i].vel.z = 0.0;
            }
        
      }
  }

  float calculateDistance(vec3 center, vec3 pos){
      float dist = sqrt(pow((center.x - pos.x),2) + pow((center.y - pos.y), 2) + pow((center.z - pos.z), 2));
      return dist;
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
    if(!stop){
        if (ready){
            createExplosion();
            if(!set){
                start = glfwGetTime();
                set = true;
            }
        }
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
  float start;
  bool ready = false;
  bool stop = false;
  bool set = false;

  std::vector<Particle> explosions;

  std::vector<Particle> mParticles;
};

int main(int argc, char** argv)
{
  Viewer viewer;
  viewer.run();
  return 0;
}
