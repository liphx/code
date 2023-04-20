# hexo 搭建网站

- https://hexo.io/zh-cn/docs/
- https://nodejs.org/en/download/

`npm install -g hexo-cli`

## 使用

`hexo <command>`

```
Commands:
* clean     - Remove generated files and cache.
* config    - Get or set configurations.
* deploy    - Deploy your website.
* generate  - Generate static files.
* help      - Get help on a command.
* init      - Create a new Hexo folder.
* list      - List the information of the site
* migrate   - Migrate your site from other system to Hexo.
* new       - Create a new post.
* publish   - Moves a draft post from _drafts to _posts folder.
* render    - Render files with renderer plugins.
* server    - Start the server.
* version   - Display version information.
```

## 配置

`_config.yml` 设置网站的信息、主题等

## source

除 `_posts` 目录外（`init` 时会创建 `_posts/hello-world.md`)，以 `_` 开头的文件和隐藏文件会被忽略。`Markdown` 和 `HTML` 文件会被解析并放到 public 文件夹，其他文件直接被拷贝过去。

## 主题

`themes` 目录存放主题，启用主题需要在 `_config.yml` 中设置，如 `theme: next`

`next` 主题下载 <https://github.com/theme-next/hexo-theme-next/>

`next` 的配置文件 `themes/next/_config.yml`

## 搜索功能

`npm install hexo-generator-search --save`

修改 `_config.yml`

```yaml
search:
  path: search.xml
  field: all
  format: html
  limit: 10000
```

`field` 字段可以为 `post/page/all`， `hexo g` 命令后该插件会将搜索内容放到 `search.xml`

修改 `themes/next/_config.yml`

```yaml
local_search:
  enable: true
```

修改完后我发现搜索的结果一直为空，调试了下源代码，发现如下问题

文件 `themes/next/source/js/local-search.js`

```js
// Only match articles with not empty titles
datas = datas.filter(data => data.title).map(data => {
    data.title = data.title.trim();
    data.content = data.content ? data.content.trim().replace(/<[^>]+>/g, '') : '';
    data.url = decodeURIComponent(data.url).replace(/\/{2,}/g, '/');
    return data;
});
```

这里只会匹配带`title` 的文件，去掉 `filter(data => data.title)` 即可

## 同步

文档使用 `git` 来同步，启用 `hexo` 的服务器(`hexo s -p 80 &`)每10分钟(`crontab`)执行如下脚本进行更新

```shell
cd $hexo_dir/source &&
git pull origin master &&
hexo g
```

## 评论功能

修改 `themes/next/layout/_layout.swig`, 添加前端代码

```html
<textarea id="text" style="height:120px; width:80%; resize:none"></textarea>
<button id="submit">评论</button>
<script type="text/javascript">
    window.onload = function() {
    var b = document.getElementById("submit");
        b.onclick = function() {
            var comment = document.getElementById("text").value;
            if (comment == "") {
                alert("none");
            } else {
                var httpRequest = new XMLHttpRequest();
                httpRequest.open("POST", "/api/comment");
                httpRequest.setRequestHeader("Content-type", "application/json");
                httpRequest.send(JSON.stringify({"msg": comment, "time": new Date().toLocaleString(),"url": window.location.href }));
                alert("success");
                window.location.href="/comment.html";
            }
        }
    }
</script>
```

后端代码

```js
var http = require('http');
var server = http.createServer();
var fs = require('fs');

comment_path = 'source/comment.md'

function deal_msg(msg) {
    str = ''
    str = str + '`' + msg['time'] + '` ' + '[link](' + msg['url'] + ')\n'
    str = str + msg['msg'] + '\n\n'
    return str
}

function comment(req, res) {
    if (req.url != '/api/comment') {
        return;
    }
    var buff = ''
    req.on("data", function(data) {
        buff += data.toString();
    });

    var json;
    req.on("end", function() {
        json = JSON.parse(buff);
        if (msg.length != 0) {
            fs.writeFile(comment_path, deal_msg(json), { flag: "a"}, function(err) {});
        }
    });
}

server.on('request', comment);
server.listen(8080);
```

设置`hexo` 监听`8000` 端口(`hexo s -p 8000 &`)，并使用 `nginx` 反向代理

```nginx
location / {
    proxy_pass http://localhost:8000;
}

location /api/ {
    proxy_pass http://localhost:8080;
}
```
