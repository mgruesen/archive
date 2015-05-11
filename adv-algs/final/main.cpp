#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <omp.h>
#include <cmath>

#include "mat.hpp"

//#define debug
//#define average
using namespace std;

int num_procs = 0;

// Prints timing information for a parallel vs serial matrix trials
void 
report(double s_time, double p_time)
{
  cout << "Serial runtime: " << s_time << " sec\nParallel runtime: " << 
    p_time << " sec\n";
  cout << "Speedup: " << s_time / p_time << "\nEfficiency (p=" << 
    num_procs << "): " << (s_time / p_time) / num_procs << "\n\n";
}

// Converts a start and end time to a float representing the interval length
// in seconds.
double 
GetElapsedTime(clock_t end, clock_t begin)
{
  return (double)(end - begin) / CLOCKS_PER_SEC;
}

// Checks that every element in a == b
void
validate_equals(int size, float* a, float* b)
{
  for (int i = 0; i < size; i++)
    for (int j = 0; j < size; j++)
      if (abs(a[i * size + j] - b[i * size + j]) > 0.1) {
        cerr << "error(validate_equals): a[" << i << "," <<
          j << "] != b[" << i << "," << j << "]\n";    
        return;
      }
}

// The base matrix transpose test function
void
mt_base(int size, mat::matrix* a)
{
  // Result matrices
  mat::matrix* serial_res = new mat::matrix(size, size);
  mat::matrix* parallel_res = new mat::matrix(size, size);

  // Timing variables
  clock_t begin, end;
  double ser_time, par_time;

  // Do serial mtrans
  #ifdef average
  double total_time = 0;
    for (int i = 0; i < 10; i++) {
      ser_time = omp_get_wtime();
      serial_res = mat::trans_ser_base(a);
      ser_time = omp_get_wtime() - ser_time;
      total_time += ser_time;
    }
    ser_time = total_time / 10;
  #else
    ser_time = omp_get_wtime();
    serial_res = mat::trans_ser_base(a);
    ser_time = omp_get_wtime() - ser_time;
  #endif

  // Do paralell mtrans
  #ifdef average 
  total_time = 0;
    for (int i = 0; i < 10; i++) {
      par_time = omp_get_wtime();
      parallel_res = mat::trans_par_base(a);
      par_time = omp_get_wtime() - par_time;
      total_time += par_time;
    }
    par_time = total_time / 10;
  #else
    par_time = omp_get_wtime();
    parallel_res = mat::trans_par_base(a);
    par_time = omp_get_wtime() - par_time;
  #endif
  
  cout << "Base implementation\n";
  cout << "A:" << size << "x" << size << " B:" << size <<
    "x" << size << endl;
  // Check results
  validate_equals(size, serial_res->vals, parallel_res->vals);
  // Report timings, speedup, efficiency
  report(ser_time, par_time);
}

// The optimization 1 transpose test function
void
mt_opt1(int size, mat::matrix* a)
{
  // Result matrices
  mat::matrix* serial_res = new mat::matrix(size, size);
  mat::matrix* parallel_res = new mat::matrix(size, size);

  // Timing variables
  clock_t begin, end;
  double ser_time, par_time;

  // Do serial mtrans
  #ifdef average
    double total_time = 0;
    for (int i = 0; i < 10; i++) {
      ser_time = omp_get_wtime();
      serial_res = mat::trans_ser_opt1(a);
      ser_time = omp_get_wtime() - ser_time;
      total_time += ser_time;
    }
    ser_time = total_time / 10;
  #else
    ser_time = omp_get_wtime();
    serial_res = mat::trans_ser_opt1(a);
    ser_time = omp_get_wtime() - ser_time;
  #endif

  // Do paralell mtrans
  #ifdef average
    total_time = 0;
    for (int i = 0; i < 10; i++) {
      par_time = omp_get_wtime();
      parallel_res = mat::trans_par_opt1(a);
      par_time = omp_get_wtime() - par_time;
      total_time += par_time;
    }
    par_time = total_time / 10;
  #else
    par_time = omp_get_wtime();
    parallel_res = mat::trans_par_opt1(a);
    par_time = omp_get_wtime() - par_time;
  #endif
  
  cout << "Optimization 1: Row Caching\n";
  cout << "A:" << size << "x" << size << " B:" << size <<
  "x" << size << endl;
  // Check results
  validate_equals(size, serial_res->vals, parallel_res->vals);
  // Report timings, speedup, efficiency
  report(ser_time, par_time);
}

// The optimization 2 transpose test function
void
mt_opt2(int size, mat::matrix* a)
{
  // Result matrices
  mat::matrix* serial_res = new mat::matrix(size, size);
  mat::matrix* parallel_res = new mat::matrix(size, size);

  // Timing variables
  clock_t begin, end;
  double ser_time, par_time;

  // Do serial mtrans
  #ifdef average
    double total_time = 0;
    for (int i = 0; i < 10; i++) {
      ser_time = omp_get_wtime();
      serial_res = mat::trans_ser_opt2(a);
      ser_time = omp_get_wtime() - ser_time;
      total_time += ser_time;
    }
    ser_time = total_time / 10;
  #else
    ser_time = omp_get_wtime();
    serial_res = mat::trans_ser_opt2(a);
    ser_time = omp_get_wtime() - ser_time;
  #endif

  // Do paralell mtrans
  #ifdef average
    total_time = 0;
    for (int i = 0; i < 10; i++) {
      par_time = omp_get_wtime();
      parallel_res = mat::trans_par_opt2(a);
      par_time = omp_get_wtime() - par_time;
      total_time += par_time;
    }
    par_time = total_time / 10;
  #else
    par_time = omp_get_wtime();
    parallel_res = mat::trans_par_opt2(a);
    par_time = omp_get_wtime() - par_time;   
  #endif

  cout << "Optimization 2: Row Caching and Blocking\n";
  cout << "A:" << size << "x" << size << " B:" << size <<
  "x" << size << endl;  
  // Check results
  validate_equals(size, serial_res->vals, parallel_res->vals);
  // Report timings, speedup, efficiency
  report(ser_time, par_time);
}

// The optimization 3 transpose test function
void
mt_opt3(int size, mat::matrix* a)
{
  // Result matrices
  mat::matrix* serial_res = new mat::matrix(size, size);
  mat::matrix* parallel_res = new mat::matrix(size, size);

  // Timing variables
  clock_t begin, end;
  double ser_time, par_time;

  // Do serial mtrans
  #ifdef average
    double total_time = 0;
    for (int i = 0; i < 10; i++) {
      ser_time = omp_get_wtime();
      serial_res = mat::trans_ser_opt3(a);
      ser_time = omp_get_wtime() - ser_time;
      total_time += ser_time;
    }
    ser_time = total_time / 10;
  #else
    ser_time = omp_get_wtime();
    serial_res = mat::trans_ser_opt3(a);
    ser_time = omp_get_wtime() - ser_time;
  #endif

  // Do paralell mtrans
  #ifdef average
    total_time = 0;
    for (int i = 0; i < 10; i++) {
      par_time = omp_get_wtime();
      parallel_res = mat::trans_par_opt3(a);
      par_time = omp_get_wtime() - par_time;
      total_time += par_time;
    }
    par_time = total_time / 10;
  #else
    par_time = omp_get_wtime();
    parallel_res = mat::trans_par_opt3(a);
    par_time = omp_get_wtime() - par_time;   
  #endif

  cout << "Optimization 3: Row Caching, Blocking, and Loop Unrolling\n";
  cout << "A:" << size << "x" << size << " B:" << size <<
  "x" << size << endl;  
  // Check results
  validate_equals(size, serial_res->vals, parallel_res->vals);
  // Report timings, speedup, efficiency
  report(ser_time, par_time);
}

// The base multiply test function
void
mm_base(int size, mat::matrix* a, mat::matrix* b)
{
  // Result matrices
  mat::matrix* serial_res = new mat::matrix(size, size);
  mat::matrix* parallel_res = new mat::matrix(size, size);

  // Timing variables
  clock_t begin, end;
  double ser_time, par_time;

  // Do serial mmult
  #ifdef average
    double total_time = 0;
    for (int i = 0; i < 10; i++) {
      ser_time = omp_get_wtime();
      mat::mult_ser_base(size, a->vals, b->vals, serial_res->vals);
      ser_time = omp_get_wtime() - ser_time;
      total_time += ser_time;
    }
    ser_time = total_time / 10;
  #else
    ser_time = omp_get_wtime();
    mat::mult_ser_base(size, a->vals, b->vals, serial_res->vals);
    ser_time = omp_get_wtime() - ser_time;
  #endif

  // Do paralell mmult
  #ifdef average
    total_time = 0;
    for (int i = 0; i < 10; i++) {
      par_time = omp_get_wtime();
      mat::mult_par_base(size, a->vals, b->vals, parallel_res->vals);
      par_time = omp_get_wtime() - par_time;
      total_time += par_time;
    }
    par_time = total_time / 10;
  #else
    par_time = omp_get_wtime();
    mat::mult_par_base(size, a->vals, b->vals, parallel_res->vals);
    par_time = omp_get_wtime() - par_time;
  #endif

  cout << "Base implementation\n";
  cout << "A:" << size << "x" << size << " B:" << size <<
  "x" << size << endl;  
  // Check results
  validate_equals(size, serial_res->vals, parallel_res->vals);
  // Report timings, speedup, efficiency
  report(ser_time, par_time);
}

// The optimization 1 multiply test function
void
mm_opt1(int size, mat::matrix* a, mat::matrix* b)
{
  // Result matrices
  mat::matrix* serial_res = new mat::matrix(size, size);
  mat::matrix* parallel_res = new mat::matrix(size, size);

  // Timing variables
  clock_t begin, end;
  double ser_time, par_time;

  // Do serial mmult
  #ifdef average
    double total_time = 0;
    for (int i = 0; i < 10; i++) {
      ser_time = omp_get_wtime();
      mat::mult_ser_opt1(size, a->vals, b->vals, serial_res->vals);
      ser_time = omp_get_wtime() - ser_time;
      total_time += ser_time;
    }
    ser_time = total_time / 10;
  #else
    ser_time = omp_get_wtime();
    mat::mult_ser_opt1(size, a->vals, b->vals, serial_res->vals);
    ser_time = omp_get_wtime() - ser_time;
  #endif

  // Do paralell mmult
  #ifdef average
    total_time = 0;
    for (int i = 0; i < 10; i++) {
      par_time = omp_get_wtime();
      mat::mult_par_opt1(size, a->vals, b->vals, parallel_res->vals);
      par_time = omp_get_wtime() - par_time;
      total_time += par_time;
    }
    par_time = total_time / 10;
  #else
    par_time = omp_get_wtime();
    mat::mult_par_opt1(size, a->vals, b->vals, parallel_res->vals);
    par_time = omp_get_wtime() - par_time;
  #endif

  cout << "Optimization 1: Row Caching\n";
  cout << "A:" << size << "x" << size << " B:" << size <<
  "x" << size << endl;  
  // Check results
  validate_equals(size, serial_res->vals, parallel_res->vals);
  // Report timings, speedup, efficiency
  report(ser_time, par_time);
}

// The optimization 2 multiply test function
void
mm_opt2(int size, mat::matrix* a, mat::matrix* b)
{
  // Result matrices
  mat::matrix* serial_res = new mat::matrix(size, size);
  mat::matrix* parallel_res = new mat::matrix(size, size);

  // Transposed B
  mat::matrix* b_t = mat::trans_ser(b);

  // Timing variables 
  clock_t begin, end;
  double ser_time, par_time;

  // Do serial mmult
  #ifdef average
    double total_time = 0;
    for (int i = 0; i < 10; i++) {
      ser_time = omp_get_wtime();
      mat::mult_ser_opt2(size, a->vals, b, serial_res->vals);
      ser_time = omp_get_wtime() - ser_time;
      total_time += ser_time;
    }
    ser_time = total_time / 10;
  #else
    ser_time = omp_get_wtime();
    mat::mult_ser_opt2(size, a->vals, b, serial_res->vals);
    ser_time = omp_get_wtime() - ser_time;
  #endif

  // Do paralell mmult
  #ifdef average
    total_time = 0;
    for (int i = 0; i < 10; i++) {
      par_time = omp_get_wtime();
      mat::mult_par_opt2(size, a->vals, b, parallel_res->vals);
      par_time = omp_get_wtime() - par_time;
      total_time += par_time;
    }
    par_time = total_time / 10;
  #else
    par_time = omp_get_wtime();
    mat::mult_par_opt2(size, a->vals, b, parallel_res->vals);
    par_time = omp_get_wtime() - par_time;
  #endif

  cout << "Optimization 2: Row Caching and Transposition\n";
  cout << "A:" << size << "x" << size << " B:" << size <<
  "x" << size << endl;  
  // Check results
  validate_equals(size, serial_res->vals, parallel_res->vals);
  // Report timings, speedup, efficiency
  report(ser_time, par_time);
}

// The optmization 3 multiply test function
void
mm_opt3(int size, mat::matrix* a, mat::matrix* b)
{
  // Result matrices
  mat::matrix* serial_res = new mat::matrix(size, size);
  mat::matrix* parallel_res = new mat::matrix(size, size);

  // Timing variables 
  clock_t begin, end;
  double ser_time, par_time;

  // Do serial mmult
  #ifdef average
    double total_time = 0;
    for (int i = 0; i < 10; i++) {
      ser_time = omp_get_wtime();
      mat::mult_ser_opt3(size, a->vals, b, serial_res->vals);
      ser_time = omp_get_wtime() - ser_time;
      total_time += ser_time;
    }
    ser_time = total_time / 10;
  #else
    ser_time = omp_get_wtime();
    mat::mult_ser_opt3(size, a->vals, b, serial_res->vals);
    ser_time = omp_get_wtime() - ser_time;
  #endif

  // Do paralell mmult
  #ifdef average
    total_time = 0;
    for (int i = 0; i < 10; i++) {
      par_time = omp_get_wtime();
      mat::mult_par_opt3(size, a->vals, b, parallel_res->vals);
      par_time = omp_get_wtime() - par_time;
      total_time += par_time;
    }
    par_time = total_time / 10;
  #else
    par_time = omp_get_wtime();
    mat::mult_par_opt3(size, a->vals, b, parallel_res->vals);
    par_time = omp_get_wtime() - par_time;
  #endif

  cout << "Optimization 3: Row Caching, Transposition, and Blocking\n";
  cout << "A:" << size << "x" << size << " B:" << size <<
  "x" << size << endl;  
  // Check results
  validate_equals(size, serial_res->vals, parallel_res->vals);
  // Report timings, speedup, efficiency
  report(ser_time, par_time);
}

// The optimization 4 multiply test function
void
mm_opt4(int size, mat::matrix* a, mat::matrix* b)
{
  // Result matrices
  mat::matrix* serial_res = new mat::matrix(size, size);
  mat::matrix* parallel_res = new mat::matrix(size, size);

  // Timing variables 
  clock_t begin, end;
  double ser_time, par_time;

  // Do serial mmult
  #ifdef average
    double total_time = 0;
    for (int i = 0; i < 10; i++) {
      ser_time = omp_get_wtime();
      mat::mult_ser_opt4(size, a->vals, b, serial_res->vals);
      ser_time = omp_get_wtime() - ser_time;
      total_time += ser_time;
    }
    ser_time = total_time / 10;
  #else
    ser_time = omp_get_wtime();
    mat::mult_ser_opt4(size, a->vals, b, serial_res->vals);
    ser_time = omp_get_wtime() - ser_time;
  #endif

  // Do paralell mmult
  #ifdef average
    total_time = 0;
    for (int i = 0; i < 10; i++) {
      par_time = omp_get_wtime();
      mat::mult_par_opt4(size, a->vals, b, parallel_res->vals);
      par_time = omp_get_wtime() - par_time;
      total_time += par_time;
    }
    par_time = total_time / 10;
  #else
    par_time = omp_get_wtime();
    mat::mult_par_opt4(size, a->vals, b, parallel_res->vals);
    par_time = omp_get_wtime() - par_time;
  #endif

  cout << "Optimization 4: Row Caching, Transposition, Blocking, and Loop Unrolling\n";
  cout << "A:" << size << "x" << size << " B:" << size <<
  "x" << size << endl;  
  // Check results
  validate_equals(size, serial_res->vals, parallel_res->vals);
  // Report timings, speedup, efficiency
  report(ser_time, par_time);
}

// Reads two matrices from an input file (first the number of rows for the first
// matrix, then the number of columns, then the actual elements, and then
// similarly for the other matrix). It then multiplies them in both serial and
// parallel and times both results, then reports the timing results via
// report above.
void 
runMultiplyTest(int argc, char** argv)
{
  if (argc != 1) {
    cerr << "error(runMultiplyTest): invalid number of arguments\n";
    return;
  }
  int size = atoi(argv[0]); 
  mat::matrix* a = mat::generate_random_matrix(size);
  mat::matrix* b = mat::generate_random_matrix(size);
  cout << "\nMatrix Multiplication\n";
  if (size <= 1024) {
    mm_base(size, a, b);
    mm_opt1(size, a, b);
  }
  if (size <= 2048)
    mm_opt2(size, a, b);
  if (size >= 512) {
    mm_opt3(size, a, b);
    mm_opt4(size, a, b);
  }
}

// Runs the eigendecomposition and recomposition test routine
void
decompose(int argc, char** argv)
{
  if (argc != 1) {
    cerr << "error(decompose): invalid number of arguments\n";
    return;
  }
  //const string inFile(argv[0]);
  //ifstream ifs(inFile);
  //if (1 || ifs.good()) {
  int size = atoi(argv[0]);
  // Result matrices
  mat::matrix* a = mat::generate_random_matrix(size);
  mat::matrix* q = new mat::matrix(size, size);
  mat::matrix* s = new mat::matrix(size, size);
  mat::matrix* ser;
  mat::matrix* par;

  // Timing variables
  clock_t begin, end;
  double ser_time, par_time;

  // Do serial decomp
  #ifdef average
    double total_time = 0;
    for (int i = 0; i < 10; i++) {
      ser_time = omp_get_wtime();
      mat::eigendecomp_ser(*a, *q, *s);
      ser = mat::eigenrecomp_ser(*q, *s);
      ser_time = omp_get_wtime() - ser_time;
      total_time += ser_time;
    }
    ser_time = total_time / 10;
  #else
    ser_time = omp_get_wtime();
    mat::eigendecomp_par(*a, *q, *s);
    ser = mat::eigenrecomp_ser(*q, *s);
    ser_time = omp_get_wtime() - ser_time;
  #endif
  // Do paralell decomp
  #ifdef average
    total_time = 0;
    for (int i = 0; i < 10; i++) {
      par_time = omp_get_wtime();
      mat::eigendecomp_par(*a, *q, *s);
      par = mat::eigenrecomp_par(*q, *s);
      par_time = omp_get_wtime() - par_time;
      total_time += par_time;
    }
    par_time = total_time / 10;
  #else
    par_time = omp_get_wtime();
    mat::eigendecomp_par(*a, *q, *s);
    par = mat::eigenrecomp_par(*q, *s);
    par_time = omp_get_wtime() - par_time;
  #endif

  cout << "Eigendecomposition\n";
  cout << "A:" << size << "x" << size << " B:" << size <<
  "x" << size << endl;  
  // Check results
  //validate_equals(size, serial_res->vals, parallel_res->vals);
  // Report timings, speedup, efficiency
  report(ser_time, par_time);
}

// Verify functions work as expected
bool sanity_test()
{
  mat::matrix* a = new mat::matrix(4,4);
  for (int i = 0; i < a->rows; i++) {
    for (int j = 0; j < a->cols; j++) {
      a->vals[i * a->cols + j] = i == j ? 1 : i*j;
    }
  }
  a->print();
  cout << endl;
  mat::matrix* b = mat::trans_ser_base(a);
  
  b->vals[11] = 0;
  b->print();
  cin.get();
  mat::matrix* c = new mat::matrix(4,4);
  mat::mult_ser_base(4, a->vals, b->vals, c->vals);
  c->print();
  cin.get();
  mat::matrix* d = mat::invert_ser(*a);
  d->print();
  cin.get();
  mat::eigendecomp_ser(*a,*b,*c);
  cin.get();
  delete a, b, c, d;
  return true;
}

// Runs the matrix transposition optimizations using the provided size
void
runTransposeTest(int argc, char** argv)
{
  if (argc != 1) {
    cerr << "error(runMultiplyTest): invalid number of arguments\n";
    return;
  }
  int size = atoi(argv[0]); 
  mat::matrix* a = mat::generate_random_matrix(size);
  cout << "\nMatrix Transposition\n";
  mt_base(size, a);
  mt_opt1(size, a);
  if (size >= 512) {
    mt_opt2(size, a);
    mt_opt3(size, a);
  }
}


// Program entry point. Arguments should include the operation to be performed,
// along with an input file.
int 
main(int argc, char** argv)
{
  #ifdef debug
    sanity_test();
  #endif
  if (argc < 3) { 
    std::cerr << "error(main): invalid number of arguments\n";
    return -1;
  }
  // Set number of threads
  #pragma omp parallel
  {
    #pragma omp master
    {
      num_procs = omp_get_num_threads();
    }
  }
  const string op(argv[1]);
  switch(op[1])
  {
    case 'e': // Eigendecomposition test
      decompose(argc - 2, argv + 2);
    break;
    case 'm': // Multiply test
      runMultiplyTest(argc - 2, argv + 2);
    break;
    case 't': // Transpose test
      runTransposeTest(argc - 2, argv + 2);
    break;
  }
  return 0;
}
