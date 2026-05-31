import numpy as np

def mod_add(a, b, q):
    return (a + b) % q

def mod_matmul(A, B, q):
    A = np.asarray(A, dtype=np.int64)
    B = np.asarray(B, dtype=np.int64)
    return np.array((A @ B) % q, dtype=np.int64)

