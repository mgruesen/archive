#include <iostream>
#include <cstdlib> // for rand
#include <ctime> // for time
#include <omp.h>
#include <cstring>
#include <sys/time.h>

#include "Eigen/Core"
#include "Eigen/Dense"
#include "Eigen/Eigenvalues"
#include "Eigen/LU"
#include "mat.hpp"

using namespace std;
using namespace Eigen;

// Print a matrix row by row
void mat::matrix::print()
{
  int row;
  for (int i = 0; i < this->rows; i++) {
    row = i * this->cols;
    for (int j = 0; j < this->cols; j++) {
      std::cout << this->vals[row + j] << " ";
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

// Generic serial matrix multiply function call. Determines best optimization
// to use with the given size
void
mat::mult_ser(int size, float* a, float* b, float* c)
{
  mat::matrix* mat_b = new mat::matrix(size, size);
  mat_b->vals = b;
  if (size > 512)
    mat::mult_ser_opt4(size, a, mat_b, c);
  else
    mat::mult_ser_opt2(size, a, mat_b, c);
}

// Generic parallel matrix multiply function call. Determines best optimization
// to use with the given size
void
mat::mult_par(int size, float* a, float* b, float* c)
{
  mat::matrix* mat_b = new mat::matrix(size, size);
  mat_b->vals = b;
  if (size > 512)
    mat::mult_par_opt4(size, a, mat_b, c);
  else
    mat::mult_par_opt2(size, a, mat_b, c);
}

// Generic serial matrix transpose function call. Determines best optimization
// to use with the given size
mat::matrix*
mat::trans_ser(matrix* a)
{
  int size = a->cols;
  if (size > 512)
    return mat::trans_ser_opt3(a);
  else
    return mat::trans_ser_opt1(a);
}

// Generic serial matrix transpose function call. Determines best optimization
// to use with the given size
mat::matrix*
mat::trans_par(matrix* a)
{
  int size = a->cols;
  if (size > 512)
    return mat::trans_par_opt3(a);
  else
    return mat::trans_par_opt1(a);
}

// Base serial matrix multiplication implementation.
void 
mat::mult_ser_base(int size, float* a, float* b, float* c)
{
  float l_sum;
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      l_sum = 0;
      for(int k = 0; k < size; k++) {
        l_sum += a[i * size + k] * b[k * size + j];
      }
      c[i * size + j] = l_sum;
    }
  }
}


// Base parallel matrix multiplication implementation.
void 
mat::mult_par_base(int size, float* a, float* b, float* c)
{
  int i,j,k;
  float l_sum;
  #pragma omp parallel for  private(i, j, k, l_sum) schedule(static) 
  for (i = 0; i < size; i++) {      
    for (j = 0; j < size; j++) {
      l_sum = 0;     
      for (k = 0; k < size; k++) {
        l_sum += a[i * size + k] * b[k * size + j];
      }
      c[i * size + j] = l_sum;
    }
  }  
}


// Serial matrix multiplication with row caching.
void 
mat::mult_ser_opt1(int size, float* a, float* b, float* c)
{
  float l_sum;
  float* a_row = new float[size];
  float* c_row = new float[size];
  for (int i = 0; i < size; i++) {
    a_row = &a[i * size];
    for (int j = 0; j < size; j++) {
      l_sum = 0;      
      for(int k = 0; k < size; k++) {
        l_sum += a_row[k] * b[k * size + j];
      }
      c_row[j] = l_sum;
    }
    memcpy(&c[i * size], c_row, size);
  }
}


// Parallel matrix multiplication with row caching.
void 
mat::mult_par_opt1(int size, float* a, float* b, float* c)
{
  float* a_row = new float[size];
  float* c_row;
  int i,j,k;
  float l_sum;
  #pragma omp parallel for shared(a,b,c) private(i, j, k, l_sum, a_row, c_row) schedule(static) 
  for (i = 0; i < size; i++) {      
    c_row = new float[size];           
    a_row = &a[i * size];
    for (j = 0; j < size; j++) {
      l_sum = 0;
      for (k = 0; k < size; k++) {
        l_sum += a_row[k] * b[k * size + j];
      }
      c_row[j] = l_sum;
    }
    memcpy(&c[i * size], c_row, size);
  }  
}


// Serial matrix multiplication with row caching and transposition.
void 
mat::mult_ser_opt2(int size, float* a, mat::matrix* b, float* c)
{
  float* b_t = mat::trans_ser(b)->vals;
  float l_sum;
  float* a_row = new float[size];
  float* b_row = new float[size];
  float* c_row = new float[size];
  for (int i = 0; i < size; i++) {
    a_row = &a[i * size];    
    for (int j = 0; j < size; j++) {
      l_sum = 0;
      b_row = &b_t[j * size];
      for(int k = 0; k < size; k++) {        
        l_sum += a_row[k] * b_row[k];
      }
      c_row[j] = l_sum;
    }
    memcpy(&c[i * size], c_row, size);
  }
}


// Parallel matrix multiplication with row caching and transposition.
void 
mat::mult_par_opt2(int size, float* a, mat::matrix* b, float* c)
{
  float* b_t = mat::trans_par(b)->vals;
  float* a_row = new float[size];
  float* b_row = new float[size];
  float* c_row;
  int i,j,k;
  float l_sum;
  #pragma omp parallel for shared(a,b_t,c) private(i, j, k, l_sum, a_row, b_row, c_row) schedule(static)  
  for (i = 0; i < size; i++) {      
    a_row = &a[i * size];
    c_row = new float[size];
    for (j = 0; j < size; j++) {
      l_sum = 0;         
      b_row = &b_t[j * size];
      for (k = 0; k < size; k++) {        
        l_sum += a_row[k] * b_row[k];
      }
      c_row[j] = l_sum;
    }
    memcpy(&c[i * size], c_row, size);
  }  
}


// Serial matrix multiplication with row caching, transposition,
// and striping.
void 
mat::mult_ser_opt3(int size, float* a, mat::matrix* b, float* c)
{
  float* b_t = mat::trans_ser(b)->vals;
  float l_sum;
  int kk_size = 512;
  float* a_row = new float[kk_size];
  float* b_row = new float[kk_size];
  float* c_row = new float[size];
  for (int i = 0; i < size; i++) {     
    for (int j = 0; j < size; j++) { 
      l_sum = 0;
      for (int k = 0; k < size; k += kk_size) {
        a_row = &a[i * size + k];   
        b_row = &b_t[j * size + k];        
        for (int kk = 0; kk < kk_size; kk++)           
          l_sum += a_row[kk] * b_row[kk];
      } 
      c_row[j] = l_sum;     
    }
    memcpy(&c[i * size], c_row, size);
  }
}


// Parallel matrix multiplication with row caching, transposition, 
// and striping.
void 
mat::mult_par_opt3(int size, float* a, mat::matrix* b, float* c)
{
  float* b_t = mat::trans_par(b)->vals;
  int kk_size = 512;
  float* a_row = new float[kk_size];
  float* b_row = new float[kk_size];
  float* c_row;
  float l_sum;
  int i, j, k, kk, b_val;
  #pragma omp parallel for shared(kk_size, a, b_t, c) private(i, j, k, kk, l_sum, a_row, b_row, c_row, b_val) schedule(static) 
  for (i = 0; i < size; i++) {
    c_row = new float[size];
    for (j = 0; j < size; j++) {
      l_sum = 0;
      for (k = 0; k < size; k += kk_size) {
        a_row = &a[i * size + k];
        b_row = &b_t[j * size + k];
        for (kk = 0; kk < kk_size; kk++)
          l_sum += a_row[kk] * b_row[kk];
      }
      c_row[j] = l_sum;
    }
    memcpy(&c[i * size], c_row, size);
  }  
}


// Serial matrix multiplication with row caching, transposition,
// striping, and loop unrolling.
void 
mat::mult_ser_opt4(int size, float* a, mat::matrix* b, float* c)
{
  float* b_t = mat::trans_ser(b)->vals;
  float l_sum;
  int kk_size = 512;
  float* a_row = new float[kk_size];
  float* b_row = new float[kk_size];
  float* c_row = new float[size];  
  for (int i = 0; i < size; i++) {     
    for (int j = 0; j < size; j++) { 
      l_sum = 0;
      for(int k = 0; k < size; k += kk_size) {   
        a_row = &a[i * size + k];   
        b_row = &b_t[j * size + k];
        for (int kk = 0; kk < kk_size; kk += 8) {          
          l_sum += a_row[kk] * b_row[kk];
          l_sum += a_row[kk + 1] * b_row[kk + 1];
          l_sum += a_row[kk + 2] * b_row[kk + 2];
          l_sum += a_row[kk + 3] * b_row[kk + 3];
          l_sum += a_row[kk + 4] * b_row[kk + 4];
          l_sum += a_row[kk + 5] * b_row[kk + 5];
          l_sum += a_row[kk + 6] * b_row[kk + 6];
          l_sum += a_row[kk + 7] * b_row[kk + 7];
        }
      } 
      c_row[j] = l_sum;     
    }
    memcpy(&c[i * size], c_row, size);
  }
}


// Parallel matrix multiplication with row caching, transposition, 
// striping, and loop unrolling.
void 
mat::mult_par_opt4(int size, float* a, mat::matrix* b, float* c)
{
  float* b_t = mat::trans_par(b)->vals;
  int kk_size = 512;
  float* a_row = new float[size];
  float* b_row = new float[size];
  float* c_row;
  float l_sum;
  int i, j, k, b_val;
  #pragma omp parallel for shared(kk_size, a, b_t, c) private(i, j, k, l_sum, a_row, b_row, c_row, b_val) schedule(static) 
  for (i = 0; i < size; i++) {
    c_row = new float[size];
    for (j = 0; j < size; j++) {
      l_sum = 0;      
      for (k = 0; k < size; k += kk_size) {
        a_row = &a[i * size + k];
        b_row = &b_t[j * size + k];
        for (int kk = 0; kk < kk_size; kk += 8) {          
          l_sum += a_row[kk] * b_row[kk];
          l_sum += a_row[kk + 1] * b_row[kk + 1];
          l_sum += a_row[kk + 2] * b_row[kk + 2];
          l_sum += a_row[kk + 3] * b_row[kk + 3];
          l_sum += a_row[kk + 4] * b_row[kk + 4];
          l_sum += a_row[kk + 5] * b_row[kk + 5];
          l_sum += a_row[kk + 6] * b_row[kk + 6];
          l_sum += a_row[kk + 7] * b_row[kk + 7];
        }
      }
      c_row[j] = l_sum;
    }
    memcpy(&c[i * size], c_row, size);
  }  
}


// Base serial matrix transpose implementation 
mat::matrix* 
mat::trans_ser_base(mat::matrix* a)
{
  mat::matrix* a_t = new mat::matrix(a->cols, a->rows);
  for (int i = 0; i < a->rows; i++)
    for (int j = 0; j < a->cols; j++)
    a_t->vals[j * a_t->cols + i] = a->vals[i * a->cols + j];
  return a_t;
}


// Base parallel matrix transpose implementation 
mat::matrix* 
mat::trans_par_base(mat::matrix* a)
{
  mat::matrix* a_t = new mat::matrix(a->cols, a->rows);
  #pragma omp parallel for shared(a_t)
  for (int i = 0; i < a->rows; i++) {
    for (int j = 0; j < a->cols; j++) 
      a_t->vals[j * a_t->cols + i] = a->vals[i * a->cols + j];
  }
  return a_t;
}


// Transposes a matrix in serial with row caching
mat::matrix* 
mat::trans_ser_opt1(mat::matrix* a)
{
  mat::matrix* a_t = new mat::matrix(a->cols, a->rows);
  float* a_row = new float[a->cols];
  for(int i = 0; i < a->rows; i++){
    a_row = &a->vals[i * a->cols];
    for (int j = 0; j < a->cols; j++) {
      a_t->vals[j * a_t->cols + i] = a_row[j];
    }
  }
  return a_t;
}


// Transposes a matrix in parallel with row caching
mat::matrix* 
mat::trans_par_opt1(mat::matrix* a)
{
  mat::matrix* a_t = new mat::matrix(a->cols, a->rows);
  float* a_row = new float[a->cols];
  #pragma omp parallel for shared(a, a_t) private(a_row)
  for (int i = 0; i < a->rows; i++) {
    a_row = &a->vals[i * a->cols];
    for (int j = 0; j < a->cols; j++)
      a_t->vals[j * a_t->cols + i] = a_row[j];
  }
  return a_t;
}

// Transposes a matrix in serial with row caching and striping
mat::matrix*
mat::trans_ser_opt2(mat::matrix* a)
{
  mat::matrix* a_t = new mat::matrix(a->cols, a->rows);
  int jj_size = 512;
  float* a_row = new float[jj_size];
  for(int i = 0; i < a->rows; i++){    
    for (int j = 0; j < a->cols; j += jj_size) {
      a_row = &a->vals[i * a->cols + j];
      for (int jj = 0; jj < jj_size; jj++)
        a_t->vals[(jj + j) * a_t->cols + i] = a_row[jj];
    } 
  }
  return a_t;
}

// Transposes a matrix in parallel with row caching and striping
mat::matrix*
mat::trans_par_opt2(mat::matrix* a)
{
  mat::matrix* a_t = new mat::matrix(a->cols, a->rows);
  int jj_size = 512;
  float* a_row = new float[jj_size];
  #pragma omp parallel for shared(a, a_t) private(a_row)
  for(int i = 0; i < a->rows; i++){    
    for (int j = 0; j < a->cols; j += jj_size) {
      a_row = &a->vals[i * a->cols + j];
      for (int jj = 0; jj < jj_size; jj++)
        a_t->vals[(jj + j) * a_t->cols + i] = a_row[jj];
    }
  }
  return a_t;  
}

// Transposes a matrix in serial with row caching, striping, and
// loop unrolling.
mat::matrix* 
mat::trans_ser_opt3(mat::matrix* a)
{
  mat::matrix* a_t = new mat::matrix(a->cols, a->rows);
  int jj_size = 512;
  float* a_row = new float[jj_size];
  for (int i = 0; i < a->rows; i++) {    
    for (int j = 0; j < a->cols; j += jj_size) {
      a_row = &a->vals[i * a->cols + j];
      for (int jj = 0; jj < jj_size; jj+=8) {
        a_t->vals[jj * a_t->cols + i] = a_row[jj];
        a_t->vals[(jj + j + 1) * a_t->cols + i] = a_row[jj + 1];
        a_t->vals[(jj + j + 2) * a_t->cols + i] = a_row[jj + 2];
        a_t->vals[(jj + j + 3) * a_t->cols + i] = a_row[jj + 3];
        a_t->vals[(jj + j + 4) * a_t->cols + i] = a_row[jj + 4];
        a_t->vals[(jj + j + 5) * a_t->cols + i] = a_row[jj + 5];
        a_t->vals[(jj + j + 6) * a_t->cols + i] = a_row[jj + 6];
        a_t->vals[(jj + j + 7) * a_t->cols + i] = a_row[jj + 7]; 
      }
    }
  }
  return a_t; 
}


// Transposes a matrix in parallel with row caching, striping, and
// loop unrolling.
mat::matrix* 
mat::trans_par_opt3(mat::matrix* a)
{
  mat::matrix* a_t = new mat::matrix(a->cols, a->rows);
  int jj_size = 512;
  float* a_row = new float[jj_size];
  #pragma omp parallel for shared(a, a_t) private(a_row)
  for (int i = 0; i < a->rows; i++) {    
    for (int j = 0; j < a->cols; j += jj_size) {
      a_row = &a->vals[i * a->cols + j];
      for (int jj = 0; jj < jj_size; jj+=8) {
        a_t->vals[jj * a_t->cols + i] = a_row[jj];
        a_t->vals[(jj + j + 1) * a_t->cols + i] = a_row[jj + 1];
        a_t->vals[(jj + j + 2) * a_t->cols + i] = a_row[jj + 2];
        a_t->vals[(jj + j + 3) * a_t->cols + i] = a_row[jj + 3];
        a_t->vals[(jj + j + 4) * a_t->cols + i] = a_row[jj + 4];
        a_t->vals[(jj + j + 5) * a_t->cols + i] = a_row[jj + 5];
        a_t->vals[(jj + j + 6) * a_t->cols + i] = a_row[jj + 6];
        a_t->vals[(jj + j + 7) * a_t->cols + i] = a_row[jj + 7]; 
      }
    }
  }
  return a_t;
}

// Generates a matrix with random values with given dimension
mat::matrix* 
mat::generate_random_matrix(int size)
{
  std::srand(std::time(0)); 
  mat::matrix* ret = new mat::matrix(size, size);
  int ub = size * size;
  for (int i = 0; i < ub; i++)
    ret->vals[i] = std::rand();
  return ret;
}

// Returns the next power
int 
next_power(int width, int height)
{
  int limit = 2;
  while (limit < width && limit < height) 
    limit *= 2;
  return limit;
}

// Returns a sub-matrix of the given width and height
mat::matrix* 
sub_matrix(mat::matrix& a, int left, int top, int width, int height){
  mat::matrix* ret = new mat::matrix(width, height);
  if (width == 1 && height == 1) {
    ret->vals[0] = a.vals[top * a.cols + left];
    return ret;
  }
  for (int i = top; i < top + height; i++)
    for (int j = left; j < left + width; j++)
      ret->vals[i * ret->cols + j] = a.vals[i * a.cols + j];
  return ret;
}

// Subtracts two matrices
void 
subtract(mat::matrix& a, mat::matrix& b, mat::matrix& c){
  for (int i = 0; i < a.rows; i++) {
    for (int j = 0; j < a.cols; j++) {
      int index = i * a.cols + j;
      c.vals[index] = a.vals[index] - b.vals[index];
    }
  }
  c.print();
}

// Parallel matrix inversion
mat::matrix*
mat::invert_par(mat::matrix& a)
{
  Map<MatrixXf>  mxd(a.vals,a.rows,a.cols);
  auto mxdi = mxd.inverse();
  mat::matrix* answer = new mat::matrix(a.rows,a.cols);
  int* answer_row;
  int i, j;
  #pragma omp parallel for shared(mxdi) private(i,j,answer_row)
  for (i = 0; i < a.rows ; i++){
    answer_row = new int[a.cols];
    for (j = 0; j < a.cols; j++)
      answer_row[j] = (float)mxdi.block(i,j,1,1)(0,0);
    memcpy(&answer->vals[i*a.cols], answer_row, a.cols);
  }
  return answer;
   
}

// Serial matrix inversion
mat::matrix* 
mat::invert_ser(mat::matrix& a)
{
  Map<MatrixXf>  mxd(a.vals,a.rows,a.cols);
  auto mxdi = mxd.inverse();
  mat::matrix* answer = new mat::matrix(a.rows,a.cols);
  for (int i = 0; i < a.rows ; i++)
    for (int j = 0; j < a.cols; j++)
      answer->vals[i*a.cols+j] = (float)mxdi.block(i,j,1,1)(0,0);
  return answer;
  
  
}

// Serial eigendecomposition implementation
void 
mat::eigendecomp_ser(matrix& a, matrix& q, matrix& s)
{
   if(a.cols != a.rows) return;
   Map<MatrixXf>* eigenmatrix = new Map<MatrixXf>(a.vals, a.rows, a.cols);
   EigenSolver<MatrixXf>* es = new EigenSolver<MatrixXf>(*eigenmatrix);
   VectorXcf values =  es->eigenvalues();
   auto  vectors = es->eigenvectors();
   for(int i = 0; i < q.rows ; i++){
     for(int j = 0; j < q.cols; j++){
       q.vals[i*q.cols+j] = ((complex<float>)vectors.block(i,j,1,1)(0,0)).real();
     }
   }
   for(int i = 0; i < s.rows ; i++){
     for(int j = 0; j < s.cols; j++){
       s.vals[i*s.cols+j] = (i==j) ? ((complex<float>)values(i)).real() : 0;
     }
   }
}

// Parallel eigendecomposition implementation
void 
mat::eigendecomp_par(matrix& a, matrix& q, matrix& s)
{
   if(a.cols != a.rows) return;
   Map<MatrixXf>* eigenmatrix = new Map<MatrixXf>(a.vals, a.rows, a.cols);
   EigenSolver<MatrixXf>* es = new EigenSolver<MatrixXf>(*eigenmatrix);
   VectorXcf values =  es->eigenvalues();
   auto  vectors = es->eigenvectors();
   int i, j;
   int* q_row;
   int* s_row;
   #pragma omp parallel for private(i,j, q_row, s_row) shared(vectors,values)
   for(i = 0; i < q.rows ; i++){
    q_row = new int[q.cols];
    s_row = new int[q.cols];
     for(j = 0; j < q.cols; j++){
       q_row[j] = ((complex<float>)vectors.block(i,j,1,1)(0,0)).real();
       s_row[j] = (i==j) ? ((complex<float>)values(i)).real() : 0;
     }
     memcpy(&q.vals[i*q.cols], q_row, q.cols);
     memcpy(&s.vals[i*s.cols], s_row, q.cols);
   }
}

// Serial eigenrecomposition implementation
mat::matrix* 
mat::eigenrecomp_ser(matrix& q, matrix& s)
{   
   matrix* c = new matrix(q.rows,s.cols);
   mult_ser(q.cols,q.vals,s.vals,c->vals); 

   matrix* q_1 = invert_ser(q);
   matrix* c_ = new matrix(q.cols,q.cols);
   mult_ser(q.cols,c->vals,q_1->vals,c_->vals);

   return c_;
}

// Parallel eigenrecomposition implementation
mat::matrix* 
mat::eigenrecomp_par(matrix& q, matrix& s)
{
   matrix* c = new matrix(q.rows,s.cols);
   mult_par(q.cols,q.vals,s.vals,c->vals); 

   matrix* q_1 = invert_ser(q);
   matrix* c_ = new matrix(q.cols,q.cols);
   mult_par(q.cols,c->vals,q_1->vals,c_->vals);

   return c_;
}

