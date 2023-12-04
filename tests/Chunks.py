import http.client, os

extensions = {
    'text/plain': '.txt',
    'image/jpeg': '.jpg',
    'image/png': '.png',
    'application/pdf': '.pdf',
    'application/json': '.json',
    'application/octet-stream': '.bin',
    'video/mp4': '.mp4',
    'audio/mpeg': '.mp3'
}

def send_file_in_chunks(host, endpoint, port, file_path, chunk_size=20):
    connection = http.client.HTTPConnection(host, port)
    
    file_extension = os.path.splitext(file_path)[1]
    
    if file_extension not in extensions.values():
        print(f'The file type "{file_extension}" is not supported')
        return
    
    else:
        content_type = next(key for key, value in extensions.items() if value == file_extension)

    # Send the file in chunks
    with open(file_path, 'rb') as file:
        headers = {
            'Host': f'{host}:{port}',
            'Content-Type': content_type,
            'Transfer-Encoding': 'chunked',
        }

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
    port = 8080
    endpoint = '/cgi-bin/cgi_chunk.py'
    file_path = '../assets/jpg/5Mb.jpg'

    send_file_in_chunks(host, endpoint, port, file_path)

# ../assets/chunks.txt
# ../assets/1Mb_text.txt
# ../assets/png/1Mb.png
# ../assets/png/fdf.png
# ../assets/jpg/500Kb.jpg
# ../assets/jpg/5Mb.jpg
# ../assets/700Kb_audio.mp3
# ../assets/sample.pdf
