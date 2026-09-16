# Mini-DeepBugger Data Schema

## Experimental Unit

One function = one sample.

## Required Fields

### sample_id
Unique identifier of the sample.

Example:
F000001

### source
Dataset or project from which the sample originated.

Example:
SARD

### project
Original software/test project.

### function_name
Name of the analyzed function, when available.

### label

0 = Benign
1 = Vulnerable

### CWE
CWE identifier, when available.

Example:
CWE-XXX

### CVE
CVE identifier, when available.

Not required for every sample.

### architecture

Initial experiment:
x86-64

### compiler
Compiler used to generate the binary.

Example:
GCC or Clang

### optimization
Compiler optimization level.

Example:
-O0

### binary_path
Path to the compiled binary.

### function_address
Function address when available.

### instructions
Disassembled instructions belonging to the function.

### basic_blocks
Basic blocks belonging to the function.

### cfg_edges
Control-flow edges between basic blocks.

### graph_features
Node/graph features generated for the A-CFG.