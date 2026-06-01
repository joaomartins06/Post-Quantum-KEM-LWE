import numpy as np
from cryptography.hazmat.primitives.kdf.hkdf import HKDF
from cryptography.hazmat.primitives import hashes


def derive_key(mu, info: bytes) -> bytes:

    #convert mu to bytes
    mu_bytes = mu.tobytes()

    hkdf = HKDF(algorithm=hashes.SHA256(), length=32, salt=None,
             info=info).derive(mu_bytes)

    return hkdf