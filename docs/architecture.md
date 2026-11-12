# Kiến trúc dự án

## Phân lớp

`rubik_core` không phụ thuộc OpenGL, nên test toán và Rubik chạy được trên máy không có cửa sổ đồ họa.

- `Math`: `Vector2D`, `Vector3D`, `Matrix3x3`, `Matrix4x4`. Quy ước vector cột `P' = M * P`; dữ liệu nội bộ row-major và được chuyển sang column-major trước khi gửi OpenGL.
- `Geometry`: `Vertex`, `Edge`, `PolygonFace`, `Mesh`. Cube có đúng 8 đỉnh, 12 cạnh, 6 mặt; winding CCW nhìn từ bên ngoài.
- `Rubik`: `Cubie`, `RubiksCube`, `RubikMoves`, `MoveHistory`, `RotationAnimator`. Đây là nguồn sự thật của trạng thái Rubik.
- `Algorithms`: Bresenham, Cohen-Sutherland, Scanline Fill, RasterBuffer và de Casteljau.
- `Graphics`: camera, projection, lighting và renderer FreeGLUT.
- `UI`: ánh xạ bàn phím, chuột và nút panel sang lệnh logic/animation.

## Dòng dữ liệu một bước quay

1. `InputManager` tạo `Move` và đưa vào `RotationAnimator`.
2. Animator chọn một transaction, tăng góc nhìn theo `deltaTime` và smoothstep.
3. Renderer chỉ xoay tạm 9 cubie của layer đang hoạt động.
4. Khi đủ thời gian, animator gọi `RubiksCube::applyMove` đúng một lần.
5. Cube cập nhật vị trí nguyên và hướng sticker bằng phép quay rời rạc, rồi ghi history.

Vì trạng thái logic không tích lũy float, hàng nghìn bước quay không làm cubie trôi khỏi lưới.

## Trạng thái Cubie

Mỗi cubie có:

- `id` và `home` không đổi;
- `position` nguyên hiện tại;
- mảng 6 sticker theo hướng thế giới `+X,-X,+Y,-Y,+Z,-Z`;
- bản sao solved để kiểm tra `isSolved`;
- cờ `visible`, lõi `(0,0,0)` không render.

## Render pipeline

`projection * view * model * vertex` được OpenGL fixed pipeline thực hiện bằng các ma trận do chương trình tự dựng. Mặt dùng Face Model; wireframe duyệt danh sách Edge. Depth test xử lý độ sâu theo pixel; culling loại mặt có winding sau. Lighting được tính trên CPU để công thức Lambert/Gouraud dễ trình bày.

## Phụ thuộc

FreeGLUT chỉ liên kết với executable `Rubik3D`. `rubik_core` và `rubik_tests` không liên kết OpenGL/FreeGLUT.

