using System;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace pa2
{
    /// <summary>
    /// Represents a Binary PGM image file
    /// </summary>
    class BinaryImage
    {
        public static char[] delim = { ' ', '\r', '\t', '\n' };        
        
        /// <summary>
        /// Compact Image class. Width and Height are unsigned shorts (16 bits)
        /// Grayscale level and the pixels are all bytes (8 bits)
        /// </summary>
        class Image { 
            public ushort width, height;
            public byte grayLevel;
            public byte[,] pixels;
        }

        private Image img;

        public BinaryImage()
        {
            img = new Image();         
        }

        /// <summary>
        /// Reads an ASCII formatted PGM image file and converts it to a binary representation
        /// </summary>
        /// <param name="fname">Input file name</param>
        /// <returns>Size of input file</returns>
        public long FromFile(string fname)
        {
            try
            {         
                using (StreamReader sr = new StreamReader(fname))
                {
                    int i, j, row;
                    string[] temp;
                    string line;
                    sr.ReadLine();//p2                    
                    line = sr.ReadLine();
                    if (line[0] == '#')
                        line = sr.ReadLine();
                    temp = line.Split(delim, 2);//width, height
                    ushort w = Convert.ToUInt16(temp[0]);
                    ushort h = Convert.ToUInt16(temp[1]);
                    setDimensions(w, h);                       
                    img.grayLevel = Convert.ToByte(sr.ReadLine());//temp[0]);    
                    string[] values = sr.ReadToEnd().Split(delim, StringSplitOptions.RemoveEmptyEntries);
                    sr.Close();
                    for (i = 0; i < img.height; i++)
                    {
                        row = i * img.width;
                        for (j = 0; j < img.width; j++)
                        {
                            img.pixels[i, j] = Convert.ToByte(int.Parse(values[row + j]));
                        }
                    }
                }
            }            
            catch (IOException e)
            {
                System.Console.Error.WriteLine(e.Message);
                System.Console.Error.WriteLine(e.StackTrace);
                return -1;
            }
            return new FileInfo(fname).Length;
        }

        /// <summary>
        /// Sets the dimensions of the image. Max width/height is 65535.
        /// </summary>
        /// <param name="w">Width</param>
        /// <param name="h">Height</param>
        private void setDimensions(ushort w, ushort h)
        {
            img.width = w;
            img.height = h;
            img.pixels = new byte[h,w];
        } 

        /// <summary>
        /// Writes the binary representation of the image to a binary file
        /// </summary>
        /// <param name="fname">Output file name</param>
        /// <returns>Size of file written</returns>
        public long ToFile(string fname)
        {
            try {
                BinaryWriter bw = new BinaryWriter(File.OpenWrite(fname));                 
                bw.Write(img.width);
                bw.Write(img.height);
                bw.Write(img.grayLevel);
                for (int i = 0; i < img.height; i++)
                {
                    for (int j = 0; j < img.width; j++)
                    {
                        bw.Write(img.pixels[i,j]);
                    }
                }
                bw.Close();
            }
            catch (Exception e)
            {
                System.Console.Error.WriteLine(e.Message);
                System.Console.Error.WriteLine(e.StackTrace);
                return -1;
            }
            return new FileInfo(fname).Length;
        }
    }
}
