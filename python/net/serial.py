import numpy as np
import struct   
from lwe.params import LWEParams

def params_to_bytes(params) -> bytes:
    #realistically, we also have to send the parameters
    #they are usually part of the "public key" in real implementations
    return struct.pack('>IIId', params.n, params.q, params.m, params.sigma)

def params_from_bytes(data: bytes):
    n, q, m, sigma = struct.unpack('>IIId', data)
    return LWEParams(n=int(n), q=int(q), m=int(m), sigma=float(sigma))


def pk_to_bytes(pk):
    #serialize pk=(A, b) into bytes
    A_bytes = pk[0].tobytes()
    b_bytes = pk[1].tobytes()
    return A_bytes + b_bytes


def pk_from_bytes(data, params) -> tuple:

    #each int64 has 8 bytes, so we split the data at 8*m*n
    A_bytes = data[:params.m * params.n * 8] 
    b_bytes = data[params.m * params.n * 8:] 

    A = np.frombuffer(A_bytes, dtype=np.int64).reshape((params.m, params.n)).copy()
    b = np.frombuffer(b_bytes, dtype=np.int64).copy()
    return (A, b)


def ct_to_bytes(ct):
    #serialize ct=(u, v) into bytes
    u_bytes = ct[0].tobytes()
    v_bytes = ct[1].tobytes()
    return u_bytes + v_bytes


def ct_from_bytes(data, params) -> tuple:

    #each int64 has 8 bytes, so we split the data at 8*n*n
    u_bytes = data[:params.n * params.n * 8] 
    v_bytes = data[params.n * params.n * 8:] 

    u = np.frombuffer(u_bytes, dtype=np.int64).reshape((params.n, params.n)).copy()
    v = np.frombuffer(v_bytes, dtype=np.int64).copy()
    return (u, v)