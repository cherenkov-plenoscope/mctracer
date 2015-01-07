#include <iostream> 
#include <string>
#include <math.h>

#include "gtest/gtest.h"
#include "FiniteStateMashine.h"

using namespace std;

namespace frog {
	enum frog_states {
		start_or_found_ws_wait_for_q,
		found_q_wait_for_u,
		found_u_wait_for_arbitrary_number_of_a,
		found_at_least_one_a_wait_for_k,
		found_k
	};

	enum frog_events {
		q,
		u,
		a,
		k,
		ws
	};
}
// The fixture for testing class Foo.
class FiniteStateMashineTEST : public ::testing::Test {
protected:
	// You can remove any or all of the following functions if its body
	// is empty.
	bool exception_detected = false;
	FiniteStateMashineTEST() {
	// You can do set-up work for each test here.
	}

	virtual ~FiniteStateMashineTEST() {
	// You can do clean-up work that doesn't throw exceptions here.
	}

	// If the constructor and destructor are not enough for setting up
	// and cleaning up each test, you can define the following methods:

	virtual void SetUp() {
	// Code here will be called immediately after the constructor (right
	// before each test).
		exception_detected = false;
	}

	virtual void TearDown() {
	// Code here will be called immediately after each test (right
	// before the destructor).
	}

	FiniteStateMashine frog_acceptor()const {
		FiniteStateMashine fsm;
		fsm.define_state(frog::start_or_found_ws_wait_for_q);
		fsm.define_state(frog::found_q_wait_for_u);
		fsm.define_state(frog::found_u_wait_for_arbitrary_number_of_a);
		fsm.define_state(frog::found_at_least_one_a_wait_for_k);
		fsm.define_state(frog::found_k);

		fsm.define_state_to_state_when_event(
			frog::start_or_found_ws_wait_for_q, 
			frog::start_or_found_ws_wait_for_q,
			frog::ws
		);

		fsm.define_state_to_state_when_event(
			frog::start_or_found_ws_wait_for_q, 
			frog::found_q_wait_for_u,
			frog::q
		);

		fsm.define_state_to_state_when_event(
			frog::found_q_wait_for_u, 
			frog::found_u_wait_for_arbitrary_number_of_a, 
			frog::u
		);

		fsm.define_state_to_state_when_event(
			frog::found_u_wait_for_arbitrary_number_of_a, 
			frog::found_at_least_one_a_wait_for_k, 
			frog::a
		);

		fsm.define_state_to_state_when_event(
			frog::found_at_least_one_a_wait_for_k, 
			frog::found_at_least_one_a_wait_for_k, 
			frog::a
		);

		fsm.define_state_to_state_when_event(
			frog::found_at_least_one_a_wait_for_k, 
			frog::found_k, 
			frog::k
		);

		fsm.define_state_to_state_when_event(
			frog::found_k, 
			frog::start_or_found_ws_wait_for_q, 
			frog::ws
		);	

		fsm.initialize_in_state(frog::start_or_found_ws_wait_for_q);

		return fsm;		
	}

	// Objects declared here can be used by all tests in the test case for Foo.
};
//------------------------------------------------------------------------------
TEST_F(FiniteStateMashineTEST, define_state) {

	FiniteStateMashine fsm;
	fsm.define_state(0);
	fsm.define_state(1);
	fsm.define_state(2);
	fsm.define_state(3);
}
//------------------------------------------------------------------------------
TEST_F(FiniteStateMashineTEST, add_duplicate_state) {

	try{
		FiniteStateMashine fsm;
		fsm.define_state(0);
		fsm.define_state(0);
	}catch(TracerException &e) {
		exception_detected = true;
		EXPECT_EQ(FINITE_STATE_MASHINE_HAS_DUPLICATE_STATE,e.type());
	}
	EXPECT_TRUE(exception_detected);
}
//------------------------------------------------------------------------------
TEST_F(FiniteStateMashineTEST, transition_state_must_be_defined) {

	try{
		FiniteStateMashine fsm;
		fsm.define_state(0);
		fsm.define_state_to_state_when_event(0,1,1337);
	}catch(TracerException &e) {
		exception_detected = true;
		EXPECT_EQ(FINITE_STATE_MASHINE_UNKNOWN_STATE,e.type());
	}
	EXPECT_TRUE(exception_detected);
}
//------------------------------------------------------------------------------
TEST_F(FiniteStateMashineTEST, define_valid_transitions) {

	FiniteStateMashine fsm;
	fsm.define_state(0);
	fsm.define_state(1);
	fsm.define_state(2);
	fsm.define_state(3);
	fsm.define_state_to_state_when_event(0,1,1337);
	fsm.define_state_to_state_when_event(1,2,42);
}
//------------------------------------------------------------------------------
TEST_F(FiniteStateMashineTEST, no_transition_for_unknown_event) {

	FiniteStateMashine fsm;
	fsm.define_state(0);
	fsm.define_state(1);
	fsm.define_state(2);
	fsm.define_state_to_state_when_event(0,1,1337);
	fsm.define_state_to_state_when_event(1,2,42);
	EXPECT_FALSE(fsm.transition_given_event(9999));
}
//------------------------------------------------------------------------------
TEST_F(FiniteStateMashineTEST, must_not_define_state_after_transition) {
	
	try{
		FiniteStateMashine fsm;
		fsm.define_state(0);
		fsm.define_state(1);
		fsm.define_state(2);
		fsm.define_state_to_state_when_event(0,1,1337);
		fsm.define_state_to_state_when_event(1,2,42);

		fsm.transition_given_event(1337);

		fsm.define_state(3);
	}catch(TracerException &e) {
		exception_detected = true;
		EXPECT_EQ(FINITE_STATE_MASHINE_DEFINE_STATE_AFTER_TRANSITION, e.type());
	}
	EXPECT_TRUE(exception_detected);
}
//------------------------------------------------------------------------------
TEST_F(FiniteStateMashineTEST, init_in_invalid_state) {
	
	try{
		FiniteStateMashine fsm;
		fsm.define_state(0);
		fsm.define_state(1);
		fsm.define_state(2);
		fsm.define_state_to_state_when_event(0,1,1337);
		fsm.define_state_to_state_when_event(1,2,42);

		fsm.initialize_in_state(3);

		fsm.transition_given_event(1337);
	}catch(TracerException &e) {
		EXPECT_EQ(FINITE_STATE_MASHINE_UNKNOWN_STATE, e.type());
		exception_detected = true;
	}
	EXPECT_TRUE(exception_detected);
}
//------------------------------------------------------------------------------
TEST_F(FiniteStateMashineTEST, init_in_valid_state) {
	
	FiniteStateMashine fsm;
	fsm.define_state(0);
	fsm.define_state(1);
	fsm.define_state(2);
	fsm.define_state_to_state_when_event(0,1,1337);
	fsm.define_state_to_state_when_event(1,2,42);
	fsm.initialize_in_state(0);
	fsm.transition_given_event(1337);
}
//------------------------------------------------------------------------------
TEST_F(FiniteStateMashineTEST, frog_acceptor_valid_quak) {

	FiniteStateMashine fsm = frog_acceptor();

	EXPECT_TRUE(fsm.transition_given_event(frog::ws));
	EXPECT_TRUE(fsm.transition_given_event(frog::ws));
	EXPECT_TRUE(fsm.transition_given_event(frog::ws));
	EXPECT_TRUE(fsm.transition_given_event(frog::ws));

	EXPECT_TRUE(fsm.transition_given_event(frog::q));
	EXPECT_TRUE(fsm.transition_given_event(frog::u));
	EXPECT_TRUE(fsm.transition_given_event(frog::a));
	EXPECT_TRUE(fsm.transition_given_event(frog::k));

	EXPECT_TRUE(fsm.transition_given_event(frog::ws));

	EXPECT_TRUE(fsm.transition_given_event(frog::q));
	EXPECT_TRUE(fsm.transition_given_event(frog::u));
	EXPECT_TRUE(fsm.transition_given_event(frog::a));
	EXPECT_TRUE(fsm.transition_given_event(frog::k));

	EXPECT_TRUE(fsm.transition_given_event(frog::ws));
	EXPECT_TRUE(fsm.transition_given_event(frog::ws));
	EXPECT_TRUE(fsm.transition_given_event(frog::ws));
	EXPECT_TRUE(fsm.transition_given_event(frog::ws));

	EXPECT_TRUE(fsm.transition_given_event(frog::q));
	EXPECT_TRUE(fsm.transition_given_event(frog::u));
	EXPECT_TRUE(fsm.transition_given_event(frog::a));
	EXPECT_TRUE(fsm.transition_given_event(frog::a));
	EXPECT_TRUE(fsm.transition_given_event(frog::a));
	EXPECT_TRUE(fsm.transition_given_event(frog::a));
	EXPECT_TRUE(fsm.transition_given_event(frog::a));
	EXPECT_TRUE(fsm.transition_given_event(frog::k));
}
//------------------------------------------------------------------------------
TEST_F(FiniteStateMashineTEST, frog_acceptor_bad_quak) {
	
	FiniteStateMashine fsm = frog_acceptor();

	EXPECT_TRUE (fsm.transition_given_event(frog::q));
	EXPECT_FALSE(fsm.transition_given_event(frog::q));
	EXPECT_TRUE (fsm.transition_given_event(frog::u));
	EXPECT_TRUE (fsm.transition_given_event(frog::a));
	EXPECT_TRUE (fsm.transition_given_event(frog::a));
	EXPECT_TRUE (fsm.transition_given_event(frog::a));
	EXPECT_TRUE (fsm.transition_given_event(frog::a));
	EXPECT_TRUE (fsm.transition_given_event(frog::k));
}
//------------------------------------------------------------------------------
TEST_F(FiniteStateMashineTEST, frog_acceptor_quak_bad_in_first_element) {
	
	FiniteStateMashine fsm = frog_acceptor();

	EXPECT_FALSE(fsm.transition_given_event(frog::k));
	EXPECT_TRUE (fsm.transition_given_event(frog::q));
	EXPECT_TRUE (fsm.transition_given_event(frog::u));
	EXPECT_TRUE (fsm.transition_given_event(frog::a));
	EXPECT_TRUE (fsm.transition_given_event(frog::k));
}
//------------------------------------------------------------------------------
TEST_F(FiniteStateMashineTEST, frog_acceptor_quak_bad_in_last_element) {
	
	FiniteStateMashine fsm = frog_acceptor();

	EXPECT_TRUE (fsm.transition_given_event(frog::q));
	EXPECT_TRUE (fsm.transition_given_event(frog::u));
	EXPECT_TRUE (fsm.transition_given_event(frog::a));
	EXPECT_FALSE(fsm.transition_given_event(frog::q));
}