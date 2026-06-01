import numpy as np

from lwe.params import LWEParams, DEFAULT_PARAMS
from lwe.modular import mod_add, mod_matmul
from lwe.sampler import sample_gaussian, sample_uniform

def encaps(pk, params: LWEParams = DEFAULT_PARAMS):

    #shared secret, binary string of length n
    mu = sample_uniform(q=2, size=(params.n,))

    #generate random matrix R, binary matrix m x n
    R = sample_uniform(q=2, size=(params.m, params.n))

    #u = A^T * R (mod q), shape (n,n)
    u = mod_matmul(pk[0].T, R, params.q)

    #b^T * R (mod q)
    bt_r_aux = (pk[1] @ R) % params.q

    #v = b^T R + mu * floor(q/2) mod q 
    v = mod_add(bt_r_aux, (params.q // 2) * mu, params.q)

    #compute the cyphertext (u, v)
    cyphertext = (u, v)

    return cyphertext, mu.astype(np.uint8)


def decaps(cyphertext, sk, params: LWEParams = DEFAULT_PARAMS):

    u, v = cyphertext

    #w = v - u * s mod q
    w = (v - u.T @ sk) % params.q
    w_new = np.zeros(w.shape, dtype=np.uint8)

    #approximate w to 0 or 1 based on its value
    w_new = ((w >= params.q // 4) & (w <= 3 * params.q // 4)).astype(np.uint8)

    return w_new
