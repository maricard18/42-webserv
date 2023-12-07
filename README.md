# **webserver**
This project was graded <strong>110/100</strong>.
[Subject](./assets/subject.pdf)

## 📝 **Authorship**

- [Mario Henriques](https://github.com/maricard18) ([**maricard**](https://profile.intra.42.fr/users/maricard))
- [Bruno Costa](https://github.com/BrunoCostaGH) ([**bsilva-c**](https://profile.intra.42.fr/users/bsilva-c))

## 🧬 **Cloning**

To successfully clone this repository, use this command

```shell
git clone https://github.com/maricard18/42-webserver.git 
```

## 📒 **About**
This project guides you in building a C++ HTTP server, offering a practical understanding of the HTTP protocol. <br>
Test your server with real browsers and delve into the fundamentals of networking. <br>
Gain insights into web security while creating a functional web server, even if web development isn't your primary focus. <br>
Webserv provides a hands-on, educational experience for anyone interested in the core technologies of the internet.


## 🎥 **Demo**

<p align="center">
  <img src="./assets/demo.gif" width="100%">
</p>


## 📦 **Compilation**
To compile the webserver you should run `make` with the configuration file of your choice. <br><br>
This rule will generate a `webserv` file, which is the zipped version of all the object files. <br><br>
To launch the executable you should follow this syntax...

```sh
$ ./webserv config_file/server.conf
```

Where `config_file/server.conf` is the name of a file that represents the configuration of the webserver. <br><br>
You can find example of configuration files in the `config_files` folder. <br>


## 🕹️ **Configuration File rules**

```bash
Server {
    root /absolute/path;
    index index.html; # optional
    autoindex true; # optional, default is false
    server_name _; # optional
    listen 0.0.0.0:8080; # optional IP, default is 0.0.0.0:8080
    client_max_body_size 1M; # optional, default is 1M
    upload_store /path/to/uploaded/files; # optional, default is /uploads
    error_page 404 /relative/path/to/error/page.html; # optional

    location /delete/ {
        # root /path/to/root;
        # index index_file.html;
        # autoindex false/true;
        # allow_methods GET POST DELETE;
    }

    location .py {
        # path to executable
        cgi_pass /absolute/path/to/executable;
    }
}
```

## 💫 **Testing**

This project was tested using self-made tests. <br><br>
If you wish to access some of these tests, please send us a message on Slack.

---
> Feel free to ask me any questions through Slack (**maricard**). <br>
> GitHub [@maricard18](https://github.com/maricard18) &nbsp;&middot;&nbsp;
> Linkedin [Mario Henriques](https://www.linkedin.com/in/mario18) &nbsp;&middot;&nbsp;
> [42 Porto](https://www.42porto.com/en)
