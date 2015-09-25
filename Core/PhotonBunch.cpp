#include "PhotonBunch.h"

namespace PhotonBunch {

	std::vector<Photon> raw_matrix2photons(std::vector<std::vector<double>> &raw_matrix) {

		std::vector<Photon> photon_bunch;

		for(std::vector<double> raw_row : raw_matrix)
			photon_bunch.push_back(raw_row2photon(raw_row));

		return photon_bunch;
	}

	std::vector<std::vector<double>> photons2raw_matrix(std::vector<Photon> &photon_bunch) {

		std::vector<std::vector<double>> raw_matrix;

		for(Photon ph : photon_bunch)
			raw_matrix.push_back(photon2raw_row(ph));

		return raw_matrix;		
	}

	std::vector<double> photon2raw_row(Photon &ph) {
		std::vector<double> raw_row;
		raw_row.reserve(8);

		raw_row.push_back(double(ph.get_id()));

		raw_row.push_back(ph.Support().x());
		raw_row.push_back(ph.Support().y());
		raw_row.push_back(ph.Support().z());

		raw_row.push_back(ph.Direction().x());
		raw_row.push_back(ph.Direction().y());
		raw_row.push_back(ph.Direction().z());

		raw_row.push_back(ph.get_wavelength());

		return raw_row;
	}

	Photon raw_row2photon(std::vector<double> &raw_row) {

		assert_raw_row_size_matches_photon(raw_row);
		const double id = raw_row[0];
		const Vector3D support(raw_row[1], raw_row[2], raw_row[3]);
		const Vector3D direction(raw_row[4], raw_row[5], raw_row[6]);
		const double wavelength = raw_row[7];

		Photon ph(support, direction, wavelength);
		ph.set_id(id);
		return ph;
	}

	void assert_raw_row_size_matches_photon(std::vector<double> &raw_row) {
		if(raw_row.size() != 8) {
			std::stringstream out;
			out << "PhotonBunch, raw row of doubles to photon.\n";
			out << "Expected row to have exactly 8 columns, but actual it has ";
			out << raw_row.size() << " columns.\n";
			throw TracerException(out.str());
		}
	}
}