#ifndef MAT_HPP
#define MAT_HPP

namespace mat 
{
  // Data structure representing our matrix implementation. Holds essentially
  // just the dimensions of the matrix and a column-major array with the values.
  class matrix 
  {
    // Allows direct access to all variables.
    public:
      matrix():rows(0),cols(0){ };
      matrix(int r, int c):rows(r),cols(c){ allocate(); }
      ~matrix(){ delete[] vals; }

      // Defined in mat.cpp
      void 
      print();

      int rows;
      int cols;
      float* vals;
  
    // Internal functions for managing heap memory.
    private:
      void 
      allocate()
      {
        vals = new float[cols * rows]();
      }

      void 
      reallocate()
      {
        delete[] vals;
        allocate();
      }
  };

  // Multiplication and transposition routines defined in mat.cpp.
  matrix* 
  trans_ser(matrix*);

  matrix* 
  trans_par(matrix*);

  void
  mult_ser(int, float*, float*, float*);

  void
  mult_par(int, float*, float*, float*);

  void 
  mult_ser_base(int, float*, float*, float*);

  void 
  mult_par_base(int, float*, float*, float*);
  
  void 
  mult_ser_opt1(int, float*, float*, float*);

  void 
  mult_par_opt1(int, float*, float*, float*);

  void 
  mult_ser_opt2(int, float*, matrix*, float*);

  void 
  mult_par_opt2(int, float*, matrix*, float*);

  void 
  mult_ser_opt3(int, float*, matrix*, float*);

  void 
  mult_par_opt3(int, float*, matrix*, float*);

  void 
  mult_ser_opt4(int, float*, matrix*, float*);

  void 
  mult_par_opt4(int, float*, matrix*, float*);

  matrix* 
  trans_ser_base(matrix*);

  matrix* 
  trans_par_base(matrix*);

  matrix* 
  trans_ser_opt1(matrix*);

  matrix* 
  trans_par_opt1(matrix*);

  matrix* 
  trans_ser_opt2(matrix*);

  matrix* 
  trans_par_opt2(matrix*);

  matrix* 
  trans_ser_opt3(matrix*);

  matrix* 
  trans_par_opt3(matrix*);

  matrix*
  generate_random_matrix(int);

  void 
  eigendecomp_ser(matrix& a, matrix& q, matrix& s);
  
  void 
  eigendecomp_par(matrix& a, matrix& q, matrix& s);

  matrix* 
  eigenrecomp_ser(matrix& q, matrix& s);

  matrix* 
  eigenrecomp_par(matrix& q, matrix& s);

  matrix* 
  invert_ser(matrix& a);

  matrix*
  invert_par(matrix& a);
}
#endif
