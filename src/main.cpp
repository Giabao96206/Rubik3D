#include <GL/freeglut.h>
#include <chrono>
#include <memory>
#include "Graphics/Renderer.h"
#include "UI/InputManager.h"

namespace {
using Clock=std::chrono::steady_clock;
rubik::RubiksCube cube;rubik::Camera3D camera;rubik::RotationAnimator animator;rubik::Renderer renderer;
std::unique_ptr<rubik::InputManager> input;int width=1280,height=760;Clock::time_point previous;double fps=0;int frames=0;double fpsTime=0;
void display(){renderer.render(cube,camera,animator,width,height,fps,input->showHelp(),input->algorithmEntry(),input->algorithmText(),input->algorithmMessage());}
void reshape(int w,int h){width=std::max(640,w);height=std::max(480,h);glViewport(0,0,width,height);}
void keyboard(unsigned char k,int x,int y){input->keyboard(k,x,y);glutPostRedisplay();}
void mouse(int b,int s,int x,int y){input->mouseButton(b,s,x,y,width);glutPostRedisplay();}
void motion(int x,int y){input->mouseMotion(x,y);glutPostRedisplay();}
void idle(){const auto now=Clock::now();const double dt=std::min(.1,std::chrono::duration<double>(now-previous).count());previous=now;animator.update(dt,cube);camera.update(dt);++frames;fpsTime+=dt;if(fpsTime>=.5){fps=frames/fpsTime;frames=0;fpsTime=0;}glutPostRedisplay();}
}
int main(int argc,char**argv){glutInit(&argc,argv);glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGBA|GLUT_DEPTH|GLUT_MULTISAMPLE);glutInitWindowSize(width,height);glutInitWindowPosition(80,50);glutCreateWindow("Rubik 3D - Computer Graphics Lab");glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);renderer.initialize();input=std::make_unique<rubik::InputManager>(cube,camera,animator,renderer);previous=Clock::now();glutDisplayFunc(display);glutReshapeFunc(reshape);glutKeyboardFunc(keyboard);glutMouseFunc(mouse);glutMotionFunc(motion);glutIdleFunc(idle);glutMainLoop();input.reset();return 0;}
