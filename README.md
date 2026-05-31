# Post-Quantum KEM — LWE
 
A from-scratch implementation of a Regev-style LWE-based Key Encapsulation Mechanism (KEM) in Python, including a two-party TCP handshake producing a shared session key.
 
**Status:** Layer 1 — LWE primitives (in progress)
 
## Structure
 
```
Post-Quantum-KEM-LWE/
    lwe/        # primitives: sampler, modular ops, params
    kem/        # keygen, encaps, decaps, kdf
    net/        # tcp server, client, wire protocol
    tests/      # pytest
    notebooks/  # sampler validation, parameter sweep
```
 
## Security note
 
Educational implementation. IND-CPA only (no Fujisaki-Okamoto transform). Do not use in production.
 
## Requirements
 
Python 3.12, NumPy, SciPy, cryptography
 
```bash
pip install numpy scipy cryptography pytest
```