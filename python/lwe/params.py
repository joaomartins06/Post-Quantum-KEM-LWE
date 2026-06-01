from dataclasses import dataclass

@dataclass
class LWEParams:
    n: int #LWE dimension
    q: int  #modulus
    m: int #number of samples
    sigma: float  #std of the error distribution

DEFAULT_PARAMS = LWEParams(n=256, q=3329, m=512, sigma=3.2)