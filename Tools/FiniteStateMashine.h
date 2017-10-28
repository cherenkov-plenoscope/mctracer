//=================================
// include guard
#ifndef __FINITESTATEMASHINE_H_INCLUDE__
#define __FINITESTATEMASHINE_H_INCLUDE__

//=================================
// forward declared dependencies

//=================================
// included dependencies
#include "Tools/StringTools.h"

namespace fsm {
	class State2StateOnEvent{
	private:
		bool valid = false;
		int from_state;
		int to_state;
		int event;
	public:
		bool is_valid()const {return valid;};
		void init(const int from_state, const int to_state, const int event) {
			valid = true;
			this->from_state = from_state;
			this->to_state = to_state;
			this->event = event;
		};
		int get_from_state()const {return from_state;};
		int get_to_state()const {return to_state;};
		int get_event()const {return event;};
	};
}
//=================================
class FiniteStateMashine {

	bool transition_made = false;
	std::vector<int> states;

	std::vector<fsm::State2StateOnEvent> valid_transitions;

	int state;
public:
	void define_state(const int new_state);
	void define_state_to_state_when_event(
		const int from_state, const int to_state, const int event
	);
	bool transition_given_event(const int event);
	void initialize_in_state(const int init_state);
	int get_state()const;
private:
	void assert_state_is_unique(const int new_state)const;
	void assert_state_has_been_defined(const int a_state)const;
	void assert_no_transition_made_so_far()const;
	bool state_is_already_defined(const int new_state)const;
	fsm::State2StateOnEvent get_transition_for_current_state_given(
		const int event
	)const;
};
#endif // __FINITESTATEMASHINE_H_INCLUDE__ 
