#include "pipeline/context.h"

#include "jpeglib.h"
#include <stdio.h>

#include "image_processing/utils.h"
#include "utils/lodepng.h"

Context::Context() {}

void Context::add_image(std::string img_name, Image img)
{
  // We want an overwrite behavior, so first try to delete img if it already exists
  delete_image(img_name);

  if (img.type == ImageType::GRAY) {
    add_gray_image(img_name, img.gray_img);
  } else if (img.type == ImageType::RGBA) {
    add_rgba_image(img_name, img.rgba_img);
  } else if (img.type == ImageType::FULL) {
    add_gray_image(img_name, img.gray_img);
    add_rgba_image(img_name, img.rgba_img);
  }
}

Image Context::get_image(std::string img_name)
{
  Image img;
  auto rgba_img = get_rgba_image(img_name);
  if (rgba_img->get_cols() != 0) {
    img.rgba_img = rgba_img;
    img.type = ImageType::RGBA;
  }
  auto gray_img = get_gray_image(img_name);
  if (gray_img->get_cols() != 0) {
    img.gray_img = gray_img;
    img.type = ImageType::GRAY;
  }
  if ((rgba_img->get_cols() != 0) & (gray_img->get_cols() != 0)) {
    img.type = ImageType::FULL;
  }
  return img;
}

void Context::delete_image(std::string img_name)
{
  imgs.erase(img_name);
  gray_imgs.erase(img_name);
}

void Context::save_image(std::string img_name, std::string filepath)
{
  Image img = get_image(img_name);
  bool is_png = filepath.substr(filepath.size() - 3) == "png";
  if (img.type == ImageType::FULL || img.type == ImageType::RGBA) {
    if (is_png) {
      save_rgba_image_png(img.rgba_img, filepath);
    } else {
      save_rgba_image(img.rgba_img, filepath);
    }
    return;
  }
  if (img.type == ImageType::GRAY) {
    if (is_png) {
      save_gray_image_png(img.gray_img, filepath);
    } else {
      save_gray_image(img.gray_img, filepath);
    }
    return;
  }
}

void Context::add_rgba_image(std::string img_name, std::shared_ptr<Matrix<uint32_t>> img)
{
  imgs.insert({ img_name, img });
}

std::shared_ptr<Matrix<uint32_t>> Context::get_rgba_image(std::string img_name)
{
  auto img = imgs.find(img_name);
  if (img == imgs.end()) {
    return std::make_shared<Matrix<uint32_t>>();
  }
  return img->second;
}

void Context::add_gray_image(std::string img_name, std::shared_ptr<Matrix<uint8_t>> img)
{
  gray_imgs.insert({ img_name, img });
}

std::shared_ptr<Matrix<uint8_t>> Context::get_gray_image(std::string img_name, bool convert_color_img)
{
  auto img = gray_imgs.find(img_name);
  if (img == gray_imgs.end()) {
    if (convert_color_img) {
      auto color_img = get_rgba_image(img_name);
      if (color_img->get_rows() == 0) {
        return std::make_shared<Matrix<uint8_t>>();
      } else {
        return ip::rgba_to_gray(color_img);
      }
    } else {
      return std::make_shared<Matrix<uint8_t>>();
    }
  }
  return img->second;
}

void Context::save_gray_image(std::string img_name)
{
  auto gray_img = get_gray_image(img_name);
  if (!gray_img) {
    // No image, no-op
    return;
  }

  save_gray_image_png(gray_img, img_name + ".png");
}

void Context::save_gray_image_png(std::shared_ptr<Matrix<uint8_t>> gray_img, std::string filepath)
{
  size_t img_width = gray_img->get_cols();
  size_t img_height = gray_img->get_rows();
  std::vector<uint8_t> png_data;
  png_data.resize(gray_img->get_cols() * gray_img->get_rows() * 4);
  for (size_t row = 0; row < img_height; ++row) {
    for (size_t col = 0; col < img_width; ++col) {
      uint8_t pixel = gray_img->operator()(row, col);
      png_data[(row * img_width + col) * 4] = pixel;
      png_data[(row * img_width + col) * 4 + 1] = pixel;
      png_data[(row * img_width + col) * 4 + 2] = pixel;
      png_data[(row * img_width + col) * 4 + 3] = 255;
    }
  }

  lodepng::encode(filepath, png_data, gray_img->get_cols(), gray_img->get_rows());
}

void Context::save_rgba_image(std::string img_name)
{
  auto rgba_img = get_rgba_image(img_name);
  if (!rgba_img) {
    // No image, no-op
    return;
  }

  save_rgba_image_png(rgba_img, img_name + ".png");
}

void Context::save_rgba_image_png(std::shared_ptr<Matrix<uint32_t>> rgba_img, std::string filepath)
{
  size_t img_width = rgba_img->get_cols();
  size_t img_height = rgba_img->get_rows();
  std::vector<uint8_t> png_data;
  png_data.resize(rgba_img->get_cols() * rgba_img->get_rows() * 3);
  for (size_t row = 0; row < img_height; ++row) {
    for (size_t col = 0; col < img_width; ++col) {
      RGBAPixel rgba_pixel = RGBAPixel(rgba_img->operator()(row, col));
      png_data[(row * img_width + col) * 3] = rgba_pixel.r;
      png_data[(row * img_width + col) * 3 + 1] = rgba_pixel.g;
      png_data[(row * img_width + col) * 3 + 2] = rgba_pixel.b;
    }
  }

  lodepng::encode(filepath, png_data, img_width, img_height, LodePNGColorType::LCT_RGB);
}

void Context::save_rgba_image(std::shared_ptr<Matrix<uint32_t>> rgba_img, std::string filepath)
{
  printf("Starting JPEG compression");
  // Step 0, create a raw buffer with image data
  size_t img_width = rgba_img->get_cols();
  size_t img_height = rgba_img->get_rows();
  JSAMPLE* image_buffer = new JSAMPLE[img_height * img_width * 3];
  for (size_t row = 0; row < img_height; ++row) {
    for (size_t col = 0; col < img_width; ++col) {
      RGBAPixel rgba_pixel = RGBAPixel(rgba_img->operator()(row, col));
      image_buffer[(row * img_width + col) * 3] = rgba_pixel.r;
      image_buffer[(row * img_width + col) * 3 + 1] = rgba_pixel.g;
      image_buffer[(row * img_width + col) * 3 + 2] = rgba_pixel.b;
    }
  }

  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JSAMPROW row_pointer[1];
  FILE* outfile;

  // Step 1, JPEG compression
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_compress(&cinfo);

  // Step 2, data destination
  if ((outfile = fopen(filepath.c_str(), "wb")) == NULL) {
    fprintf(stderr, "can't open %s\n", filepath.c_str());
    exit(1);
  }
  jpeg_stdio_dest(&cinfo, outfile);

  // Step 3, JPEG compression parameters
  cinfo.image_width = img_width; /* image width and height, in pixels */
  cinfo.image_height = img_height;
  cinfo.input_components = 3;     /* # of color components per pixel */
  cinfo.in_color_space = JCS_RGB; /* colorspace of input image */
  jpeg_set_defaults(&cinfo);
  /* Now you can set any non-default parameters you wish to.
   * Here we just illustrate the use of quality (quantization table) scaling:
   */
  jpeg_set_quality(&cinfo, 90, TRUE /* limit to baseline-JPEG values */);

  /* Step 4: Start compressor */

  /* TRUE ensures that we will write a complete interchange-JPEG file.
   * Pass TRUE unless you are very sure of what you're doing.
   */
  jpeg_start_compress(&cinfo, TRUE);

  /* Step 5: while (scan lines remain to be written) */
  /*           jpeg_write_scanlines(...); */

  /* Here we use the library's state variable cinfo.next_scanline as the
   * loop counter, so that we don't have to keep track ourselves.
   * To keep things simple, we pass one scanline per call; you can pass
   * more if you wish, though.
   */
  int row_stride = img_width * 3; /* JSAMPLEs per row in image_buffer */

  while (cinfo.next_scanline < cinfo.image_height) {
    /* jpeg_write_scanlines expects an array of pointers to scanlines.
     * Here the array is only one element long, but you could pass
     * more than one scanline at a time if that's more convenient.
     */
    row_pointer[0] = &image_buffer[cinfo.next_scanline * row_stride];
    (void)jpeg_write_scanlines(&cinfo, row_pointer, 1);
  }

  /* Step 6: Finish compression */

  jpeg_finish_compress(&cinfo);
  /* After finish_compress, we can close the output file. */
  fclose(outfile);

  /* Step 7: release JPEG compression object */

  /* This is an important step since it will release a good deal of memory. */
  jpeg_destroy_compress(&cinfo);
  delete[] image_buffer;
}

void Context::save_gray_image(std::shared_ptr<Matrix<uint8_t>> gray_img, std::string filepath)
{
  auto rgba_img = ip::gray_to_rgba(gray_img);
  save_rgba_image(rgba_img, filepath);
}

std::vector<std::string> Context::get_image_names()
{
  std::vector<std::string> image_names;
  std::set<std::string> image_names_set;

  for (auto iter = imgs.begin(); iter != imgs.end(); ++iter) {
    image_names_set.insert(iter->first);
  }

  for (auto iter = gray_imgs.begin(); iter != gray_imgs.end(); ++iter) {
    image_names_set.insert(iter->first);
  }

  std::copy(image_names_set.begin(), image_names_set.end(), std::back_inserter(image_names));
  return image_names;
}
