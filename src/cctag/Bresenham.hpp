#ifndef _CCTAG_BRENSENHAM_HPP_
#define _CCTAG_BRENSENHAM_HPP_

#include <cctag/geometry/Point.hpp>

#include <cstddef>

#include <opencv/cv.h>

#include "Types.hpp"

namespace cctag {

class EdgePoint;

/** @brief descent in the gradient direction from a maximum gradient point (magnitude sense) to another one.
 *
 */

EdgePoint* gradientDirectionDescent(
  const EdgePointCollection& canny,
  const EdgePoint& p,
  const int dir,
  const std::size_t nmax,
  const cv::Mat & dx, 
  const cv::Mat & dy,
  int thrGradient);

} // namespace cctag

#endif