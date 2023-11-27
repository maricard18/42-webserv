import http.client

def send_file_in_chunks(host, endpoint, port, file_path, chunk_size=20):
    connection = http.client.HTTPConnection(host, port)

    # Open the file and read it in chunks
    with open(file_path, 'rb') as file:
        # Set up headers for chunked transfer encoding
        headers = {
            'Host': f'{host}:{port}',
            'Content-Type': 'application/octet-stream',
            'Transfer-Encoding': 'chunked',
        }

        # Send the HTTP POST request with headers
        connection.request('POST', endpoint, headers=headers)

        while True:
            chunk = file.read(chunk_size)
            if not chunk:
                break  # Reached the end of the file

            # Send the current chunk
            chunk_size_hex = hex(len(chunk))[2:].encode('utf-8')
            connection.send(chunk_size_hex + b'\r\n')
            connection.send(chunk + b'\r\n')

        # Send the final chunk to indicate the end
        connection.send(b'0\r\n\r\n')

        # Get the server's response
        response = connection.getresponse()
        print(f"Status Code: {response.status}")

if __name__ == "__main__":
    host = 'localhost'
    endpoint = '/cgi-bin/cgi_post.py'
    port = 8080
    file_path = '../assets/chunks.txt'

    send_file_in_chunks(host, endpoint, port, file_path)
