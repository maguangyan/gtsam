/* ----------------------------------------------------------------------------

 * GTSAM Copyright 2010, Georgia Tech Research Corporation, 
 * Atlanta, Georgia 30332-0415
 * All Rights Reserved
 * Authors: Frank Dellaert, et al. (see THANKS for the full author list)

 * See LICENSE for the license information

 * -------------------------------------------------------------------------- */

/**
 * @file    testISAM.cpp
 * @brief   Unit tests for ISAM
 * @author  Michael Kaess
 */

#include <boost/foreach.hpp>
#include <boost/assign/std/list.hpp> // for operator +=
using namespace boost::assign;

#include <CppUnitLite/TestHarness.h>

#include <gtsam/inference/BayesNet.h>
#include <gtsam/inference/IndexConditional.h>
#include <gtsam/inference/SymbolicFactorGraph.h>
#include <gtsam/inference/ISAM.h>

using namespace std;
using namespace gtsam;

typedef ISAM<IndexConditional> SymbolicISAM;

/* ************************************************************************* */
// Some numbers that should be consistent among all smoother tests

double sigmax1 = 0.786153, sigmax2 = 0.687131, sigmax3 = 0.671512, sigmax4 =
		0.669534, sigmax5 = sigmax3, sigmax6 = sigmax2, sigmax7 = sigmax1;

/* ************************************************************************* */

//// SLAM example from RSS sqrtSAM paper
//SymbolicConditional::shared_ptr x3(new SymbolicConditional("x3")),
//		x2(new SymbolicConditional("x2","x3")),
//		x1(new SymbolicConditional("x1","x2","x3")),
//		l1(new SymbolicConditional("l1","x1","x2")),
//		l2(new SymbolicConditional("l2","x1","x3"));
//
//// ISAM for sqrtSAM example
//SymbolicISAM createSlamSymbolicISAM(){
//	// Create using insert
//	SymbolicISAM bayesTree_slam;
//	bayesTree_slam.insert(x3);
//	bayesTree_slam.insert(x2);
//	bayesTree_slam.insert(x1);
//	bayesTree_slam.insert(l2);
//	bayesTree_slam.insert(l1);
//	return bayesTree_slam;
//}

/* ************************************************************************* */

//// Conditionals for ASIA example from the tutorial with A and D evidence
//SymbolicConditional::shared_ptr
//	B(new SymbolicConditional("B")),
//	L(new SymbolicConditional("L", "B")),
//	E(new SymbolicConditional("E", "B", "L")),
//	S(new SymbolicConditional("S", "L", "B")),
//	T(new SymbolicConditional("T", "E", "L")),
//	X(new SymbolicConditional("X", "E"));
//
//// ISAM for Asia example
//SymbolicISAM createAsiaSymbolicISAM() {
//	SymbolicISAM bayesTree;
//	bayesTree.insert(B);
//	bayesTree.insert(L);
//	bayesTree.insert(E);
//	bayesTree.insert(S);
//	bayesTree.insert(T);
//	bayesTree.insert(X);
//	return bayesTree;
//}
//
///* ************************************************************************* */
//TEST( ISAM, iSAM )
//{
//	SymbolicISAM bayesTree = createAsiaSymbolicISAM();
//
//	// Now we modify the Bayes tree by inserting a new factor over B and S
//
//	// New conditionals in modified top of the tree
//	SymbolicConditional::shared_ptr
//		S_(new SymbolicConditional("S")),
//		B_(new SymbolicConditional("B", "S")),
//		L_(new SymbolicConditional("L", "B", "S"));
//
//	// Create expected Bayes tree
//	SymbolicISAM expected;
//	expected.insert(S_);
//	expected.insert(B_);
//	expected.insert(L_);
//	expected.insert(E);
//	expected.insert(T);
//	expected.insert(X);
//
//	// create new factors to be inserted
//	SymbolicFactorGraph factorGraph;
//	factorGraph.push_factor("B","S");
//	factorGraph.push_factor("B");
//
//	// do incremental inference
//	bayesTree.update(factorGraph);
//
//	// Check whether the same
//  CHECK(assert_equal(expected,bayesTree));
//}
//
///* ************************************************************************* */
//TEST( ISAM, iSAM_slam )
//{
//	// Create using insert
//	SymbolicISAM bayesTree_slam = createSlamSymbolicISAM();
//
//	//New conditionals for the expected Bayes tree
//	SymbolicConditional::shared_ptr
//			l1_(new SymbolicConditional("l1","x1","x2","x3"));
//
//	// Create expected Bayes tree
//	SymbolicISAM expected_slam;
//	expected_slam.insert(x3);
//	expected_slam.insert(x2);
//	expected_slam.insert(x1);
//	expected_slam.insert(l1_);
//	expected_slam.insert(l2);
//
//
//	// create new factors to be inserted
//	SymbolicFactorGraph factorGraph_slam;
//	factorGraph_slam.push_factor("x3","l1");
//	factorGraph_slam.push_factor("x3");
//
//	// do incremental inference
//	bayesTree_slam.update(factorGraph_slam);
//
//	// Check whether the same
//	CHECK(assert_equal(expected_slam,bayesTree_slam));
//}

/* ************************************************************************* */
int main() { TestResult tr; return TestRegistry::runAllTests(tr);}
/* ************************************************************************* */
