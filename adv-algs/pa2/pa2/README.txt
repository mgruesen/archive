PA2 README

Requirements: Microsoft .NET Framework 4.0 or greater

Execution:
pa2.exe [option] [args...]

Options:
 -s [ASCII PGM file] [rank]
Runs through options 1 to 4 for a given PGM file with the given rank for approximation
Checks for an SVD header and matrix file with the input file prefix. If it is not found
it will decompose the image file for later use

-0 [ASCII PGM file]
Decomposes the given file into a header and matrix file for SVD

-1 [ASCII PGM file]
Compresses the ASCII PGM file into a Binary PGM file

-2 [Binary PGM file]
Inflates the Binary PGM file into an ASCII PGM file

-3 [Header File] [SVD Matrices File] [rank]
Converts the ASCII SVD matrices file into a Binary SVD matrices file with the header included

-4 [Binary SVD Matrices File]
Inflates the Binaray SVD matrices file, approximates the original image, and writes it to an ASCII PGM file

-5 [PCA File]
Performs PCA on the given input file (Not fully functional)