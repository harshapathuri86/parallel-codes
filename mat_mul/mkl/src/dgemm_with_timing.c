//==============================================================
//
// SAMPLE SOURCE CODE - SUBJECT TO THE TERMS OF SAMPLE CODE LICENSE AGREEMENT,
// http://software.intel.com/en-us/articles/intel-sample-source-code-license-agreement/
//
// Copyright 2016-2018 Intel Corporation
//
// THIS FILE IS PROVIDED "AS IS" WITH NO WARRANTIES, EXPRESS OR IMPLIED,
// INCLUDING BUT NOT LIMITED TO ANY IMPLIED WARRANTY OF MERCHANTABILITY, FITNESS
// FOR A PARTICULAR PURPOSE, NON-INFRINGEMENT OF INTELLECTUAL PROPERTY RIGHTS.
//
// =============================================================
/*******************************************************************************
 *   This example measures performance of computing the real matrix product
 *   C=alpha*A*B+beta*C using Intel(R) MKL function dgemm, where A, B, and C are
 *   matrices and alpha and beta are double precision scalars.
 *
 *   In this simple example, practices such as memory management, data
 *alignment, and I/O that are necessary for good programming style and high
 *Intel(R) MKL performance are omitted to improve readability.
 ********************************************************************************/

#include "mkl.h"
#include <stdio.h>
#include <stdlib.h>

/* Consider adjusting LOOP_COUNT based on the performance of your computer */
/* to make sure that total run time is at least 1 second */

int main(int argc, char const *argv[])
{
  double *A, *B, *C;
  int m, n, p, i, r;
  double alpha, beta;
  double s_initial, s_elapsed;
  int LOOP_COUNT=10;
  printf(
      "\n This example measures performance of Intel(R) MKL function dgemm \n"
      " computing real matrix C=alpha*A*B+beta*C, where A, B, and C \n"
      " are matrices and alpha and beta are double precision scalars\n\n");

  m = 1000, p = 1000, n = 1000;
  if(argc>=4){
    m = atoi(argv[1]);
    p = atoi(argv[2]);
    n = atoi(argv[3]);
  }
  if(argc>=5) LOOP_COUNT = atoi(argv[4]);
  printf(" Initializing data for matrix multiplication C=A*B for matrix \n"
         " A(%ix%i) and matrix B(%ix%i)\n\n",
         m, p, p, n);
  alpha = 1.0;
  beta = 0.0;

  printf(" Allocating memory for matrices aligned on 64-byte boundary for "
         "better \n"
         " performance \n\n");
  A = (double *)mkl_malloc(m * p * sizeof(double), 64);
  B = (double *)mkl_malloc(p * n * sizeof(double), 64);
  C = (double *)mkl_malloc(m * n * sizeof(double), 64);
  if (A == NULL || B == NULL || C == NULL)
  {
    printf("\n ERROR: Can't allocate memory for matrices. Aborting... \n\n");
    mkl_free(A);
    mkl_free(B);
    mkl_free(C);
    return 1;
  }

  printf(" Intializing matrix data \n\n");
  for (i = 0; i < (m * p); i++)
  {
    A[i] = (double)(i + 1);
  }

  for (i = 0; i < (p * n); i++)
  {
    B[i] = (double)(-i - 1);
  }

  for (i = 0; i < (m * n); i++)
  {
    C[i] = 0.0;
  }

  printf(" Making the first run of matrix product using Intel(R) MKL dgemm "
         "function \n"
         " via CBLAS interface to get stable run time measurements \n\n");
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, p, alpha, A, p,
              B, n, beta, C, n);

  printf(" Measuring performance of matrix product using Intel(R) MKL dgemm "
         "function \n"
         " via CBLAS interface \n\n");
  s_initial = dsecnd();
  for (r = 0; r < LOOP_COUNT; r++)
  {
    cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, p, alpha, A, p,
                B, n, beta, C, n);
  }
  s_elapsed = (dsecnd() - s_initial) / LOOP_COUNT;

  printf(" == Matrix multiplication using Intel(R) MKL dgemm completed == \n"
         " == at %.5f milliseconds == \n\n",
         (s_elapsed * 1000));

  printf(" Deallocating memory \n\n");
  mkl_free(A);
  mkl_free(B);
  mkl_free(C);

  if (s_elapsed < 0.9 / LOOP_COUNT)
  {
    s_elapsed = 1.0 / LOOP_COUNT / s_elapsed;
    i = (int)(s_elapsed * LOOP_COUNT) + 1;
    printf(" It is highly recommended to define LOOP_COUNT for this example on "
           "your \n"
           " computer as %i to have total execution time about 1 second for "
           "reliability \n"
           " of measurements\n\n",
           i);
  }

  printf(" Example completed. \n\n");
  return 0;
}
