// Copyright 2014 Sebastian A. Mueller
#include "Core/Function/ConcatFunction.h"
#include <sstream>
#include <algorithm>
#include <exception>
using std::vector;


namespace Function {

Concat::Concat() {}

Concat::Concat(const vector<Func1D*> conc) {
    init(conc);
}

void Concat::init(const vector<Func1D*> conc) {
    this->conc = conc;
    assert_limits_do_fit();
    adopt_new_limits();
}

void Concat::assert_limits_do_fit()const {
    for (unsigned int i = 0; i < conc.size()-1; i++) {
        if (func_does_not_match_limit_of_next_func(i)) {
            std::stringstream info;
            info << "Function::Concat::" << __func__ << "()\n";
            info << __FILE__ << ", " << __LINE__ << "\n";
            info << "Expected limits of functions to be concatenated to fit. ";
            info << "function[" << i << "].upper_limit == ";
            info << "function[" << i+1 << "].lower_limit, but actual\n";
            info << "f[" << i << "].upper_limit = ";
            info << conc.at(i)->limits().get_upper() << "\n";
            info << "f[" << i+1 << "].lower_limit = ";
            info << conc.at(i+1)->limits().lower() << "\n";
            throw std::logic_error(info.str());
        }
    }
}

bool Concat::func_does_not_match_limit_of_next_func(const unsigned int i)const {
    return conc.at(i)->limits().get_upper() !=
        conc.at(i+1)->limits().lower();
}

void Concat::adopt_new_limits() {
    limits_ = Limits(
        conc.front()->limits().lower(),
        conc.back()->limits().get_upper());
}

double Concat::evaluate(const double x)const {
    limits_.assert_contains(x);
    const Func1D* sub_f = get_sub_function_responsible_for(x);
    return sub_f->evaluate(x);
}

const Func1D* Concat::get_sub_function_responsible_for(const double x)const {
    return  (*  std::upper_bound(
                    conc.begin(),
                    conc.end(),
                    x,
                    compare_upper_limit));
}

bool Concat::compare_upper_limit(const double x, const Func1D *f) {
    return f->limits().get_upper() > x;
}

}  // namespace Function
