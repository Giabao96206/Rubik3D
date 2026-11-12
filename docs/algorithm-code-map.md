# Bản đồ thuật toán trong mã nguồn

Tài liệu này dùng để tra nhanh **thuật toán nằm ở file nào, hàm nào, đoạn nào và được sử dụng ở đâu**. Các liên kết dòng trỏ trực tiếp tới mã nguồn trên GitHub.

## Thuật toán đồ họa 2D

| Thuật toán | Đoạn cài đặt | Nơi sử dụng | Chức năng |
|---|---|---|---|
| Bresenham Line | [`bresenhamLine()` - `src/Algorithms/RasterAlgorithms.cpp`, dòng 12](../src/Algorithms/RasterAlgorithms.cpp#L12) | [`RasterBuffer::drawLine()`, dòng 61](../src/Algorithms/RasterAlgorithms.cpp#L61), [`Renderer::drawNet()`, dòng 68](../src/Graphics/Renderer.cpp#L68) | Tính các pixel gần đoạn thẳng bằng số nguyên; dùng vẽ viền sticker trong 2D Net. |
| Cohen-Sutherland | [`cohenSutherlandClip()` - dòng 26](../src/Algorithms/RasterAlgorithms.cpp#L26) | [`Renderer::drawNet()`, dòng 80](../src/Graphics/Renderer.cpp#L80) | Xén đoạn thẳng vào cửa sổ chữ nhật bằng mã vùng 4 bit. |
| Scanline Fill | [`scanlineFill()` - dòng 42](../src/Algorithms/RasterAlgorithms.cpp#L42) | [`RasterBuffer::fillPolygon()`, dòng 62](../src/Algorithms/RasterAlgorithms.cpp#L62), [`Renderer::drawNet()`, dòng 68](../src/Graphics/Renderer.cpp#L68) | Tìm giao điểm theo từng dòng quét và tô các khoảng pixel nằm trong đa giác. |
| Raster Buffer | [`RasterBuffer::drawLine()` và `fillPolygon()` - dòng 61-62](../src/Algorithms/RasterAlgorithms.cpp#L61-L62) | [`glDrawPixels()` trong `Renderer::drawNet()`, dòng 82](../src/Graphics/Renderer.cpp#L82) | Lưu kết quả raster RGB trên CPU rồi đưa toàn bộ buffer lên màn hình. |
| Biến đổi 2D thuần nhất | [`Matrix3x3::translation/scaling/rotation()` - dòng 11-16](../src/Math/Matrix3x3.cpp#L11-L16) | [Ma trận kết hợp của 2D Net - `Renderer.cpp`, dòng 71](../src/Graphics/Renderer.cpp#L71) | Tịnh tiến, quay và thay đổi tỷ lệ 2D Net bằng ma trận 3x3. |

## Toán và phép biến đổi 3D

| Thuật toán | Đoạn cài đặt | Nơi sử dụng | Chức năng |
|---|---|---|---|
| Ma trận tịnh tiến, tỷ lệ, quay 3D | [`Matrix4x4::translation/scaling/rotationX/Y/Z()` - dòng 12-16](../src/Math/Matrix4x4.cpp#L12-L16) | Camera, model và các bài test ma trận | Tự tạo ma trận biến đổi 4x4 theo quy ước vector cột `P' = M * P`. |
| Nhân ma trận 4x4 | [`Matrix4x4::operator*()` - dòng 38](../src/Math/Matrix4x4.cpp#L38) | Các phép biến đổi kết hợp `T * R * S` | Kết hợp nhiều phép biến đổi theo đúng thứ tự. |
| Biến đổi điểm/vector | [`transformPoint()` và `transformVector()` - dòng 45-57](../src/Math/Matrix4x4.cpp#L45-L57) | Test ma trận và xử lý tọa độ | Nhân điểm thuần nhất hoặc vector hướng với ma trận. |
| Perspective Projection | [`Matrix4x4::perspective()` - dòng 18](../src/Math/Matrix4x4.cpp#L18) | [`Camera3D::projectionMatrix()`, dòng 16](../src/Graphics/Camera3D.cpp#L16) | Tạo phép chiếu gần lớn, xa nhỏ theo FOV, aspect, near và far plane. |
| Orthographic Projection | [`Matrix4x4::orthographic()` - dòng 24](../src/Math/Matrix4x4.cpp#L24) | [`Camera3D::projectionMatrix()`, dòng 16](../src/Graphics/Camera3D.cpp#L16) | Tạo phép chiếu song song, kích thước không giảm theo độ sâu. |
| View/LookAt | [`Matrix4x4::lookAt()` - dòng 29](../src/Math/Matrix4x4.cpp#L29) | [`Camera3D::viewMatrix()`, dòng 15](../src/Graphics/Camera3D.cpp#L15) | Xây dựng hệ trục camera từ hướng nhìn, vector up và vị trí mắt. |

## Mô hình và logic Rubik

| Thuật toán | Đoạn cài đặt | Nơi sử dụng | Chức năng |
|---|---|---|---|
| Tạo Face Model/Wireframe | [`Mesh::unitCube()` - `src/Geometry/Mesh.cpp`, dòng 5](../src/Geometry/Mesh.cpp#L5) | [`Renderer::drawCubie()`, dòng 46](../src/Graphics/Renderer.cpp#L46) | Tạo một cubie từ 8 đỉnh, 12 cạnh và 6 mặt; Solid duyệt mặt, Wireframe duyệt cạnh. |
| Ánh xạ ký hiệu bước quay | [`signedAxisTurns()` - `RubikMoves.cpp`, dòng 49](../src/Rubik/RubikMoves.cpp#L49) | [`RubiksCube::applyMove()`, dòng 75](../src/Rubik/RubiksCube.cpp#L75) | Chuyển U/D/L/R/F/B, dấu `'` và `2` thành trục, layer và chiều quay. |
| Quay Rubik rời rạc | [`RubiksCube::rotateDiscrete()` - dòng 60](../src/Rubik/RubiksCube.cpp#L60) | [`applyMove()`, dòng 75](../src/Rubik/RubiksCube.cpp#L75) | Cập nhật tọa độ nguyên và hướng sticker của đúng 9 cubie, không tích lũy sai số float. |
| Phân tích chuỗi Rubik | [`parseAlgorithm()` - `RubikMoves.cpp`, dòng 34](../src/Rubik/RubikMoves.cpp#L34) | [Ô nhập thuật toán - `InputManager.cpp`, dòng 13](../src/UI/InputManager.cpp#L13) | Đọc chuỗi như `R U R' U' F2` thành danh sách `Move`. |
| Kiểm tra hoàn thành | [`RubiksCube::isSolved()` - dòng 83](../src/Rubik/RubiksCube.cpp#L83) | Panel trạng thái và test Rubik | So sánh vị trí/hướng sticker hiện tại với trạng thái solved ban đầu. |
| Shuffle | [`RubiksCube::scramble()` - dòng 101](../src/Rubik/RubiksCube.cpp#L101) | Nút Shuffle và Space | Sinh bước ngẫu nhiên hợp lệ, tránh hai bước liên tiếp trên cùng trục. |
| Undo/Redo | [`undo()` và `redo()` - dòng 115-123](../src/Rubik/RubiksCube.cpp#L115-L123) | Nút Undo/Redo, `Ctrl+Z`, `Ctrl+Y` | Undo áp dụng bước nghịch đảo; redo thực hiện lại bước đã hoàn tác. |

## Animation và đường cong

| Thuật toán | Đoạn cài đặt | Nơi sử dụng | Chức năng |
|---|---|---|---|
| Animation theo deltaTime | [`RotationAnimator::update()` - dòng 15](../src/Rubik/RotationAnimator.cpp#L15) | Vòng lặp `idle()` trong [`main.cpp`](../src/main.cpp#L16) | Quản lý hàng đợi transaction; chỉ commit logic khi animation kết thúc. |
| Smoothstep easing | [`visualAngleDegrees()` - dòng 9](../src/Rubik/RotationAnimator.cpp#L9) | Góc quay tạm của layer trong renderer | Dùng `t²(3-2t)` để vận tốc đầu/cuối bằng 0 và chuyển động mượt hơn. |
| de Casteljau/Bézier | [`deCasteljau()` - `Bezier.cpp`, dòng 6](../src/Algorithms/Bezier.cpp#L6) | [`Camera3D::position()`, dòng 14](../src/Graphics/Camera3D.cpp#L14) | Nội suy lặp các điểm kiểm soát để tạo quỹ đạo Camera Tour. |
| Lấy mẫu đường Bézier | [`sampleBezier()` - dòng 13](../src/Algorithms/Bezier.cpp#L13) | API thuật toán và test | Tạo danh sách điểm gần đúng trên đường cong theo số segment. |

## Khử khuất, chiếu sáng và tô bóng

| Thuật toán/kỹ thuật | Đoạn cài đặt | Nơi sử dụng | Chức năng |
|---|---|---|---|
| Tính pháp tuyến và kiểm tra mặt trước | [`isFrontFacing()` - `Lighting.cpp`, dòng 7](../src/Graphics/Lighting.cpp#L7) | Đối chiếu với Back-face Culling trong renderer | Tính `N=(B-A)x(C-A)` và kiểm tra dấu `N.(Eye-A)`. |
| Back-face Culling | [Bật/tắt `GL_CULL_FACE` - `Renderer.cpp`, dòng 38](../src/Graphics/Renderer.cpp#L38) | Viewport 3D, phím C | Loại bỏ polygon quay lưng theo winding CCW. Phần tính toán nguyên lý CPU nằm trong `isFrontFacing()`. |
| Z-buffer/Depth Buffer | [Khởi tạo `GL_DEPTH_TEST` - dòng 26](../src/Graphics/Renderer.cpp#L26) | Viewport 3D | So sánh độ sâu fragment để mặt gần che mặt xa. Đây là depth buffer phần cứng OpenGL. |
| Lambert Lighting | [`lambertIntensity()` - `Lighting.cpp`, dòng 5](../src/Graphics/Lighting.cpp#L5) | [`Renderer::drawCubie()`, dòng 58](../src/Graphics/Renderer.cpp#L58) | Tính `I = IaKa + IdKd max(0, N dot L)` và nhân với màu sticker. |
| Flat Shading | [Chọn pháp tuyến mặt - `Renderer.cpp`, dòng 57-60](../src/Graphics/Renderer.cpp#L57-L60) | Lighting mode, phím G | Dùng một pháp tuyến cho toàn mặt nên cường độ trên mặt gần như đồng nhất. |
| Gouraud Shading | [Chọn pháp tuyến đỉnh và màu đỉnh - dòng 57-60](../src/Graphics/Renderer.cpp#L57-L60) | Lighting mode, phím G | Tính sáng tại từng đỉnh rồi để OpenGL nội suy màu trên polygon. |

## Vị trí kiểm thử

| Nhóm | File test | Nội dung |
|---|---|---|
| Ma trận 2D/3D | [`tests/test_matrix.cpp`](../tests/test_matrix.cpp) | Identity, tịnh tiến, quay 90 độ, `T*R*S`, tọa độ thuần nhất. |
| Rubik và animation | [`tests/test_rubik.cpp`](../tests/test_rubik.cpp) | `U^4`, `R R'`, `F2 F2`, chuỗi nghịch đảo, shuffle, undo/redo, transaction animation. |
| Raster, Bézier, Lambert | [`tests/test_algorithms.cpp`](../tests/test_algorithms.cpp) | Bresenham tám octant, Cohen-Sutherland, Scanline Fill, de Casteljau, Lambert và Camera Tour. |

> Ghi chú: số dòng tương ứng với phiên bản trên nhánh `main`. Nếu mã được format hoặc bổ sung, hãy ưu tiên tìm theo **tên hàm** ghi trong bảng.
