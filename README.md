# Rubik 3D - Computer Graphics Lab

Ứng dụng mô phỏng Rubik 3x3x3 bằng C++17, OpenGL và FreeGLUT. Dự án tách logic Rubik khỏi renderer, tự cài đặt toán ma trận, thuật toán raster 2D, phép chiếu, chiếu sáng Lambert/Gouraud và đường cong Bézier.

## Tính năng

- 27 cubie (26 cubie hiển thị và một lõi ẩn), mỗi cubie có ID, vị trí nguyên và hướng sticker riêng.
- Các bước `U D L R F B`, dấu phẩy, `2`; animation theo `deltaTime`, hàng đợi transaction.
- Shuffle, reset, undo/redo, lịch sử bước và kiểm tra solved.
- Camera orbit/zoom, phép chiếu perspective/orthographic tự dựng bằng ma trận 4x4.
- Solid, Wireframe từ danh sách cạnh, Solid + Wireframe, Lighting, 2D Net.
- Back-face Culling, Z-buffer, Lambert Flat và Gouraud.
- Raster buffer riêng cho Bresenham, Cohen-Sutherland và Scanline Fill.
- Camera Tour dùng de Casteljau trên đường cong Bézier.
- Test headless cho toán, logic Rubik, animation và thuật toán.

## Tải mã nguồn và build trên Windows

### 1. Yêu cầu

- Visual Studio 2022 trở lên với Desktop development with C++ và Windows SDK.
- Git.
- CMake 3.20 trở lên (có thể chọn CMake tools khi cài Visual Studio).
- vcpkg và FreeGLUT bản x64.

### 2. Clone repository

```powershell
git clone https://github.com/Giabao96206/Rubik3D.git
cd Rubik3D
```

### 3. Cài FreeGLUT bằng vcpkg

Nếu chưa có vcpkg:

```powershell
git clone https://github.com/microsoft/vcpkg.git C:\vcpkg
C:\vcpkg\bootstrap-vcpkg.bat
C:\vcpkg\vcpkg.exe install freeglut:x64-windows
```

Nếu đã có vcpkg, chỉ cần chạy:

```powershell
C:\vcpkg\vcpkg.exe install freeglut:x64-windows
```

Nếu vcpkg nằm ở thư mục khác, thay `C:/vcpkg` trong các lệnh bên dưới bằng đường dẫn thực tế.

### 4. Configure và build bằng Visual Studio 2022

Mở **Developer PowerShell for VS 2022**, chuyển tới thư mục repository rồi chạy:

```powershell
cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

### 5. Chạy kiểm thử

```powershell
ctest --test-dir build -C Release --output-on-failure
```

### 6. Chạy chương trình

```powershell
.\build\Release\Rubik3D.exe
```

File `freeglut.dll` được CMake tự động chép cạnh executable sau khi build.

### Build bằng Ninja (tùy chọn)

Với Ninja một cấu hình, executable nằm trực tiếp trong thư mục build:

```powershell
cmake -S . -B build-ninja -G Ninja -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=C:/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build-ninja
ctest --test-dir build-ninja --output-on-failure
.\build-ninja\Rubik3D.exe
```

### Chỉ build lõi và test, không cần FreeGLUT

Nếu máy chưa có FreeGLUT hoặc không cần mở cửa sổ OpenGL:

```powershell
cmake -S . -B build-core -DRUBIK3D_BUILD_APP=OFF
cmake --build build-core --config Release
ctest --test-dir build-core -C Release --output-on-failure
```

## Điều khiển nhanh

| Phím/thao tác | Chức năng |
|---|---|
| `U D L R F B` | Quay mặt tương ứng |
| `Shift` + phím mặt | Quay ngược |
| `Space` | Shuffle 25 bước có animation |
| `Ctrl+Z`, `Ctrl+Y` | Undo, redo |
| Kéo chuột trái | Orbit camera |
| Con lăn | Zoom |
| `P` | Perspective/Orthographic |
| `W` | Chuyển rendering mode |
| `N` | 2D Net raster |
| `K`, `C`, `G` | Lighting, Culling, Flat/Gouraud |
| `T` | Camera Tour Bézier |
| `A` | Nhập chuỗi thuật toán, ví dụ `R U R' U'` |
| `Q/E`, `+/-` | Quay và scale 2D Net |
| `H` | Trợ giúp |
| `Esc` | Thoát |

Panel bên phải có nút bước quay, Shuffle, Reset, Undo/Redo, Lighting, Projection, tốc độ animation và các bước gần nhất. Trong chế độ nhập thuật toán, Enter đưa cả chuỗi vào hàng đợi, Backspace sửa và Esc hủy.

## Tài liệu

- [Kiến trúc](docs/architecture.md)
- [Thuật toán và nội dung bảo vệ](docs/algorithms.md)
- [Bản đồ thuật toán trong mã nguồn](docs/algorithm-code-map.md)
- [Hướng dẫn sử dụng](docs/user-guide.md)
- [Kiểm thử](docs/testing.md)

## Quy ước màu và tọa độ

Trục `+X` là Right (đỏ), `-X` Left (cam), `+Y` Up (trắng), `-Y` Down (vàng), `+Z` Front (xanh lá), `-Z` Back (xanh dương). Trạng thái logic chỉ dùng tọa độ `{-1,0,1}`; float chỉ dùng cho frame animation.

