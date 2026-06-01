import socket 


def send_msg(sock, data: bytes):
    #compute the header, length of the data in bytes
    length_header = len(data).to_bytes(4, byteorder='big')
    #send everything together
    sock.sendall(length_header + data)


def recv_msg(sock) -> bytes:
    #compute the length of the received message
    #one should be able to compute with socket.recv(4) but 
    #we never know, I guess :)
    raw_len = _recv_exact(sock, 4)
    #convert into an int
    n = int.from_bytes(raw_len, byteorder='big')
    #get the full data
    data = _recv_exact(sock, n)
    return data


def _recv_exact(sock, n)-> bytes:
    #socket recv may return less bytes than requested, so we stream until 
    #we get everything, instead of getting a partial message at a time
    buf = b""
    while len(buf) < n:
        chunk = sock.recv(n - len(buf))
        if not chunk:
            raise ConnectionError("Socket closed")
        buf += chunk
    return buf

