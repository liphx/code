server {
    listen 80;
    listen [::]:80;
    server_name liph.ink www.liph.ink;

    location / {
        proxy_pass http://static.liph.ink;
    }

    location /api/ {
        proxy_pass http://localhost:9001;
    }
}

server {
    listen 80;
    listen [::]:80;
    server_name static.liph.ink;
    location / {
        root /home/liph/static.liph.ink/public/;
        index index.html;
        charset urf-8;
    }

    error_page 404 /404.html;
    location /404.html {
        root /home/liph/static.liph.ink/public/error_page/;
        charset utf-8;
    }
}

server {
    listen 80;
    listen [::]:80;
    server_name resource.liph.ink;
    location / {
        root /home/liph/www/resource/;
        index index.html;
        charset urf-8;
        autoindex on;
        autoindex_exact_size off;
        autoindex_localtime on;
    }
}
