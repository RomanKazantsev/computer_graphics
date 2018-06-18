/*
Copyright (c) 2018 Roman Kazantsev
Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:
The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.
THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
*/

#include <vector>
#include <algorithm>
#include <fstream>
#include <cmath>

#include "utils.h"

using namespace std;

inline double clamp(double x) { return x<0 ? 0 : x>1 ? 1 : x; }
inline int toInt(double x) { return int(pow(clamp(x), 1 / 2.2) * 255 + .5); }

int WriteImageToBmp(Vector3D const *image, int width, int height) {
	int filesize = 54 + 3 * width * height;  //w is your image width, h is image height, both int

	std::vector<unsigned char> data(3 * width * height);
	std::fill(data.begin(), data.end(), 0);

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++)
		{
			int x = i, y = (height - 1) - j;
			int r = toInt(image[y * width + x].x);
			int g = toInt(image[y * width + x].y);
			int b = toInt(image[y * width + x].z);

			data[(x + y * width) * 3 + 2] = (unsigned char)(r);
			data[(x + y * width) * 3 + 1] = (unsigned char)(g);
			data[(x + y * width) * 3 + 0] = (unsigned char)(b);
		}
	}

	unsigned char bmpfileheader[14] = { 'B','M', 0,0,0,0, 0,0, 0,0, 54,0,0,0 };
	unsigned char bmpinfoheader[40] = { 40,0,0,0, 0,0,0,0, 0,0,0,0, 1,0, 24,0 };
	unsigned char bmppad[3] = { 0,0,0 };

	bmpfileheader[2] = (unsigned char)(filesize);
	bmpfileheader[3] = (unsigned char)(filesize >> 8);
	bmpfileheader[4] = (unsigned char)(filesize >> 16);
	bmpfileheader[5] = (unsigned char)(filesize >> 24);

	bmpinfoheader[4] = (unsigned char)(width);
	bmpinfoheader[5] = (unsigned char)(width >> 8);
	bmpinfoheader[6] = (unsigned char)(width >> 16);
	bmpinfoheader[7] = (unsigned char)(width >> 24);
	bmpinfoheader[8] = (unsigned char)(height);
	bmpinfoheader[9] = (unsigned char)(height >> 8);
	bmpinfoheader[10] = (unsigned char)(height >> 16);
	bmpinfoheader[11] = (unsigned char)(height >> 24);

	std::ofstream file("z_out.bmp", ios::out | ios::binary | ios::ate);

	if (file.is_open())
	{
		file.write((const char *)bmpfileheader, sizeof(bmpfileheader));
		file.write((const char *)bmpinfoheader, sizeof(bmpinfoheader));
		for (int i = 0; i < height; i++)
		{
			file.write((const char *)&data[width * (height - i - 1) * 3], 3 * width);
			file.write((const char *)bmppad, (4 - (width * 3) % 4) % 4);
		}
		file.close();
	}
	else {
		return -1;
	}

	return 0;
}

// operations
Vector3D& Vector3D::operator= (Vector3D const & other) {
	x = other.x;
	y = other.y;
	z = other.z;

	return *this;
}

Vector3D Vector3D::operator+ (Vector3D const &other) const {
	Vector3D res;
	res.x = this->x + other.x;
	res.y = this->y + other.y;
	res.z = this->z + other.z;

	return res;
}
Vector3D Vector3D::operator- (Vector3D const &other) const {
	Vector3D res;
	res.x = this->x - other.x;
	res.y = this->y - other.y;
	res.z = this->z - other.z;

	return res;
}
Vector3D Vector3D::operator* (double scalar) const {
	Vector3D res;
	res.x = this->x * scalar;
	res.y = this->y * scalar;
	res.z = this->z * scalar;

	return res;
}
Vector3D Vector3D::mult(Vector3D const &other) const{
	Vector3D res;
	res.x = this->x * other.x;
	res.y = this->y * other.y;
	res.z = this->z * other.z;

	return res;
}
Vector3D& Vector3D::norm() {
	return *this = *this * (1.0f / sqrt(x * x + y * y + z * z));
}
double Vector3D::dot(Vector3D const& other) const {
	return x * other.x + y * other.y + z * other.z;
}
Vector3D Vector3D::operator% (Vector3D const& other) const {
	Vector3D res;
	res.x = y * other.z - z * other.y;
	res.y = z * other.x - x * other.z;
	res.z = x * other.y - y * other.x;

	return res;
}
