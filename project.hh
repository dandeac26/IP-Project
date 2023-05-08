#ifndef PROJECT_HH
#define PROJECT_HH

#include "lab.hh"

namespace utcn::ip {
class Project : public Lab {
  static inline std::map<int, std::string> LAB_MENU = {
      {1,
       "Copy the R, G and B channels of a color, RGB24 image (CV_8UC3 type) "
       "into three matrices of type CV_8UC1 (grayscale images)"},
      {2,
       "Convert a color RGB24 image (CV_8UC3 type) to a grayscale image "
       "(CV_8UC1)"},
      {3, "Converting from grayscale to black and white"},
      {4,
       "Compute the H, S and V values from the R, G, B channels of an image"},
      {5,
       "Check if the position indicated by the pair (i,j) (row, column) is "
       "inside the image img"},
      {6, "test"},
  };

  static void testSplitRGB();

  static void testColorToGrayscale();

  static void testColorToBW();

  static void testRGBToHSV();

  static void testBGR2HSV();

  static void testIsInside();

 public:
  void runLab() override;
};
}  // namespace utcn::ip

#endif  // PROJECT_HH