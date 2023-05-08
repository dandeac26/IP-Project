#include "project.hh"

void utcn::ip::Project::runLab() {
  int op;
  do {
    utcn::ip::Project::printMenu(LAB_MENU);
    std::cin >> op;
    switch (op) {
      case 0:
        break;
      case 1:
        testSplitRGB();
        break;
      case 2:
        testColorToGrayscale();
        break;
      case 3:
        testColorToBW();
        break;
      case 4:
        testRGBToHSV();
        break;
      case 5:
        testIsInside();
        break;
      case 6:
        testBGR2HSV();
        break;
      default:
        std::cout << "Invalid selection" << std::endl;
    }
  } while (op != 0);
}

void utcn::ip::Project::testBGR2HSV() {
  const std::string abs_image_path = fileutil::getSingleFileAbsPath();
  if (!abs_image_path.empty()) {
    cv::Mat src = cv::imread(abs_image_path);
    int height = src.rows;
    int width = src.cols;

    // HSV components
    cv::Mat H = cv::Mat(height, width, CV_8UC1);
    cv::Mat S = cv::Mat(height, width, CV_8UC1);
    cv::Mat V = cv::Mat(height, width, CV_8UC1);

    // Defining pointers to each matrix (8 bits/pixels) of the individual
    // components H, S, V
    uchar *lpH = H.data;
    uchar *lpS = S.data;
    uchar *lpV = V.data;

    cv::Mat hsvImg;
    cv::cvtColor(src, hsvImg, cv::COLOR_BGR2HSV);

    // Defining the pointer to the HSV image matrix (24 bits/pixel)
    uchar *hsvDataPtr = hsvImg.data;

    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
        int hi = i * width * 3 + j * 3;
        int gi = i * width + j;

        lpH[gi] = hsvDataPtr[hi] * 510 / 360;  // lpH = 0 .. 255
        lpS[gi] = hsvDataPtr[hi + 1];          // lpS = 0 .. 255
        lpV[gi] = hsvDataPtr[hi + 2];          // lpV = 0 .. 255
      }
    }

    cv::imshow("input image", src);
    cv::imshow("H", H);
    cv::imshow("S", S);
    cv::imshow("V", V);

    cv::waitKey();
  }
}

void utcn::ip::Project::testSplitRGB() {
  // Implement code here

  cv::Mat src, dst;
  const std::string path_to_src = ASSETS_DIR "Images/Lena_24bits.bmp";
  src = cv::imread(path_to_src, cv::IMREAD_COLOR);  // Read the image

  if (!src.data) {
    std::cout << "Could not open or find the image" << std::endl;
    return;
  }
  int height = src.rows;
  int width = src.cols;
  cv::Mat dst1 = cv::Mat(height, width, CV_8UC1);
  cv::Mat dst2 = cv::Mat(height, width, CV_8UC1);
  cv::Mat dst3 = cv::Mat(height, width, CV_8UC1);

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      cv::Vec3b val = src.at<cv::Vec3b>(i, j);
      dst1.at<uchar>(i, j) = val[0];
      dst2.at<uchar>(i, j) = val[1];
      dst3.at<uchar>(i, j) = val[2];
    }
  }

  // Display each channel in a separate window
  cv::imshow("Red Channel", dst3);
  cv::imshow("Green Channel", dst2);
  cv::imshow("Blue Channel", dst1);

  // Wait for a key press to exit
  cv::waitKey();

  // Close all windows
}

void utcn::ip::Project::testColorToGrayscale() {
  // Implement code here
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

////////////// 3 ////////////////////
void utcn::ip::Project::testColorToBW() {
  // Implement code here
  int thresh = 0;
  std::cin >> thresh;
  if (thresh < 0 || thresh > 255) {
    std::cout << "Threshold should be between 0 and 255\n";
    return;
  }
  // get image
  cv::Mat src;
  const std::string path_to_src = ASSETS_DIR "Images/eight.bmp";
  src = cv::imread(path_to_src, cv::IMREAD_GRAYSCALE);  // Read the image

  if (!src.data) {
    std::cout << "Could not open or find the image" << std::endl;
    return;
  }

  int height = src.rows;
  int width = src.cols;
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

  cv::imshow("input grayscale image", src);
  cv::imshow("BW image", dst);
  cv::waitKey();
}

void utcn::ip::Project::testRGBToHSV() {
  cv::Mat src;
  const std::string path_to_src = ASSETS_DIR "Images/Lena_24bits.bmp";
  src = cv::imread(path_to_src, cv::IMREAD_COLOR);  // Read the image

  if (!src.data) {
    std::cout << "Could not open or find the image" << std::endl;
    return;
  }
  int height = src.rows;
  int width = src.cols;
  cv::Mat H = cv::Mat(height, width, CV_8UC1);
  cv::Mat S = cv::Mat(height, width, CV_8UC1);
  cv::Mat V = cv::Mat(height, width, CV_8UC1);

  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      cv::Vec3b val = src.at<cv::Vec3b>(i, j);
      float R = (float)val[2] / 255;
      float G = (float)val[1] / 255;
      float B = (float)val[0] / 255;

      float M = std::max(R, std::max(G, B));
      float m = std::min(R, std::min(G, B));
      float C = (float)M - m;

      V.at<uchar>(i, j) = M;

      if (M != 0)
        S.at<uchar>(i, j) = (float)C / M;
      else  // grayscale
        S.at<uchar>(i, j) = (float)0;
      if (C != 0) {
        if (M == R) H.at<uchar>(i, j) = (float)60 * (G - B) / C;
        if (M == G) H.at<uchar>(i, j) = (float)120 + (float)60 * (B - R) / C;
        if (M == B) H.at<uchar>(i, j) = (float)240 + (float)60 * (R - G) / C;
      }
    }
  }

  // Display each channel in a separate window
  cv::imshow("H Channel", H);
  cv::imshow("S Channel", S);
  cv::imshow("V Channel", V);

  // Wait for a key press to exit
  cv::waitKey();
}

void utcn::ip::Project::testIsInside() {
  // Implement code here
}