#include <filesystem>

namespace fs = std::filesystem;

class texture{
  public:
  texture(fs::path);
  texture(int width, int height, const std::vector<RGBColor>& pixelData);
  ~texture();
  
  RGBColor getPixel(float u, float v) const;
  RGBColor getPixel(int x, int y) const;

  int getWidth() const { return m_width; }
  int getHeight() const { return m_height; }

  void loadImage(fs::path);

  private:
  int m_width;
  int m_height;
  std::vector<RGBColor> m_pixels;
}

class obj{
  public:
  int verticies;
  std::array<vector3, verticies> vertexPosData;
  texture objTexture;

  obj(int vertex, array<vector3, vertex> vertexData, &texture Texture){vertex(verticies), vertexData(vertexPosData), Texture(objTexture)};
}

obj camera(1, {vector3(0, 0, 0)}, null);
