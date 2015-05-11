using System;
using System.Collections.Generic;
using System.IO;
using System.Linq;
using System.Text;

namespace pa2
{
    /// <summary>
    /// Represents an ASCII format PGM image file
    /// </summary>
    public class ASCII_Image
    {
        /// <summary>
        /// Compact ASCII PGM image class containing the header, dimensions, gray level and pixels
        /// </summary>
        public class Image
        {
            public const string header = "P2";
            public int width, height;
            public int grayLevel;
            public int[,] pixels;

            public Image()
            {
                width = height = grayLevel = 0;
            }

            public Image(int x, int y)
            {
                width = x;
                height = y;
                grayLevel = 255;
                pixels = new int[y, x];
            }
        }

        private Image img;

        public ASCII_Image()
        {
            img = new Image();
        }

        public ASCII_Image(int w, int h)
        {
            img = new Image();
            setDimensions(w, h);
        }

        public Image GetImage() { return img; }

        /// <summary>
        /// Copies the input image
        /// </summary>
        /// <param name="i"></param>
        public void SetImage(Image i)
        {
            img.width = i.width;
            img.height = i.height;
            img.grayLevel = i.grayLevel;
            img.pixels = i.pixels;
        }

        /// <summary>
        /// Sets dimensions of the image internally
        /// </summary>
        /// <param name="w">Width of image</param>
        /// <param name="h">Height of image</param>
        private void setDimensions(int w, int h)
        {
            img.width = w;
            img.height = h;
            img.pixels = new int[h, w];
        }
        /// <summary>
        /// Reads in a binary PGM format file and converts it to an ASCII representation
        /// </summary>
        /// <param name="fname">Input file name</param>
        /// <returns>Size of input file</returns>
        public long FromFile(string fname)
        {
            try
            {                
                BinaryReader br = new BinaryReader(File.Open(fname, FileMode.Open));
                int w = br.ReadUInt16();
                int h = br.ReadUInt16();
                int gval = br.ReadByte();
                setDimensions(w, h);
                img.grayLevel = gval;
                for (int i = 0; i < img.height; i++)
                {
                    for (int j = 0; j < img.width; j++)
                    {
                        img.pixels[i, j] = Convert.ToInt32(br.ReadByte());
                    }
                }
                br.Close();
            }
            catch (Exception e)
            {
                System.Console.Error.WriteLine(e.Message);
                System.Console.Error.WriteLine(e.StackTrace);
                return -1;
            }
            return new FileInfo(fname).Length;
        }

        /// <summary>
        /// Writes the ASCII representation of the image to file
        /// </summary>
        /// <param name="fname">Output file name</param>
        /// <returns>Size of file written</returns>
        public long ToFile(string fname)
        {
            try
            {
                StreamWriter sw = new StreamWriter(fname);
                string line = "";
                int counter = 0;
                sw.WriteLine("P2");
                sw.WriteLine(String.Format("{0} {1}", img.width, img.height));
                sw.WriteLine(String.Format("{0}", img.grayLevel));
                for (int i = 0; i < img.height; i++)
                {
                    for (int j = 0; j < img.width; j++)
                    {
                        line += img.pixels[i, j] + " ";
                        if (++counter == 17)
                        {
                            sw.WriteLine(line);
                            line = "";
                            counter = 0;
                        }
                    }
                }
                if (counter > 0)
                    sw.WriteLine(line);
                sw.Close();
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
