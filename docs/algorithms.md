# Thuật toán và nội dung bảo vệ

## 1. Tọa độ thuần nhất và ma trận

**Mục đích.** Biểu diễn tịnh tiến, quay, tỷ lệ và phép chiếu bằng cùng phép nhân ma trận.

Với vector cột 3D `P=(x,y,z,1)^T`, chương trình dùng `P' = M P`. Phép ghép `P' = T R S P` nghĩa là scale trước, quay sau, cuối cùng tịnh tiến. Ma trận 4x4 cần thiết vì tịnh tiến không thể biểu diễn bằng ma trận tuyến tính 3x3 tác động lên `(x,y,z)`; thành phần `w` đưa affine và projection về một khuôn dạng.

- Cài đặt: `include/Math/Matrix4x4.h`, `src/Math/Matrix4x4.cpp`.
- Sử dụng: cubie/model, camera view, orthographic và perspective.
- Test: identity, translation, ba phép quay 90 độ và `T*R*S`.
- Câu hỏi bảo vệ: đổi thứ tự `TRS` thành `SRT` có giống không? Không, nhân ma trận không giao hoán.

## 2. Logic quay Rubik rời rạc

**Mục đích.** Giữ hoán vị và hướng sticker chính xác sau nhiều bước.

Với quay dương 90 độ quanh Z: `(x,y,z) -> (-y,x,z)`. Quanh X: `(x,y,z)->(x,-z,y)`; quanh Y: `(x,y,z)->(z,y,-x)`. Chương trình lặp phép ánh xạ nguyên 1-3 lần thay vì lưu kết quả `sin/cos` float. Mỗi sticker được đổi sang vector hướng, quay cùng công thức, rồi ánh xạ về một trong sáu Direction.

- Cài đặt: `RubiksCube::rotateDiscrete`, `signedAxisTurns`.
- Sử dụng: mọi bước U/D/L/R/F/B.
- Độ phức tạp: O(27), thực tế chỉ sửa 9 cubie của layer.
- Test: `U^4=I`, `R R'=I`, `F2 F2=I`, chuỗi và nghịch đảo.
- Câu hỏi: tại sao không tích lũy float? Vì sai số làm tọa độ lệch khỏi `{-1,0,1}` và phá việc chọn layer.

## 3. Bresenham Line

**Mục đích.** Chọn pixel gần đoạn thẳng bằng số nguyên.

Với sai số tích lũy `err`, mỗi bước tăng theo trục chính và khi `2*err` vượt ngưỡng sẽ tăng trục phụ. Bản cài đặt đối xứng xử lý đủ tám octant, đoạn đảo chiều, đứng và ngang.

- Cài đặt: `bresenhamLine`, `RasterBuffer::drawLine`.
- Sử dụng: viền từng sticker của 2D Net.
- Độ phức tạp: `O(max(|dx|,|dy|))`, bộ nhớ O(n) trong hàm trả điểm.
- Ví dụ: `(0,0)->(5,2)` cho 6 pixel, luôn chứa hai đầu mút.
- Test: tám octant.
- Câu hỏi: ưu điểm so với `y=mx+b`? Không cần phép chia/float và không lỗi với đường đứng.

## 4. Cohen-Sutherland

**Mục đích.** Xén đoạn thẳng vào cửa sổ chữ nhật.

Mỗi đầu mút có outcode 4 bit: Left=1, Right=2, Bottom=4, Top=8. Nếu `(codeA | codeB)==0` thì nhận; nếu `(codeA & codeB)!=0` thì loại; còn lại thay đầu mút ngoài bằng giao điểm với một biên và lặp.

- Cài đặt: `cohenSutherlandClip`.
- Sử dụng: xén đoạn minh họa trong RasterBuffer.
- Độ phức tạp: O(1), số biên hữu hạn.
- Test: đoạn trong, ngoài cùng phía, cắt trái-phải.
- Câu hỏi: OR và AND biểu diễn trivial accept/reject thế nào?

## 5. Scanline Fill

**Mục đích.** Tô đa giác bằng các khoảng pixel theo từng dòng quét.

Với mỗi y, thuật toán lấy giao điểm với cạnh thỏa quy tắc nửa mở `[ymin,ymax)`, sắp xếp hoành độ, rồi tô từng cặp `[ceil(x0), floor(x1)]`. Quy tắc nửa mở tránh đếm một đỉnh chung hai lần.

- Cài đặt: `scanlineFill`, `RasterBuffer::fillPolygon`.
- Sử dụng: tô 54 sticker của 2D Net.
- Độ phức tạp bản minh họa: O(H * E log E); có thể tối ưu bằng Edge Table/Active Edge Table.
- Test: đa giác chữ nhật và điểm nội bộ.
- Câu hỏi: vì sao giao điểm phải ghép cặp? Mỗi cặp là một khoảng đi vào/đi ra vùng trong đa giác.

## 6. Biến đổi 2D

2D Net áp dụng `T(center+offset) * R(angle) * S(scale) * T(-center)` trên mỗi đỉnh sticker trước khi raster hóa. Đây là minh họa trực tiếp tọa độ thuần nhất 3x3; Q/E thay angle, +/- thay scale.

- Cài đặt: `Matrix3x3`, `Renderer::drawNet`.
- Test: rotation + translation trong `test_matrix.cpp`.

## 7. Camera View và Projection

View matrix dựng cơ sở camera trực chuẩn: `f=normalize(target-eye)`, `s=normalize(f x up)`, `u=s x f`. Perspective dùng `f=1/tan(FOV/2)` và phép chia phối cảnh theo `w`; orthographic chỉ scale/translate hộp quan sát.

- Cài đặt: `Matrix4x4::lookAt/perspective/orthographic`, `Camera3D`.
- Khác biệt: perspective có gần lớn xa nhỏ; orthographic giữ kích thước theo độ sâu.
- Camera orbit chỉ thay view matrix, không chạm `RubiksCube`.
- Câu hỏi: vì sao đổi projection không đổi logic? Vì nó chỉ tác động pipeline quan sát sau model.

## 8. Back-face Culling và Z-buffer

Pháp tuyến của tam giác `N=(B-A)x(C-A)`. Mặt trước thỏa dấu tích vô hướng `N.(Eye-A)` theo quy ước winding. Renderer cung cấp kiểm tra CPU `isFrontFacing` và dùng cùng winding CCW cho OpenGL culling.

Z-buffer lưu độ sâu gần nhất tại mỗi pixel; fragment mới chỉ ghi nếu qua depth test. Culling loại cả primitive dựa trên hướng, còn Z-buffer phân giải che khuất giữa các mặt còn lại. Hai kỹ thuật bổ sung chứ không thay nhau.

- Cài đặt: `Lighting::isFrontFacing`, `Mesh::unitCube`, `Renderer::initialize/drawScene3D`.
- Minh họa: nút/phím C bật tắt culling; depth luôn bật ở viewport 3D.
- Câu hỏi: tích có hướng còn dùng làm gì? Tạo pháp tuyến và cơ sở camera.

## 9. Lambert, Flat và Gouraud

Mô hình CPU:

`I = Ia*Ka + Id*Kd*max(0, N dot L)`.

Flat dùng một pháp tuyến mặt nên toàn mặt có cường độ gần như nhau. Gouraud tính cường độ tại từng đỉnh với pháp tuyến đỉnh, OpenGL nội suy màu qua mặt. Gouraud mượt và nhanh nhưng có thể bỏ lỡ highlight nằm giữa polygon; Phong sẽ nội suy pháp tuyến rồi tính sáng từng fragment. Bản này không triển khai GLSL Phong để giữ trọng tâm vào thuật toán bắt buộc.

- Cài đặt: `lambertIntensity`, `shadeColor`, `Renderer::drawCubie`.
- Test: mặt hướng nguồn sáng sáng hơn mặt quay lưng, cường độ trong [0,1].
- Minh họa: K hoặc nút Light; G đổi Flat/Gouraud.

## 10. Bézier và de Casteljau

Với các điểm kiểm soát `P_i^0`, mỗi thế hệ:

`P_i^r(t) = (1-t) P_i^(r-1)(t) + t P_(i+1)^(r-1)(t)`.

Điểm cuối `P_0^n(t)` nằm trên đường cong Bézier. Camera Tour lấy mẫu theo thời gian trên năm điểm kiểm soát và mỗi frame dựng view matrix nhìn vào tâm Rubik.

- Cài đặt: `deCasteljau`, `sampleBezier`, `Camera3D::startTour/update/position`.
- Độ phức tạp mỗi mẫu: O(n^2), bộ nhớ O(n).
- Test: bảo toàn hai đầu mút và điểm giữa của Bézier bậc hai.
- Minh họa: nhấn T, tour kéo dài 8 giây.
- Câu hỏi: ưu điểm de Casteljau? Ổn định số và thể hiện trực quan quá trình nội suy hình học.

## 11. Animation transaction

Animator sử dụng `deltaTime`, chuẩn hóa `t=elapsed/duration`, rồi smoothstep `t^2(3-2t)` để vận tốc đầu/cuối bằng 0. Logic chỉ commit khi `elapsed>=duration`; các lệnh mới ở trong deque.

- Cài đặt: `RotationAnimator`.
- Test: cube còn solved giữa animation và chỉ đổi khi transaction kết thúc.
- Câu hỏi: phân biệt logic rotation, visual rotation, camera rotation? Chúng lần lượt sửa trạng thái rời rạc, model matrix tạm thời và view matrix.
