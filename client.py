import socket
import json
import pickle

def test():
	ip_port = ("127.0.0.1", 8880)
	sk = socket.socket()
	sk.connect(ip_port)

	imagePath = input("Please enter your image path:\n")
	imagePath = imagePath.encode()
	sk.sendall(imagePath)

	data = sk.recv(4096)
	results_data = pickle.loads(data)
	print(results_data)

	sk.close()

if __name__ == '__main__':
	while True:
		test()