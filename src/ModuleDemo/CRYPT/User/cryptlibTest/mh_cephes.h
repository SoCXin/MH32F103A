#ifndef __MH_CEPHES_H
#define __MH_CEPHES_H

double mh_cephes_igamc(double a, double x);
double mh_cephes_igam(double a, double x);
double mh_cephes_lgam(double x);
double mh_cephes_p1evl(double x, double *coef, int N);
double mh_cephes_polevl(double x, double *coef, int N);
double mh_cephes_erf(double x);
double mh_cephes_erfc(double x);
double mh_cephes_normal(double x);

#endif /*  __MH_CEPHES_H  */
