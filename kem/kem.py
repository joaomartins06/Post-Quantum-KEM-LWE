import numpy as np

from lwe.params import LWEParams, DEFAULT_PARAMS
from lwe.modular import mod_add, mod_matmul
from lwe.sampler import sample_gaussian, sample_uniform

def encaps(pk, params: LWEParams = DEFAULT_PARAMS):

    #shared secret, binary string of length n
    mu = sample_uniform(q=2, size=(params.n,))

    #generate random r, binary string of length m
    r = sample_uniform(q=2, size=(params.m,))

    #u = A^T * r (mod q)
    u = mod_matmul(pk[0].T, r, params.q)

    #b^T * r (mod q)
    bt_r_aux = mod_matmul(pk[1].T, r, params.q)

    #v = b^T r + mu * floor(q/2) mod q
    v = mod_add(bt_r_aux, (params.q // 2) * mu, params.q)

    #compute the cyphertext (u, v)
    cyphertext = (u, v)

    return cyphertext, mu


def decaps(cyphertext, sk, params: LWEParams = DEFAULT_PARAMS):

    u, v = cyphertext

    #w = v - s * u mod q
    w = (v - sk * u) % params.q
    w_new = np.zeros(w.shape, dtype=np.uint8)

    for i in range(len(w)):
        #if w_i is closer to 0 than to q/2, set mu_i = 0, else mu_i = 1
        if w[i] < params.q // 4 or w[i] > 3 * params.q // 4:
            w_new[i] = 0
        else:
            w_new[i] = 1

    return w_new
