//=================================
// include guard
#ifndef __MMCSCORSIKASUBBLOCKPARSER_H_INCLUDE__
#define __MMCSCORSIKASUBBLOCKPARSER_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include <algorithm>
#include "MmcsCorsikaSubBlock.h"
#include "Functions.h"
//=================================
class MmcsCorsikaSubBlockParser {
    MmcsCorsikaSubBlock mmcs_sub_block;
public:
    void create_from_mmcs_sub_block(const MmcsCorsikaSubBlock &mmcs_sub_block);    
   	virtual void print()const;
    virtual std::string get_print()const;
private:
    
	void fill_from_until(
        std::vector<float> &vec, 
        const uint begin, 
        const uint end
    );

	void fill_from(float &num_to_fill, const uint at);
};
#endif // __MMCSCORSIKASUBBLOCKPARSER_H_INCLUDE__ 