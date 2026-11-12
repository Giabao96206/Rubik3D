#include "Algorithms/Bresenham.h"
#include "Algorithms/CohenSutherland.h"
#include "Algorithms/ScanlineFill.h"
#include "Algorithms/RasterBuffer.h"

#include <algorithm>
#include <cmath>
#include <stdexcept>

namespace rubik {

	std::vector<PixelPoint> bresenhamLine(PixelPoint a, PixelPoint b) {
		std::vector<PixelPoint> out;
		const int dx = std::abs(b.x - a.x), sx = a.x < b.x ? 1 : -1;
		const int dy = -std::abs(b.y - a.y), sy = a.y < b.y ? 1 : -1;
		int error = dx + dy;
		while (true) { out.push_back(a);if (a == b)break;const int e2 = 2 * error;if (e2 >= dy) { error += dy;a.x += sx; }if (e2 <= dx) { error += dx;a.y += sy; } }
		return out;
	}

	namespace {
		enum Code { Inside = 0, Left = 1, Right = 2, Bottom = 4, Top = 8 };
		int code(const Point2D& p, const Rect2D& r) { int c = Inside;if (p.x < r.xmin)c |= Left;else if (p.x > r.xmax)c |= Right;if (p.y < r.ymin)c |= Bottom;else if (p.y > r.ymax)c |= Top;return c; }
	}

	std::optional<LineSegment2D> cohenSutherlandClip(LineSegment2D line, const Rect2D& r) {
		if (r.xmin > r.xmax || r.ymin > r.ymax)throw std::invalid_argument("Invalid clipping rectangle");
		int ca = code(line.a, r), cb = code(line.b, r);
		while (true) {
			if (!(ca | cb))return line;
			if (ca & cb)return std::nullopt;
			const int outside = ca ? ca : cb; Point2D p;
			if (outside & Top) { p.x = line.a.x + (line.b.x - line.a.x) * (r.ymax - line.a.y) / (line.b.y - line.a.y);p.y = r.ymax; }
			else if (outside & Bottom) { p.x = line.a.x + (line.b.x - line.a.x) * (r.ymin - line.a.y) / (line.b.y - line.a.y);p.y = r.ymin; }
			else if (outside & Right) { p.y = line.a.y + (line.b.y - line.a.y) * (r.xmax - line.a.x) / (line.b.x - line.a.x);p.x = r.xmax; }
			else { p.y = line.a.y + (line.b.y - line.a.y) * (r.xmin - line.a.x) / (line.b.x - line.a.x);p.x = r.xmin; }
			if (outside == ca) { line.a = p;ca = code(line.a, r); }
			else { line.b = p;cb = code(line.b, r); }
		}
	}

	std::vector<PixelPoint> scanlineFill(const std::vector<PixelPoint>& polygon) {
		std::vector<PixelPoint> filled;if (polygon.size() < 3)return filled;
		int ymin = polygon[0].y, ymax = ymin;for (const auto& p : polygon) { ymin = std::min(ymin, p.y);ymax = std::max(ymax, p.y); }
		for (int y = ymin;y < ymax;++y) {
			std::vector<double> xs;
			for (std::size_t i = 0, j = polygon.size() - 1;i < polygon.size();j = i++) {
				const auto& a = polygon[j];const auto& b = polygon[i];
				// Quy tắc nửa mở [ymin,ymax) tránh đếm đỉnh chung hai lần.
				if ((a.y <= y && b.y > y) || (b.y <= y && a.y > y))xs.push_back(a.x + (double)(y - a.y) * (b.x - a.x) / (b.y - a.y));
			}
			std::sort(xs.begin(), xs.end());
			for (std::size_t i = 0;i + 1 < xs.size();i += 2) { const int begin = (int)std::ceil(xs[i]), end = (int)std::floor(xs[i + 1]);for (int x = begin;x <= end;++x)filled.push_back({ x,y }); }
		}
		return filled;
	}

	RasterBuffer::RasterBuffer(int w, int h) :width_(w), height_(h), data_(static_cast<std::size_t>(w* h * 3)) { if (w <= 0 || h <= 0)throw std::invalid_argument("Invalid raster size");clear(); }
	void RasterBuffer::clear(std::array<std::uint8_t, 3> c) { for (int y = 0;y < height_;++y)for (int x = 0;x < width_;++x)setPixel(x, y, c); }
	void RasterBuffer::setPixel(int x, int y, std::array<std::uint8_t, 3> c) { if (x < 0 || y < 0 || x >= width_ || y >= height_)return;const auto i = static_cast<std::size_t>((y * width_ + x) * 3);data_[i] = c[0];data_[i + 1] = c[1];data_[i + 2] = c[2]; }
	void RasterBuffer::drawLine(PixelPoint a, PixelPoint b, std::array<std::uint8_t, 3> c) { for (const auto& p : bresenhamLine(a, b))setPixel(p.x, p.y, c); }
	void RasterBuffer::fillPolygon(const std::vector<PixelPoint>& p, std::array<std::uint8_t, 3> c) { for (const auto& point : scanlineFill(p))setPixel(point.x, point.y, c); }

} // namespace rubik

