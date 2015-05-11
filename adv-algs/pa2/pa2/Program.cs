using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.IO;

namespace pa2
{
    /// <summary>
    /// A crude 3-byte floating point number class
    /// </summary>
    class ThreeQuarterFloat
    {
        static class FieldFlags
        {
            public const byte Sign = 0x80;
            public const byte ExpUpper = 0x7F;
            public const byte ExpLower = 0x80;
            public const byte SigUpper = 0x7F;
            public const byte SigLower = 0xFF;
        }

        const int bias = 127;
        private float orig;
        private float approx;
        private float acc;

        byte[] raw = new byte[3];

        public ThreeQuarterFloat(float d)
        {
            orig = d;
            normalize(d);
        }

        public ThreeQuarterFloat(byte[] data)
        {
            raw = data;
        }

        public byte[] GetBitfield() { return raw; }

        public float GetValue()
        {
            byte[] val = new byte[4];
            val[0] = 0x0;
            for (int i = 0; i < 3; i++)
            {
                val[i + 1] = raw[i];
            }
            return BitConverter.ToSingle(val, 0);
        }

        public float GetAccuracy() { return acc; }

        private void normalize(float d)
        {
            for (int i = 0; i < raw.Length; i++)
                raw[i] = 0x00;
            byte[] input = BitConverter.GetBytes(d);
            if (input.Length != 4)
            {
                System.Console.Error.WriteLine("error: (normalize) byte conversion for {0}", d);
            }
            //copy 3 upper bytes
            for (int i = 0; i < 3; i++)
            {
                raw[i] = input[i+1];
            }
        }
    }
 
    /// <summary>
    /// Main function for PA2. Contains the options required in the write up as well as additional utility functions
    /// </summary>
    class Program
    {        
        public static char[] delim = { ' ' };

        // Prints the options that the program can 
        public static void Usage()
        {
            System.Console.Error.WriteLine("pa2.exe -s Run Simulation: runs program for options 0-5\nargs: [inputFile] [k]\ninputFile is a PGM file, k is rank\n");
            System.Console.Error.WriteLine("pa2.exe -0 Run SVD on PGM File\nargs: [inputFile]\ninputFile is a PGM\n");
            System.Console.Error.WriteLine("pa2.exe -1 Run PGM ASCII to PGM Binary conversion\nargs: [inputFile]\ninputFile is a PGM ASCII file\n");
            System.Console.Error.WriteLine("pa2.exe -2 Run PGM Binary to PGM ASCII conversion\nargs: [inputFile]\ninputFile is a PGM Binary file\n");
            System.Console.Error.WriteLine("pa2.exe -3 Run SVD ASCII to SVD Binary conversion\nargs: [headerFile] [matricesFile] [k]\nheaderFile is an ASCII PGM header, matricesFile is an ASCII SVD file, k is the rank of approximation\n");
            System.Console.Error.WriteLine("pa2.exe -4 Run SVD Binary to PGM ASCII conversion\nargs: [inputFile]\ninputFile is an SVD Binary file with the header encoded\n");
            System.Console.Error.WriteLine("pa2.exe -5 Run PCA algorithm\nargs: [inputFile]\ninputFile reads in a matrix, starting with 2 lines containing m and n, then m*n lines containing one value each, read into the matrix from left to right, top to bottom.\n");
            System.Console.ReadKey(false);          
        }

        // Program entry point
        static void Main(string[] args)
        {
            if (args.Length < 2)
            {
                Usage();
                return;
            }
            string ip_name;
            string op_name;
            long inLength;
            long outLength;

            // Switch between the different options for different program functionality (see usage() above).
            switch (args[0])
            {
                // Run Simulation
                case "-s":
                    if (args.Length < 3)
                    {
                        System.Console.Error.WriteLine("Invalid usage; Must supply at least 2 arguments\n");
                        Usage();
                        return;
                    }
                    //sim vars
                    ip_name = args[1];
                    int rank_sim = Convert.ToInt32(args[2]);
                    op_name = ip_name.Substring(0, ip_name.LastIndexOf('.'));
                    string bin_out_name = op_name + "_b.pgm";
                    string ascii_out_name = op_name + "2.pgm";
                    string decomp_hdr_name = op_name + "_hdr.txt";
                    string decomp_out_name = op_name + "_svd.txt";
                    string svd_out_name = op_name + "_b.pgm.SVD";
                    string recomp_out_name = op_name + "_k.pgm";
                    double rate_sim;
                    SVD svd_sim = new SVD();

                    //clear any exisiting files
                    if (File.Exists(bin_out_name)) File.Delete(bin_out_name);
                    if (File.Exists(ascii_out_name)) File.Delete(ascii_out_name);
                    if (File.Exists(svd_out_name)) File.Delete(svd_out_name);
                    if (File.Exists(recomp_out_name)) File.Delete(recomp_out_name);
            
                    //decomp for later on
                    Console.Write("Checking for SVD for {0}...", ip_name);
                    if (!File.Exists(decomp_hdr_name) || !File.Exists(decomp_out_name))
                    {
                        Console.WriteLine("not found");                        
                        Console.Write("Decomposing {0} to {1}, {2}...", ip_name, decomp_hdr_name, decomp_out_name);
                        svd_sim.Decompose(ip_name, op_name);
                        Console.WriteLine("done!\n");
                    }
                    else Console.WriteLine("found\n");

                    //run opt1 (pgm ascii->pgm bin)
                    BinaryImage bi_sim = new BinaryImage();
                    Console.Write("Compressing {0} to {1} (PGM ASCII => PGM Binary)...", ip_name, bin_out_name);
                    inLength = bi_sim.FromFile(ip_name);
                    outLength = bi_sim.ToFile(bin_out_name);
                    Console.WriteLine("done!");
                    rate_sim = (inLength - outLength) / (double)inLength;
                    Console.WriteLine("Input size: {0} Output size: {1} Compression rate: {2:0.0##}\n", inLength, outLength, rate_sim);

                    //run opt2 (pgm bin->pgm ascii)                    
                    ASCII_Image ai_sim = new ASCII_Image();
                    Console.Write("Inflating {0} to {1} (PGM Binary => PGM ASCII)...", bin_out_name, ascii_out_name);
                    inLength = ai_sim.FromFile(bin_out_name);
                    outLength = ai_sim.ToFile(ascii_out_name);
                    Console.WriteLine("done!");                    
                    Console.WriteLine("Input size: {0} Output size: {1}\n", inLength, outLength);

                    //run opt3 (svd ascii to svd bin)
                    svd_sim = new SVD();
                    Console.Write("Compressing {0} to {1} (SVD ASCII => SVD Binary)...", decomp_out_name, svd_out_name);
                    inLength = svd_sim.FromFile(decomp_hdr_name, decomp_out_name, rank_sim);
                    outLength = svd_sim.ToFile(svd_out_name, rank_sim);
                    Console.WriteLine("done!");
                    rate_sim = (inLength - outLength) / (double)inLength;
                    Console.WriteLine("Input size: {0} Output size: {1} Compression rate: {2:0.0##}\n", inLength, outLength, rate_sim);

                    //run opt4 (svd bin to pgm ascii)
                    svd_sim = new SVD();
                    Console.Write("Recomposing {0} from {1} to {2} (SVD Binary => PGM ASCII)...", ip_name, svd_out_name, recomp_out_name);
                    svd_sim.Recompose(svd_out_name, recomp_out_name);
                    Console.WriteLine("done!\n");
                    Console.WriteLine("Simulation complete\n\nPress any key to exit...");
                    break;
                // Decompose PGM file
                case "-0":                    
                    ip_name = args[1];
                    op_name = ip_name.Substring(0, ip_name.LastIndexOf('.')) + "_decomp.txt";
                    System.Console.WriteLine("Generating decomposition for {0}...", ip_name);
                    SVD decomp = new SVD();
                    decomp.Decompose(ip_name, op_name);
                    System.Console.WriteLine("Decomposition of {0} complete", ip_name);
                    break;
                // PGM Ascii -> PGM Binary
                case "-1":                    
                    ip_name = args[1];
                    op_name = ip_name.Substring(0, ip_name.LastIndexOf('.')) + "_b.pgm";                    
                    Console.Write("Converting {0} to {1} (ASCII to Binary) ... ", ip_name, op_name);
                    BinaryImage bi = new BinaryImage();
                    inLength = bi.FromFile(ip_name);
                    outLength = bi.ToFile(op_name);
                    if (outLength > 0)
                    {
                        Console.WriteLine("done!");
                        double rate = (inLength - outLength) / (double)inLength;
                        Console.WriteLine("Original File Size: {0} Compressed File Size: {1}\nCompression Rate: {2:0.0##}", inLength, outLength, rate);
                    }
                    else
                        Console.WriteLine("\nError: output file write failed");
                    break;
                // PGM Binary -> PGM Ascii
                case "-2":
                    ip_name = args[1];
                    op_name = ip_name.Substring(0, ip_name.LastIndexOf('_')) + "2.pgm";
                    Console.Write("Converting {0} to {1} (Binary to ASCII) ... ", ip_name, op_name);
                    ASCII_Image ai = new ASCII_Image();
                    inLength = ai.FromFile(ip_name);
                    outLength = ai.ToFile(op_name);                    
                    if (outLength > 0){
                        Console.WriteLine("done!");
                        Console.WriteLine("Original File Size: {0} Inflated File Size: {1}", inLength, outLength);
                    }
                    else
                        Console.WriteLine("\nError: output file write failed");
                    break;
                // SVD Ascii -> SVD Binary
                case "-3":
                    string header_name = args[1];
                    ip_name = args[2];
                    op_name = ip_name.Substring(0, ip_name.LastIndexOf('.')) + "_b.pgm.SVD";
                    int rank = Convert.ToInt32(args[3]);
                    Console.Write("Compressing SVD for {0} using rank {1}...", ip_name, rank);
                    SVD svd = new SVD();
                    inLength = svd.FromFile(header_name, ip_name, rank);
                    outLength = svd.ToFile(op_name, rank);
                    if (outLength > 0)
                    {
                        Console.WriteLine("done!");
                        double rate = (inLength - outLength) / (double)inLength;
                        Console.WriteLine("Original File Size: {0} Compressed File Size: {1}\nCompression Rate: {2:0.0##}", inLength, outLength, rate);
                    }
                    break;
                // SVD Binary -> PGM Ascii
                case "-4":
                    ip_name = args[1];
                    op_name = ip_name.Substring(0, ip_name.LastIndexOf('_')) + "_k.pgm";
                    Console.Write("Recomposing {0}...", ip_name);
                    SVD recomp = new SVD();
                    recomp.Recompose(ip_name, op_name);
                    Console.WriteLine("done!");
                    break;
                // PCA
                case "-5":
                    ip_name = args[1];

                    //Read in a matrix to perform PCA on (see usage() above).
                    StreamReader sr = new StreamReader(ip_name);
                    int x = int.Parse(sr.ReadLine());  // Number of variables
                    int y = int.Parse(sr.ReadLine());  // Number of trials/experiments/data records
                    Matrix m = new Matrix(y, x);
                    int j = 0;
                    while (!sr.EndOfStream)
                    {
                        for (int i = 0; i < x; i++)
                        {
                            string s = sr.ReadLine();
                            m.elements[j, i] = double.Parse(s);
                        }
                        j++;
                    }
                    sr.Close();
                    PCA p = new PCA(m, 4);  // Constructor handles the transformation and output; nothing else to do
                    break;
            }
            System.Console.ReadKey(true);
        }
    }
}
