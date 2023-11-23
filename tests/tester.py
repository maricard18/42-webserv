import requests
import threading
        
def test_a_lot_of_requests(url):
    for i in range(1):
        response = requests.get(url)
        status_code = response.status_code
        if (status_code != 200):
            return 0
    return 1 

def test_web_server(url, port):
    
    status_code = test_a_lot_of_requests(url)
    if (status_code == 0):
        print(f"Something went wrong :( on {port}")
    print(f"{port} done")

def test_multiple_ports(base_url, ports):
    threads = []

    for port in ports:
        url = f"{base_url}:{port}"
        thread = threading.Thread(target=test_web_server, args=(url, port,))
        threads.append(thread)
        thread.start()

    # Wait for all threads to finish
    for thread in threads:
        thread.join()

if __name__ == "__main__":
    base_server_url = "http://localhost"  # Replace with your C++ web server base URL
    target_ports = [8081, 8080]  # Replace with the ports you want to test

    test_multiple_ports(base_server_url, target_ports)
