server {
  listen 443 ssl;
  server_name lipenghua.com www.lipenghua.com;
  location / {
    root /home/liph/liphx.github.io/docs;
    index index.html;
    charset urf-8;
    try_files $uri $uri/ @rewrite;
  }
  location @rewrite {
    rewrite ^(.+)$ $1.html permanent;
    rewrite ^(.+)/$ $1.html permanent;
  }
  error_page 404 /404.html;
  ssl_certificate /etc/letsencrypt/live/lipenghua.com/fullchain.pem;
  ssl_certificate_key /etc/letsencrypt/live/lipenghua.com/privkey.pem;
  ssl_protocols TLSv1 TLSv1.1 TLSv1.2 TLSv1.3;
  ssl_ciphers HIGH:!aNULL:!MD5;
}

server {
  listen 443 ssl;
  server_name elden_ring.lipenghua.com;
  location / {
    root /home/liph/elden_ring/docs;
    index index.html;
    charset urf-8;
  }
  ssl_certificate /etc/letsencrypt/live/*.lipenghua.com/fullchain.pem;
  ssl_certificate_key /etc/letsencrypt/live/*.lipenghua.com/privkey.pem;
  ssl_protocols TLSv1 TLSv1.1 TLSv1.2 TLSv1.3;
  ssl_ciphers HIGH:!aNULL:!MD5;
}
