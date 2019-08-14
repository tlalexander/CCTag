/*
 * Copyright 2016, Simula Research Laboratory
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */
#include <cctag/utils/Defines.hpp>
#include <cctag/ImagePyramid.hpp>
#include <cctag/utils/VisualDebug.hpp>

#include <iostream>
#include <string>

namespace cctag {

ImagePyramid::ImagePyramid( std::size_t width, std::size_t height, std::size_t nLevels, bool cuda_allocates )
{
  _levels.clear();
  _levels.resize(nLevels);
  for(int i = 0; i < nLevels ; ++i)
  {
    _levels[i] = new Level( width, height, i, cuda_allocates );
    width /= 2;
    height /= 2;
  }
}

void ImagePyramid::build( const Plane<uint8_t>& src, float thrLowCanny, float thrHighCanny, const cctag::Parameters* params )
{
#ifdef CCTAG_WITH_CUDA
    if( params->_useCuda ) {
        std::cerr << __FILE__ << ":" << __LINE__ << std::endl
                  << "    must not call " << __FUNCTION__ << " with CUDA enables" << std::endl;
        exit( -1 );
    }
#endif // CCTAG_WITH_CUDA

    /* The pyramid building function is never called if CUDA is used.
     */
  _levels[0]->setLevel( src , thrLowCanny, thrHighCanny, params );
  
  for(int i = 1; i < _levels.size() ; ++i)
  {
    _levels[i]->setLevel( _levels[i-1]->getSrc(), thrLowCanny, thrHighCanny, params );
  }
  
  if( params->_debugDir == "" ) return;

  for(int i = 0; i < _levels.size() ; ++i)
  {
    std::stringstream outFilenameCanny;
    outFilenameCanny << "cannyLevel" << i;
    CCTagVisualDebug::instance().initBackgroundImage(_levels[i]->getEdges());
    CCTagVisualDebug::instance().newSession(outFilenameCanny.str());
    
#ifdef CCTAG_EXTRA_LAYER_DEBUG
    {
        std::ostringstream o;
        o << params->_debugDir << "cannyLevel" << i << "_wt" << ".pgm";
        writePlanePGM( o.str(), _levels[i]->getCannyNotThin(), SCALED_WRITING );
    }
#endif

    {
        std::ostringstream o;
        o << params->_debugDir << "src_" << i << ".pgm";
        writePlanePGM( o.str(), _levels[i]->getSrc(), SCALED_WRITING );
    }

    {
        std::ostringstream o;
        o << params->_debugDir << "dx_" << i << ".pgm";
        writePlanePGM( o.str(), _levels[i]->getDx(), SCALED_WRITING );
    }

    {
        std::ostringstream o;
        o << params->_debugDir << "dy_" << i << ".pgm";
        writePlanePGM( o.str(), _levels[i]->getDy(), SCALED_WRITING );
    }
  }
}

ImagePyramid::~ImagePyramid()
{
  for(auto & _level : _levels)
  {
    delete _level;
  }
};

std::size_t ImagePyramid::getNbLevels() const
{
  return _levels.size();
}

Level* ImagePyramid::getLevel( std::size_t level ) const
{
        return _levels[level];
}

}


