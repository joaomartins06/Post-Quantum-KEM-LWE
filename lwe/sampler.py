import numpy as np

def sample_gaussian(sigma, size):
    return np.rint(np.random.normal(0, sigma, size)).astype(np.int64)


def sample_uniform(q, size):
    return np.random.randint(0, q, size).astype(np.int64)