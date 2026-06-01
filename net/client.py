import socket
from kem.kem import encaps
from kem.kdf import derive_key
from lwe.params import LWEParams, DEFAULT_PARAMS
from net.wire import recv_msg, send_msg
from net.serial import params_from_bytes, pk_from_bytes, ct_to_bytes


sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
sock.connect(("localhost", 9999))

print("Client: Connected to server, waiting for parameters...")

params_bytes = recv_msg(sock)
params = params_from_bytes(params_bytes)

print("Client: Parameters received from server.")

pk_bytes = recv_msg(sock)
pk = pk_from_bytes(pk_bytes, params)

print("Client: Public key received from server.")

ct, mu = encaps(pk, params)
ct_bytes = ct_to_bytes(ct)
send_msg(sock, ct_bytes)

print("Client: Ciphertext sent to server.")


derived_key = derive_key(mu, info=b"lwe-kem-v1")

print("Client: Derived key:", derived_key.hex())
