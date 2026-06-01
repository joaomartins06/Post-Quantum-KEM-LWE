import socket
from kem.keygen import keygen
from kem.kem import decaps
from kem.kdf import derive_key
from lwe.params import LWEParams, DEFAULT_PARAMS
from net.wire import recv_msg, send_msg
from net.serial import params_to_bytes, pk_to_bytes, ct_from_bytes


N = DEFAULT_PARAMS.n
Q = DEFAULT_PARAMS.q
M = DEFAULT_PARAMS.m
SIGMA = DEFAULT_PARAMS.sigma

params = LWEParams(n=N, q=Q, m=M, sigma=SIGMA)

server_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_sock.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
server_sock.bind(("localhost", 9999))
server_sock.listen(1)
print("Server listening on port 9999...")

conn, addr = server_sock.accept()
print(f"Server: Connection by {addr}")

params_bytes = params_to_bytes(params)
send_msg(conn, params_bytes)

print("Server: Parameters sent to client.")

key = keygen(params)
pk_bytes = pk_to_bytes(key[0])
send_msg(conn, pk_bytes)

print("Server: Public key sent to client.")

ct_bytes = recv_msg(conn)
ct = ct_from_bytes(ct_bytes, params)

print("Server: Ciphertext received from client.")

mu = decaps(ct, key[1], params)

print("Server: Shared secret established.")

derived_key = derive_key(mu, info=b"lwe-kem-v1")

print("Server: Derived key:", derived_key.hex())
