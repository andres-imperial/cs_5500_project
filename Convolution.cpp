#include "Convolution.hpp"
#include <cmath>
#include <vector>

namespace {
std::vector<int> genDeltas(int size) {
  if (size % 2 != 1) {
    throw std::runtime_error("size of filter needs to be odd");
  }

  std::vector<int> deltas;

  for (int delta = -(size / 2); delta <= size / 2; ++delta) {
    deltas.push_back(delta);
  }

  return deltas;
}
}

Image convolve(const Image &img, const Image &filter) {
  Image newImage = Image(img.getWidth(), img.getHeight());

  for (unsigned int row = 0; row < img.getHeight(); ++row) {
    for (unsigned int col = 0; col < img.getWidth(); ++col) {
      newImage(col, row) = convolve(col, row, img, filter);
    }
  }

  return newImage;
}

Image::Rgb convolve(int col, int row, const Image &img, const Image &filter) {
  if (filter.getWidth() != filter.getHeight()) {
    throw std::runtime_error("filter needs to have square dimensions");
  }

  Image::Rgb result;
  std::vector<int> deltas = genDeltas(filter.getWidth());
  for (int dc : deltas) {
    for (int dr : deltas) {
      auto filterVal =
          filter(dc + filter.getWidth() / 2, dr + filter.getWidth() / 2);
      auto imgVal = getImageValue(col + dc, row + dr, ApronStyle::EXTEND, img);
      result += imgVal * filterVal;
    }
  }

  return result;
}

Image::Rgb getImageValue(int col, int row, ApronStyle apron, const Image &img) {
  if (img.pixelInView(col, row)) {
    return img(col, row);
  } else if (apron == ApronStyle::ZERO) {
    return 0;
  } else {
    if (col < 0) {
      if (row < 0) {
        if (apron == ApronStyle::EXTEND)
          return img(0, 0);
        else if (apron == ApronStyle::MIRROR)
          return img(abs(col), abs(row));
        else if (apron == ApronStyle::WRAP)
          return img(img.getWidth() + col, img.getHeight() + row);
      } else if (row > img.getHeight() - 1) {
        if (apron == ApronStyle::EXTEND)
          return img(0, img.getHeight() - 1);
        else if (apron == ApronStyle::MIRROR)
          return img(abs(col), 2 * img.getHeight() - row - 2);
        else if (apron == ApronStyle::WRAP)
          return img(img.getWidth() + col, row - img.getHeight());
      } else {
        if (apron == ApronStyle::EXTEND)
          return img(0, row);
        else if (apron == ApronStyle::MIRROR)
          return img(abs(col), row);
        else if (apron == ApronStyle::WRAP)
          return img(img.getWidth() + col, row);
      }
    } else if (col > img.getWidth() - 1) {
      if (row < 0) {
        if (apron == ApronStyle::EXTEND)
          return img(img.getWidth() - 1, 0);
        else if (apron == ApronStyle::MIRROR)
          return img(2 * img.getWidth() - col - 2, abs(row));
        else if (apron == ApronStyle::WRAP)
          return img(col - img.getWidth(), img.getHeight() + row);
      } else if (row > img.getHeight() - 1) {
        if (apron == ApronStyle::EXTEND)
          return img(img.getWidth() - 1, img.getHeight() - 1);
        else if (apron == ApronStyle::MIRROR)
          return img(2 * img.getWidth() - col - 2,
                     2 * img.getHeight() - row - 2);
        else if (apron == ApronStyle::WRAP)
          return img(col - img.getWidth(), row - img.getHeight());
      } else {
        if (apron == ApronStyle::EXTEND)
          return img(img.getWidth() - 1, row);
        else if (apron == ApronStyle::MIRROR)
          return img(2 * img.getWidth() - col - 2, row);
        else if (apron == ApronStyle::WRAP)
          return img(col - img.getWidth(), row);
      }
    } else {
      if (row < 0) {
        if (apron == ApronStyle::EXTEND)
          return img(col, 0);
        else if (apron == ApronStyle::MIRROR)
          return img(col, abs(row));
        else if (apron == ApronStyle::WRAP)
          return img(col, img.getHeight() + row);
      } else if (row > img.getHeight() - 1) {
        if (apron == ApronStyle::EXTEND)
          return img(col, img.getHeight() - 1);
        else if (apron == ApronStyle::MIRROR)
          return img(col, 2 * img.getHeight() - row - 2);
        else if (apron == ApronStyle::WRAP)
          return img(col, row - img.getHeight());
      }
    }
  }
  return 0;
}
