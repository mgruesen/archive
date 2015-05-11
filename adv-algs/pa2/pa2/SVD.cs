using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;
using System.Diagnostics;

using System.Drawing;

namespace pa2
{
    /// <summary>
    /// Simple matrix class for matrix manipulation and representation
    /// </summary>
    public struct Matrix
    {
        // Backing 2D array containing the actual values
        public double[,] elements;
        // Dimensions of the matrix.
        public int x, y;

        public Matrix(int m, int n)
        {
            this.y = m;
            this.x = n;
            elements = new double[m,n];
        }

        // Transpose this matrix and store it in the same object.
        public void transpose()
        {
            // Interchange the elements
            double[,] newElements = new double[x,y];
            for (int i = 0; i < x; i++)
            {
                for (int j = 0; j < y; j++)
                {
                    newElements[i, j] = elements[j, i];
                }
            }
            elements = newElements;
            // Interchange the dimensions
            int t = x;
            this.x = this.y;
            this.y = t;
        }

        // Multiply this matrix by m and return the result
        public Matrix multiply(Matrix m){      
            // Check that the inner dimensions match
            if (this.x != m.y) 
                throw new InvalidOperationException("Matrix dimension mismatch");
            // Eventual result
            Matrix n = new Matrix(this.y, m.x);

            // Compute the product
            for (int i = 0; i < this.y; i++)
            {
                for (int j = 0; j < m.x; j++)
                {
                    double sum = 0;
                    for (int k = 0; k < this.x; k++)
                    {
                        sum += this.elements[i, k] * m.elements[k, j];
                    }
                    n.elements[i, j] = sum; 
                }
            }
            return n;
        }
        
        // Scale this matrix by the constant factor a
        public void scale(float a) {
            for (int i = 0; i < this.x; i++)
            {
                for (int j = 0; j < this.y; j++)
                {
                    elements[i, j] *= a;
                }
            }
        }

        // Return a deep copy of this matrix
        public Matrix copy()
        {
            Matrix m2 = new Matrix(this.x, this.y);
            for (int i = 0; i < this.x; i++)
            {
                for (int j = 0; j < this.y; j++)
                {
                    m2.elements[i,j] = this.elements[i,j];
                }
            }
            return m2;
        }

        // Truncate the elements to a fixed number of decimal places
        public void round(int decimals)
        {
            for (int i = 0; i < x; i++)
            {
                for (int j = 0; j < y; j++)
                {
                    string s = elements[j,i].ToString();
                    if (s.Length - s.IndexOf('.') < decimals)
                    {
                        elements[j, i] = double.Parse(s);
                        continue;
                    }
                    elements[j, i] = double.Parse(s.Substring(0,s.IndexOf('.') + decimals));
                }
            }
        }

        // Print the matrix
        public void print()
        {
            for (int i = 0; i < y; i++)
            {
                for (int j = 0; j < x; j++)
                {
                    Console.Write(elements[i,j]+"\t");
                }
                Console.Write('\n');
            }
        }

        // Another print function.
        public void Print(string lbl)
        {
            System.Console.WriteLine(lbl + ":");
            for (int i = 0; i < y; i++)
            {
                for (int j = 0; j < x; j++)
                {
                    Console.Write("{0:0.000000000###} ", elements[i, j]);
                }
                Console.Write("\n");
            }
            Console.Write("\n");
        }
    }

    /// <summary>
    /// A class to perform Singular Value Decomposition functions
    /// </summary>
    class SVD
    {
        public static char[] delim = { ' ', '\r', '\t', '\n' };
        Matrix U;
        double[] S;
        Matrix V;
        int K;

        /// <summary>
        /// Approximates the original image from its SVD and writes it to an ASCII PGM file
        /// </summary>
        /// <param name="ip_name">Input File (SVD Binary)</param>
        /// <param name="op_name">Output File (ASCII PGM)</param>
        public void Recompose(string ip_name, string op_name)
        {
            BinaryReader br = new BinaryReader(File.OpenRead(ip_name));
            short m = br.ReadInt16();
            short n = br.ReadInt16();
            short level = br.ReadByte();
            short rank = br.ReadByte();
            Matrix U = new Matrix(m, rank);
            double[] S = new double[rank];
            Matrix V = new Matrix(rank, n);
            for (int i = 0; i < m; i++)
            {
                for (int j = 0; j < rank; j++)            
                {
                    U.elements[i, j] = Convert.ToDouble(br.ReadSingle());
                }
            }
            for (int i = 0; i < rank; i++)
            {
                S[i] = Convert.ToDouble(br.ReadSingle());
            }
            for (int i = 0; i < rank; i++)
            {
                for (int j = 0; j < n; j++)
                {
                    V.elements[i, j] = Convert.ToDouble(br.ReadSingle());
                }
            }
            if (br.BaseStream.Position != br.BaseStream.Length)
            {
                Console.Error.WriteLine("\nError: Did not read all bytes");
                br.Close(); 
                return;
            }
            br.Close(); 
            
            Matrix res = new Matrix(U.y, V.x);
            ASCII_Image.Image img = new ASCII_Image.Image(V.x, U.y);
            for (int i = 0; i < rank; i++)
            {
                for (int j = 0; j < res.y; j++)
                {
                    for(int k = 0; k < res.x; k++)
                    {                        
                        img.pixels[j, k] += Convert.ToInt32((U.elements[j,i] * S[i] * V.elements[i,k]) + 0.5);
                    }
                }
            }
            writeRecomp(op_name, img);
        }

        /// <summary>
        /// Writes the ASCII format for a PGM image file
        /// </summary>
        /// <param name="op_name">Output file name</param>
        /// <param name="a">ASCII image</param>
        private void writeRecomp(string op_name, ASCII_Image.Image a)
        {
            string line = "";
            int counter = 0;
            StreamWriter sw = new StreamWriter(File.OpenWrite(op_name));
            sw.WriteLine("P2");
            sw.WriteLine("{0} {1}", a.width, a.height);
            sw.WriteLine("255");
            for (int i = 0; i < a.height; i++)
            {
                for (int j = 0; j < a.width; j++)
                {
                    line += a.pixels[i, j] + " ";
                    if (++counter == 17)
                    {
                        sw.WriteLine(line);
                        line = "";
                        counter = 0;
                    }
                }
            }
            if(counter > 0)
                sw.WriteLine(line);
            sw.Close();
        }

        /// <summary>
        /// Performs the Singular Value Decomposition for a given ASCII PGM file
        /// </summary>
        /// <param name="ip_name">Input File Name (ASCII PGM)</param>
        /// <param name="op_name">Output File Name Prefix</param>
        public void Decompose(string ip_name, string op_name)
        {
            StreamReader sr = new StreamReader(File.OpenRead(ip_name));
            string line = "";
            string[] header;
            sr.ReadLine();//p2
            line = sr.ReadLine();//comment
            if (line[0] == '#')
                line = sr.ReadLine();
            header = line.Split(delim, 2, StringSplitOptions.None);                       
            Matrix m = new Matrix(Convert.ToInt32(header[1]), Convert.ToInt32(header[0]));
            int row;
            string[] cells = sr.ReadToEnd().Split(delim, StringSplitOptions.RemoveEmptyEntries);
            for (int i = 0; i < m.y; i++)
            {
                row = i * m.x;
                for (int j = 0; j < m.x; j++)
                {
                    m.elements[i,j] = Convert.ToDouble(int.Parse(cells[row + j]));
                }
            }
            sr.Close();
            U = new Matrix(m.y, m.y);
            S = new double[m.y * m.x];
            V = new Matrix(m.x, m.x);
            alglib.rmatrixsvd(m.elements, m.y, m.x, 2, 2, 2, out S, out U.elements, out V.elements);
            V.transpose();
            writeDecomp(op_name, U, S, V);
        }

        /// <summary>
        /// Writes the ASCII PGM header file and SVD file
        /// </summary>
        /// <param name="op_name">Output File Name Prefix</param>
        /// <param name="U">U Matrix</param>
        /// <param name="S">S Matrix</param>
        /// <param name="V">V_t Matrix</param>
        private void writeDecomp(string op_name, Matrix U, double[] S, Matrix V)
        {
            string hdr_name = op_name + "_hdr.txt";
            string decomp_name = op_name + "_svd.txt";
            StreamWriter sw = new StreamWriter(File.OpenWrite(decomp_name));
            for (int i = 0; i < U.y; i++)
            {
                for (int j = 0; j < U.x; j++)
                {
                    sw.Write(U.elements[i, j] + " ");
                }
            }
            for (int i = 0; i < U.y; i++)
            {
                for (int j = 0; j < V.y; j++)
                {
                    if (i == j)
                        sw.Write(S[i] + " ");
                    else
                        sw.Write("0 ");
                }
            }
            for (int i = 0; i < V.y; i++)
            {
                for (int j = 0; j < V.x; j++)
                {
                    sw.Write(V.elements[i, j] + " ");
                }
            }
            sw.Close();
            sw = new StreamWriter(File.OpenWrite(hdr_name));
            sw.WriteLine("{0} {1} {2}", V.x, U.x, 255);
            sw.Close();
        }

        /// <summary>
        /// Reads an ASCII format header and SVD
        /// </summary>
        /// <param name="hdr">Header file</param>
        /// <param name="input">SVD input file</param>
        /// <param name="k">Rank of approximation</param>
        /// <returns>Size of SVD input file (not header)</returns>
        public long FromFile(string hdr, string input, int k)
        {
            StreamReader sr = new StreamReader(File.OpenRead(hdr));
            int m, n, scale;
            string line = sr.ReadLine();
            string[] header = line.Split(delim, 3, StringSplitOptions.None);
            m = Convert.ToInt32(header[1]);
            n = Convert.ToInt32(header[0]);
            scale = Convert.ToInt32(header[2]);
            sr.Close();
            
            sr = new StreamReader(File.OpenRead(input));
            if (k > n || k > m)
            {
                Console.Error.WriteLine("err - FromFile(): rank is greater than dimensions given");
                return - 1;
            }
            K = k;
            U = new Matrix(m, m);
            S = new double[Math.Max(m,n)];
            V = new Matrix(n, n);
            int row, off = 0;
            string[] cells = sr.ReadToEnd().Split(delim, StringSplitOptions.RemoveEmptyEntries);
            for (int i = 0; i < m; i++)
            {
                row = i * m;
                for (int j = 0; j < m; j++)
                {
                    U.elements[i, j] = Convert.ToDouble(cells[off + row + j]);
                }
            }
            off += m * m;
            int s_count = 0;
            for (int i = 0; i < m; i++)
            {
                row = i * n;
                for (int j = 0; j < n; j++) 
                { 
                    if (i == j) S[s_count++] = Convert.ToDouble(cells[off + row + j]);
                }
            }
            off += m * n;
            for (int i = 0; i < n; i++)
            {
                row = i * n;
                for (int j = 0; j < n; j++)
                {
                    V.elements[i, j] = Convert.ToDouble(cells[off + row + j]);
                }
            }
            V.transpose();
            sr.Close();
            return (new FileInfo(input).Length) + (new FileInfo(hdr).Length);
        }

        /// <summary>
        /// Writes the header and SVD to a binary format file
        /// </summary>
        /// <param name="outname">Output file name</param>
        /// <param name="rank">Rank of approximation</param>
        /// <returns>Size of file written</returns>
        public long ToFile(string outname, int rank)
        {
            BinaryWriter bw = new BinaryWriter(File.OpenWrite(outname));
            //header to recompose
            bw.Write((UInt16)U.y);
            bw.Write((UInt16)V.x);
            bw.Write((byte)255);
            bw.Write((byte)rank);
            for (int i = 0; i < U.y; i++)
            {
                for (int j = 0; j < rank; j++)      
                {
                    bw.Write((float)U.elements[i, j]);
                }
            }
            for (int i = 0; i < rank; i++)
            {
                bw.Write((float)S[i]);
            }
            for (int i = 0; i < rank; i++)
            {
                for (int j = 0; j < V.x; j++)
                {
                    bw.Write((float)V.elements[i, j]);
                }
            }
            bw.Close();
            return new FileInfo(outname).Length;
        }
    }
}