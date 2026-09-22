#!/usr/bin/env bash
# 服务器上一键更新站点（放到 /var/www/example 的同级目录，或服务器任意位置）
# 用法：bash update-site.sh
# 首次部署见 docs/备案通过后上线清单.md

set -euo pipefail

SITE_DIR="/var/www/example"          # ← 改成你的站点目录
BRANCH="main"

cd "$SITE_DIR"

if [ ! -d .git ]; then
  echo "❌ $SITE_DIR 还不是 git 仓库。首次部署先执行："
  echo "   git clone git@github.com:redwenzi101/redwenzi-blog.git $SITE_DIR"
  exit 1
fi

echo "→ 拉取最新代码"
git fetch --depth=1 origin "$BRANCH"
git reset --hard "origin/$BRANCH"

# 站点是纯静态文件，不需要构建；如果以后接入构建，在这里加构建命令

echo "→ 刷新 Nginx 缓存目录权限（如有需要可删掉）"
chown -R www-data:www-data "$SITE_DIR" 2>/dev/null || true

echo "✅ 更新完成：$(date '+%F %T')"
