import requests

def send_multipart_chunks(file_path, endpoint_url, chunk_size=1000000):
    with open(file_path, 'rb') as file:
        while True:
            chunk = file.read(chunk_size)
            if not chunk:
                break

            files = {'file': ('filename', chunk)}
            requests.post(endpoint_url, files=files)
            
        print("finished sendind chunked requests")

if __name__ == "__main__":
    server_url = 'http://localhost:8080/cgi-bin/cgi_post.py'
    file_path = '/nfs/homes/maricard/Documents/cursus/webserver/assets/1Mb_text.txt'

    send_multipart_chunks(file_path, server_url)
