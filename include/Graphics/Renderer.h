#pragma once
#include <string>
#include "Geometry/Mesh.h"
#include "Graphics/Camera3D.h"
#include "Graphics/Lighting.h"
#include "Rubik/RotationAnimator.h"
#include "Math/Vector2D.h"

namespace rubik {
enum class RenderMode { Solid, Wireframe, SolidWire, Lighting, Net2D };
enum class ShadingMode { Flat, Gouraud };

class Renderer {
public:
    Renderer();
    void initialize();
    void render(const RubiksCube& cube,const Camera3D& camera,const RotationAnimator& animator,int width,int height,double fps,bool showHelp,bool algorithmEntry,const std::string& algorithmText,const std::string& algorithmMessage);
    void cycleMode();
    void setMode(RenderMode mode){mode_=mode;}
    RenderMode mode()const{return mode_;}
    void toggleLighting(){lighting_=!lighting_;}
    void toggleCulling(){culling_=!culling_;}
    void toggleShading(){shading_=shading_==ShadingMode::Flat?ShadingMode::Gouraud:ShadingMode::Flat;}
    bool lighting()const{return lighting_;} bool culling()const{return culling_;}
    void rotateNet(double degrees){netAngleDegrees_+=degrees;}
    void scaleNet(double factor);
    void moveNet(double dx,double dy){netOffset_.x+=dx;netOffset_.y+=dy;}
    static constexpr int panelWidth(){return 270;}
private:
    Mesh cubeMesh_;
    RenderMode mode_{RenderMode::SolidWire};
    ShadingMode shading_{ShadingMode::Flat};
    bool lighting_{true}; bool culling_{true}; Light light_{};
    double netAngleDegrees_{0.0},netScale_{1.0}; Vector2D netOffset_{};
    void drawScene3D(const RubiksCube&,const Camera3D&,const RotationAnimator&,int,int);
    void drawCubie(const Cubie&,const RotationAnimator&);
    void drawNet(const RubiksCube&,int,int);
    void drawPanel(const RubiksCube&,const Camera3D&,const RotationAnimator&,int,int,double,bool,bool,const std::string&,const std::string&);
};
std::string renderModeName(RenderMode mode);
}

