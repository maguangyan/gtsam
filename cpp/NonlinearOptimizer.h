/**
 * NonlinearOptimizer.h
 * @brief: Encapsulates nonlinear optimization state
 * @Author: Frank Dellaert
 * Created on: Sep 7, 2009
 */

#ifndef NONLINEAROPTIMIZER_H_
#define NONLINEAROPTIMIZER_H_

#include <boost/shared_ptr.hpp>
#include "NonlinearFactorGraph.h"
#include "VectorConfig.h"

namespace gtsam {

	/**
	 * The class NonlinearOptimizer encapsulates an optimization state.
	 * Typically it is instantiated with a NonlinearFactorGraph and an initial config
	 * and then one of the optimization routines is called. These recursively iterate
	 * until convergence. All methods are functional and return a new state.
	 *
	 * The class is parameterized by the Graph type, Config class type and the linear solver type.
	 * the config type is in order to be able to optimize over non-vector configurations as well.
	 * To use in code, include <gtsam/NonlinearOptimizer-inl.h> in your cpp file
	 * (the trick in http://www.ddj.com/cpp/184403420 did not work).
	 */
	template<class FactorGraph, class Config, class LinearSolver = Factorization<FactorGraph, Config> >
	class NonlinearOptimizer {
	public:

		// For performance reasons in recursion, we store configs in a shared_ptr
		typedef boost::shared_ptr<const Config> shared_config;
		typedef boost::shared_ptr<const FactorGraph> shared_graph;
		typedef boost::shared_ptr<const Ordering> shared_ordering;
		typedef boost::shared_ptr<const LinearSolver> shared_solver;

		enum verbosityLevel {
			SILENT,
			ERROR,
			LAMBDA,
			CONFIG,
			DELTA,
			TRYLAMBDA,
			TRYCONFIG,
			TRYDELTA,
			LINEAR,
			DAMPED
		};

	private:

		// keep a reference to const versions of the graph and ordering
		// These normally do not change
		const shared_graph graph_;
		const shared_ordering ordering_;

		// keep a configuration and its error
		// These typically change once per iteration (in a functional way)
		const shared_config config_;
		const double error_;

		// keep current lambda for use within LM only
		// TODO: red flag, should we have an LM class ?
		const double lambda_;

		// the linear system solver
		const shared_solver solver_;

		// Recursively try to do tempered Gauss-Newton steps until we succeed
		NonlinearOptimizer try_lambda(const GaussianFactorGraph& linear,
				verbosityLevel verbosity, double factor) const;

	public:

		/**
		 * Constructor
		 */
		NonlinearOptimizer(shared_graph graph, shared_ordering ordering,
				shared_config config, shared_solver solver = shared_solver(new LinearSolver),
				double lambda = 1e-5);

		/**
		 * Copy constructor
		 */
		NonlinearOptimizer(const NonlinearOptimizer<FactorGraph, Config> &optimizer) :
		  graph_(optimizer.graph_), ordering_(optimizer.ordering_), config_(optimizer.config_),
		  error_(optimizer.error_), lambda_(optimizer.lambda_), solver_(optimizer.solver_) {}

		/**
		 * Return current error
		 */
		double error() const {
			return error_;
		}

		/**
		 * Return current lambda
		 */
		double lambda() const {
			return lambda_;
		}

		/**
		 * Return the config
		 */
		shared_config config() const{
			return config_;
		}

		/**
		 *  linearize and optimize
		 *  This returns an VectorConfig, i.e., vectors in tangent space of Config
		 */
		VectorConfig linearizeAndOptimizeForDelta() const;

		/**
		 * Do one Gauss-Newton iteration and return next state
		 */
		NonlinearOptimizer iterate(verbosityLevel verbosity = SILENT) const;

		/**
		 * Optimize a solution for a non linear factor graph
		 * @param relativeTreshold
		 * @param absoluteTreshold
		 * @param verbosity Integer specifying how much output to provide
		 */
		NonlinearOptimizer
		gaussNewton(double relativeThreshold, double absoluteThreshold,
				verbosityLevel verbosity = SILENT, int maxIterations = 100) const;

		/**
		 * One iteration of Levenberg Marquardt
		 */
		NonlinearOptimizer iterateLM(verbosityLevel verbosity = SILENT,
				double lambdaFactor = 10) const;

		/**
		 * Optimize using Levenberg-Marquardt. Really Levenberg's
		 * algorithm at this moment, as we just add I*\lambda to Hessian
		 * H'H. The probabilistic explanation is very simple: every
		 * variable gets an extra Gaussian prior that biases staying at
		 * current value, with variance 1/lambda. This is done very easily
		 * (but perhaps wastefully) by adding a prior factor for each of
		 * the variables, after linearization.
		 *
		 * @param relativeThreshold
		 * @param absoluteThreshold
		 * @param verbosity    Integer specifying how much output to provide
		 * @param lambdaFactor Factor by which to decrease/increase lambda
		 */
		NonlinearOptimizer
		levenbergMarquardt(double relativeThreshold, double absoluteThreshold,
				verbosityLevel verbosity = SILENT, int maxIterations = 100,
				double lambdaFactor = 10) const;

	};

	/**
	 * Check convergence
	 */
	bool check_convergence (double relativeErrorTreshold,
			double absoluteErrorTreshold,
			double currentError, double newError,
			int verbosity);

} // gtsam

#endif /* NONLINEAROPTIMIZER_H_ */
