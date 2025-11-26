
#include <algorithm>
#include <cmath>
#include <stdio.h>

struct vec4 {
  float x, y, z, w;
  vec4(float x = 0, float y = 0, float z = 0, float w = 0)
      : x(x), y(y), z(z), w(w) {}
};

struct vec2 {
  float x, y;
  vec2(float x = 0, float y = 0) : x(x), y(y) {}
  vec2 yx() const { return vec2(y, x); }
  vec4 xyyx() const { return vec4(x, y, y, x); }
};

struct vec3 {
  float x, y, z;
  vec3(float x = 0, float y = 0, float z = 0) : x(x), y(y), z(z) {};
  vec2 xz() const { return vec2(x, z); }
  float length() const { return sqrtf(x * x + y * y + z * z); }
  vec3 fract() const { return vec3(x - floor(x), y - floor(y), z - floor(z)); }
};

struct Mat2 {
  float m[2][2];
};

struct Pixel32 {
  unsigned char r, g, b, a;
};

Pixel32 packColor(vec4 color) {
  float r = std::clamp(color.x, 0.0f, 1.0f);
  float g = std::clamp(color.y, 0.0f, 1.0f);
  float b = std::clamp(color.z, 0.0f, 1.0f);
  float a = std::clamp(color.w, 0.0f, 1.0f);

  return Pixel32{
      (unsigned char)(r * 255.0f + 0.5f), (unsigned char)(g * 255.0f + 0.5f),
      (unsigned char)(b * 255.0f + 0.5f), (unsigned char)(a * 255.0f + 0.5f)};
}

Mat2 rotate2D(float angle) {
  float s = sin(angle);
  float c = cos(angle);

  return Mat2{c, -s, s, c};
}

vec2 operator*(const vec2 &a, const Mat2 &m) {
  return vec2{a.x * m.m[0][0] + a.y * m.m[0][1],
              a.x * m.m[1][0] + a.y * m.m[1][1]};
}

vec3 operator*(const vec3 &a, const vec3 &b) {
  return vec3(a.x * b.x, a.y * b.y, a.z * b.z);
}
vec2 operator*(const vec2 &a, float s) { return vec2(a.x * s, a.y * s); }
vec2 operator+(const vec2 &a, float s) { return vec2(a.x + s, a.y + s); }
vec2 operator*(float s, const vec2 &a) { return a * s; }
vec2 operator-(const vec2 &a, const vec2 &b) {
  return vec2(a.x - b.x, a.y - b.y);
}
vec2 operator+(const vec2 &a, const vec2 &b) {
  return vec2(a.x + b.x, a.y + b.y);
}
vec2 operator*(const vec2 &a, const vec2 &b) {
  return vec2(a.x * b.x, a.y * b.y);
}
vec2 operator/(const vec2 &a, float s) { return vec2(a.x / s, a.y / s); }
float dot(const vec2 &a, const vec2 &b) { return a.x * b.x + a.y * b.y; }
vec2 abs(const vec2 &a) { return vec2(fabsf(a.x), fabsf(a.y)); }
vec2 &operator+=(vec2 &a, const vec2 &b) {
  a = a + b;
  return a;
}
vec2 &operator+=(vec2 &a, float s) {
  a = a + s;
  return a;
}
vec2 cos(const vec2 &a) { return vec2(cosf(a.x), cosf(a.y)); }
vec4 sin(const vec4 &a) {
  return vec4(sinf(a.x), sinf(a.y), sinf(a.z), sinf(a.w));
}
vec4 exp(const vec4 &a) {
  return vec4(expf(a.x), expf(a.y), expf(a.z), expf(a.w));
}
vec4 tanh(const vec4 &a) {
  return vec4(tanhf(a.x), tanhf(a.y), tanhf(a.z), tanhf(a.w));
}
vec4 operator+(const vec4 &a, float s) {
  return vec4(a.x + s, a.y + s, a.z + s, a.w + s);
}
vec4 operator*(const vec4 &a, float s) {
  return vec4(a.x * s, a.y * s, a.z * s, a.w * s);
}
vec4 operator*(float s, const vec4 &a) { return a * s; }
vec4 operator+(const vec4 &a, const vec4 &b) {
  return vec4(a.x + b.x, a.y + b.y, a.z + b.z, a.w + b.w);
}
vec4 &operator+=(vec4 &a, const vec4 &b) {
  a = a + b;
  return a;
}
vec4 operator-(float s, const vec4 &a) {
  return vec4(s - a.x, s - a.y, s - a.z, s - a.w);
}
vec4 operator/(const vec4 &a, const vec4 &b) {
  return vec4(a.x / b.x, a.y / b.y, a.z / b.z, a.w / b.w);
}

vec4 operator*(const vec4 &a, const vec4 &b) {
  return vec4(a.x * b.x, a.y * b.y, a.z * b.z, a.w * b.w);
}

vec4 operator/(const vec4 &a, float s) {
  return vec4(a.x / s, a.y / s, a.z / s, a.w / s);
}

vec4 maxVec(vec4 v, float s) {
  return vec4{std::fmax(v.x, s), std::fmax(v.y, s), std::fmax(v.z, s),
              std::fmax(v.w, s)};
}

unsigned char toByte(float c) {
  if (c < 0.0f)
    c = 0.0f;
  if (c > 1.0f)
    c = 1.0f;
  return (unsigned char)(c * 255.0f);
}

int main() {
  char buf[256];
  for (int i = 0; i < 240; ++i) {
    snprintf(buf, sizeof(buf), "output-%03d.pam", i);
    const char *output_path = buf;
    FILE *f = fopen(output_path, "wb");
    int w = 16 * 60;
    int h = 9 * 60;
    fprintf(f, "P7\n");
    fprintf(f, "WIDTH %d\n", w);
    fprintf(f, "HEIGHT %d\n", h);
    fprintf(f, "DEPTH 4\n"); // 4 Channels
    fprintf(f, "MAXVAL 255\n");
    fprintf(f, "TUPLTYPE RGB_ALPHA\n");
    fprintf(f, "ENDHDR\n");
    vec2 r = {(float)w, (float)h};
    float t = ((float)i / 240) * 2 * M_PI;
    for (int y = 0; y < h; ++y) {
      for (int x = 0; x < w; ++x) {
        vec4 o = {0, 0, 0, 0};

        vec2 FC = {(float)x, (float)y};

        for (float j = 0.0f, d = 0.0f, s = 0.0f; j++ < 1e2;) {
          vec2 uv = (FC * 2. - r) / r.y * d * rotate2D(t / 2.);
          vec3 p = vec3(uv.x, uv.y, d - 8.);
          vec2 xz = p.xz() * rotate2D(t / 2.);
          p.x = xz.x;
          p.z = xz.y;

          d += s =
              .012 + .07 * std::abs(std::fmax(sin((p.fract() * p).length()),
                                              p.length() - 4.) -
                                    j / 1e2);
          vec4 color = 1.3 * sin(vec4(1, 2, 3, 1) + j * .3) / s;
          vec3 p2 = p * p;
          float dist = -std::sqrtf(p2.x * p2.x + p2.y * p2.y + p2.z * p2.z);
          vec4 result =
              vec4(std::fmax(color.x, dist), std::fmax(color.y, dist),
                   std::fmax(color.z, dist), std::fmax(color.w, dist));
          o += result;
        }
        if (x == 0 && y == 0) {
          printf("Before tanh: o = (%f, %f, %f, %f)\n", o.x, o.y, o.z, o.w);
        }
        o = tanh(o * o / 8e5);
        if (x == 0 && y == 0) {
          printf("After tanh: o = (%f, %f, %f, %f)\n", o.x, o.y, o.z, o.w);
        }

        o.w = 1.0f;

        Pixel32 pixel = packColor(o);
        fputc(toByte(o.x), f); // R
        fputc(toByte(o.y), f); // G
        fputc(toByte(o.z), f); // B
        fputc(toByte(o.w), f);
      }
    }
    fclose(f);
    printf("Generated %s (%3d/%3d)\n", output_path, i + 1, 240);
  }
  system("ffmpeg -i output-%03d.pam -r 60 output.mp4");
  return 0;
}
