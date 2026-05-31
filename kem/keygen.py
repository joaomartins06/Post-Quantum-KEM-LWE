from lwe.params import LWEParams, DEFAULT_PARAMS
from lwe.modular import mod_add, mod_matmul
from lwe.sampler import sample_gaussian, sample_uniform

def keygen(params: LWEParams = DEFAULT_PARAMS):
    #generate secret key 
    sk = sample_uniform(q=params.q, size=(params.n,))
    
    #gerate public key (A, b)
    #start with matrix A
    A = sample_uniform(q=params.q, size=(params.m, params.n))
    
    #generate the error
    e = sample_gaussian(sigma=params.sigma, size=(params.m,))
    
    #compute b = A * s + e (mod q)
    b = mod_add(mod_matmul(A, sk, params.q), e, params.q)
    
    #(pk, sk)
    return (A, b), sk 

