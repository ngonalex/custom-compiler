#include "frontend/combinators/v1_combinators/ae.h"
#include "frontend/combinators/v1_combinators/open_paren.h"
#include "frontend/combinators/v1_combinators/close_paren.h"
#include "frontend/combinators/v1_combinators/num_parser.h"
#include "frontend/combinators/v1_combinators/single_op.h"
#include "frontend/combinators/basic_combinators/and_combinator.h"
#include "frontend/combinators/basic_combinators/or_combinator.h"

#define super NullParser

ParseStatus AEParser::parse(std::string inputProgram) {
	if (inputProgram.size() == 0) {
		return super::parse(inputProgram);
	}

	// first: num
	NumParser num;
	
	// second: ae op ae
	AndCombinator ae_op;
	AndCombinator ae_op_ae;

	AEParser lhs_second;		// Is this
	AEParser rhs_second;		// legal??!?
	SingleOperatorParser single_second;	
	
	ae_op.firstParser = reinterpret_cast<NullParser *>(&lhs_second);
	ae_op.secondParser = reinterpret_cast<NullParser *>(&single_second);
	ae_op_ae.firstParser = reinterpret_cast<NullParser *>(&ae_op);
	ae_op_ae.secondParser = reinterpret_cast<NullParser *>(&rhs_second);
	
	// third: ( ae op ae )
	AndCombinator opn_ae;
	AndCombinator opn_ae_op;
	AndCombinator opn_ae_op_ae;
	AndCombinator opn_ae_op_ae_cls;
	
	OpenParenParser open_paren;
	CloseParenParser close_paren;
	AEParser lhs_third;		// Is this
	AEParser rhs_third;		// legal??!?
	SingleOperatorParser single_third;	
	
	opn_ae.firstParser = reinterpret_cast<NullParser *>(&open_paren);
	opn_ae.secondParser = reinterpret_cast<NullParser *>(&lhs_third);
	opn_ae_op.firstParser = reinterpret_cast<NullParser *>(&opn_ae_op);
	opn_ae_op.secondParser = reinterpret_cast<NullParser *>(&single_third);
	opn_ae_op_ae.firstParser = reinterpret_cast<NullParser *>(&opn_ae_op);
	opn_ae_op_ae.secondParser = reinterpret_cast<NullParser *>(&rhs_third);
	opn_ae_op_ae_cls.firstParser = reinterpret_cast<NullParser *>(&opn_ae_op_ae);
	opn_ae_op_ae_cls.secondParser = reinterpret_cast<NullParser *>(&close_paren);
	
	OrCombinator first_second;
	OrCombinator first_second_third;
	
	first_second.firstParser = reinterpret_cast<NullParser *>(&num);
	first_second.secondParser = reinterpret_cast<NullParser *>(&ae_op_ae);
	first_second_third.firstParser = reinterpret_cast<NullParser *>(&first_second);
	first_second_third.secondParser = reinterpret_cast<NullParser *>(&opn_ae_op_ae_cls);
	
	return first_second_third.parse(inputProgram);
}
