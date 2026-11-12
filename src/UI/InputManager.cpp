#include "UI/InputManager.h"
#include <GL/freeglut.h>
#include <cctype>
#include <random>

namespace rubik {
InputManager::InputManager(RubiksCube&c,Camera3D&cam,RotationAnimator&a,Renderer&r):cube_(c),camera_(cam),animator_(a),renderer_(r){}
void InputManager::enqueueFace(Face f,bool inv){animator_.enqueue({f,inv?-1:1});}
void InputManager::enqueueScramble(int count){std::mt19937 rng(std::random_device{}());std::uniform_int_distribution<int>f(0,5),t(0,2);const int turns[]={1,-1,2};Face previous=Face::U;bool have=false;for(int i=0;i<count;){Move m{static_cast<Face>(f(rng)),turns[t(rng)]};if(have&&axisOf(m.face)==axisOf(previous))continue;animator_.enqueue(m);previous=m.face;have=true;++i;}}
void InputManager::keyboard(unsigned char key,int,int){
    if(algorithmEntry_){
        if(key==27){algorithmEntry_=false;algorithmText_.clear();algorithmMessage_="Algorithm entry cancelled";return;}
        if(key==13){try{const auto moves=parseAlgorithm(algorithmText_);for(const auto&m:moves)animator_.enqueue(m);algorithmMessage_="Queued "+std::to_string(moves.size())+" moves";algorithmEntry_=false;algorithmText_.clear();}catch(const std::exception&e){algorithmMessage_=e.what();}return;}
        if(key==8){if(!algorithmText_.empty())algorithmText_.pop_back();return;}
        const unsigned char upper=static_cast<unsigned char>(std::toupper(key));if(upper=='U'||upper=='D'||upper=='L'||upper=='R'||upper=='F'||upper=='B'||upper=='M'||upper=='E'||upper=='S')algorithmText_+=static_cast<char>(upper);else if(key=='\''||key=='2'||key==' ')algorithmText_+=static_cast<char>(key);return;
    }
    const int mods=glutGetModifiers();const bool shift=(mods&GLUT_ACTIVE_SHIFT)!=0,ctrl=(mods&GLUT_ACTIVE_CTRL)!=0;const unsigned char lower=static_cast<unsigned char>(std::tolower(key));
    if(key==27){glutLeaveMainLoop();return;}if(ctrl&&lower=='z'){if(!animator_.busy())cube_.undo();return;}if(ctrl&&lower=='y'){if(!animator_.busy())cube_.redo();return;}
    switch(lower){case 'u':enqueueFace(Face::U,shift);break;case 'd':enqueueFace(Face::D,shift);break;case 'l':enqueueFace(Face::L,shift);break;case 'r':enqueueFace(Face::R,shift);break;case 'f':enqueueFace(Face::F,shift);break;case 'b':enqueueFace(Face::B,shift);break;case 'm':enqueueFace(Face::M,shift);break;case 'e':enqueueFace(Face::E,shift);break;case 's':enqueueFace(Face::S,shift);break;
      case 'p':camera_.toggleProjection();break;case 'w':renderer_.cycleMode();break;case 'n':renderer_.setMode(RenderMode::Net2D);break;case 'k':renderer_.toggleLighting();break;case 'c':renderer_.toggleCulling();break;case 'g':renderer_.toggleShading();break;case 't':camera_.startTour();break;case '[':renderer_.rotateNet(-5);break;case ']':renderer_.rotateNet(5);break;case '+':renderer_.scaleNet(1.08);break;case '-':renderer_.scaleNet(1.0/1.08);break;case 'a':algorithmEntry_=true;algorithmText_.clear();algorithmMessage_.clear();break;case 'h':showHelp_=!showHelp_;break;case ' ':enqueueScramble(25);break;default:break;}
}
void InputManager::mouseButton(int button,int state,int x,int y,int width){
    if(button==3&&state==GLUT_DOWN){camera_.zoom(-.5);return;}if(button==4&&state==GLUT_DOWN){camera_.zoom(.5);return;}
    if(button!=GLUT_LEFT_BUTTON)return;if(state==GLUT_DOWN){if(x>=width-Renderer::panelWidth()){clickPanel(x,y,width);dragging_=false;}else{dragging_=true;lastX_=x;lastY_=y;}}else dragging_=false;
}
void InputManager::mouseMotion(int x,int y){if(!dragging_)return;camera_.orbit((x-lastX_)*.35,(lastY_-y)*.35);lastX_=x;lastY_=y;}
void InputManager::clickPanel(int x,int y,int width){const int local=x-(width-Renderer::panelWidth());if(local<18||local>238)return;if(y>=394&&y<426){const int slice=(local-18)/76;if(slice<3&&(local-18)%76<68)enqueueFace(slice==0?Face::M:slice==1?Face::E:Face::S,false);return;}if(y<58||y>=394)return;const int col=(local-18)/116;if((local-18)%116>104)return;const int row=(y-58)/42;if((y-58)%42>32)return;const int index=row*2+col;
    switch(index){case 0:enqueueFace(Face::U,false);break;case 1:enqueueFace(Face::D,false);break;case 2:enqueueFace(Face::L,false);break;case 3:enqueueFace(Face::R,false);break;case 4:enqueueFace(Face::F,false);break;case 5:enqueueFace(Face::B,false);break;case 6:enqueueFace(Face::U,true);break;case 7:enqueueFace(Face::R,true);break;case 8:enqueueScramble(25);break;case 9:animator_.clear();cube_.reset();break;case 10:if(!animator_.busy())cube_.undo();break;case 11:if(!animator_.busy())cube_.redo();break;case 12:renderer_.toggleLighting();break;case 13:camera_.toggleProjection();break;case 14:animator_.setDuration(animator_.duration()+.05);break;case 15:animator_.setDuration(animator_.duration()-.05);break;default:break;}}
}

