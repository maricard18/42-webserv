# This python program sends a chunked request to our webserver

import http.client

def send_file_line_by_line(host, endpoint, port, file_path):
    connection = http.client.HTTPConnection(host, port)

    # Open the file and read it line by line
    with open(file_path, 'r') as file:
        # Set up headers for chunked transfer encoding
        headers = {
            'Host': f'{host}:{port}',
            'Content-Type': 'application/octet-stream',
            'Transfer-Encoding': 'chunked',
        }

        # Send the HTTP POST request with headers
        connection.request('POST', endpoint, headers=headers)

        for line in file:
            # Send the current line as a chunk
            line_data = line.encode('utf-8')
            chunk_size_hex = hex(len(line_data))[2:].encode('utf-8')
            connection.send(chunk_size_hex + b'\r\n')
            connection.send(line_data + b'\r\n')
            connection.flush()  # Flush the data to ensure it is sent immediately


        # Send the final chunk to indicate the end
        connection.send(b'0\r\n\r\n')

	# Get server response
    response = connection.getresponse()
    print("Chunked request sent!")
    print(f"Response status code: {response.status}")

if __name__ == "__main__":
    host = 'localhost'
    endpoint = '/cgi-bin/cgi_post.py'
    port = 8080
    file_path = '../assets/chunked_file.txt'

    send_file_line_by_line(host, endpoint, port, file_path)
