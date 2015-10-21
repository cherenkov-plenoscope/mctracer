#include "FiniteStateMashine.h"
//------------------------------------------------------------------------------
void FiniteStateMashine::define_state(const int new_state) {
	assert_no_transition_made_so_far();
	assert_state_is_unique(new_state);
	states.push_back(new_state);
}
//------------------------------------------------------------------------------
void FiniteStateMashine::assert_state_is_unique(const int new_state)const {
	if(state_is_already_defined(new_state)){
		std::stringstream info;
		info << "FiniteStateMashine:\n";
		info << "There must not be duplicate states. ";
		info << "Expected state: " << new_state << " to be unique, ";
		info << "but actual its a duplicate.\n";
		throw TracerException(info.str());
	}
}
//------------------------------------------------------------------------------
bool FiniteStateMashine::state_is_already_defined(const int new_state)const {
	for(int state : states) 
		if(state == new_state)
			return true;
	return false;
}
//------------------------------------------------------------------------------
void FiniteStateMashine::define_state_to_state_when_event(
	const int from_state, const int to_state, const int event
) {
	assert_no_transition_made_so_far();
	assert_state_has_been_defined(from_state);
	assert_state_has_been_defined(to_state);

	fsm::State2StateOnEvent transition; 
	transition.init(from_state, to_state, event);
	valid_transitions.push_back(transition);
}
//------------------------------------------------------------------------------
void FiniteStateMashine::assert_state_has_been_defined(const int a_state)const {
	if(!state_is_already_defined(a_state)) {
		std::stringstream info;
		info << "FiniteStateMashine:\n";
		info << "Request for unknown state. ";
		info << "Expected state: " << a_state << " to be defined previous, ";
		info << "but actual its not.\n";
		throw TracerException(info.str());		
	}
}
//------------------------------------------------------------------------------
bool FiniteStateMashine::transition_given_event(const int event) {
	transition_made = true;

	fsm::State2StateOnEvent transition_to_make = 
		get_transition_for_current_state_given(event);

	if(transition_to_make.is_valid()) {
		state = transition_to_make.get_to_state();
		return true;
	}else{
		return false;
	}
}
//------------------------------------------------------------------------------
void FiniteStateMashine::assert_no_transition_made_so_far()const {
	if(transition_made) {
		std::stringstream info;
		info << "FiniteStateMashine:\n";
		info << "Further defining behaviour of FSM ";
		info << "after first state transition. ";
		info << "Expected no transition took place yet, ";
		info << "but actual it does.\n";
		throw TracerException(info.str());		
	}
}
//------------------------------------------------------------------------------
void FiniteStateMashine::initialize_in_state(const int init_state) {
	assert_state_has_been_defined(init_state);
	state = init_state;
}
//------------------------------------------------------------------------------
fsm::State2StateOnEvent FiniteStateMashine::get_transition_for_current_state_given(
	const int event
)const {
	for(fsm::State2StateOnEvent valid_transition : valid_transitions)
		if(valid_transition.get_from_state() == state && valid_transition.get_event() == event )
			return valid_transition;

	fsm::State2StateOnEvent invalid_transition;	
	return invalid_transition;
}
//------------------------------------------------------------------------------
int FiniteStateMashine::get_state()const {
	return state;
}