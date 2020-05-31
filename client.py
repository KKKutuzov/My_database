import socket
import struct

def getStrFromServer(sock,a):
    size = str(a)+'s'
    packer = struct.Struct(size)
    data = sock.recv(a)
    return data.decode('utf-8')
def getIntFromServer(sock):
    unpacker = struct.Struct('i')
    data = sock.recv(unpacker.size,socket.MSG_WAITALL)
    int_val = unpacker.unpack(data)
    return int_val[0]
def sendStrToServer(sock,z):
    k = len(z)
    f = str(k)+'s'
    packer = struct.Struct('i')
    packed_data = packer.pack(k)
    sock.sendall(packed_data)
    packer = struct.Struct(f)
    packed_data = packer.pack(z.encode('utf-8'))
    sock.sendall(packed_data)

sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_address = ('localhost', 5555)
sock.connect(server_address)
print("Добро пожаловать в наше приложениие!","Напишите ваш запрос:",sep='\n')
while(1):
    z = input()
    if z=="stop":
        sendStrToServer(sock,z);
        break
    if z=="exit end":
        sendStrToServer(sock,z)
        break
    sendStrToServer(sock,z)
    a = getIntFromServer(sock)
    print("Сервер ответил:")
    for i in range(a):
        b = getIntFromServer(sock)
        print(getStrFromServer(sock,b))
    print("Ввведите ваш запрос:")
