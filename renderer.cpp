class texture{
  public:
  Texture(const std::string& filePath);
  Texture(int width, int height, const std::vector<RGBColor>& pixelData);
  ~Texture();
  
  Color getPixel(float u, float v) const;
  Color getPixel(int x, int y) const;

  int getWidth() const { return m_width; }
  int getHeight() const { return m_height; }

  private:
  int m_width;
  int m_height;
  std::vector<RGBColor> m_pixels;
  void loadImage(const std::string& filePath);
}

class obj{
  int verticies;
  std::array<vector3, verticies> vertexPosData;
  texture objTexture;

}
