#ifndef _ROM_2DTRANSFORM_HPP_
#define _ROM_2DTRANSFORM_HPP_

#include "../geometry/Ellipse.hpp"
#include "../algebra/matrix/operation.hpp"
//#include <rom/vision/viewGeometry/projectiveTransformation/2DTransform.hpp>

#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/foreach.hpp>
#include <vector>

namespace rom {
namespace vision {
namespace viewGeometry {

namespace ublas = boost::numeric::ublas;

template<class C>
inline void projectiveTransform( const ublas::bounded_matrix<double, 3, 3>& tr, std::vector<C>& v )
{
	BOOST_FOREACH( C & p, v )
	{
		C ptAux = (C) ublas::prec_prod( tr, p );
		p = rom::numerical::normalize( ptAux );
	}
}

inline void projectiveTransform( const ublas::bounded_matrix<double, 3, 3>& tr, rom::numerical::geometry::Ellipse& ellipse )
{
	ellipse.setMatrix( ublas::prec_prod( ublas::trans( tr ), ( ublas::bounded_matrix<double, 3, 3>) ublas::prec_prod( ellipse.matrix(), tr ) ) );
}

inline void projectiveTransform( const ublas::bounded_matrix<double, 3, 3>& tr, const ublas::bounded_matrix<double, 3, 3>& ttr, rom::numerical::geometry::Ellipse& ellipse )
{
	ellipse.setMatrix( ublas::prec_prod( ttr, ( ublas::bounded_matrix<double, 3, 3>) ublas::prec_prod( ellipse.matrix(), tr ) ) );
}

}
}
}

#endif

