using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Collections;

namespace pa2
{
    public class PCA
    {
        // Single function that performs PCA on the input matrix and displays the results
        public PCA(Matrix m, int rank)
        {
            Console.WriteLine("m (original)");
            m.print();
            Console.WriteLine();
            int info = 0;
            double[] _s = new double[Math.Max(m.y,m.x)];
            double[,] _v = new double[m.y,m.x];
            alglib.pca.pcabuildbasis(m.elements, m.y, m.x, ref info, ref _s, ref _v);
            Matrix v = new Matrix(_s.Length, _s.Length);
            v.elements = _v;
            
            // Build a diagonal matrix out of the list of eigenvalues
            Matrix d = new Matrix(_s.Length, _s.Length);
            for (int i = 0; i < _s.Length; i++)
            {
                for (int j = 0; j < _s.Length; j++)
                {
                    if (i == j && i < rank) d.elements[i, j] = _s[i];
                    else d.elements[j, i] = 0;
                }
            }
            Matrix vt = v.copy();
            vt.transpose();
            
            Matrix s = v.multiply(d).multiply(vt);
            s.round(3);
            Console.WriteLine();
            v.print();
            Console.WriteLine();
            m.transpose();
            Matrix mm = s.multiply(m);
            mm.print();
            Console.ReadKey();
            
        }
    }
}
