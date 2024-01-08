server {
    listen 80;
    listen [::]:80;
    server_name lipenghua.com www.lipenghua.com;
    location / {
        root /home/liph/liphx.github.io/docs;
        index index.html;
        charset urf-8;
    }

    error_page 404 /404.html;
}
