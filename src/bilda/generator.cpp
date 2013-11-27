/* 
 * author: wei zhang
 * date: 2013/10/23
 * description: This file contains a set of statistical function
 *  implementations for bayesian analysis in C++. These functions
 *  are mainly utilised for generating random numbers.
 */

#include "generator.h"

double rexp(double scale){
    double rval;

    rval = -log(drand48()) / scale;

    return rval;
}

// This gaussian generator is based on the algorithm proposed in 1964
// by Marsaglia and Bray
double rnormal(double mu, double sigma){
	static int iset=0;
	static double gset;
	double fac,rsq,v1,v2;
   
	if  (iset == 0) {
		do {
			v1=2.0*drand48()-1.0;
			v2=2.0*drand48()-1.0;
			rsq=v1*v1+v2*v2;
		} while (rsq >= 1.0 || rsq == 0.0);
		fac=sqrt(-2.0*log(rsq)/rsq);
		gset=mu + sigma*v1*fac;
		iset=1;
		return mu + sigma*v2*fac;
	} else {
		iset=0;
		return gset;
	}
}


double rnorm(double u, double sigma){
    double y1, y2, u1, u2, s, b, x;
    
    s = 1.0;
    while (s >= 1){
        y1 = drand48();
        y2 = drand48();
	 
  		u1 = 2*y1 - 1;
  		u2 = 2*y2 - 1;

  		s = pow (u1,2) + pow (u2,2);
		if (s >= 1) continue;
		b = sqrt(( - (2 * log(s) / s)));

		x = b * u1;
  		x = mu + sigma * x;
    }

	return x;
}


double rgamma (double shape, double scale){
   int i;
   double  b, h, r, g, f, x, r2, d, gamma1=10;

   b = shape - 1.0;
   
   if (b >=0) {
	   h = sqrt (3 * shape - 0.75);
       do {
          do {
             do {
                r = drand48 ();
                g = r - pow(r, 2);
             } while (g <= 0.0);
             f = (r - 0.5) * h /sqrt(g);
   			 x = b + f;
           } while (x <= 0.0);
   			r2 = drand48 ();
   			d = 64 * g * (pow(r2*g, 2));
   			if (d <= 0) {
               gamma1 = x;
               break;
            }
   			if (d*x < (x - 2*f*f)) {
                gamma1 = x;
                 break;
            }
       } while (log(d) > 2*(b*log(x/b) - f));
       gamma1 = x;
       gamma1 = (1 / scale) * gamma1;
   }
   else if (b < 0) {
	   x = rgamma (shape+1, 1);
	   r = drand48 ();
	   x = x*pow(r, 1/shape);
	   gamma1 = x / scale;
   }
   return gamma1;
}     	


double rigamma (double shape, double scale){
    double temp;
    temp = rgamma (shape, scale);
    temp = 1 / temp;
    return temp;
} 


double rlognorm (double mu, double sigma){
	double x;
    x = rnorm (mu, sigma);
	x = exp(x);
	return x;
}


void multinomial (int *count, int n, int k, const rowvec &p){
    int i, j;
    double u, *z, sum;
    
    z = new double[k+1];
    for (i = 0; i < k; i++) count[i] = 0;
    z[0] = 0; 
    
    for (i = 1; i <= k; i++) {
        sum = 0;
        for (j = 0; j < i; j++) sum+= p(j);
        z[i] = sum;
    }
  	
    for (j = 0; j < n; j++){
        u = drand48();
        for (i = 0; i < k; i++){
            if ((z[i] < u) && (u <= z[i+1])) count[i]++;
        }
    }
    free(z);  
}  


void multinormal(rowvec &out, const rowvec &mean, const mat &disp){
	int i, dim = disp.n_rows;
	rowvec x(dim), z(dim);

	mat l = chol(disp);

	for (i = 0; i < dim; i++) {
		z(i) = rnorm (0, 1);
	}

	x = disp * z;
	for (i = 0; i < dim; i++) out(i) = mean(i) + x.a[i];   
}


void rinvertwishart (mat &wishart, const mat &LAMDA, int m){
	int i, j, dim = LAMDA.n_rows;
	rowvec temp(dim), mean(dim);

	mat X(m, dim), tX(dim, m), temp2(dim, dim);

	for (i = 0; i < dim; i++) mean(i) = 0.0;
	for (i = 0; i < m; i++) {
		multinormal (temp, mean, LAMDA);
		for (j = 0; j < dim; j++) X(i,j) = temp(j);
	}
	
	temp2 = X.t()*X;
	wishart = inv(temp2);
}


void rwishart (mat &wishart, const mat &LAMDA, int m){
	int i, j, dim = LAMDA.n_rows;
	rowvec temp(dim), mean(dim);

	mat X(m, dim), tX(dim, m);

	for (i = 0; i < dim; i++) mean(i) = 0.0;
	for (i = 0; i < m; i++) {
		multinormal (temp, mean, LAMDA);
		for (j = 0; j < dim; j++) X(i,j) = temp(j);
	}
	
	wishart = X.t()*X;
}


void rdirichlet (rowvec &x, rowvec const &phi, int dim){
	int i;
	double sum = 0.0;
	rowvec theta(dim);

	for (i = 0; i < dim; i++){
		theta(i) = rgamma(phi(i),1);
	}
	sum = sum(theta);
	
	for (i = 0; i < dim; i++) x(i) = theta(i) / sum;

}


double rweibull(double shape, double scale){
	double x;
	x = rexp(1.0);
	x = pow((x / scale), 1/shape);
	return x;
}
