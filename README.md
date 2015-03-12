# HttpServer

Written in C with libev4 on Mac Os Yosemite (platform-dependent!!!)

Features:

- GET and HEAD requests support (with minimal amount of HTTP headers in response)
- highload web-server for static files on http://localhost:80/
- provides simultaneous downloading of several very large files (some Gb) under highload (for example ab -n 50000 -c 100 -r <url>)
- uses all processor resources (prefork, accept in workers; IO mux with non-blocking socket IO and sendfile)

More information about given task you can find <a href="https://github.com/init/http-test-suite">here</a>

Developed in 2015 in <a href="http://tech-mail.ru">Технопарк@Mail.Ru</a>
by Alexey Halaidzhy
