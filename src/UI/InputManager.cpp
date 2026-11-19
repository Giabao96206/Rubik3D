
#include "UI/InputManager.h"

#include <GL/freeglut.h>
#include <cctype>
#include <random>
#include <string>
#include <exception>

using namespace std;

namespace rubik {

	// ==========================================
	// 1. KHỞI TẠO INPUT MANAGER
	// ==========================================

	InputManager::InputManager(
		RubiksCube& c,
		Camera3D& cam,
		RotationAnimator& a,
		Renderer& r
	) : cube_(c),
		camera_(cam),
		animator_(a),
		renderer_(r) {
	}

	// ==========================================
	// 2. THÊM LỆNH XOAY MẶT RUBIK
	// ==========================================

	void InputManager::enqueueFace(Face f, bool inv) {
		animator_.enqueue({ f, inv ? -1 : 1 });
	}

	// ==========================================
	// 3. XÁO TRỘN RUBIK NGẪU NHIÊN
	// ==========================================

	void InputManager::enqueueScramble(int count) {
		mt19937 rng(random_device{}());

		uniform_int_distribution<int> f(0, 5);
		uniform_int_distribution<int> t(0, 2);

		const int turns[] = { 1, -1, 2 };

		Face previous = Face::U;
		bool have = false;

		for (int i = 0; i < count;) {
			Move m{
				static_cast<Face>(f(rng)),
				turns[t(rng)]
			};

			if (have && axisOf(m.face) == axisOf(previous))
				continue;

			animator_.enqueue(m);

			previous = m.face;
			have = true;
			++i;
		}
	}

	// ==========================================
	// 4. XỬ LÝ SỰ KIỆN BÀN PHÍM
	// ==========================================

	void InputManager::keyboard(unsigned char key, int, int) {

		// Chế độ nhập chuỗi thuật toán Rubik
		if (algorithmEntry_) {

			// ESC: Hủy nhập thuật toán
			if (key == 27) {
				algorithmEntry_ = false;
				algorithmText_.clear();
				algorithmMessage_ = "Algorithm entry cancelled";
				return;
			}

			// ENTER: Thực hiện chuỗi thuật toán
			if (key == '\r' || key == '\n') {
				try {
					const auto moves = parseAlgorithm(algorithmText_);
					if (moves.empty()) {
						algorithmMessage_ = "Enter a move, for example R U x y'";
						return;
					}

					for (const auto& m : moves) {
						animator_.enqueue(m);
					}

					algorithmMessage_ =
						"Queued " + to_string(moves.size()) + " moves";

					algorithmEntry_ = false;
					algorithmText_.clear();
				}
				catch (const exception& e) {
					algorithmMessage_ = e.what();
				}

				return;
			}

			// BACKSPACE: Xóa ký tự cuối
			if (key == 8) {
				if (!algorithmText_.empty())
					algorithmText_.pop_back();

				algorithmMessage_.clear();
				return;
			}

			if (const auto move = parseMove(string(1, static_cast<char>(key)))) {
				algorithmText_ += toString(*move);
			}
			else if (key == '\'' || key == '2' || key == ' ') {
				algorithmText_ += static_cast<char>(key);
			}
			algorithmMessage_.clear();

			return;
		}

		// Lấy trạng thái phím Shift và Ctrl
		const int mods = glutGetModifiers();

		const bool shift = (mods & GLUT_ACTIVE_SHIFT) != 0;
		const bool ctrl = (mods & GLUT_ACTIVE_CTRL) != 0;

		const unsigned char lower =
			static_cast<unsigned char>(tolower(key));

		// ESC: Thoát chương trình
		if (key == 27) {
			glutLeaveMainLoop();
			return;
		}

		// CTRL + Z: Undo
		if (ctrl && (key == 26 || lower == 'z')) {
			if (!animator_.busy())
				cube_.undo();

			return;
		}

		// CTRL + Y: Redo
		if (ctrl && (key == 25 || lower == 'y')) {
			if (!animator_.busy())
				cube_.redo();

			return;
		}

		// Xử lý các phím chức năng
		switch (lower) {

			// Xoay các mặt Rubik
		case 'u':
			enqueueFace(Face::U, shift);
			break;

		case 'd':
			enqueueFace(Face::D, shift);
			break;

		case 'l':
			enqueueFace(Face::L, shift);
			break;

		case 'r':
			enqueueFace(Face::R, shift);
			break;

		case 'f':
			enqueueFace(Face::F, shift);
			break;

		case 'b':
			enqueueFace(Face::B, shift);
			break;

			// Xoay các lớp giữa
		case 'm':
			enqueueFace(Face::M, shift);
			break;

		case 'e':
			enqueueFace(Face::E, shift);
			break;

		case 's':
			enqueueFace(Face::S, shift);
			break;
		case 'x':
			if (!ctrl) enqueueFace(Face::X, shift);
			break;
		case 'y':
			enqueueFace(Face::Y, shift);
			break;

			// Chuyển đổi phép chiếu
		case 'p':
			camera_.toggleProjection();
			break;

			// Chuyển chế độ hiển thị
		case 'w':
			renderer_.cycleMode();
			break;

			// Hiển thị Rubik 2D
		case 'n':
			renderer_.setMode(RenderMode::Net2D);
			break;

			// Bật/tắt ánh sáng
		case 'k':
			renderer_.toggleLighting();
			break;

			// Bật/tắt loại bỏ mặt khuất
		case 'c':
			renderer_.toggleCulling();
			break;

			// Chuyển chế độ tô bóng
		case 'g':
			renderer_.toggleShading();
			break;

			// Camera di chuyển theo đường Bezier
		case 't':
			camera_.startTour();
			break;

			// Xoay lưới Rubik 2D
		case '[':
			renderer_.rotateNet(-5);
			break;

		case ']':
			renderer_.rotateNet(5);
			break;

			// Phóng to / thu nhỏ lưới 2D
		case '+':
			renderer_.scaleNet(1.08);
			break;

		case '-':
			renderer_.scaleNet(1.0 / 1.08);
			break;

			// Nhập thuật toán Rubik
		case 'a':
			algorithmEntry_ = true;
			algorithmText_.clear();
			algorithmMessage_.clear();
			break;

			// Bật/tắt hướng dẫn
		case 'h':
			showHelp_ = !showHelp_;
			break;

			// SPACE: Xáo trộn Rubik
		case ' ':
			enqueueScramble(25);
			break;

		default:
			break;
		}
	}

	// ==========================================
	// 5. XỬ LÝ NÚT CHUỘT
	// ==========================================

	void InputManager::mouseButton(
		int button,
		int state,
		int x,
		int y,
		int width
	) {
		// Cuộn chuột lên: Zoom in
		if (button == 3 && state == GLUT_DOWN) {
			camera_.zoom(-0.5);
			return;
		}

		// Cuộn chuột xuống: Zoom out
		if (button == 4 && state == GLUT_DOWN) {
			camera_.zoom(0.5);
			return;
		}

		if (button != GLUT_LEFT_BUTTON)
			return;

		if (state == GLUT_DOWN) {

			// Nhấn vào bảng điều khiển
			if (x >= width - Renderer::panelWidth()) {
				clickPanel(x, y, width);
				dragging_ = false;
			}
			else {
				// Bắt đầu kéo xoay camera
				dragging_ = true;
				lastX_ = x;
				lastY_ = y;
			}
		}
		else {
			dragging_ = false;
		}
	}

	// ==========================================
	// 6. XỬ LÝ KÉO CHUỘT XOAY CAMERA
	// ==========================================

	void InputManager::mouseMotion(int x, int y) {
		if (!dragging_)
			return;

		camera_.orbit(
			(x - lastX_) * 0.35,
			(lastY_ - y) * 0.35
		);

		lastX_ = x;
		lastY_ = y;
	}

	// ==========================================
	// 7. XỬ LÝ CLICK BẢNG ĐIỀU KHIỂN
	// ==========================================

	void InputManager::clickPanel(int x, int y, int width) {
		const int local = x - (width - Renderer::panelWidth());

		if (local < 18 || local > 238)
			return;

		// Các nút xoay lớp giữa M, E, S
		if (y >= 394 && y < 426) {
			const int slice = (local - 18) / 76;

			if (slice < 3 && (local - 18) % 76 < 68) {
				enqueueFace(
					slice == 0 ? Face::M :
					slice == 1 ? Face::E : Face::S,
					false
				);
			}

			return;
		}

		if (y < 58 || y >= 394)
			return;

		const int col = (local - 18) / 116;

		if ((local - 18) % 116 > 104)
			return;

		const int row = (y - 58) / 42;

		if ((y - 58) % 42 > 32)
			return;

		const int index = row * 2 + col;

		switch (index) {

		case 0:
			enqueueFace(Face::U, false);
			break;

		case 1:
			enqueueFace(Face::D, false);
			break;

		case 2:
			enqueueFace(Face::L, false);
			break;

		case 3:
			enqueueFace(Face::R, false);
			break;

		case 4:
			enqueueFace(Face::F, false);
			break;

		case 5:
			enqueueFace(Face::B, false);
			break;

		case 6:
			enqueueFace(Face::U, true);
			break;

		case 7:
			enqueueFace(Face::R, true);
			break;

			// Xáo trộn Rubik
		case 8:
			enqueueScramble(25);
			break;

			// Reset Rubik
		case 9:
			animator_.clear();
			cube_.reset();
			break;

			// Undo
		case 10:
			if (!animator_.busy())
				cube_.undo();
			break;

			// Redo
		case 11:
			if (!animator_.busy())
				cube_.redo();
			break;

			// Bật/tắt ánh sáng
		case 12:
			renderer_.toggleLighting();
			break;

			// Chuyển phép chiếu
		case 13:
			camera_.toggleProjection();
			break;

			// Giảm tốc độ xoay
		case 14:
			animator_.setDuration(animator_.duration() + 0.05);
			break;

			// Tăng tốc độ xoay
		case 15:
			animator_.setDuration(animator_.duration() - 0.05);
			break;

		default:
			break;
		}
	}

} // namespace rubik
