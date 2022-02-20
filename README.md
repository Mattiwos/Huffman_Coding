# Huffman Coding
Description:
	I will be implementing data compression called Huffman, after it’s original creator who helped found the Computer Science Department at UC Santa Cruz. The main key to the data compression is entropy, which is the measure of the amount of information in  a set of symbols. Using this information you are able to encode data by assigning the least amount of bits to the most common symbol and the greatest number of bits to the least common symbol thus compressing the original data.
## How To Run Program
```
make
```
- Used to build all programs
### Encode
```
./encode [-flag]
```
- Used to run encode file using huffman coding.
```
-h             Program usage and help.
-v             Print compression statistics.
-i infile      Input file to compress.
-o outfile     Output of compressed data.
```
- Available flags.

### Decode
```
./decode [-flag]
```
- Used to run decode file encoded using huffman coding.
```
-h             Program usage and help.
-v             Print compression statistics.
-i infile      Input file to decompress.
-o outfile     Output of decompressed data.
```
- Available flags.
---

## How to use Makefile
```
make
**or**
make all
```
- Used to build Program
```
make clean
```
- Used to removes all files that compiler generated.
```
make format
```
- Format all source code, including the header files.
```
make scan-build
```
- Tests binary file for any missed errors
```
make [flag]
```
- Used to build specific files.
```
encode: Builds just the encoder.
decode: Builds just the decoder.
entropy: Builds just the supplied entropy-measure program.
```
- Flags used to only build specific programs.

