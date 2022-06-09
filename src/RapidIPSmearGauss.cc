#include "RapidIPSmearGauss.h"
#include <vector>
#include "TMath.h"
#include "TRandom.h"
#include "Math/Point3D.h"
#include "Math/Vector3D.h"

std::pair<double,double> RapidIPSmearGauss::smearIP(double pt, ROOT::Math::XYZVector ip_vec) {

	const double sigma_ = gRandom->Gaus(0.,intercept_ + slope_/pt);
	const double smear_ = ip+sigma_;
	return std::pair<double,double>(std::fabs(smear_),std::fabs(sigma_));

}
