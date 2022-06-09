#include "RapidIPSmearGauss.h"
#include <vector>
#include "TMath.h"
#include "TRandom.h"
#include "Math/Point3D.h"
#include "Math/Vector3D.h"

std::pair<double,double> RapidIPSmearGauss::smearIP(double pt, ROOT::Math::XYZVector ip_vec) {

	const double sigma1_ = gRandom->Gaus(0.,intercept_ + slope_/pt);
	const double sigma2_ = gRandom->Gaus(0.,intercept_ + slope_/pt);
	ROOT::Math::XYZVector ip_sm(ip_vec.X()+sigma1_, ip_vec.Y()+sigma2_, ip_vec.Z());
	const double smear_ = sqrt(ip_sm.Mag2());
	return std::pair<double,double>(smear_,1.2533*(intercept_ + slope_/pt));

}
