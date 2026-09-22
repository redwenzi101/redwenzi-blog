#!/usr/bin/env bash
# redwenzi 博客 · 阿里云 ECS 一键上线（Ubuntu 22.04）
#
# 用法（在你的 ECS 上执行）：
#   sudo bash bootstrap.sh redwenzi.icu 你的邮箱@example.com
#
# 前提：阿里云安全组已放行 80 / 443；DNSPod 已把域名 A 记录指向这台机器的公网 IP。
# 这个脚本可重复执行：已装的包不重复装，已克隆的仓库只做 git pull。

set -euo pipefail

DOMAIN="${1:?用法: sudo bash bootstrap.sh 域名 邮箱}"
EMAIL="${2:?请提供邮箱（Let's Encrypt 证书通知用）}"
REPO="https://github.com/redwenzi101/redwenzi-blog.git"
SITE_DIR="/var/www/${DOMAIN%%.*}"
CONF="/etc/nginx/conf.d/${DOMAIN%%.*}.conf"

if [ "$(id -u)" -ne 0 ]; then
  echo "❌ 请用 sudo 运行：sudo bash bootstrap.sh $DOMAIN $EMAIL"
  exit 1
fi

echo "→ [1/5] 安装 nginx / git / certbot"
export DEBIAN_FRONTEND=noninteractive
apt-get update -y
apt-get install -y nginx git certbot python3-certbot-nginx

echo "→ [2/5] 拉取站点到 $SITE_DIR"
mkdir -p "$SITE_DIR"
if [ -d "$SITE_DIR/.git" ]; then
  git -C "$SITE_DIR" pull --ff-only || echo "⚠️ git pull 没成功，先手工看看 $SITE_DIR 的状态再继续"
elif [ -n "$(ls -A "$SITE_DIR" 2>/dev/null)" ]; then
  # 已经用压缩包/手动上传过站点文件的情况下，直接用现有文件
  echo "   ℹ $SITE_DIR 里已经有站点文件（没有 .git），跳过 clone，直接用现有文件"
  echo "     以后想用 git 一键更新，可清空该目录后执行：sudo git clone $REPO $SITE_DIR"
else
  git clone "$REPO" "$SITE_DIR"
fi

echo "→ [3/5] 写 Nginx 配置 $CONF"
cat > "$CONF" <<CONF_EOF
server {
    listen 80;
    listen [::]:80;
    server_name $DOMAIN www.$DOMAIN;

    root $SITE_DIR;
    index index.html;
    charset utf-8;

    gzip on;
    gzip_comp_level 6;
    gzip_min_length 1024;
    gzip_types text/plain text/css text/xml application/javascript application/json application/xml image/svg+xml;

    add_header X-Content-Type-Options nosniff;
    add_header Referrer-Policy strict-origin-when-cross-origin;

    location ~* \.(css|js|png|jpe?g|webp|svg|ico|woff2)\$ {
        expires 30d;
        add_header Cache-Control "public";
    }
    location ~* \.html\$ {
        add_header Cache-Control "no-cache";
    }

    # 这些目录不该被公开访问
    location ~ ^/(\.git|tools|deploy|docs)/ { deny all; }

    location / { try_files \$uri \$uri/ =404; }

    error_page 404 /404.html;
    location = /404.html { internal; }
}
CONF_EOF

nginx -t
systemctl reload nginx

echo "→ [4/5] 申请 HTTPS 证书（含 www；www 没备案就只签主域名）"
if certbot --nginx -d "$DOMAIN" -d "www.$DOMAIN" --redirect --agree-tos -m "$EMAIL" --non-interactive; then
  echo "   ✔ 主域名 + www 都签好了"
else
  echo "   ⚠️ 带上 www 没成功，改为只签主域名"
  certbot --nginx -d "$DOMAIN" --redirect --agree-tos -m "$EMAIL" --non-interactive || {
    echo "   ❌ 证书还是没签成功。先确认：DNSPod 的 A 记录已指向本机公网 IP、安全组放行 80/443、域名能 http 打开。"
    echo "      确认后重跑：sudo certbot --nginx -d $DOMAIN -d www.$DOMAIN --redirect"
  }
fi

echo "→ [5/5] 自检"
echo "   站点目录：$SITE_DIR"
echo "   配置文件：$CONF"
systemctl is-active --quiet nginx && echo "   nginx：运行中"
echo ""
echo "✅ 完成。现在打开 https://$DOMAIN 看看"
echo "   以后更新站点：bash $SITE_DIR/deploy/update-site.sh"
