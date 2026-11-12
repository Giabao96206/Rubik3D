#include "Graphics/Renderer.h"

#include <GL/freeglut.h>
#include <algorithm>
#include <array>
#include <cmath>
#include <sstream>
#include "Algorithms/CohenSutherland.h"
#include "Algorithms/RasterBuffer.h"
#include "Math/Matrix3x3.h"

namespace rubik {
namespace {
std::array<float,3> colorOf(StickerColor color){
    switch(color){case StickerColor::White:return {0.96f,0.96f,0.92f};case StickerColor::Yellow:return {1.0f,.82f,.05f};case StickerColor::Red:return {.88f,.08f,.08f};case StickerColor::Orange:return {1.0f,.35f,.04f};case StickerColor::Green:return {.05f,.68f,.22f};case StickerColor::Blue:return {.08f,.28f,.92f};default:return {.035f,.04f,.05f};}
}
std::array<std::uint8_t,3> byteColor(StickerColor c){auto f=colorOf(c);return {static_cast<std::uint8_t>(f[0]*255),static_cast<std::uint8_t>(f[1]*255),static_cast<std::uint8_t>(f[2]*255)};}
void text(float x,float y,const std::string&s,void*font=GLUT_BITMAP_HELVETICA_12){glRasterPos2f(x,y);for(unsigned char c:s)glutBitmapCharacter(font,c);}
void rect(float x,float y,float w,float h,float r,float g,float b){glColor3f(r,g,b);glBegin(GL_QUADS);glVertex2f(x,y);glVertex2f(x+w,y);glVertex2f(x+w,y+h);glVertex2f(x,y+h);glEnd();}
bool layerContains(const Cubie&c,const Move&m){const Axis a=axisOf(m.face);const int v=a==Axis::X?c.position.x:a==Axis::Y?c.position.y:c.position.z;return v==layerOf(m.face);}
void applyVisualRotation(const Move&m,double angle){const Axis a=axisOf(m.face);glRotated(angle,a==Axis::X?1:0,a==Axis::Y?1:0,a==Axis::Z?1:0);}
}

std::string renderModeName(RenderMode m){switch(m){case RenderMode::Solid:return "Solid";case RenderMode::Wireframe:return "Wireframe";case RenderMode::SolidWire:return "Solid + Wire";case RenderMode::Lighting:return "Lighting";case RenderMode::Net2D:return "2D Net";}return "Unknown";}
Renderer::Renderer():cubeMesh_(Mesh::unitCube()){}
void Renderer::initialize(){glClearColor(.025f,.032f,.045f,1);glEnable(GL_DEPTH_TEST);glDepthFunc(GL_LEQUAL);glLineWidth(1.6f);}
void Renderer::cycleMode(){mode_=static_cast<RenderMode>((static_cast<int>(mode_)+1)%5);}
void Renderer::scaleNet(double factor){netScale_=std::clamp(netScale_*factor,.55,1.65);}

void Renderer::render(const RubiksCube&cube,const Camera3D&camera,const RotationAnimator&anim,int w,int h,double fps,bool help,bool algorithmEntry,const std::string&algorithmText,const std::string&algorithmMessage){
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    const int content=std::max(1,w-panelWidth());
    if(mode_==RenderMode::Net2D)drawNet(cube,content,h);else drawScene3D(cube,camera,anim,content,h);
    drawPanel(cube,camera,anim,w,h,fps,help,algorithmEntry,algorithmText,algorithmMessage);glutSwapBuffers();
}

void Renderer::drawScene3D(const RubiksCube&cube,const Camera3D&camera,const RotationAnimator&anim,int w,int h){
    glViewport(0,0,w,h);glEnable(GL_DEPTH_TEST);if(culling_){glEnable(GL_CULL_FACE);glCullFace(GL_BACK);glFrontFace(GL_CCW);}else glDisable(GL_CULL_FACE);
    glMatrixMode(GL_PROJECTION);glLoadMatrixf(camera.projectionMatrix(static_cast<double>(w)/std::max(1,h)).toColumnMajorFloat().data());
    glMatrixMode(GL_MODELVIEW);glLoadMatrixf(camera.viewMatrix().toColumnMajorFloat().data());
    glBegin(GL_LINES);glColor3f(1,0.25f,.25f);glVertex3f(0,0,0);glVertex3f(4,0,0);glColor3f(.25f,1,.3f);glVertex3f(0,0,0);glVertex3f(0,4,0);glColor3f(.3f,.5f,1);glVertex3f(0,0,0);glVertex3f(0,0,4);glEnd();
    for(const auto&c:cube.cubies())if(c.visible)drawCubie(c,anim);
}

void Renderer::drawCubie(const Cubie&c,const RotationAnimator&anim){
    glPushMatrix();if(anim.activeMove()&&layerContains(c,*anim.activeMove()))applyVisualRotation(*anim.activeMove(),anim.visualAngleDegrees());
    constexpr double spacing=1.04,size=.96;glTranslated(c.position.x*spacing,c.position.y*spacing,c.position.z*spacing);glScaled(size,size,size);
    const bool solid=mode_!=RenderMode::Wireframe;
    if(solid){
        for(const auto&face:cubeMesh_.faces){
            const auto base=colorOf(c.stickers[face.direction]);glBegin(GL_QUADS);
            for(int index:face.vertices){
                double intensity=1.0;
                if(lighting_||mode_==RenderMode::Lighting){
                    const auto&p=cubeMesh_.vertices[index].position;
                    const Vector3D n=shading_==ShadingMode::Flat?face.normal:Vector3D{p.x,p.y,p.z}.normalized();
                    intensity=lambertIntensity(n,p,light_);
                }
                const auto shaded=shadeColor(base,intensity);glColor3fv(shaded.data());const auto&p=cubeMesh_.vertices[index].position;glVertex3d(p.x,p.y,p.z);
            }glEnd();
        }
    }
    if(mode_==RenderMode::Wireframe||mode_==RenderMode::SolidWire){glDisable(GL_CULL_FACE);glColor3f(.01f,.012f,.016f);glBegin(GL_LINES);for(const auto&e:cubeMesh_.edges){const auto&a=cubeMesh_.vertices[e.begin].position;const auto&b=cubeMesh_.vertices[e.end].position;glVertex3d(a.x,a.y,a.z);glVertex3d(b.x,b.y,b.z);}glEnd();if(culling_)glEnable(GL_CULL_FACE);}
    glPopMatrix();
}

void Renderer::drawNet(const RubiksCube&cube,int w,int h){
    glViewport(0,0,w,h);glDisable(GL_DEPTH_TEST);glDisable(GL_CULL_FACE);RasterBuffer buffer(std::max(1,w),std::max(1,h));buffer.clear({8,12,20});
    const int cell=std::max(12,std::min(w/18,h/14)), ox=(w-12*cell)/2,oy=(h-9*cell)/2;
    constexpr double pi=3.14159265358979323846;const Point2D center{w*.5,h*.5};
    const Matrix3x3 netTransform=Matrix3x3::translation(center.x+netOffset_.x,center.y+netOffset_.y)*Matrix3x3::rotation(netAngleDegrees_*pi/180.0)*Matrix3x3::scaling(netScale_,netScale_)*Matrix3x3::translation(-center.x,-center.y);
    const std::array<std::pair<Face,std::pair<int,int>>,6> faces={{{Face::U,{3,6}},{Face::L,{0,3}},{Face::F,{3,3}},{Face::R,{6,3}},{Face::B,{9,3}},{Face::D,{3,0}}}};
    for(const auto&entry:faces)for(int row=0;row<3;++row)for(int col=0;col<3;++col){
        const int x=ox+(entry.second.first+col)*cell,y=oy+(entry.second.second+(2-row))*cell;
        std::vector<PixelPoint> poly;for(const Point2D p:std::array<Point2D,4>{{{static_cast<double>(x+2),static_cast<double>(y+2)},{static_cast<double>(x+cell-2),static_cast<double>(y+2)},{static_cast<double>(x+cell-2),static_cast<double>(y+cell-2)},{static_cast<double>(x+2),static_cast<double>(y+cell-2)}}}){const auto q=netTransform.transformPoint(p);poly.push_back({static_cast<int>(std::lround(q.x)),static_cast<int>(std::lround(q.y))});}
        buffer.fillPolygon(poly,byteColor(cube.colorAt(entry.first,row,col)));for(std::size_t i=0;i<poly.size();++i)buffer.drawLine(poly[i],poly[(i+1)%poly.size()],{22,25,32});
    }
    // Đoạn minh họa được xén Cohen-Sutherland trước khi raster hóa Bresenham.
    if(auto clipped=cohenSutherlandClip({{-50.0,20.0},{static_cast<double>(w+40),static_cast<double>(h-30)}},{0,0,static_cast<double>(w-1),static_cast<double>(h-1)}))
        buffer.drawLine({static_cast<int>(clipped->a.x),static_cast<int>(clipped->a.y)},{static_cast<int>(clipped->b.x),static_cast<int>(clipped->b.y)},{42,54,70});
    glMatrixMode(GL_PROJECTION);glLoadIdentity();glOrtho(0,w,0,h,-1,1);glMatrixMode(GL_MODELVIEW);glLoadIdentity();glPixelStorei(GL_UNPACK_ALIGNMENT,1);glRasterPos2i(0,0);glDrawPixels(buffer.width(),buffer.height(),GL_RGB,GL_UNSIGNED_BYTE,buffer.data().data());
}

void Renderer::drawPanel(const RubiksCube&cube,const Camera3D&camera,const RotationAnimator&anim,int w,int h,double fps,bool help,bool algorithmEntry,const std::string&algorithmText,const std::string&algorithmMessage){
    glViewport(0,0,w,h);glDisable(GL_DEPTH_TEST);glDisable(GL_CULL_FACE);glMatrixMode(GL_PROJECTION);glLoadIdentity();glOrtho(0,w,h,0,-1,1);glMatrixMode(GL_MODELVIEW);glLoadIdentity();
    const float x=static_cast<float>(w-panelWidth());rect(x,0,static_cast<float>(panelWidth()),static_cast<float>(h),.055f,.065f,.085f);rect(x,0,4.0f,static_cast<float>(h),.15f,.58f,.95f);
    glColor3f(.9f,.94f,1);text(x+22,34,"RUBIK 3D LAB",GLUT_BITMAP_HELVETICA_18);
    const char* buttons[]={"U","D","L","R","F","B","U'","R'","Shuffle","Reset","Undo","Redo","Light","Projection","Speed -","Speed +"};
    for(int i=0;i<16;++i){const int col=i%2,row=i/2;const float bx=x+18.0f+static_cast<float>(col*116),by=58.0f+static_cast<float>(row*42);rect(bx,by,104.0f,32.0f,.105f,.125f,.16f);glColor3f(.82f,.88f,.98f);text(bx+12,by+21,buttons[i]);}
    std::ostringstream status;status<<"State: "<<(cube.isSolved()?"SOLVED":"MIXED");glColor3f(cube.isSolved()?.35f:1,cube.isSolved()?1:.72f,.45f);text(x+20,414,status.str());
    glColor3f(.72f,.78f,.88f);text(x+20,436,"Mode: "+renderModeName(mode_));text(x+20,458,std::string("Projection: ")+(camera.projectionMode()==ProjectionMode::Perspective?"Perspective":"Orthographic"));
    text(x+20,480,std::string("Lighting: ")+(lighting_?"On":"Off")+" | Culling: "+(culling_?"On":"Off"));
    text(x+20,502,std::string("Shading: ")+(shading_==ShadingMode::Flat?"Flat":"Gouraud"));
    status.str("");status<<"Moves: "<<cube.history().done.size()<<" | FPS: "<<static_cast<int>(fps+.5);text(x+20,524,status.str());
    status.str("");status<<"Animation: "<<(anim.busy()?"running":"idle")<<" | "<<anim.duration()<<"s";text(x+20,546,status.str());
    text(x+20,568,std::string("Camera tour: ")+(camera.tourActive()?"running":"idle"));
    if(anim.activeMove())text(x+20,590,"Current: "+toString(*anim.activeMove()));
    std::string recent="Recent:";const auto&done=cube.history().done;const std::size_t begin=done.size()>7?done.size()-7:0;for(std::size_t i=begin;i<done.size();++i)recent+=" "+toString(done[i]);text(x+20,612,recent);
    if(!algorithmMessage.empty())text(x+20,634,algorithmMessage);
    glColor3f(.56f,.64f,.76f);text(x+20,static_cast<float>(h-78),"A algorithm | W modes | N net");text(x+20,static_cast<float>(h-58),"T camera tour | Q/E net rotate");text(x+20,static_cast<float>(h-38),"Drag orbit | Wheel zoom | H help");
    if(help){rect(24,24,static_cast<float>(std::max(260,w-panelWidth()-48)),204,.04f,.05f,.07f);glColor3f(.9f,.94f,1);text(44,54,"Controls",GLUT_BITMAP_HELVETICA_18);text(44,82,"U D L R F B: turn face; hold Shift for inverse");text(44,106,"Space: animated scramble | Ctrl+Z/Y: undo/redo");text(44,130,"P: projection | W: render modes | N: 2D raster net");text(44,154,"K: Lambert light | G: Flat/Gouraud | C: culling");text(44,178,"T: Bezier camera tour | Q/E: rotate net | +/-: scale net");text(44,202,"Esc: exit");}
    if(algorithmEntry){const float overlayWidth=static_cast<float>(std::max(300,w-panelWidth()-96));rect(48,static_cast<float>(h-96),overlayWidth,56,.045f,.055f,.075f);glColor3f(.9f,.94f,1);text(66,static_cast<float>(h-72),"Algorithm (Enter to queue, Esc to cancel):",GLUT_BITMAP_HELVETICA_12);glColor3f(.3f,.75f,1);text(66,static_cast<float>(h-50),algorithmText+"_");}
}
} // namespace rubik

