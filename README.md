# Post-Quantum KEM — LWE

A from-scratch implementation of a Regev-style LWE-based Key Encapsulation Mechanism (KEM) in Python and C.

The Python implementation includes keygen, encaps, decaps, HKDF-SHA256 key derivation, and a two-party TCP handshake producing a shared session key. 
Then, as Python was a bit simple and I miss doing something in C, I decided to implement it on C :) 

## Structure

Post-Quantum-KEM-LWE/
python/
lwe/        # primitives: sampler, modular ops, params
kem/        # keygen, encaps, decaps, kdf
net/        # tcp server, client, wire protocol
tests/      # pytest
c/
include/    # header files
src/        # source files
tests/      # test programs
Makefile


## Requirements

### Python
Python 3.12, NumPy, SciPy, cryptography

```bash
pip install numpy scipy cryptography pytest
```

### C
C99, GCC, OpenSSL (libssl-dev)

```bash
sudo apt install libssl-dev
```