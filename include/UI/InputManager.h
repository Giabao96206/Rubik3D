#pragma once
#include <string>
#include "Graphics/Renderer.h"
namespace rubik {
class InputManager {
public:
    InputManager(RubiksCube&cube,Camera3D&camera,RotationAnimator&animator,Renderer&renderer);
    void keyboard(unsigned char key,int x,int y);
    void mouseButton(int button,int state,int x,int y,int windowWidth);
    void mouseMotion(int x,int y);
    bool showHelp()const{return showHelp_;}
    bool algorithmEntry()const{return algorithmEntry_;}
    const std::string& algorithmText()const{return algorithmText_;}
    const std::string& algorithmMessage()const{return algorithmMessage_;}
private:
    RubiksCube&cube_;Camera3D&camera_;RotationAnimator&animator_;Renderer&renderer_;
    bool dragging_{false},showHelp_{false};int lastX_{},lastY_{};
    bool algorithmEntry_{false};std::string algorithmText_,algorithmMessage_;
    void enqueueFace(Face face,bool inverseMove);
    void enqueueScramble(int count);
    void clickPanel(int x,int y,int windowWidth);
};
}

