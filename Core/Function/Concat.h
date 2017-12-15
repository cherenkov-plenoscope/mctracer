// Copyright 2014 Sebastian A. Mueller
#ifndef CORE_FUNCTION_CONCAT_H_
#define CORE_FUNCTION_CONCAT_H_

#include <vector>
#include "Func1D.h"

namespace Function {

class Concat : public Func1D {
    std::vector<Func1D*> conc;

 public:
    Concat();
    explicit Concat(const std::vector<Func1D*> conc);
    void init(const std::vector<Func1D*> conc);
    double evaluate(const double x)const;

 private:
    void assert_limits_do_fit()const;
    void adopt_new_limits();
    const Func1D* get_sub_function_responsible_for(const double x)const;
    static bool compare_upper_limit(const double x, const Func1D *f);
    bool func_does_not_match_limit_of_next_func(const unsigned int i)const;
};

}  // namespace Function

#endif  // CORE_FUNCTION_CONCAT_H_
