/* 
 * author: wei zhang
 * date: 2013/10/23
 * description: This file contains a set of statistical function
 *  definitions for Bayesian analysis in C++. These functions are
 *  mainly utilised for generating random numbers.
 */

#ifndef _GENERATOR_H
#define _GENERATOR_H

#include <iostream>
#include <armadillo>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>

using namespace std;
using namespace arma;

/*-- all the following generators are based on uniform distribution-- */
double rexp(double scale);
double rlognorm (double mu, double sigma);
double rnorm(double mu, double sigma);

// generator of one dimensional gaussian distribution
double rnormal(double mu, double sigma);

// generator of gamma distribution
double rgamma(double shape, double scale);

// generator of inverse gamma distribution
double rigamma (double shape, double scale);

// generator of multinomial distribution
void multinomial (int *count, int n, int k, const vector &p);

// generator of multi-dimensional normal distribution
void multinormal(rowvec &out, const rowvec &mean, const mat &disp);

// generator of wishart and invert wishart distribution
void rinvertwishart (mat &wishart, const mat &LAMDA, int m);
void rwishart (mat &wishart, const mat &LAMDA, int m);

// generator of dirichlet distribution
void rdirichlet (rowvec &x, rowvec const &phi, int dim);

// generator of weibull distribution
double rweibull(double shape, double scale);

#endif
