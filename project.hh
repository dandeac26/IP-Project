#ifndef PROJECT_HH
#define PROJECT_HH

#include "lab.hh"

namespace utcn::ip {
class Project : public Lab {
  static inline std::map<int, std::string> LAB_MENU = {
      {11, "Select image file"},
      {1, "Convert Color to grayscale"},
      {2, "BLACK AND WHITE: converts an image to black and white"},
      {3, "SEPIA filter: adds a warm brown color to the image"},
      {4, "VIGNETTE filter: adds dark faded edges to an image"},
      {5, "Cat ears.. yes cat years, Attila's idea"},
  };

  static void testVignette();

  static void testColorToGrayscale();

  static void testColorToBW();

  static void testSepiaFilter();

  static void selectImageFile();

  static void addCatEars();

 public:
  void runLab() override;
};
}  // namespace utcn::ip

#endif  // PROJECT_HH