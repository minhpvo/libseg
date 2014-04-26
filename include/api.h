#ifndef _LIBMATTING_API_H_
#define _LIBMATTING_API_H_

#include <memory>
#include <cstdint>
#include <string.h>

#include "utils.h"

// Contains the current state of the matting
class Matter {
 public:
  // The image is given in the lab colorspace. Each of lab_l, lab_a, lab_b is
  // a W*H array stored in row-major order
  // Matter makes an internal copy of the image
  Matter(uint8_t* lab_l, uint8_t* lab_a, uint8_t* lab_b, int W, int H);
  ~Matter();


  // Add a scribble. Note that empty scribbles (0 pixels) will be ignored
  //
  // Note that we do not store all scribbles as a simple boolean mask because
  // of the way user interaction is handled (see Bai09). Scribbles ordering is
  // important to let user do local corrections that do not influence
  // the whole image. In other words, doing the matting with the same set of
  // scribbles but in different order might result in different result.
  void AddScribble(const Scribble& s);

  int NumScribbles() {
    return scribbles.size();
  }

  // Fill mask with the foreground mask resulting from the matting.
  // 255 indicates foreground pixels, 0 background.
  void GetForegroundMask(uint8_t* mask);

  void GetForegroundLikelihood(double* out);
  void GetBackgroundLikelihood(double* out);

  void GetForegroundDist(double* out);
  void GetBackgroundDist(double* out);

 private:
  int W, H;
  std::unique_ptr<uint8_t[]> lab_l, lab_a, lab_b;
  // TODO: We do not actually need the pdf for each pixel of the image. Use
  // a simple lookup table of pixel intensity to pdf instead
  std::unique_ptr<double[]> fg_pdf, bg_pdf;
  std::unique_ptr<double[]> fg_likelihood, bg_likelihood;
  std::unique_ptr<double[]> fg_dist, bg_dist;
  std::unique_ptr<uint8_t[]> final_mask;

  // Initially false, true when at least one scribble has been added to bg/fg
  bool bg_scribbled_, fg_scribbled_;

  uint8_t* channels[3];

  std::vector<Scribble> scribbles;
};

#endif
