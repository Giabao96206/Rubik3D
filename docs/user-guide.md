# Hướng dẫn sử dụng

## Khởi động

Build theo README rồi chạy `Rubik3D.exe`. Cửa sổ gồm viewport 3D bên trái và panel điều khiển bên phải.

## Thao tác Rubik

- Nhấn `U D L R F B` hoặc click nút cùng tên.
- Giữ Shift để tạo bước nghịch đảo, ví dụ `Shift+R` tương ứng `R'`.
- Các lệnh mới khi animation đang chạy được xếp hàng, không sửa trạng thái giữa transaction.
- `Shuffle`/Space tạo 25 bước hợp lệ, tránh hai bước liên tiếp trên cùng trục.
- Undo/Redo chỉ chạy khi hàng đợi animation rỗng.
- Reset xóa hàng đợi, trạng thái và history.
- Nhấn `A` để nhập chuỗi như `R U R' U' F2`; Enter đưa các bước hợp lệ vào hàng đợi, Backspace sửa và Esc hủy. Panel hiển thị bảy bước gần nhất.

## Camera và hiển thị

- Kéo chuột trái trong viewport để thay đổi yaw/pitch; pitch bị giới hạn để tránh lật camera.
- Cuộn để thay đổi bán kính quan sát.
- `P` hoặc nút Projection đổi perspective/orthographic.
- `W` lần lượt chuyển Solid, Wireframe, Solid+Wire, Lighting, 2D Net.
- `K`, `C`, `G` bật/tắt lighting, culling và Flat/Gouraud.
- `T` chạy camera tour Bézier 8 giây và luôn nhìn vào tâm Rubik.

## 2D Net

Nhấn `N`. Sáu mặt được trải thành hình chữ thập và đồng bộ với trạng thái 3D. Mỗi ô được tô bằng Scanline Fill, viền bằng Bresenham, đoạn minh họa được xén Cohen-Sutherland. `Q/E` quay net và `+/-` đổi tỷ lệ bằng ma trận thuần nhất 3x3.

## Trợ giúp và trạng thái

Nhấn `H` để xem overlay. Panel hiển thị solved/mixed, mode, projection, lighting/culling, shading, số bước, FPS, animation và camera tour.

