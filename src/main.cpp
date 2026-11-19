
#include <GL/freeglut.h>

#ifdef _WIN32
#include <imm.h>
#endif

#include <algorithm>
#include <chrono>
#include <memory>

#include "Graphics/Renderer.h"
#include "UI/InputManager.h"

using namespace std;

namespace {

    // ==========================================
    // 1. KHAI BÁO BIẾN TOÀN CỤC
    // ==========================================

    using Clock = chrono::steady_clock;

    rubik::RubiksCube cube;
    rubik::Camera3D camera;
    rubik::RotationAnimator animator;
    rubik::Renderer renderer;

    unique_ptr<rubik::InputManager> input;

    int width = 1280;
    int height = 760;

    Clock::time_point previous;

    double fps = 0;
    int frames = 0;
    double fpsTime = 0;

    // ==========================================
    // 2. HÀM HIỂN THỊ RUBIK
    // ==========================================

    void display() {
        renderer.render(
            cube,
            camera,
            animator,
            width,
            height,
            fps,
            input->showHelp(),
            input->algorithmEntry(),
            input->algorithmText(),
            input->algorithmMessage()
        );
    }

    // ==========================================
    // 3. XỬ LÝ THAY ĐỔI KÍCH THƯỚC CỬA SỔ
    // ==========================================

    void reshape(int w, int h) {
        width = max(640, w);
        height = max(480, h);

        glViewport(0, 0, width, height);
    }

    // ==========================================
    // 4. XỬ LÝ SỰ KIỆN BÀN PHÍM
    // ==========================================

    void keyboard(unsigned char k, int x, int y) {
        input->keyboard(k, x, y);

        glutPostRedisplay();
    }

    // ==========================================
    // 5. XỬ LÝ SỰ KIỆN CHUỘT
    // ==========================================

    void mouse(int b, int s, int x, int y) {
        input->mouseButton(b, s, x, y, width);

        glutPostRedisplay();
    }

    // ==========================================
    // 6. XỬ LÝ DI CHUYỂN CHUỘT
    // ==========================================

    void motion(int x, int y) {
        input->mouseMotion(x, y);

        glutPostRedisplay();
    }

    // ==========================================
    // 7. CẬP NHẬT HOẠT ẢNH VÀ FPS
    // ==========================================

    void idle() {
        const auto now = Clock::now();

        const double dt = min(
            0.1,
            chrono::duration<double>(now - previous).count()
        );

        previous = now;

        // Cập nhật hoạt ảnh xoay Rubik
        animator.update(dt, cube);

        // Cập nhật camera
        camera.update(dt);

        // Tính FPS
        ++frames;
        fpsTime += dt;

        if (fpsTime >= 0.5) {
            fps = frames / fpsTime;

            frames = 0;
            fpsTime = 0;
        }

        // Yêu cầu vẽ lại màn hình
        glutPostRedisplay();
    }
}

// ==========================================
// 8. HÀM MAIN - KHỞI CHẠY CHƯƠNG TRÌNH
// ==========================================

int main(int argc, char** argv) {

#ifdef _WIN32
    // Cửa sổ Rubik chỉ nhận lệnh ASCII. IME có thể giữ phím Enter để chốt chữ.
    ImmDisableIME(GetCurrentThreadId());
#endif

    // Khởi tạo FreeGLUT
    glutInit(&argc, argv);

    // Thiết lập chế độ hiển thị
    glutInitDisplayMode(
        GLUT_DOUBLE |
        GLUT_RGBA |
        GLUT_DEPTH |
        GLUT_MULTISAMPLE
    );

    // Thiết lập kích thước và vị trí cửa sổ
    glutInitWindowSize(width, height);
    glutInitWindowPosition(80, 50);

    // Tạo cửa sổ OpenGL
    glutCreateWindow("Rubik 3D - Computer Graphics Lab");

    // Thiết lập hành vi khi đóng cửa sổ
    glutSetOption(
        GLUT_ACTION_ON_WINDOW_CLOSE,
        GLUT_ACTION_GLUTMAINLOOP_RETURNS
    );

    // Khởi tạo Renderer
    renderer.initialize();

    // Khởi tạo InputManager
    input = make_unique<rubik::InputManager>(
        cube,
        camera,
        animator,
        renderer
    );

    // Khởi tạo thời gian
    previous = Clock::now();

    // Đăng ký các hàm callback
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutIdleFunc(idle);

    // Bắt đầu vòng lặp chính
    glutMainLoop();

    // Giải phóng bộ nhớ
    input.reset();

    return 0;
}
