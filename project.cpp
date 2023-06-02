#include "project.hh"
std::string currentImagePath;

void utcn::ip::Project::runLab() {
  int op;

  selectImageFile();
  do {
    utcn::ip::Project::printMenu(LAB_MENU);
    std::cin >> op;
    switch (op) {
      case 0:
        break;
      case 11:
        selectImageFile();
        break;
      case 1:
        testColorToGrayscale();
        break;
      case 2:
        testColorToBW();
        break;
      case 3:
        testSepiaFilter();
        break;
      case 4:
        testVignette();
        break;
      case 5:
        addCatEars();
        break;
      default:
        std::cout << "Invalid selection" << std::endl;
    }
  } while (op != 0);
}

int resize_value = 200;
int rotate_angle = 180;
double g_vignetteStrength = 1.0;

void on_trackbar(int value, void*);

void utcn::ip::Project::selectImageFile() {
  currentImagePath = fileutil::getSingleFileAbsPath();
}

void utcn::ip::Project::testVignette() {
  if (currentImagePath.empty()) {
    std::cout << "No image file selected" << std::endl;
    return;
  }
  cv::Mat src = cv::imread(currentImagePath, cv::IMREAD_COLOR);

  int rows = src.rows;
  int cols = src.cols;

  // Calculate center of the image
  cv::Point center = cv::Point(cols / 2, rows / 2);
  // The diagonal
  double maxDist = std::sqrt((cols / 2) * (cols / 2) + (rows / 2) * (rows / 2));

  cv::Mat mask = src.clone();
  cv::namedWindow(
      "Vignette Filter",
      1);         // Create a named window where the trackbar will be displayed
  int value = 0;  // initial value of trackbar
  cv::createTrackbar("Strength", "Vignette Filter", &value, 100, on_trackbar);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      double distance = std::sqrt((center.y - i) * (center.y - i) +
                                  (center.x - j) * (center.x - j));
      double vignetteStrength = distance / maxDist;

      for (int ch = 0; ch < 3; ++ch) {
        mask.at<cv::Vec3b>(i, j)[ch] = cv::saturate_cast<uchar>(
            (1 - vignetteStrength) * mask.at<cv::Vec3b>(i, j)[ch]);
      }
    }
  }
  cv::imshow("input image", src);
  cv::imshow("Vignette Filter", mask);
  cv::waitKey();
}

void on_trackbar(int value, void*) {
  // We use 100 because the trackbar has a range from 0 to 100
  g_vignetteStrength = 1.0 - (double)value / 100;

  if (currentImagePath.empty()) {
    std::cout << "No image file selected" << std::endl;
    return;
  }
  cv::Mat src = cv::imread(currentImagePath, cv::IMREAD_COLOR);

  int rows = src.rows;
  int cols = src.cols;

  cv::Point center = cv::Point(cols / 2, rows / 2);

  double maxDist = std::sqrt((cols / 2) * (cols / 2) + (rows / 2) * (rows / 2));

  cv::Mat mask = src.clone();
  cv::namedWindow("Vignette Filter", 1);

  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      double distance = std::sqrt((center.y - i) * (center.y - i) +
                                  (center.x - j) * (center.x - j));
      double vignetteStrength = distance / maxDist * g_vignetteStrength;

      for (int ch = 0; ch < 3; ++ch) {
        mask.at<cv::Vec3b>(i, j)[ch] = cv::saturate_cast<uchar>(
            (1 - vignetteStrength) * mask.at<cv::Vec3b>(i, j)[ch]);
      }
    }
  }
  cv::imshow("Vignette Filter", mask);
}

void utcn::ip::Project::testColorToGrayscale() {
  cv::Mat src;
  const std::string path_to_src = ASSETS_DIR "Images/Lena_24bits.bmp";
  src = cv::imread(path_to_src, cv::IMREAD_COLOR);  // Read the image

  if (!src.data) {
    std::cout << "Could not open or find the image" << std::endl;
    return;
  }
  int height = src.rows;
  int width = src.cols;
  cv::Mat dst = cv::Mat(height, width, CV_8UC1);

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      cv::Vec3b val = src.at<cv::Vec3b>(i, j);
      dst.at<uchar>(i, j) = (val[0] + val[1] + val[2]) / 3;
    }
  }

  cv::imshow("input image", src);
  cv::imshow("grayscale image", dst);
  cv::waitKey();
}

void utcn::ip::Project::testColorToBW() {
  int thresh = 0;
  std::cout << "Enter threshold value: ";
  std::cin >> thresh;
  if (thresh < 0 || thresh > 255) {
    std::cout << "Threshold should be between 0 and 255\n";
    return;
  }

  if (currentImagePath.empty()) {
    std::cout << "No image file selected" << std::endl;
    return;
  }
  cv::Mat src = cv::imread(currentImagePath, cv::IMREAD_COLOR);

  if (!src.data) {
    std::cout << "Could not open or find the image" << std::endl;
    return;
  }

  int height = src.rows;
  int width = src.cols;

  cv::imshow("input image", src);

  // Convert color to grayscale :

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      cv::Vec3b val = src.at<cv::Vec3b>(i, j);
      src.at<uchar>(i, j) = (val[0] + val[1] + val[2]) / 3;
    }
  }

  cv::Mat dst = cv::Mat(height, width, CV_8UC1);

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      uchar val = src.at<uchar>(i, j);
      if (val < thresh)
        dst.at<uchar>(i, j) = 0;
      else
        dst.at<uchar>(i, j) = 255;
    }
  }

  cv::imshow("BW image", dst);
  cv::waitKey();
}

int ears_height_pos = 45;
int ears_width_pos = 28;

void on_trackbar_width(int width, void*) {
  ears_width_pos = width - 200;

  if (currentImagePath.empty()) {
    std::cout << "No image file selected" << std::endl;
    return;
  }
  cv::Mat src = cv::imread(currentImagePath, cv::IMREAD_COLOR);

  cv::Mat cat_ears =
      cv::imread(ASSETS_DIR "Images/cat_ears.png", cv::IMREAD_UNCHANGED);
  if (cat_ears.empty()) {
    std::cout << "Could not open or find the cat ears image" << std::endl;
    return;
  }

  cv::Point2f center(cat_ears.cols / 2.0, cat_ears.rows / 2.0);

  // Compute the rotation matrix
  cv::Mat rotationMatrix =
      cv::getRotationMatrix2D(center, (double)rotate_angle - 180, 1.0);

  // Apply the rotation to the image
  cv::Mat rotatedImage;
  cv::warpAffine(cat_ears, rotatedImage, rotationMatrix, cat_ears.size());

  int start_row = src.rows / 4 - ears_height_pos;
  int start_col = src.cols / 4 + ears_width_pos;

  int ears_height = src.rows / 2;
  int ears_width = src.cols / 2;

  cv::resize(rotatedImage, rotatedImage,
             cv::Size(ears_width + resize_value - 200,
                      ears_height + resize_value - 200));

  for (int row = 0; row < rotatedImage.rows; row++) {
    for (int col = 0; col < rotatedImage.cols; col++) {
      cv::Vec4b& pixel = rotatedImage.at<cv::Vec4b>(row, col);
      if (pixel[3] > 0) {
        src.at<cv::Vec3b>(start_row + row, start_col + col) =
            cv::Vec3b(pixel[0], pixel[1], pixel[2]);
      }
    }
  }
  cv::imshow("Add Cat Ears", src);
}

void on_trackbar_height(int height, void*) {
  ears_height_pos = height - 200;

  if (currentImagePath.empty()) {
    std::cout << "No image file selected" << std::endl;
    return;
  }
  cv::Mat src = cv::imread(currentImagePath, cv::IMREAD_COLOR);

  cv::Mat cat_ears =
      cv::imread(ASSETS_DIR "Images/cat_ears.png", cv::IMREAD_UNCHANGED);
  if (cat_ears.empty()) {
    std::cout << "Could not open or find the cat ears image" << std::endl;
    return;
  }
  cv::Point2f center(cat_ears.cols / 2.0, cat_ears.rows / 2.0);

  // Compute the rotation matrix
  cv::Mat rotationMatrix =
      cv::getRotationMatrix2D(center, (double)rotate_angle - 180, 1.0);

  // Apply the rotation to the image
  cv::Mat rotatedImage;
  cv::warpAffine(cat_ears, rotatedImage, rotationMatrix, cat_ears.size());
  int start_row = src.rows / 4 - ears_height_pos;
  int start_col = src.cols / 4 + ears_width_pos;

  int ears_height = src.rows / 2;
  int ears_width = src.cols / 2;

  cv::resize(rotatedImage, rotatedImage,
             cv::Size(ears_width + resize_value - 200,
                      ears_height + resize_value - 200));

  for (int row = 0; row < rotatedImage.rows; row++) {
    for (int col = 0; col < rotatedImage.cols; col++) {
      cv::Vec4b& pixel = rotatedImage.at<cv::Vec4b>(row, col);
      if (pixel[3] > 0) {
        src.at<cv::Vec3b>(start_row + row, start_col + col) =
            cv::Vec3b(pixel[0], pixel[1], pixel[2]);
      }
    }
  }
  cv::imshow("Add Cat Ears", src);
}

void on_trackbar_rotate(int angle, void*) {
  // We use 100 because the trackbar has a range from 0 to 360
  rotate_angle = angle;

  if (currentImagePath.empty()) {
    std::cout << "No image file selected" << std::endl;
    return;
  }
  cv::Mat src = cv::imread(currentImagePath, cv::IMREAD_COLOR);

  cv::Mat cat_ears =
      cv::imread(ASSETS_DIR "Images/cat_ears.png", cv::IMREAD_UNCHANGED);
  if (cat_ears.empty()) {
    std::cout << "Could not open or find the cat ears image" << std::endl;
    return;
  }
  // Get the image center
  cv::Point2f center(cat_ears.cols / 2.0, cat_ears.rows / 2.0);

  // Compute the rotation matrix
  cv::Mat rotationMatrix =
      cv::getRotationMatrix2D(center, (double)angle - 180, 1.0);

  // Apply the rotation to the image
  cv::Mat rotatedImage;
  cv::warpAffine(cat_ears, rotatedImage, rotationMatrix, cat_ears.size());
  int start_row = src.rows / 4 - ears_height_pos;
  int start_col = src.cols / 4 + ears_width_pos;

  int ears_height = src.rows / 2;
  int ears_width = src.cols / 2;

  cv::resize(rotatedImage, rotatedImage,
             cv::Size(ears_width + resize_value - 200,
                      ears_height + resize_value - 200));

  for (int row = 0; row < rotatedImage.rows; row++) {
    for (int col = 0; col < rotatedImage.cols; col++) {
      cv::Vec4b& pixel = rotatedImage.at<cv::Vec4b>(row, col);
      if (pixel[3] > 0) {
        src.at<cv::Vec3b>(start_row + row, start_col + col) =
            cv::Vec3b(pixel[0], pixel[1], pixel[2]);
      }
    }
  }
  cv::imshow("Add Cat Ears", src);
}

// trackbar to resize cat_ears.png
void on_trackbar_resize(int res, void*) {
  // We use 100 because the trackbar has a range from 0 to 360
  resize_value = res;

  if (currentImagePath.empty()) {
    std::cout << "No image file selected" << std::endl;
    return;
  }
  cv::Mat src = cv::imread(currentImagePath, cv::IMREAD_COLOR);

  cv::Mat cat_ears =
      cv::imread(ASSETS_DIR "Images/cat_ears.png", cv::IMREAD_UNCHANGED);
  if (cat_ears.empty()) {
    std::cout << "Could not open or find the cat ears image" << std::endl;
    return;
  }
  // Get the image center
  cv::Point2f center(cat_ears.cols / 2.0, cat_ears.rows / 2.0);

  // Compute the rotation matrix
  cv::Mat rotationMatrix =
      cv::getRotationMatrix2D(center, (double)rotate_angle - 180, 1.0);

  // Apply the rotation to the image
  cv::Mat rotatedImage;
  cv::warpAffine(cat_ears, rotatedImage, rotationMatrix, cat_ears.size());
  int start_row = src.rows / 4 - ears_height_pos;
  int start_col = src.cols / 4 + ears_width_pos;

  int ears_height = src.rows / 2;
  int ears_width = src.cols / 2;

  cv::resize(rotatedImage, rotatedImage,
             cv::Size(ears_width + resize_value - 200,
                      ears_height + resize_value - 200));

  for (int row = 0; row < rotatedImage.rows; row++) {
    for (int col = 0; col < rotatedImage.cols; col++) {
      cv::Vec4b& pixel = rotatedImage.at<cv::Vec4b>(row, col);
      if (pixel[3] > 0) {
        src.at<cv::Vec3b>(start_row + row, start_col + col) =
            cv::Vec3b(pixel[0], pixel[1], pixel[2]);
      }
    }
  }
  cv::imshow("Add Cat Ears", src);
}

void utcn::ip::Project::addCatEars() {
  if (currentImagePath.empty()) {
    std::cout << "No image file selected" << std::endl;
    return;
  }
  cv::Mat src = cv::imread(currentImagePath, cv::IMREAD_COLOR);

  cv::Mat cat_ears =
      cv::imread(ASSETS_DIR "Images/cat_ears.png", cv::IMREAD_UNCHANGED);
  if (cat_ears.empty()) {
    std::cout << "Could not open or find the cat ears image" << std::endl;
    return;
  }

  int start_row = src.rows / 4 - ears_height_pos;
  int start_col = src.cols / 4 + ears_width_pos;

  cv::namedWindow("Add Cat Ears", 1);
  int height_pos = 200;
  int width_pos = 200;
  int rotate_pos = 180;
  int res = 200;
  cv::createTrackbar("move_ears_height", "Add Cat Ears", &height_pos, 330,
                     on_trackbar_height);
  cv::createTrackbar("move_ears_width", "Add Cat Ears", &width_pos, 330,
                     on_trackbar_width);
  cv::createTrackbar("move_ears_rotate", "Add Cat Ears", &rotate_pos, 360,
                     on_trackbar_rotate);
  cv::createTrackbar("move_ears_resize", "Add Cat Ears", &res, 400,
                     on_trackbar_resize);

  // Resize the cat ears image to fit the head in the image

  // use the size of the src image to determine the size of the cat ears image

  int ears_height = src.rows / 2;
  int ears_width = src.cols / 2;

  cv::resize(cat_ears, cat_ears, cv::Size(ears_width, ears_height));

  for (int row = 0; row < cat_ears.rows; row++) {
    for (int col = 0; col < cat_ears.cols; col++) {
      cv::Vec4b& pixel = cat_ears.at<cv::Vec4b>(row, col);
      // If the cat ears image pixel is not fully transparent
      if (pixel[3] > 0) {
        // Copy pixel from cat ears image to the main image
        src.at<cv::Vec3b>(start_row + row, start_col + col) =
            cv::Vec3b(pixel[0], pixel[1], pixel[2]);
      }
    }
  }

  cv::imshow("Add Cat Ears", src);
  cv::waitKey();
}

void utcn::ip::Project::testSepiaFilter() {
    if (currentImagePath.empty()) {
    std::cout << "No image file selected" << std::endl;
    return;
  }
  cv::Mat src = cv::imread(currentImagePath, cv::IMREAD_COLOR);

  int height = src.rows;
  int width = src.cols;
  cv::Mat dst = cv::Mat(height, width, CV_8UC3);

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      cv::Vec3b val = src.at<cv::Vec3b>(i, j);
      uchar inputBlue = val[0];
      uchar inputGreen = val[1];
      uchar inputRed = val[2];

      // Here is the sepia transformation matrix:
      /*
      cv::Mat transformationMatrix = (cv::Mat_<float>(3,3) <<
                            .393, .769, .189,
                            .349, .686, .168,
                            .272, .534, .131);

      */

      // Apply sepia formula for each channel by using the min function to
      // avoid overflow ( > 255 ).
      uchar outputRed = std::min(
          255., (inputRed * .393) + (inputGreen * .769) + (inputBlue * .189));
      uchar outputGreen = std::min(
          255., (inputRed * .349) + (inputGreen * .686) + (inputBlue * .168));
      uchar outputBlue = std::min(
          255., (inputRed * .272) + (inputGreen * .534) + (inputBlue * .131));

      dst.at<cv::Vec3b>(i, j) = cv::Vec3b(outputBlue, outputGreen, outputRed);
    }
  }

  cv::imshow("input image", src);
  cv::imshow("sepia filter image", dst);
  cv::waitKey();
}
