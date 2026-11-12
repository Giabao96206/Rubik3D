# Kiểm thử

## Chạy test

```powershell
ctest --test-dir build -C Release --output-on-failure
```

Hoặc chạy trực tiếp `rubik_tests.exe` để thấy từng case.

## Phạm vi tự động

- Matrix: identity, translation, rotation 90 độ quanh X/Y/Z, `T*R*S`, biến đổi 2D thuần nhất.
- Rubik: 27 cubie/sticker hợp lệ; `U^4`; `R R'`; `F2 F2`; chuỗi và nghịch đảo; shuffle; undo/redo.
- Animation: logic chưa đổi giữa transaction và chỉ commit một lần khi kết thúc.
- Bresenham: đủ tám octant và hai đầu mút.
- Cohen-Sutherland: accept, reject, giao hai biên.
- Scanline: điểm trong đa giác chữ nhật.
- Bézier: hai đầu mút và điểm giữa de Casteljau.
- Lambert: cường độ chuẩn hóa và mặt hướng sáng sáng hơn mặt quay lưng.

## Kiểm thử thủ công renderer

1. Mở ứng dụng, xác nhận ba mặt ngoài có màu đúng và không thấy mặt trong khi culling bật.
2. Tắt culling, đổi các mode, resize cửa sổ; không có lỗi OpenGL hay đổi logic.
3. Thực hiện `R`, quan sát đúng 9 cubie xoay mượt và số bước chỉ tăng khi kết thúc.
4. Shuffle, Undo/Redo, Reset; trạng thái panel phải khớp.
5. Đổi projection và orbit; màu/sticker không đổi.
6. Mở 2D Net; so sánh sticker với mô hình 3D; thử Q/E và +/-.
7. Chạy camera tour; camera di chuyển liên tục và luôn nhìn tâm.

## Giới hạn

Test tự động không tạo OpenGL context. Phần cửa sổ cần smoke test trên Windows có driver OpenGL; test logic vẫn chạy độc lập.

