//=================================
// include guard
#ifndef __OnlineVariance_H_INCLUDED__
#define __OnlineVariance_H_INCLUDED__

//=================================
// forward declared dependencies

//=================================
// included dependencies

// https://en.wikipedia.org/wiki/Algorithms_for_calculating_variance
// Online algorithm
//
class OnlineStatistics {
	double n_samples;
	double _sum;
	double smean;
	double m2;
public:

	OnlineStatistics();
	void add(const double x);
	double variance()const;
	double stddev()const;
	double mean()const;
	double sum()const;
	double number_of_samples()const;
};
#endif // __OnlineVariance_H_INCLUDED__